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

#include "graph.h"

#include <GL/gl.h>

#include <stdio.h>

void initObject(Object* obj, funcDraw draw) {
	int i;

	obj->pos.x = 0.0f;
	obj->pos.y = 0.0f;
	obj->pos.z = 0.0f;

	obj->scaleX = 1.0f;
	obj->scaleY = 1.0f;
	obj->scaleZ = 1.0f;

	for (i = 0; i < 16; i++) {
		obj->rotMatrix[i] = ((i % 4) == (i / 4)) ? 1.0f : 0.0f;
	}

	obj->colRed = 1.0f;
	obj->colGreen = 1.0f;
	obj->colBlue = 1.0f;

	obj->ambient = 0.2f;
	obj->diffuse = 0.8f;
	obj->shininess = 0.0f;
	
	obj->texture = -1;

	obj->draw = draw;

	obj->pickName = -1;

	obj->visible = 1;
	
	obj->subObjects = NULL;
}

void initObjectGroup(Object* obj) {
	initObject(obj, NULL);
}

/*
 * some helper functions
 */

void setObjectPosition2f(Object* obj, float x, float y) {
	obj->pos.x = x;
	obj->pos.y = y;
}

void setObjectPosition3f(Object* obj, float x, float y, float z) {
	obj->pos.x = x;
	obj->pos.y = y;
	obj->pos.z = z;
}

void setObjectColor(Object* obj, float r, float g, float b) {
	obj->colRed = r;
	obj->colGreen = g;
	obj->colBlue = b;
}

void setObjectGroupColor(Object* obj, float r, float g, float b) {
	List run;

	setObjectColor(obj, r, g, b);

	for (run = obj->subObjects; run; run = run->next) {
		Object* obj = run->info;
		setObjectGroupColor(obj, r, g, b);
	}
}

void rotateObject(Object* obj, float angle, float* axis) {
	int matrixMode;

	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();

	glRotatef(angle, axis[0], axis[1], axis[2]);

	glMultMatrixf(obj->rotMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, obj->rotMatrix);
	glPopMatrix();

	glMatrixMode(matrixMode);
}

void rotateObjectX(Object* obj, float angle) {
	float axis[] = { 1.0f, 0.0f, 0.0f };
	rotateObject(obj, angle, axis);
}

void rotateObjectY(Object* obj, float angle) {
	float axis[] = { 0.0f, 1.0f, 0.0f };
	rotateObject(obj, angle, axis);
}

void rotateObjectZ(Object* obj, float angle) {
	float axis[] = { 0.0f, 0.0f, 1.0f };
	rotateObject(obj, angle, axis);
}

void setObjectScalef(Object* obj, float scale) {
	obj->scaleX = scale;
	obj->scaleY = scale;
	obj->scaleZ = scale;
}

void setObjectScale2f(Object* obj, float x, float y) {
	obj->scaleX = x;
	obj->scaleY = y;
}

void setObjectScale3f(Object* obj, float x, float y, float z) {
	obj->scaleX = x;
	obj->scaleY = y;
	obj->scaleZ = z;
}

void addSubObject(Object* obj, Object* subObject) {
	obj->subObjects = appendElement(obj->subObjects, subObject);
}

void delSubObject(Object* obj, Object* subObject) {
	obj->subObjects = removeElement(obj->subObjects, subObject);
}

void clearSubObjects(Object* obj) {
	obj->subObjects = removeAll(obj->subObjects);
}

void setAttributes(float red, float green, float blue, float ambient, float diffuse, float shininess) {
	float matAmbient[4];
	float matDiffuse[4];
	float matSpecular[4];
	float specular;
	
	matAmbient[0] = ambient * red;
	matAmbient[1] = ambient * green;
	matAmbient[2] = ambient * blue;
	matAmbient[3] = 1.0f;

	matDiffuse[0] = diffuse * red;
	matDiffuse[1] = diffuse * green;
	matDiffuse[2] = diffuse * blue;
	matDiffuse[3] = 1.0f;

	specular = (shininess > 0.0f) ? 1.0f : 0.0f;
	matSpecular[0] = specular;
	matSpecular[1] = specular;
	matSpecular[2] = specular;
	matSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void pickList(List list) {
	List run;

	for (run = list; run; run = run->next) {
		Object* obj = run->info;
		pickObject(obj);
	}
}

void pickObject(Object* obj) {
	int isNameOnStack;
	
	if (!obj->visible) {
		return;
	}

	glPushMatrix();
	
	if (obj->pickName >= 0) {
		glPushName(obj->pickName);
		isNameOnStack = 1;
	} else {
		int stackDepth;
		glGetIntegerv(GL_NAME_STACK_DEPTH, &stackDepth);
		isNameOnStack = (stackDepth > 0);
	}

	glTranslatef(obj->pos.x, obj->pos.y, obj->pos.z);
	glMultMatrixf(obj->rotMatrix);
	glScalef(obj->scaleX, obj->scaleY, obj->scaleZ);

	if (obj->draw && isNameOnStack) {
		obj->draw();
	}

	/* call draw functions of sub objects */
	pickList(obj->subObjects);
	
	if (obj->pickName >= 0) {
		glPopName();
	}

	glPopMatrix();
}

void drawList(List list) {
	List run;

	for (run = list; run; run = run->next) {
		Object* obj = run->info;
		drawObject(obj);
	}
}

void drawObject(Object* obj) {
	if (!obj->visible) {
		return;
	}

	glPushMatrix();
	
	glTranslatef(obj->pos.x, obj->pos.y, obj->pos.z);
	glMultMatrixf(obj->rotMatrix);
	glScalef(obj->scaleX, obj->scaleY, obj->scaleZ);

	if (obj->draw) {
		int alpha = 0;

		setAttributes(obj->colRed, obj->colGreen, obj->colBlue, obj->ambient, obj->diffuse, obj->shininess);

		if (obj->texture > 0) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, obj->texture);

			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE, &alpha);
		}
		
		if (alpha > 0) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		
		/* call draw function */
		obj->draw();

		if (alpha > 0) {
			glDisable(GL_BLEND);
		}

		if (obj->texture) {
			glDisable(GL_TEXTURE_2D);
		}
	}

	drawList(obj->subObjects);

	glPopMatrix();
}

void drawTrianglesVerticesNormals(int count, float* vertices, float* normals)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);

	glDrawArrays(GL_TRIANGLES, 0, count);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
