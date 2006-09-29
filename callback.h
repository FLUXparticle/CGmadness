#ifndef _callback_h_
#define _callback_h_

#include "graph.h"

#include "types.h"

typedef void (*funcUpdate)(double interval);

typedef struct {
	Matrix projection;
	Matrix view;
	Object* world;
} Viewport;

typedef struct {
	int enabled;

	int width;
	int height;

	int framebuffer;
	int texTarget;
	int texID;
	Viewport* viewport;
} RenderTarget;

extern Viewport sgWindowViewport;

void setUpdateFunc(funcUpdate update);
	
void addRenderTarget(RenderTarget* target);

void startDisplay(void);

void startTimer(int callsPerSecond);

int pick(int x, int y); 

#endif
