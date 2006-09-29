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
void delSubObject(Object* obj, Object* subObject);
void clearSubObjects(Object* obj);

void setAttributes(float red, float green, float blue, float ambient, float diffuse, float shininess);
	
void drawObject(Object* obj);

void pickObject(Object* obj);

#endif
