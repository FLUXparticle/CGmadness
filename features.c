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

#include "features.h"

#include "shader.h"

#include <stdio.h>

/*
 * this module checks which opengl features are available
 */

static int sgFramebufferAvailable;
static int sgVertexbufferAvailable;
static int sgTwoSideStencilAvailable;
static int sgShaderAvailable;

static int gUseShadows = 0;
static int gUseSpotlight = 0;

GLhandleARB sgSpotlightShader = 0;

void initFeatures(int argc, char* argv[]) {
	sgShaderAvailable = 1;
	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
		printf("No OpenGL 2.0 shader available :(\n");
		sgShaderAvailable = 0;
	}

	sgFramebufferAvailable = 1;
	if (!GLEW_EXT_framebuffer_object || !GLEW_EXT_packed_depth_stencil) {
		printf("No framebuffer object available :(\n");
		sgFramebufferAvailable = 0;
	}

	sgVertexbufferAvailable = 1;
	if (!GLEW_ARB_vertex_buffer_object) {
		printf("No vertex buffer object available :(\n");
		sgVertexbufferAvailable = 0;
	}
	
	sgTwoSideStencilAvailable = 1;
	if (!GLEW_EXT_stencil_two_side || !GLEW_EXT_stencil_wrap) {
		printf("No two side stencil available :(\n");
		sgTwoSideStencilAvailable = 0;
	}
	
	if (hasShader()) {
		sgSpotlightShader = makeShader("spotlight.vert", "spotlight.frag");
		if (sgSpotlightShader) {
			printf("Spotlight-Shader ready :-)\n");
			setSpotlight(1);
		}
	}
}

/* has...? */

int hasShader(void) {
	return sgShaderAvailable;
}

int hasFramebuffer(void) {
	return sgFramebufferAvailable;
}

int hasVertexbuffer(void) {
	return sgVertexbufferAvailable;
}

int hasTwoSideStencil(void) {
	return sgTwoSideStencilAvailable;
}

int hasSpotlight(void) {
	return hasShader();
}

/* set */

void setShadows(int use) {
	gUseShadows = use;
}

void setSpotlight(int use) {
	gUseSpotlight = use;
}

/* use...? */

int useShadows(void) {
	return gUseShadows;
}

int useSpotlight(void) {
	return hasSpotlight() && gUseSpotlight;
}

