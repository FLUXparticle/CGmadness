#include "field.h"

#include "common.h"
#include "game.h"

#include "light.h"
#include "vector.h"
#include "features.h"

#include "types.h"
#include "debug.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	float r;
	float g;
	float b;
	float a;
} Color4;

static Vector2 gDefaultTexCoord;
static Vector3 gDefaultNormal;
static Color4 gDefaultColor;

static Vector3 gLight;

static Vector2* gTexCoords;
static Color4* gColors;
static unsigned int gVBuffers[4];

static int gCntIndices;
static int* gIndices;

static int gCntSpotlightIndices;
static int* gSpotlightIndices;

/*
 * Setzt die aktuell zu verwendende Farbe
 */
void setColor(float r, float g, float b, float a) {
	gDefaultColor.r = r;
	gDefaultColor.g = g;
	gDefaultColor.b = b;
	gDefaultColor.a = a;
}

/*
 * Setzt die aktuelle Standard Texturkoordinate 
 */
void setTexCoord(float u, float v) {
	gDefaultTexCoord.x = u;
	gDefaultTexCoord.y = v;
}

/*
 * Setzt die aktuelle Standard Normale
 */
void setNormal(float x, float y, float z) {
	gDefaultNormal.x = x;
	gDefaultNormal.y = y;
	gDefaultNormal.z = z;
}

/*
 * Ermittelt den größeren zweier Werte
 */
float max(float a, float b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

/*
 * Fügt einen neuen Vertex in das Vertexarray ein
 * mit den Standardwerten für Farbe, Normale und Textur 
 */
void addVertex(float x, float y, float z) {
	float LdotN;

	gTexCoords[sgCntVertices] = gDefaultTexCoord;
	sgNormals[sgCntVertices] = gDefaultNormal;
	gColors[sgCntVertices] = gDefaultColor;

	sgVertices[sgCntVertices].x = x;
	sgVertices[sgCntVertices].y = y;
	sgVertices[sgCntVertices].z = z;

	LdotN = max(dot(norm(sub(gLight, sgVertices[sgCntVertices])), gDefaultNormal), 0.0f) + 0.4f;

	gColors[sgCntVertices].a = LdotN;

	sgCntVertices++;
}

/*
 * Fügt dem Feld ein neues Quadrat hinzu
 */
void addSquare(Square* square) {
	int i;
	setNormal(square->normal.x, square->normal.y, square->normal.z);

	for (i = 0; i < 4; i++) {
		setTexCoord(square->texcoords[i].x, square->texcoords[i].y);
		addVertex(square->vertices[i].x, square->vertices[i].y, square->vertices[i].z);
	}
}

#define bufferdata(x) glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(*(x)) * sgCntVertices, (x), GL_STATIC_DRAW_ARB);

/*
 * Initialiserungern für das Spielfeld
 */
void initGameField(void) {
	int x, y;
	int i;
	int index = 0;
	int quad = 0;

	Square square;

	/* Arrays anlegen */
  MALLOC(gTexCoords, sgMaxVertices * sizeof(Vector2));
	MALLOC(gColors, sgMaxVertices * sizeof(Color4));

	gLight.x = sgLight[sgGameMainLight].pos[0];
	gLight.y = sgLight[sgGameMainLight].pos[1];
	gLight.z = sgLight[sgGameMainLight].pos[2];

	for (y = 0; y < sgLevel.size.y; y++) {
		for (x = 0; x < sgLevel.size.x; x++) {
			sgIndexVertices[index++] = sgCntVertices;

			/* Default Farbe ändern bei Start- / Zielfeld */	
			if (sgLevel.start.x == x && sgLevel.start.y == y) {
				setColor(0.0f, 1.0f, 0.0f, 1.0f);
			} else if (sgLevel.finish.x == x && sgLevel.finish.y == y) {
				setColor(0.0f, 0.0f, 1.0f, 1.0f);
			} else {
				setColor(1.0f, 1.0f, 1.0f, 1.0f);
			}

			/* Deckel der Blöcke zeichnen */
			getRoofSquare(x, y, &square);

			addSquare(&square);

			sgQuadInShadow[quad++] = 0;

			setColor(1.0f, 1.0f, 1.0f, 1.0f);
			
			/* Seitenwände der Blöcke zeichnen */
			for (i = 0; i < 4; i++) {
				if (getSideSquare(x, y, i, &square)) {
					addSquare(&square);
					sgQuadInShadow[quad++] = 0;
			  } else {
					sgQuadInShadow[quad++] = 1;
				}
			}
		}
	}

	/* Verwendung des Vertexbuffers, wenn vorhanden */
	if (hasVertexbuffer()) {
		glGenBuffersARB(4, gVBuffers);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[0]);
		bufferdata(sgVertices);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[1]);
		bufferdata(sgNormals);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[2]);
		bufferdata(gTexCoords);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[3]);
		bufferdata(gColors);
	}

	printf("sgMaxVertices: %d\n", sgMaxVertices);
	printf("sgCntVertices: %d\n", sgCntVertices);

	/* Indices anlegen */
	gCntIndices = 0;
	MALLOC(gIndices, sgCntVertices * sizeof(int));

	/* Spotlight anlegen */
	if (hasSpotlight()) {
		gCntSpotlightIndices = 0;
		MALLOC(gSpotlightIndices, sgCntVertices * sizeof(int));
	}
}

