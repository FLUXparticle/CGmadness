#ifndef _common_h_
#define _common_h_

#include "vector.h"

#define SCALE_Z 0.1f

typedef struct {
	int z;
	int dzx;
	int dzy;
} Plate;

typedef struct {
	Vector3 normal;
	Vector2 texcoords[4];
	Vector3 vertices[4];
} Square;

typedef struct {
	int x;
	int y;
} FieldCoord;

typedef struct {
	Plate** field;
	FieldCoord start;
	FieldCoord finish;
	FieldCoord size;
	int texture;
} Level;

extern Vector3 sgCamera;
extern Vector3 sgLookat;

extern Level sgLevel;

extern int sgMaxPlates;
extern int sgMaxQuads;
extern int sgMaxVertices;

extern int sgCntVertices;
extern Vector3* sgVertices;
extern Vector3* sgNormals;
extern int* sgIndexVertices;
extern int* sgQuadInShadow;

extern Vector3 sgForward;
extern Vector3 sgRight;

void destroyCommon(void);

int loadFieldFromFile(char* filename);
int saveFieldToFile(char* filename);

void getRoofSquare(int x, int y, Square* square); 
int getSideSquare(int x, int y, int side, Square* square);

void moveCamera(double interval, Vector3 camera, Vector3 lookat);
 
void getVertIndex(int x, int y, int* start, int* end);

#endif
