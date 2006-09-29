#ifndef _pick_h_
#define _pick_h_

#include "graph.h"

typedef void (*funcPick)(void* data);

typedef struct {
	int pickName;
	funcPick pick;
	void* data;
} Pick;

void initPick(Pick* pick, funcPick f, void* data);
	
void setObjectPick(Object* obj, Pick* pick);

void doPick(int name);

#endif
