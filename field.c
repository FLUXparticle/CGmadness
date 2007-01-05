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

#define FOG_REGION 52

#define WITHOUT_DEPTH_TEST 0
#define WITH_STENCIL_TEST 0
#define TWO_PASS 0

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

float min(float a, float b) {
	return (a < b) ? a : b;
}

float max(float a, float b) {
	return (a > b) ? a : b;
}

/*
 * F�gt einen neuen Vertex in das Vertexarray ein
 * mit den Standardwerten f�r Farbe, Normale und Textur 
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
 * F�gt dem Feld ein neues Quadrat hinzu
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
 * Initialiserungern f�r das Spielfeld
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

			/* Default Farbe �ndern bei Start- / Zielfeld */	
			if (sgLevel.start.x == x && sgLevel.start.y == y) {
				setColor(0.0f, 1.0f, 0.0f, 1.0f);
			} else if (sgLevel.finish.x == x && sgLevel.finish.y == y) {
				setColor(0.0f, 0.0f, 1.0f, 1.0f);
			} else {
				setColor(1.0f, 1.0f, 1.0f, 1.0f);
			}

			/* Deckel der Bl�cke zeichnen */
			getRoofSquare(x, y, &square);

			addSquare(&square);

			sgQuadInShadow[quad++] = 0;

			setColor(1.0f, 1.0f, 1.0f, 1.0f);
			
			/* Seitenw�nde der Bl�cke zeichnen */
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
 * Initialisierungen r�ckg�ngig machen,
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

void bsp(int startX, int startY, int sizeX, int sizeY, int viewX, int viewY) {
	if (sizeX == 0 || sizeY == 0) {
		return;
	} else if (sizeX == 1 && sizeY == 1) {
		int start;
		int end;
		int q;

		getVertIndex(startX, startY, &start, &end);
		
		for (q = start; q < end; q++) {
			gIndices[gCntIndices++] = q;
		}
	} else {
		int startX1 = startX;
		int startY1 = startY;
		int sizeX1 = sizeX;
		int sizeY1 = sizeY;

		int startX2 = startX;
		int startY2 = startY;
		int sizeX2 = sizeX;
		int sizeY2 = sizeY;

		int side;

		if (sizeX > sizeY) {
			sizeX1 = sizeX / 2;
			sizeX2 = sizeX - sizeX1;
			startX2 = startX1 + sizeX1;
			side = viewX < startX2;
		} else {
			sizeY1 = sizeY / 2;
			sizeY2 = sizeY - sizeY1;
			startY2 = startY1 + sizeY1;
			side = viewY < startY2;
		}

		if (side) {
			bsp(startX1, startY1, sizeX1, sizeY1, viewX, viewY);
			bsp(startX2, startY2, sizeX2, sizeY2, viewX, viewY);
		} else {
			bsp(startX2, startY2, sizeX2, sizeY2, viewX, viewY);
			bsp(startX1, startY1, sizeX1, sizeY1, viewX, viewY);
		}
	}
}

/*
 * Sielfelddaten aktualisieren
 */
void updateGameField(void) {
	static int mxFog = 0;
	static int myFog = 0;

	int mx = floor(sgCamera.x);
	int my = floor(sgCamera.y);
	
	if (mx != mxFog || my != myFog) {
		/* Bei eingeschr�nkter Sicht, nur einen Ausschnitt rendern */	
		if (useFog()) {
			int x1 = max(mx - FOG_REGION, 0);
			int y1 = max(my - FOG_REGION, 0);
			int x2 = min(mx + FOG_REGION + 1, sgLevel.size.x);
			int y2 = min(my + FOG_REGION + 1, sgLevel.size.y);

			gCntIndices = 0;
			bsp(x1, y1, x2 - x1, y2 - y1, mx, my);
		} else {
			gCntIndices = 0;
			bsp(0, 0, sgLevel.size.x, sgLevel.size.y, mx, my);
		}

		mxFog = mx;
		myFog = my;
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
 * wenn m�glich mit VertexBuffer
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
	glDrawElements(GL_QUADS, gCntIndices, GL_UNSIGNED_INT, gIndices);
	
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
 * Elemente die zum Spotlight geh�ren zeichnen 
 */
void drawGameFieldSpotlightParts(void) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
 
	glDisable(GL_LIGHTING);
	glDepthFunc(GL_EQUAL);
	glDepthMask(0);

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
	glDepthFunc(GL_LESS);
	glDepthMask(1);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
