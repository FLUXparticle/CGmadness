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

void mouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int cntHits = pick(x, y);

		/* process selectBuffer */
		if (cntHits > 0) {
			GLuint minDepth = 0xffffffff;
			GLuint name = -1;
			GLuint* ptr = gSelectBuffer;
			int i;
			SelectBuffer* buf;

			/* find object which was nearest to camera */
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

void startMouse(void) {
	glSelectBuffer(SELECT_BUFFER_SIZE, gSelectBuffer);
  glutMouseFunc(mouseButton);
}
