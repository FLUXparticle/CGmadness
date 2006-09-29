#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

void* dbgMalloc(int size, char* pointername, char* filename, int line) {
	void* p = malloc(size);
	if (p) {
		fprintf(stderr, "%s : %d -- malloc %s:%d\n", pointername, size, filename, line);
	} else {
		fprintf(stderr, "%s : %d -- could not malloc %s:%d\n", pointername, size, filename, line);
	}
	return p;
}

void dbgFree(void* p, char* pointername, char* filename, int line) {
	fprintf(stderr, "%s -- free %s:%d\n", pointername, filename, line);
	free(p);
}
