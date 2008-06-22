/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmail.com>
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
 */

#include "features.h"

#include "shader.h"

#include <GL/glew.h>

#include <stdio.h>
#include <string.h>

/*
 * this module checks which opengl features are available
 */

#define NO_SHADER "--no-shader"
#define NO_FRAMEBUFFER "--no-framebuffer"
#define NO_VBO "--no-vbo"

static int gFramebufferAvailable;
static int gVertexbufferAvailable;
static int gTwoSideStencilAvailable;
static int gShaderAvailable;

static int gUseBallShadow = 0;
static int gUseReflection = 0;

unsigned int sgBallShadowShader = 0;
unsigned int sgGolfballShader = 0;

void usageFeatures(void)
{
	printf("  " NO_FRAMEBUFFER
				 "  deactivate framebuffer objects - use if both reflecting balls slow down the game\n");
	printf("  " NO_SHADER
				 "       deactivate shader              - use if only the golfballs slow down the game\n");
	printf("  " NO_VBO "          deactivate vertex buffer objects\n");
	printf("\n");
}

void initFeatures(int argc, char *argv[])
{
	int i;

	int noFramebuffer = 0;
	int noShader = 0;
	int noVBO = 0;

	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], NO_FRAMEBUFFER) == 0)
		{
			noFramebuffer = 1;
		}
		else if (strcmp(argv[i], NO_SHADER) == 0)
		{
			noShader = 1;
		}
		else if (strcmp(argv[i], NO_VBO) == 0)
		{
			noVBO = 1;
		}
	}

	glewInit();

	gFramebufferAvailable = 1;
	if (!GLEW_EXT_framebuffer_object || !GLEW_EXT_packed_depth_stencil)
	{
		printf("No framebuffer object available :(\n");
		gFramebufferAvailable = 0;
	}
	else if (noFramebuffer)
	{
		printf("framebuffer object available, but deactivated :/\n");
		gFramebufferAvailable = 0;
	}

	gShaderAvailable = 1;
	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader)
	{
		printf("No OpenGL 2.0 shader available :(\n");
		gShaderAvailable = 0;
	}
	else if (noShader)
	{
		printf("OpenGL 2.0 shader available, but deactivated :/\n");
		gShaderAvailable = 0;
	}

	gVertexbufferAvailable = 1;
	if (!GLEW_ARB_vertex_buffer_object)
	{
		printf("No vertex buffer object available :(\n");
		gVertexbufferAvailable = 0;
	}
	else if (noVBO)
	{
		printf("vertex buffer object available, but deactivated :/\n");
		gVertexbufferAvailable = 0;
	}

	gTwoSideStencilAvailable = 1;
	if (!GLEW_EXT_stencil_two_side || !GLEW_EXT_stencil_wrap)
	{
		printf("No two side stencil available :(\n");
		gTwoSideStencilAvailable = 0;
	}

	if (hasShader())
	{
		sgBallShadowShader = makeShader("ballshadow.vert", "ballshadow.frag");
		if (sgBallShadowShader)
		{
			printf("BallShadow-Shader ready :-)\n");
		}

		sgGolfballShader = makeShader("golfball.vert", "golfball.frag");
		if (sgGolfballShader)
		{
			printf("Golfball-Shader ready :-)\n");
		}
	}
}

/* has...? */

int hasShader(void)
{
	return gShaderAvailable;
}

int hasFramebuffer(void)
{
	return gFramebufferAvailable;
}

int hasVertexbuffer(void)
{
	return gVertexbufferAvailable;
}

int hasTwoSideStencil(void)
{
	return gTwoSideStencilAvailable;
}

int hasBallShadowShader(void)
{
	return hasShader() && sgBallShadowShader;
}

int hasGolfballShader(void)
{
	return hasShader() && sgGolfballShader;
}

/* set */

void setBallShadow(int use)
{
	gUseBallShadow = use;
}

void setReflection(int use)
{
	gUseReflection = use;
}

/* use...? */

int useBallShadow(void)
{
	return gUseBallShadow;
}

int useReflection(void)
{
	return gUseReflection;
}
