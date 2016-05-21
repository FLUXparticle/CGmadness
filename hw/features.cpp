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

#include "features.hpp"

#include <GL/glew.h>

#include <cstdio>
#include <cstring>

/*
 * this module checks which opengl features are available
 */

#define NO_SHADER "--no-shader"
#define NO_FRAMEBUFFER "--no-framebuffer"
#define NO_VBO "--no-vbo"

static bool gFramebufferAvailable;
static bool gVertexbufferAvailable;
static bool gShaderAvailable;

static bool gUseBallShadow = 0;
static bool gUseReflection = 0;

Shader* sgBallShadowShader;
Shader* sgGolfballShader;

void usageFeatures()
{
	printf("  " NO_FRAMEBUFFER
				 "  deactivate framebuffer objects - use if both reflecting balls slow down the game\n");
	printf("  " NO_SHADER
				 "       deactivate shader              - use if only the golfballs slow down the game\n");
	printf("  " NO_VBO "          deactivate vertex buffer objects\n");
	printf("\n");
}

void initFeatures(int argc, char* argv[])
{
	int i;

	bool noFramebuffer = false;
	bool noShader = false;
	bool noVBO = false;

	glewInit();

	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], NO_FRAMEBUFFER) == 0)
		{
			noFramebuffer = true;
		}
		else if (strcmp(argv[i], NO_SHADER) == 0)
		{
			noShader = true;
		}
		else if (strcmp(argv[i], NO_VBO) == 0)
		{
			noVBO = true;
		}
	}

	gFramebufferAvailable = true;
	if (!GLEW_EXT_framebuffer_object)
	{
		printf("No framebuffer object available :(\n");
		gFramebufferAvailable = false;
	}
	else if (noFramebuffer)
	{
		printf("framebuffer object available, but deactivated :/\n");
		gFramebufferAvailable = false;
	}

	gShaderAvailable = true;
	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader)
	{
		printf("No OpenGL 2.0 shader available :(\n");
		gShaderAvailable = false;
	}
	else if (noShader)
	{
		printf("OpenGL 2.0 shader available, but deactivated :/\n");
		gShaderAvailable = false;
	}

	gVertexbufferAvailable = true;
	if (!GLEW_ARB_vertex_buffer_object)
	{
		printf("No vertex buffer object available :(\n");
		gVertexbufferAvailable = false;
	}
	else if (noVBO)
	{
		printf("vertex buffer object available, but deactivated :/\n");
		gVertexbufferAvailable = false;
	}

	if (hasShader())
	{
		sgBallShadowShader = new Shader("ballshadow.vert", "ballshadow.frag");
		if (sgBallShadowShader)
		{
			printf("BallShadow-Shader ready :-)\n");
		}

		sgGolfballShader = new Shader("golfball.vert", "golfball.frag");
		if (sgGolfballShader)
		{
			printf("Golfball-Shader ready :-)\n");
		}
	}
}

/* has...? */

bool hasShader()
{
	return gShaderAvailable;
}

bool hasFramebuffer()
{
	return gFramebufferAvailable;
}

bool hasVertexbuffer()
{
	return gVertexbufferAvailable;
}

bool hasBallShadowShader()
{
	return hasShader() && sgBallShadowShader;
}

bool hasGolfballShader()
{
	return hasShader() && sgGolfballShader;
}

/* set */

void setBallShadow(bool use)
{
	gUseBallShadow = use;
}

void setReflection(bool use)
{
	gUseReflection = use;
}

/* use...? */

bool useBallShadow()
{
	return gUseBallShadow;
}

bool useReflection()
{
	return gUseReflection;
}
