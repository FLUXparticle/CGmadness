#include "mouse.h"

#include "callback.h"
#include "pick.h"

#include <GL/glut.h>


#define SELECT_BUFFER_SIZE 512

static GLuint gSelectBuffer[SELECT_BUFFER_SIZE];

typedef struct {
	GLuint stackSize;
	GLuint minDepth;
	GLuint maxDepth;
	GLuint stackBottom;
} SelectBuffer;

/*
 * Mouse-Click Callback:
 * Ermittelt, ob ein Objekt geklickt wurde und
 * löst entpsrechende Aktionen aus
 */
void mouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int cntHits = pick(x, y);

		/* Select-Buffer auswerten */
		if (cntHits > 0) {
			GLuint minDepth = 0xffffffff;
			GLuint name = -1;
			GLuint* ptr = gSelectBuffer;
			int i;
			SelectBuffer* buf;

			/* Den Namen des Objektes herausfinden, dass am nächsten an der Kamera gepickt wurde. */
			for (i = 0; i < cntHits; i++) {
				buf = (SelectBuffer*) ptr;

				if (buf->minDepth < minDepth) {
					minDepth = buf->minDepth;
					if (buf->stackSize > 0) {
						name = buf->stackBottom;
					} else {
						name = -1;
					}
				}

				ptr += 3 + buf->stackSize;
			}

			doPick(name);
		}
	}
}

/*
 * Registriert Mouse-Button-Callback (wird ausgefuehrt, wenn eine Maustaste
 * gedrueckt oder losgelassen wird) 
 */
void startMouse(void) {
	/* SelectBuffer initialisieren */
	glSelectBuffer(SELECT_BUFFER_SIZE, gSelectBuffer);
	
  glutMouseFunc(mouseButton);
}
