#include "text.h"

#include "dataBigAlpha.h"
#include "dataSmallAlpha.h"
#include "dataDigits.h"

#include "graph.h"

#include "debug.h"

#include <GL/glut.h>

#define SCALE 0.1f

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
void addChar(Object* obj, float* x, funcDraw draw, float width) {
	Object* oChar;

	MALLOC(oChar, sizeof(Object));

	initObject(oChar, draw);

	setObjectScalef(oChar, SCALE);

	oChar->pos.x = *x;
	*x += width * SCALE;

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
			addChar(obj, &x, drawBigAlpha[i], widthBigAlpha[i]);
		}
		if (*s >= 'a' && *s <= 'z') {
			int i = *s - 'a';
			addChar(obj, &x, drawSmallAlpha[i], widthSmallAlpha[i]);
		}
		if (*s >= '0' && *s <= '9') {
			int i = *s - '0';
			addChar(obj, &x, drawDigits[i], widthDigits[i]);
		}
		if (*s == ' ') {
			x += 0.5f;
		}
	}

	return x;
}

