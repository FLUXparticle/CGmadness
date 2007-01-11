/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * $Id$
 *
 */

#include "ball.h"

#include "common.h"
#include "game.h"
#include "callback.h"
#include "objects.h"
#include "shader.h"
#include "fbuffer.h"
#include "vector.h"
#include "features.h"
#include "keyboard.h"
#include "texture.h"

#include "debug.h"

#include "GL/glew.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

/* Konstanten */
#define ACCELERATION 5.0f

#define GRAVITY 9.81f

#define JUMP 50.0f

#define FRICTION 0.99f

#define ELASTICITY 0.5f


#define CUBE_MAP_SIZE 128


RenderTarget gTargetCube[6];
static Viewport gViewportCube[6];

static Vector3 gSpeed;

static int gIsBallInPieces;

static int gBallLayout = 0;

Object sgoBall;

static GLhandleARB gShaderBall;
static int gCubeMapBall;

static int gTextureBall;

/* Vorhandene Ballfeatures abfragen */
int hasBallTexture(void) {
	return gTextureBall >= 0;
}

int hasBallReflection(void) {
	return gCubeMapBall != 0;
}

int hasBallShader(void) {
	return gShaderBall != 0;
}

/*
 * Initialisierungen für die CubeMaps
 */
void initCubeMap(void) {
	static Object oReflection;

	Matrix m;
	int i;

	initProjectMat(m, 90.0f);
	
	/* Initialisiert den Framebuffer für die Berechnung der CubeMap Textur */
	gCubeMapBall = initFBufferCube(CUBE_MAP_SIZE, CUBE_MAP_SIZE, &gTargetCube[0]);

	initObject(&oReflection, drawGameReflection);
	
	for (i = 0; i < 6; i++) {
		memcpy(&gViewportCube[i].projection[0][0], &m, sizeof(Matrix));
		gViewportCube[i].world = &oReflection;

		gTargetCube[i].viewport = &gViewportCube[i];
		addRenderTarget(&gTargetCube[i]);
	}
}

/*
 * Bestimmt die höchste Z-Position eines Quadrates
 */
float getMaxZValue(Square* square) {
	int i;
	float res = 0.0f;
	for (i = 0 ; i < 4; i++) {
		if (square->vertices[i].z > res)
			res = square->vertices[i].z;
	}
	return res;
}

int useBallShader(void) {
	return hasShader() && (gBallLayout == BALL_LAYOUT_GOLFBALL || gBallLayout == BALL_LAYOUT_GOLFBALL_METAL);
}

int useBallReflection(void) {
	return hasFramebuffer() && (gBallLayout == BALL_LAYOUT_METAL || gBallLayout == BALL_LAYOUT_GOLFBALL_METAL);
}

/*
 * Kugellayout ändern
 */
void changeBall(int layout) {
	int i;
	int reflection;

  gBallLayout = layout;

	reflection = useBallReflection();

	for (i = 0; i < 6; i++) {
		gTargetCube[i].enabled = reflection;
	}
}

/*
 * Neuberechnung der 6 Spiegelflächen der Kugel
 */
void updateReflection(void) {
	
	/* Up Vektoren und Blickrichtungen für die 6 spiegelflächen */
	static Vector3 lookat[] = {
		{  1.0f,  0.0f,  0.0f },
		{ -1.0f,  0.0f,  0.0f },
		
		{  0.0f,  1.0f,  0.0f },
		{  0.0f, -1.0f,  0.0f },

		{  0.0f,  0.0f,  1.0f },
		{  0.0f,  0.0f, -1.0f }
	};

	static Vector3 up[] = {
		{ 0.0f, -1.0f,  0.0f },
		{ 0.0f, -1.0f,  0.0f },
		
		{ 0.0f,  0.0f,  1.0f },
		{ 0.0f,  0.0f, -1.0f },
		
		{ 0.0f, -1.0f,  0.0f },
    { 0.0f, -1.0f,  0.0f }
	};

	int i;
	
	if (useBallReflection()) {
		glMatrixMode(GL_MODELVIEW);
		for (i = 0; i < 6; i++) {
			glLoadIdentity();
			gluLookAt(
					sgoBall.pos.x, sgoBall.pos.y, sgoBall.pos.z,
					sgoBall.pos.x + lookat[i].x, sgoBall.pos.y + lookat[i].y, sgoBall.pos.z + lookat[i].z,
					up[i].x, up[i].y, up[i].z
					);
			glGetFloatv(GL_MODELVIEW_MATRIX, &gViewportCube[i].view[0][0]);
		}
	}
}

/*
 * Spielkugel auf die Startposition setzen
 */
void resetBall(void) {
	Square roofSquare;
	getRoofSquare(sgLevel.start.x, sgLevel.start.y, &roofSquare);
	setObjectPosition3f(&sgoBall, sgLevel.start.x + 0.5f, sgLevel.start.y + 0.5f, getMaxZValue(&roofSquare) + 2.5f);

	gSpeed.x = 0.0f;
	gSpeed.y = 0.0f;
	gSpeed.z = 0.0f;
	
	gIsBallInPieces = 0;

	updateReflection();
}

