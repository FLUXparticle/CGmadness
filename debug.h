#ifndef _debug_h_
#define _debug_h_

#define DEBUG 0

#define PRINT_FLOAT(f) printf(#f ": %f\n", (f))
#define PRINT_VECTOR(vector) printf(#vector ".x: %f, " #vector ".y: %f, " #vector ".z: %f\n", (vector).x, (vector).y, (vector).z)

#if DEBUG

#  define MALLOC(p, size) p = dbgMalloc(size, #p, __FILE__, __LINE__)
#  define FREE(p) dbgFree(p, #p, __FILE__, __LINE__)

void* dbgMalloc(int size, char* pointername, char* filename, int line);
void dbgFree(void* p, char* pointername, char* filename, int line);

#else

#  define MALLOC(p, size) p = malloc(size)
#  define FREE(p) free(p)

#endif

#endif
