/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck
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
 *
 * $Id$
 *
 */

#ifndef _light_h_
#define _light_h_

typedef struct {
	int id;
	int enable;
	float pos[4];
	float color[4];

	int spot;
	float dir[3];
	float exponent;
	float cutoff;
} Light;

extern Light sgLight[];

int addPointLight(float x, float y, float z);
int addSpotLight(float x, float y, float z, float dx, float dy, float dz, float exponent, float cutoff);

void toggleLight(int light);

void setLights(void);

#endif
