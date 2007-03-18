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

#ifndef _graph_h_
#define _graph_h_

#include "list.h"
#include "vector.h"

typedef void (*funcDraw)(void);

typedef struct {
	Vector3 pos;
	
	float scaleX;
	float scaleY;
	float scaleZ;

	float rotMatrix[16];

	float colRed, colGreen, colBlue;
	
	float ambient;
	float diffuse;
	float shininess;

	int texture;
	funcDraw draw;

	int pickName;
  int visible;

	List subObjects;
} Object;

void initObject(Object* obj, funcDraw draw);
void initObjectGroup(Object* obj);
void setObjectPosition2f(Object* obj, float x, float y);
void setObjectPosition3f(Object* obj, float x, float y, float z);

void setObjectColor(Object* obj, float r, float g, float b);
void setObjectGroupColor(Object* obj, float r, float g, float b);

void rotateObject(Object* obj, float angle, float* axis);
void rotateObjectX(Object* obj, float angle);
void rotateObjectY(Object* obj, float angle);
void rotateObjectZ(Object* obj, float angle);

void setObjectScalef(Object* obj, float scale);
void setObjectScale2f(Object* obj, float x, float y);
void setObjectScale3f(Object* obj, float x, float y, float z);

void addSubObject(Object* obj, Object* subObject);
void delSubObject(Object* obj, const Object* subObject);
void clearSubObjects(Object* obj);

void setAttributes(float red, float green, float blue, float ambient, float diffuse, float shininess);
	
void drawObject(const Object* obj);

void pickObject(const Object* obj);

void drawTrianglesVerticesNormals(int count, const float* vertices, const float* normals);

#endif
