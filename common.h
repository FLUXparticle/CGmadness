/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
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

#ifndef _common_h_
#define _common_h_

#include "vector.h"

#define NOISE_TEXTURE 0

#define MAX_LEVEL_SIZE 100

extern float sgIdleProgress;

void createAtlas(void);
void destroyAtlas(void);

void updateLightMap(void);
void updateColorMap(void);
void updateTexCoords(void);

void initCommon(void);
void destroyCommon(void);

void newLevel(void);
int loadFieldFromFile(const char* filename);
int saveFieldToFile(const char* filename);

#endif
