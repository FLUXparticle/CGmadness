#include "game.h"

#include "common.h"

#include "graph.h"
#include "light.h"
#include "objects.h"
#include "ball.h"
#include "shadows.h"
#include "field.h"
#include "menu.h"
#include "files.h"
#include "features.h"
#include "keyboard.h"

#include "types.h"
#include "debug.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int sgRenderPass = 8;

int sgGameMainLight;
int sgGameSpotLight;

static int gIsGameRunning;

static Object goMenu;

/*
 * Hin- und herschalten zwischen Spiel und Menu
 */
void pauseGame(void) {
	showMenu(1);
	gIsGameRunning = 0;
	goMenu.visible = 1;
}

void resumeGame(void) {
	glutSetCursor(GLUT_CURSOR_NONE);
	gIsGameRunning = 1;
	goMenu.visible = 0;
}

/*
 * Update Routine für die Spielkamera
 */
void updateGameCamera(double interval, Vector3 ball) {
	Vector3 diff;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	static float distance = 5.0f;
	static float height = 2.0f;
	static Vector3 dest = { 0.0f, 0.0f, 0.0f };

  /* Kamera per Keyboard anpassen */

	/* Zoom */
	if (isKeyPressed('f')) distance += 0.1f;
	if (isKeyPressed('r') && distance > 0.5) distance -= 0.1f;

	/* Kamera um den Ball rotieren */
	if (isKeyPressed('a')) dest = sub(dest, scale(0.1f, sgRight));
	if (isKeyPressed('d')) dest = add(dest, scale(0.1f, sgRight));

	/* Hoehenaenderung */
	if (isKeyPressed('w')) height += 0.1f;
	if (isKeyPressed('s')) height -= 0.1f;

	dest.z = ball.z + height;

	diff = sub(dest, ball);
	diff = scale(distance, norm(diff));

	dest = add(ball, diff);

	moveCamera(interval, dest, ball);

	diff = sub(sgLookat, sgCamera);
	diff.z = 0.0f;
	sgForward = norm(diff);
	sgRight = norm(cross(sgForward, up));
}

/*
 * Aktualisieren der Szene / Reaktion auf Benutzereingaben 
 */
void updateGame(double interval) {
  /* Spiel animieren lassen */
	if (gIsGameRunning) {
		int i = 0;
		
		/* Ins Menü gehen */
		if (wasKeyPressed(KEY_ESC)) {
			pauseGame();
		}

		/* RenderPass setzen */
		for (i = 0; i < 9; i++) {
			if (wasKeyPressed('1' + i)) {
				sgRenderPass = i;
			}
		}

		/* Features schalten */
		if (wasFunctionPressed(1)) {
			toggleLight(sgGameSpotLight);
		}
		if (wasFunctionPressed(2)) {
			setShadows(!useShadows());
		}

		updateBall(interval);

		sgLight[sgGameSpotLight].pos[0] = sgoBall.pos.x;
		sgLight[sgGameSpotLight].pos[1] = sgoBall.pos.y;
		sgLight[sgGameSpotLight].pos[2] = sgoBall.pos.z;

		updateShadows();

		updateGameCamera(interval, sgoBall.pos);
	} else {
		/* Menu anzeigen */
		Vector3 camera;
		Vector3 lookat = goMenu.pos;
		
		lookat.x = (sgLevel.size.x + 1.0f) / 2;
		lookat.z += 5.0f;

		camera = lookat;
		camera.y -= 10.0f;
		camera.z += 2.0f;

		updateMenu(interval);

		moveCamera(interval, camera, lookat);
	}
	
	updateGameField();
}

/*
 * Zeichenroutinen für das Spiel
 */
void drawGameContent(int reflection) {
	/*
	 * Alpha-Blending scheint im Texture-Framebuffer nicht zu funkionieren, daher wird die Umgebung 
	 * in der Reflektion gleich mit Lighting gerendert. Dadurch ist der Schatten in der
	 * Reflektion allerdings etwas dunkler.
	 */
	drawGameField();
	drawShadows(!reflection);
}