/*
 * Spielkugel mit Explosiopn resetten
 */
void explodeBall(void) {
	Vector3 pos = sgoBall.pos;
	Vector3 speed = gSpeed;

	resetBall();

	gSpeed.z = -10.0f;
	
	initExplosion(pos, speed, sgoBall.pos, gSpeed);

	sgoBall.pos = pos;
	gSpeed = speed;
	
	gIsBallInPieces = 1;
}

/*
 * Ballobjekt initialisieren
 */
void initBall(void) {
	initObject(&sgoBall, drawGameBall);
	
	if (hasShader()) {
		gShaderBall = makeShader("golfball.vert", "golfball.frag");

		if (gShaderBall == 0) {
			printf("Golfball-Shader not ready :-(\n");
		} else {
			printf("Golfball-Shader ready :-)\n");
		}
	}
	
	if (hasFramebuffer()) {
		initCubeMap();
	}

	gTextureBall = loadTexture("data/ball.tga", 0);

	setObjectScalef(&sgoBall, BALL_RADIUS);
}

/* 
 * Kugel bewegen und Berechnungen fuer die BallPhysik 
 */
void animateBall(double interval) {
	int collision = 0;
	int q;
	int x;
	int y;
	int dx;
	int dy;
	
	Vector3 move = { 0.0f, 0.0f, 0.0f };

	Vector3 normal = { 0.0f, 0.0f, 0.0f };

	Vector3 ball;

	Vector3 step;

	/* Auf Benutzereingaben reagieren */
	if (isCursorPressed(CURSOR_LEFT)) {
		move = sub(move, sgRight);
	}
	
	if (isCursorPressed(CURSOR_RIGHT)) {
		move = add(move, sgRight);
	}
	
	if (isCursorPressed(CURSOR_DOWN)) {
		move = sub(move, sgForward);
	}
	
	if (isCursorPressed(CURSOR_UP)) {
		move = add(move, sgForward);
	}

	normalize(&move);

	gSpeed = add(gSpeed, scale(ACCELERATION * interval, move));


	gSpeed.z -= GRAVITY * interval;

	/* Kollisionspruefung */

	step = scale(interval, gSpeed);

	ball = add(sgoBall.pos, step);

	x = floor(ball.x);
	y = floor(ball.y);

  /* Durchlaufen aller 9 Felder um den Ball herum */
  for (dx = -1; dx <= 1; dx++) {
		for (dy = -1; dy <= 1; dy++) {
			int start;
			int end;

			/* Start und End Index fuer alle Flaechen des aktuellen Feldes bestimmen */
			getVertIndex(x + dx, y + dy, &start, &end);
			
			/* Alle Quadrate des Feldes durchlaufen */
			for (q = start; q < end; q += 4) {
				/* Aktuell zu pruefendes Quadrat holen */
				Vector3* quad = &sgVertices[q];

				Vector3 dist;
				Vector3 move;

				float l;
				int i;

				Vector3 dir = sgNormals[q];

				/* a = Ballmittelpunkt auf die Ebene projeziert */
				Vector3 a = sub(ball, quad[0]);
				a = sub(a, scale(dot(a, dir), dir));
				a = add(a, quad[0]);
				
				/* Durchlaufne der 4 Kanten eines Squares */
				for (i = 0; i < 4; i++) {
					int j = (i + 1) % 4; /* Nächste Ecke */
					Vector3 b;
					Vector3 n;
					float na;
				
					/* b = Kante */	
					b = sub(quad[j], quad[i]);

					/* n = Normale über der Kante in der Ebene des Quadarts */
					n = norm(cross(dir, b));

					/* na = Entfernung von a von der Kante */
					na = dot(sub(a, quad[i]), n);

					/* Wenn Punkt a außerhalb des Quadrats liegt, wird es auf die Kante verschoben */
					if (na < 0) {
						a = add(a, scale(-na, n));
					}
				}

				/* dist = Vektor um den Ballmittelpunkt verschoben werden muss, um im Quadrat zu liegen */	
				dist = sub(a, ball);
				l = len(dist);

				/* move = Vektor um den der Ball bewegrt werden müsste, damit er nicht mehr kollidiert */
				move = scale(-((BALL_RADIUS - l) / l), dist);
				
        /* Kollision? */
				if (l < BALL_RADIUS) {
					/* Bei Kollision wird der Ball noch etwas rotiert, damit das Rollen auf der Ebene einigemaßen aussieht... */
					Vector3 right = norm(cross(dir, step));
					Vector3 forward = norm(cross(right, dir));
					float angle = dot(sub(ball, sgoBall.pos), forward) / (2.0f * PI * BALL_RADIUS) * 360.0f;
					rotateObject(&sgoBall, angle, &right.x);

					ball = add(ball, move);

					normal = add(normal, move);
					collision = 1;
				}
			}
		}
	}

	sgoBall.pos = ball;

	normalize(&normal);

	/* Kontakt zur Oberfläche */
	if (collision) {
		float vn = dot(gSpeed, normal);
		Vector3 rebound = scale(-(1 + ELASTICITY) * vn, normal);

    if (len(rebound) > 3.0f * JUMP * ACCELERATION * interval) {
			/* Kollision zu stark */
			explodeBall();
		} else if (floor(sgoBall.pos.x) == sgLevel.finish.x && floor(sgoBall.pos.y) == sgLevel.finish.y) {
			/* Ball auf Zielfeld angekommen */
			loadNewLevel();
		} else {
			gSpeed = add(gSpeed, rebound);

			/* Spingen */
			if (isKeyPressed(' ')) {
				gSpeed = add(gSpeed, scale(JUMP * ACCELERATION * interval, normal));
			}
		}
	}

	/***/

	/* Reibung */
	gSpeed = scale(FRICTION, gSpeed);
	
	/* In den Abgrund gefallen */
	if (sgoBall.pos.z < -10.0f) {
		explodeBall();
	}

  /* Reset durch Benutzer */
	if (isKeyPressed(KEY_ENTER)) {
		explodeBall();
	}
}

