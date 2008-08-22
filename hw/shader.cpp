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

#include "shader.hpp"

#include "files.hpp"

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

void printInfoLog(GLhandleARB obj, const char *text)
{
	GLint  infologLength = 0;
	GLsizei charsWritten = 0;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

	if (infologLength > 1)
	{
		char *infoLog = new char[infologLength];
		
		glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		printf("%s: %s\n", text, infoLog);
		fflush(stdout);
		
		delete[] infoLog;
	}
}

/*
 * Erzeugt ein Programm aus einem Vertex- und einem FragmentShader
 */
GLhandleARB makeShader(const char *vertexShaderFilename,
											 const char *fragmentShaderFilename)
{
	char *vs;
	char *fs;
	GLint compiled;
	GLint linked;

	GLhandleARB v = glCreateShader(GL_VERTEX_SHADER);
	GLhandleARB f = glCreateShader(GL_FRAGMENT_SHADER);
	GLhandleARB p;

	if ((vs = textFileRead(vertexShaderFilename)) == NULL)
	{
		return 0;
	}

	if ((fs = textFileRead(fragmentShaderFilename)) == NULL)
	{
		return 0;
	}

	{
		const char *vv = vs;
		const char *ff = fs;

		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);
	}

	delete[] vs;
	delete[] fs;

	glCompileShader(v);
	printInfoLog(v, vertexShaderFilename);
	glGetObjectParameterivARB(v, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if (!compiled)
	{
		return 0;
	}

	glCompileShader(f);
	printInfoLog(f, fragmentShaderFilename);
	glGetObjectParameterivARB(f, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if (!compiled)
	{
		return 0;
	}

	p = glCreateProgram();

	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);
	printInfoLog(p, "Link");
	glGetObjectParameterivARB(p, GL_OBJECT_LINK_STATUS_ARB, &linked);
	if (!linked)
	{
		return 0;
	}

	return p;
}