/*
 * Initialisierungen rückgängig machen,
 * Speicher freigeben
 */
void destroyGameField(void) {
	if (hasSpotlight()) {
  	FREE(gSpotlightIndices);
	}

  FREE(gIndices);
	FREE(gTexCoords);
	FREE(gColors);
}

/*
 * Sielfelddaten aktualisieren
 */
void updateGameField(void) {
	/* Bei eingeschränkter Sicht, nur einen Ausschnitt rendern */	
	if (useFog()) {
		static int mxFog = 0;
		static int myFog = 0;

		int mx = floor(sgCamera.x);
		int my = floor(sgCamera.y);
		
		if (mx != mxFog || my != myFog) {
			int dx;
			int dy;

			gCntIndices = 0;
			for (dx = -52; dx <= 52; dx++ ) {
				for (dy = -52; dy <= 52; dy++) {
					int start;
					int end;
					int q;

					getVertIndex(mx + dx, my + dy, &start, &end);
					
					for (q = start; q < end; q++) {
						gIndices[gCntIndices++] = q;
					}
				}
			}

			mxFog = mx;
			myFog = my;
		}
	}

	if (useSpotlight())	{
		static int mxSpot = 0;
		static int mySpot = 0;
		static int maxSpot = 0;

		int mx = floor(sgLight[sgGameSpotLight].pos[0]);
		int my = floor(sgLight[sgGameSpotLight].pos[1]);
		int max = ceil(sgLight[sgGameSpotLight].pos[2] * tan(sgLight[1].cutoff * PI / 180.0f));

		if (mx != mxSpot || my != mySpot || max != maxSpot) {
			int dx;
			int dy;

			gCntSpotlightIndices = 0;
			for (dx = -max; dx <= max; dx++ ) {
				for (dy = -max; dy <= max; dy++) {
					int start;
					int end;
					int q;

					getVertIndex(mx + dx, my + dy, &start, &end);
					
					for (q = start; q < end; q++) {
						gSpotlightIndices[gCntSpotlightIndices++] = q;
					}
				}
			}

			mxSpot = mx;
			mySpot = my;
			maxSpot = max;
		}
	}
}

/*
 * Komplettes Spielfeldzeichnen, 
 * wenn möglich mit VertexBuffer
 */
void drawGameField(void) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_COLOR_MATERIAL);

	if (hasVertexbuffer()) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[0]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[1]);
		glNormalPointer(GL_FLOAT, 0, NULL);
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[2]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[3]);
		glColorPointer(4, GL_FLOAT, 0, NULL);
	} else {
		glVertexPointer(3, GL_FLOAT, 0, sgVertices);
		
		glNormalPointer(GL_FLOAT, 0, sgNormals);
		
		glTexCoordPointer(2, GL_FLOAT, 0, gTexCoords);
		
		glColorPointer(4, GL_FLOAT, 0, gColors);
	}

	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sgLevel.texture);
	
    /* Bei Nebel nur sichtbare Vertices zeichnen, sonst alle */
		if (useFog()) {
			glDrawElements(GL_QUADS, gCntIndices, GL_UNSIGNED_INT, gIndices);
		} else {
			glDrawArrays(GL_QUADS, 0, sgCntVertices);
		}

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);

	if (hasVertexbuffer()) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_COLOR_MATERIAL);
}

/*
 * Elemente die zum Spotlight gehören zeichnen 
 */
void drawGameFieldSpotlightParts(void) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
 
	glDisable(GL_LIGHTING);

	if (hasVertexbuffer()) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[0]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, gVBuffers[1]);
		glNormalPointer(GL_FLOAT, 0, NULL);
	} else {
		glVertexPointer(3, GL_FLOAT, 0, sgVertices);
		
		glNormalPointer(GL_FLOAT, 0, sgNormals);
	}

		/* Spotlight Vertices zeichnen */
		glDrawElements(GL_QUADS, gCntSpotlightIndices, GL_UNSIGNED_INT, gSpotlightIndices);


	if (hasVertexbuffer()) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	glEnable(GL_LIGHTING);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
