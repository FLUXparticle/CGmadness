#include "font.h"

#include "vector.h"

#include <GL/gl.h>

#include <string.h>

/*
 * Zeichnet alle Vertices aus der übergebenen Liste
 */
void drawVertexList(VertexList* vertexList) {
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
	Vector3 normal;
	int t;

	glBegin(GL_TRIANGLES);
	for (t = 0; t < vertexList->cntTriangles; t++) {
		memcpy(&p1, &vertexList->vertices[(t * 3 + 0) * 3], sizeof(Vector3));
		memcpy(&p2, &vertexList->vertices[(t * 3 + 1) * 3], sizeof(Vector3));
		memcpy(&p3, &vertexList->vertices[(t * 3 + 2) * 3], sizeof(Vector3));

		normal = cross(sub(p2, p1), sub(p3, p1));

		glNormal3fv(&normal.x);
		glVertex3fv(&p1.x);
		glVertex3fv(&p2.x);
		glVertex3fv(&p3.x);
	}
	glEnd();
	
	/* So war es gemeint. Keine Online Berechnung der Normalen */
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, vertexList->vertices);

		glDrawArrays(GL_TRIANGLES, 0, 3 * vertexList->cntVertices);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	*/
}
