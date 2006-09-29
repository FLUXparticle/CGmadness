#include "common.h"

#include "callback.h"
#include "texture.h"

#include "debug.h"

#include <GL/glu.h>

#include <stdio.h>
#include <math.h>

#define ANGLESTEP 1.0f
#define ZOOMSTEP 0.5f

Vector3 sgCamera;

int sgCntVertices;

int sgMaxPlates;
int sgMaxQuads;
int sgMaxVertices;

int gFogActive = 0;

Vector3* sgVertices;
Vector3* sgNormals;
int* sgIndexVertices;
int* sgQuadInShadow;

Level sgLevel;

Vector3 sgForward;
Vector3 sgRight;

static int gEdgeX[4] = { 0, 1, 1, 0 };
static int gEdgeY[4] = { 0, 0, 1, 1 };

Vector3 sgLookat = { 0.0f, 0.0f, 0.0f };

/*
 * Bewegt die Kamera und ihren Blickwinkel auf neue Positionen
 */
void moveCamera(double interval, Vector3 camera, Vector3 lookat) {
	Vector3 diff;
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	float error;

	/* Neue Werte bestimmen */
	diff = sub(camera, sgCamera);
	error = len(diff);
	sgCamera = add(sgCamera, scale(5.0f * interval * error, norm(diff)));

	diff = sub(lookat, sgLookat);
	error = len(diff);
	sgLookat = add(sgLookat, scale(5.0f * interval * error, norm(diff)));

	/* Kamera neu setzen */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
			sgCamera.x,  sgCamera.y, sgCamera.z,
			sgLookat.x, sgLookat.y, sgLookat.z,
			up.x, up.y, up.z
			);

	glGetFloatv(GL_MODELVIEW_MATRIX, &sgWindowViewport.view[0][0]);
}

/*
 * Ermittelt die Höhe einer bestimmten Ecke eines Feldes 
 */
int getFieldEdgeHeight(int x, int y, int edge) {
	if (x >= 0 && y >= 0 && x < sgLevel.size.x && y < sgLevel.size.y) {
		Plate* p = &sgLevel.field[x][y];

		return p->z + (gEdgeX[edge] * 2 - 1) * p->dzx + (gEdgeY[edge] * 2 - 1) * p->dzy;
	}

	/* Werte außerhalb des Bereichs */
	return -5;
}

/*
 * Ermittelt die Vertexdaten eines Deckels eines Feldes
 */
void getRoofSquare(int x, int y, Square* square) {
	int i;
	Plate* p = &sgLevel.field[x][y];
	int dzx = p->dzx;
	int	dzy = p->dzy;
	
	Vector3 ex;
	Vector3 ey;

	ex.x = 0.5f;
	ex.y = 0.0f;
	ex.z = SCALE_Z * dzx;
	
	ey.x = 0.0f;
	ey.y = 0.5f;
	ey.z = SCALE_Z * dzy;
	
	square->normal = norm(cross(ex, ey));

	for (i = 0; i < 4; i++) {
		square->vertices[i].x = x + gEdgeX[i];
		square->vertices[i].y = y + gEdgeY[i];
		square->vertices[i].z = SCALE_Z * getFieldEdgeHeight(x, y, i);
	}

	/* Deckel der BlÃ¶cke zeichnen */
	for (i = 0; i < 4; i++) {
		square->texcoords[i].x = gEdgeX[i];
		square->texcoords[i].y = gEdgeY[i];
	}
}

/*
 * Ermittelt die Vertexdaten einer übergebenen Seitenfläche eines Spielfeldes
 */
int getSideSquare(int x, int y, int side, Square* square) {
	Square roofsquare;
	int j = (side + 1) % 4;
	int dx;
	int dy;

	int i2 = (side + 3) % 4;
	int j2 = (side + 2) % 4;

	getRoofSquare(x, y, &roofsquare);

	dx = roofsquare.vertices[j].y - roofsquare.vertices[side].y;
	dy = -(roofsquare.vertices[j].x - roofsquare.vertices[side].x);

	if (getFieldEdgeHeight(x, y, side) <= getFieldEdgeHeight(x + dx, y + dy, i2) && getFieldEdgeHeight(x, y, j) <= getFieldEdgeHeight(x + dx, y + dy, j2))  {
		return 0;
	}

	square->normal.x = dx;
	square->normal.y = dy;
	square->normal.z = 0.0f;
	
	 
	square->texcoords[0].x = 0.0f;
	square->texcoords[0].y = 0.0f;
	
	square->vertices[0] = roofsquare.vertices[side];


	square->texcoords[1].x = 0.0f;
	square->texcoords[1].y = roofsquare.vertices[side].z + 0.5f;	
	
	square->vertices[1].x = roofsquare.vertices[side].x;
	square->vertices[1].y = roofsquare.vertices[side].y;
	square->vertices[1].z = -0.5f;
	

	square->texcoords[2].x = 1.0f;
	square->texcoords[2].y = roofsquare.vertices[j].z + 0.5f;

	square->vertices[2].x = roofsquare.vertices[j].x;
	square->vertices[2].y = roofsquare.vertices[j].y;
	square->vertices[2].z = -0.5f;

	square->texcoords[3].x = 1.0f;
	square->texcoords[3].y = 0.0f;
	
	square->vertices[3] = roofsquare.vertices[j];

	return 1;
}

