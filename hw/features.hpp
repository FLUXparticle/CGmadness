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

#ifndef _features_hpp_
#define _features_hpp_

#include <GL/glew.h>

extern GLhandleARB sgBallShadowShader;
extern GLhandleARB sgGolfballShader;

void usageFeatures(void);

void initFeatures(int argc, char *argv[]);

bool hasShader(void);
bool hasFramebuffer(void);
bool hasVertexbuffer(void);
bool hasBallShadowShader(void);
bool hasGolfballShader(void);

void setBallShadow(bool use);
void setReflection(bool use);

bool useBallShadow(void);
bool useReflection(void);

#endif