void drawGame(void) {
	drawGameContent(0);
}

void drawGameReflection(void) {
	drawGameContent(1);
}

/*
 * Führt alle notwendigen Initialierungen für das Laden eines neuen Levels aus
 */
void initLevel(char* filename) {
	/* Level laden */
	loadFieldFromFile(filename);
	
	/* Spielfeld */
	initGameField();

	initShadowVolumes();

	/* Kamera setzen */

	sgCamera.x = -2.0f;
	sgCamera.y = -3.0f;
	sgCamera.z =  4.0f;

	setObjectPosition3f(&goMenu, (sgLevel.size.x + 1.0f) / 2, -10.0f, 0.0f);

	sgLight[sgGameSpotLight].pos[0] = sgLevel.start.x + 0.5f;
	sgLight[sgGameSpotLight].pos[1] = sgLevel.start.y + 0.5f;
	
	updateGameCamera(0.0, goMenu.pos);

	showMenu(0);
	gIsGameRunning = 0;
	goMenu.visible = 1;

	resetBall();
}

/*
 * Level entladen
 */
void destroyLevel(void) {
	destroyGameField();
	destroyCommon();
	destroyShadowVolumes();
}

/*
 * Nächsten Levelnamen aus der Leveldatei bestimmen
 */
char* getNextLevelName(void) {
	static char* allLevels = NULL;
	static char* nextLevel = NULL;
	static char* curLevel = NULL;

	if (!allLevels) {
		allLevels = textFileRead("levels/default.lev");
		nextLevel = allLevels;
	}

	curLevel = nextLevel;

	nextLevel = strchr(curLevel, '\n');

	if (nextLevel) {
		*nextLevel = '\0';
		nextLevel++;
		return curLevel;
	} else {
		return NULL;
	}
}

/*
 * Entlädt das alte Level und lädt das nächste aus der Leveldatei
 */
void loadNewLevel(void) {
	char* nextLevelname = getNextLevelName();

	destroyLevel();

	/* Naechstes Level bestimmen */	
	if (nextLevelname) {
		initLevel(nextLevelname);
	} else {
		exit(0);
	}
}

/*
 * Initialisiert den Nebel
 */
void initFog(void) {
	float value;
	int ivalue;

	ivalue = GL_LINEAR;
	glFogiv(GL_FOG_MODE, &ivalue);
	value = FOG_START;
	glFogfv(GL_FOG_START, &value);
	value = FOG_END;
	glFogfv(GL_FOG_END, &value);
}

/* Szenengraph aufbauen */
void initGame(char* filename) {
	static Object oGame;
	static Object oLevel;
	
	/* Objektdaten initialisieren */
	initObjects();

	/* Lichtquellen setzen */
	glColor3f(1.0f, 1.0f, 1.0f);
	sgGameMainLight = addPointLight(-200.0f, -200.0f, 100.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
  sgGameSpotLight = addSpotLight(0.0f, 0.0f, 8.0f, 0.0f, 0.0f, -1.0f, 10.0f, 50.0f);

	initFog();
	
	/* Ball */
	initBall();

	/* Menu (nach Ball) */
	initMenu(&goMenu);

	/* Aktivieren */
	goMenu.visible = 0;
	gIsGameRunning = 1;

	/* Level (nach Menu) */
 	initLevel(getNextLevelName());

	/* Scene konfigurieren */
	initObjectGroup(&oGame);
	sgWindowViewport.world = &oGame;
	setUpdateFunc(updateGame);

	/* Spiel */
	initObject(&oLevel, drawGame);
	addSubObject(&oGame, &oLevel);

	/* Das Menü muss als letztes in die Welt eingefügt werden */
	addSubObject(&oGame, &goMenu);
}

