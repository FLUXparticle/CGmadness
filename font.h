#ifndef _font_h_
#define _font_h_

typedef struct {
	int cntTriangles;
	float* vertices;
} VertexList;

void drawVertexList(VertexList* vertexList);

#endif
