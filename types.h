#ifndef _types_h_
#define _types_h_

/*
 * Namens-Konventionen für Variablen:
 * Objekte                o
 * global                 g
 * globale Objekte        go
 * super global           sg
 * super globale Objekte  sgo
 */

#define PI 3.14159265358979323846

#define length(x) (sizeof(x) / sizeof(*x))

#define MATRIX_SIZE 4

typedef float Matrix[MATRIX_SIZE][MATRIX_SIZE];

#endif
