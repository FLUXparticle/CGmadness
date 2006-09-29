#include "text.h"

#include "dataBigAlpha.h"
#include "dataSmallAlpha.h"
#include "dataDigits.h"

#include "graph.h"
#include "font.h"

#include "debug.h"

#include <GL/glut.h>

/* Gibt einen String mit Hilfe von Glut aus */
void drawBitmapText(char *str) {
	for (; *str; str++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
	}
}

/*
 * Fügt einen Buchstaben zu einer ObjectGroup hinzu.
 * Es müssen die Renderfunktion und die Vertexliste übergeben werden, damit berechnet werden kann, wie breit der Buchstabe ist.
 */
void addChar(Object* obj, float* x, funcDraw draw, VertexList* vertexlist) {
	Object* oChar;
	int i;
	float minx;
	float maxx;

	MALLOC(oChar, sizeof(Object));

	initObject(oChar, draw);

	oChar->scaleZ = 0.1f;

	minx = vertexlist->vertices[0];
	maxx = vertexlist->vertices[0];

	for (i = 1; i < 3 * vertexlist->cntTriangles; i++) {
		float x = vertexlist->vertices[i * 3];
		if (x < minx) {
			minx = x;
		}
		if (x > maxx) {
			maxx = x;
		}
	}

	oChar->pos.x = *x - minx;
	*x += 1.3f * (maxx - minx);
	
	addSubObject(obj, oChar);
}

/*
 * Erzeugt ein 3D-Text Objekt
 */
float makeTextObject(Object* obj, char* text) {
	char* s;
	float x = 0;
	
	initObjectGroup(obj);

	for (s = text; *s; s++) {
		if (*s >= 'A' && *s <= 'Z') {
			int i = *s - 'A';
			addChar(obj, &x, drawBigAlpha[i], &vlBigAlpha[i]);
		}
		if (*s >= 'a' && *s <= 'z') {
			int i = *s - 'a';
			addChar(obj, &x, drawSmallAlpha[i], &vlSmallAlpha[i]);
		}
		if (*s >= '0' && *s <= '9') {
			int i = *s - '0';
			addChar(obj, &x, drawDigits[i], &vlDigits[i]);
		}
		if (*s == ' ') {
			x += 0.5f;
		}
	}

	return x;
}

