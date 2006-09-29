#include "features.h"

#include <GL/glew.h>

#include <stdio.h>

/*
 * Dieses Modul prüft ob bestimmte Features auf dem Rechner unterstützt werden und
 * bietet Funktionen an, damit das Programm leicht auf fehlende Features reagieren kann.
 */

static int sgFramebufferAvailable;
static int sgVertexbufferAvailable;
static int sgTwoSideStencilAvailable;
static int sgShaderAvailable;

static int gUseShadows = 0;
static int gUseFog = 0;

/* Fragt vorhandene Extensions ab und schaltet je nach Verfügbarkeit
 * grafische Features hinzu oder nicht
 */
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
}

/* Umgebungsvariablen abfragen */
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

void setShadows(int use) {
	gUseShadows = use;
}

void setFog(int use) {
	gUseFog = use;

	if (use) {
		glEnable(GL_FOG);
	} else {
		glDisable(GL_FOG);
	}
}

int useFog(void) {
	return gUseFog;
}

int useShadows(void) {
	return gUseShadows;
}

int hasSpotlight(void) {
	return hasShader();
}

int useSpotlight(void) {
	return hasSpotlight();
}

