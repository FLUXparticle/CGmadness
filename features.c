/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
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

#include <stdio.h>

/*
 * this module checks which opengl features are available
 */

static int gFramebufferAvailable;
static int gVertexbufferAvailable;
static int gTwoSideStencilAvailable;
static int gShaderAvailable;

static int gUseBallShadow = 0;
static int gUseReflection = 0;

GLhandleARB sgBallShadowShader = 0;
GLhandleARB sgGolfballShader = 0;

void initFeatures(int argc, char* argv[]) {
	gShaderAvailable = 1;
	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader)
	{
		printf("No OpenGL 2.0 shader available :(\n");
		gShaderAvailable = 0;
	}

	gFramebufferAvailable = 1;
	if (!GLEW_EXT_framebuffer_object || !GLEW_EXT_packed_depth_stencil)
	{
		printf("No framebuffer object available :(\n");
		gFramebufferAvailable = 0;
	}

	gVertexbufferAvailable = 1;
	if (!GLEW_ARB_vertex_buffer_object)
	{
		printf("No vertex buffer object available :(\n");
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
		if (sgBallShadowShader) {
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

