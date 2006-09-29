#ifndef _fbuffer_
#define _fbuffer_

#include "callback.h"

int initFBuffer(int width, int height, RenderTarget* context);

int initFBufferCube(int width, int height, RenderTarget context[6]);

#endif
