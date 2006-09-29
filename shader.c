#include "shader.h"

#include "files.h"

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Gibt ggf. Fehlermeldungen bei der Compilierung aus
 */
int printInfoLog(GLhandleARB obj, char* text) {
	int infologLength = 0;
	int charsWritten  = 0;
	char* infoLog;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

	if (infologLength > 1) {
		MALLOC(infoLog, infologLength);
		glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		printf("%s: %s\n", text, infoLog);
		fflush(stdout);
		FREE(infoLog);

		return 0;
	}

	return 1;
}

/*
 * Erzeugt ein Programm aus einem Vertex- und einem FragmentShader
 */
GLhandleARB makeShader(char* vertexShaderFilename, char* fragmentShaderFilename) {
	char* vs;
	char* fs;

	GLhandleARB v = glCreateShader(GL_VERTEX_SHADER);
	GLhandleARB f = glCreateShader(GL_FRAGMENT_SHADER);
	GLhandleARB p;

	if ((vs = textFileRead(vertexShaderFilename)) == NULL) {
		return 0;
	}
	
	if ((fs = textFileRead(fragmentShaderFilename)) == NULL) {
		return 0;
	}
	
	{
		const char* vv = vs;
		const char* ff = fs;
		
		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);
	}

	FREE(vs);
	FREE(fs);

	glCompileShader(v);
	if (!printInfoLog(v, vertexShaderFilename)) {
		return 0;
	}
	
	glCompileShader(f);
	if (!printInfoLog(f, fragmentShaderFilename)) {
		return 0;
	}

	p = glCreateProgram();
	
	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);
	if (!printInfoLog(p, "Link")) {
		return 0;
	}

	return p;
}