/*
 * Bestimmt die Indices an denen die Vertices
 * des gewählten Feldes im Vertexarray anzutreffen sind
 */
void getVertIndex(int x, int y, int* start, int* end) {
	if (x >= 0 && x < sgLevel.size.x && y >= 0 && y < sgLevel.size.y) {
		int index = y * sgLevel.size.x + x;
		*start = sgIndexVertices[index];
		index++;
		if (index < sgMaxPlates) {
			*end = sgIndexVertices[index];
		} else {
			*end = sgCntVertices;
		}
	} else {
		*start = 0;
		*end = 0;
	}
}

/*
 * Initialisierungen Rückgängig machen,
 * Speicher freigeben
 */
void destroyCommon(void) {
	FREE(sgLevel.field[0]);
	FREE(sgLevel.field);
	FREE(sgVertices);
	FREE(sgNormals);
	FREE(sgIndexVertices);
  FREE(sgQuadInShadow);

	sgLevel.size.x = -1;
	sgLevel.size.y = -1;
}

/*
 * Liest eine Leveldatei ein und füllt die Arrays entsprechend
 */
int loadFieldFromFile(char* filename) {
	FILE* file = fopen(filename, "rt");
	int x,y;
	int fileX, fileY;

	if (!file) return 0;

	sgLevel.texture = loadTexture("data/plate.tga", 1);

	/* Levelattribute einlesen */
	fscanf(file, "%d %d", &sgLevel.start.x, &sgLevel.start.y);
	fscanf(file, "%d %d", &sgLevel.finish.x, &sgLevel.finish.y);
  fscanf(file, "%d %d", &fileX, &fileY);

	/* Wenn keine Grösse angegeben, Größe aus Datei verwenden */
	if (sgLevel.size.x < 0 || sgLevel.size.y < 0) {
		sgLevel.size.x = fileX;
		sgLevel.size.y = fileY;
	}

	/* Daten asu der Datei in die erzeugten Arrays füllen */
	MALLOC(sgLevel.field, sgLevel.size.x * sizeof(Plate*));
	MALLOC(sgLevel.field[0], sgLevel.size.x * sgLevel.size.y * sizeof(Plate));
	for (x = 1; x < sgLevel.size.x; x++) {
		sgLevel.field[x] = sgLevel.field[0] + x * sgLevel.size.y;
	}


	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			Plate* p = &sgLevel.field[x][y];
			if (x < fileX && y < fileY) {
				fscanf(file, "%d %d %d", &p->z, &p->dzx, &p->dzy);
			} else {
				p->z = 0;
				p->dzx = 0;
				p->dzy = 0;
			}
		}

		/* Verkleinerung eines Levels */
		if (fileY > sgLevel.size.y) {
			int dummyZ;
			int dummyDZX;
			int dummyDZY;

			for (y = sgLevel.size.y; y < fileY; y++) {
				fscanf(file, "%d %d %d", &dummyZ, &dummyDZX, &dummyDZY);
			}
		}
	}

	fclose(file);
	
  /* Variablen und Datenstrukturn initialisieren */
	sgMaxPlates = sgLevel.size.x * sgLevel.size.y;
	sgMaxQuads = 5 * sgMaxPlates;
	sgMaxVertices = 4 * sgMaxQuads;
	
	sgCntVertices = 0;
	MALLOC(sgVertices, sgMaxVertices * sizeof(Vector3));
	MALLOC(sgNormals, sgMaxVertices * sizeof(Vector3));
	MALLOC(sgIndexVertices, sgMaxPlates * sizeof(int));
	MALLOC(sgQuadInShadow, sgMaxQuads * sizeof(int));
	
	return 1;
}

/*
 * Aktuell geladenes Level in eine Leveldatei schreiben
 */
int saveFieldToFile(char* filename) {
	FILE* file = fopen(filename, "wt");
	int x, y;

	if (!file) return 0;

	/* Atribute schreiben */
	fprintf(file, "%d %d\n", sgLevel.start.x, sgLevel.start.y);
	fprintf(file, "%d %d\n", sgLevel.finish.x, sgLevel.finish.y);
	fprintf(file, "%d %d\n", sgLevel.size.x, sgLevel.size.y);

	/* Daten schreiben */
	for (x = 0; x < sgLevel.size.x; x++) {
		for (y = 0; y < sgLevel.size.y; y++) {
			Plate* p = &sgLevel.field[x][y];
			fprintf(file, "%d %d %d\n", p->z, p->dzx, p->dzy);
		}
	}

	fclose(file);

	return 1;
}
