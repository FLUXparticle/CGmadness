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

#include GL_H

#include <cstdio>
#include <cstdlib>

static void printShaderInfoLog(GLuint obj, const char* text)
{
	int infologLength = 0;
	int charsWritten = 0;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		char* infoLog = new char[infologLength];

		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s: %s\n", text, infoLog);
		fflush(stdout);

		delete[] infoLog;
	}
}

static void printProgramInfoLog(GLuint obj, const char* text)
{
	int infologLength = 0;
	int charsWritten = 0;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		char* infoLog = new char[infologLength];

		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s: %s\n", text, infoLog);
		fflush(stdout);

		delete[] infoLog;
	}
}


static GLuint makeShader(const char* vertexShaderFilename,
											 const char* fragmentShaderFilename)
{
	char* vs;
	char* fs;
	GLint compiled;
	GLint linked;

	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint p;

	if ((vs = textFileRead(vertexShaderFilename)) == NULL)
	{
		return 0;
	}

	if ((fs = textFileRead(fragmentShaderFilename)) == NULL)
	{
		return 0;
	}

	{
		const char* vv = vs;
		const char* ff = fs;

		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);
	}

	delete[] vs;
	delete[] fs;

	glCompileShader(v);
	printShaderInfoLog(v, vertexShaderFilename);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		return 0;
	}

	glCompileShader(f);
	printShaderInfoLog(f, fragmentShaderFilename);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		return 0;
	}

	p = glCreateProgram();

	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);
	printProgramInfoLog(p, "Link");
	glGetProgramiv(p, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		return 0;
	}

	return p;
}

Shader::Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	mShader = makeShader(vertexShaderFilename, fragmentShaderFilename);
}

void Shader::useProgram()
{
	glUseProgram(mShader);
}

void Shader::setVariable(const char* name, Vector3& value)
{
	glUniform3fv(glGetUniformLocation(mShader, name), 1, value);
}

void Shader::setVariable(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(mShader, name), value);
}

void Shader::setVariable(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(mShader, name), value);
}