/*
 * Ball entprechend Zustand animieren lassen
 */
void updateBall(double interval) {
	if (!gIsBallInPieces) {
		animateBall(interval);
	} else {
		if (updateExplosion(interval, &gSpeed, &sgoBall.pos)) {
			resetBall();
		}
	}
	updateReflection();
}

/*
 * Veranlasst alle notwendigen Abläufe zur Darstellung des
 * jeweils aktuellen Skins
 */
void activateBallShader(void) {
	int x;
	int y;
	Matrix m;

	switch (gBallLayout) {
		case BALL_LAYOUT_DEFAULT:
			setAttributes(1.0f, 0.0f, 0.0f, 0.2f, 0.8f, 0.0f);
			break;
		case BALL_LAYOUT_TEXTURE:
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, gTextureBall);
			break;
		case BALL_LAYOUT_METAL:
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			break;
		case BALL_LAYOUT_GOLFBALL:
			break;
		case BALL_LAYOUT_GOLFBALL_METAL:
			break;
	}

	if (useBallReflection()) {
		for (x = 0; x < 4; x++) {
			for (y = 0; y < 4; y++) {
				if (x < 3 && y < 3) {
					m[x][y] = sgWindowViewport.view[y][x];
				} else {
					m[x][y] = (x == y);
				}
			}
		}

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glMultMatrixf(&m[0][0]);

		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_TEXTURE_CUBE_MAP_EXT);
		glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, gCubeMapBall);
	}

	if (useBallShader()) {
		float reflection;
		
		if (useBallReflection()) {
			reflection = 0.7f;
		} else {
			reflection = 0.0f;

			glEnable(GL_TEXTURE_CUBE_MAP_EXT);
			glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, 0);
		}
		
		glUseProgram(gShaderBall);

		glUniform1i(glGetUniformLocation(gShaderBall, "Environment"), 0);
		glUniform1f(glGetUniformLocation(gShaderBall, "useFog"), useFog());
		glUniform1f(glGetUniformLocation(gShaderBall, "reflection"), reflection);
	}
}

/*
 * Zuruecksetzen der Aktivierungen aus activateBallShader
 */
void deactivateBallShader(void) {
	switch (gBallLayout) {
		case BALL_LAYOUT_DEFAULT:
			break;
		case BALL_LAYOUT_TEXTURE:
			glDisable(GL_TEXTURE_2D);
			break;
		case BALL_LAYOUT_METAL:
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			break;
		case BALL_LAYOUT_GOLFBALL:
			break;
		case BALL_LAYOUT_GOLFBALL_METAL:
			break;
	}

	if (useBallReflection()) {
		glDisable(GL_TEXTURE_CUBE_MAP_EXT);
		
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		
		glMatrixMode(GL_MODELVIEW);
	}
	
	if (useBallShader()) {
		glUseProgram(0);

		if (!useBallReflection()) {
			glDisable(GL_TEXTURE_CUBE_MAP_EXT);
		}
	}
}

/*
 * Zeichnet die Vorschau der Kugel im Menu
 */
void drawMenuBall(void) {
	activateBallShader();

		drawBallObject(useBallShader());

	deactivateBallShader();
}

/*
 * Zeichnet den Ball im Spiel
 */
void drawGameBall(void) {
	int shader = useBallShader();

	glPushMatrix();

	/* Objekt auf Position verschieben */
	glTranslatef(sgoBall.pos.x, sgoBall.pos.y, sgoBall.pos.z);
	/* Objekt rotieren */
	glMultMatrixf(sgoBall.rotMatrix);
	/* Objekt skalieren */
	glScalef(sgoBall.scaleX, sgoBall.scaleY, sgoBall.scaleZ);

	/* Explosion? */
	if (gIsBallInPieces) {
		drawExplosion(shader);
	} else {
		drawBallObject(shader);
	}

	glPopMatrix();
}
