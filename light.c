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

#include "light.h"

#include "types.h"

#include "debug.h"

#include <GL/gl.h>

#include <string.h>

#define MAX_LIGHTS LENGTH(gLightIDs)

float gNone[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float gSome[] = { 0.2f, 0.2f, 0.2f, 1.0f };
int gLightIDs[] = { GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };

Light sgLight[MAX_LIGHTS];
int gCntLights = 0;

void setLightColor(int id, float* color) {
	glLightfv(id, GL_DIFFUSE, color);
	glLightfv(id, GL_SPECULAR, color);
}
	
void toggleLight(int index) {
	Light* light = &sgLight[index];

	light->enable = !light->enable;

	if (light->enable) {
		setLightColor(light->id, light->color);
	} else {
		setLightColor(light->id, gSome);
	}
}

void enableLight(int index) {
	if (!sgLight[index].enable) {
		toggleLight(index);
	}
}

void disableLight(int index) {
	if (sgLight[index].enable) {
		toggleLight(index);
	}
}

int addLight(float x, float y, float z) {
	int light = gCntLights++;
	Light* newLight = &sgLight[light];
	newLight->id = gLightIDs[light];
	newLight->enable = 1;
	
	glGetFloatv(GL_CURRENT_COLOR, newLight->color);

	newLight->pos.x = x;
	newLight->pos.y = y;
	newLight->pos.z = z;

	newLight->spot = 0;

	glEnable(newLight->id);

	return light;
}

int addPointLight(float x, float y, float z) {
	return addLight(x, y, z);
}

int addSpotLight(float x, float y, float z, float dx, float dy, float dz, float exponent, float cutoff) {
	int light = addLight(x, y, z);
	Light* newLight = &sgLight[light];

	newLight->dir[0] = dx;
	newLight->dir[1] = dy;
	newLight->dir[2] = dz;
	
	newLight->spot = 1;
	newLight->exponent = exponent;
	newLight->cutoff = cutoff;

	return light;
}

void setLights(void) {
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	int i;

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gNone);
	glLightfv(GL_LIGHT0, GL_SPECULAR, gNone);
	glLightfv(GL_LIGHT0, GL_POSITION, gNone);
	glEnable(GL_LIGHT0);
	
	for (i = 0; i < gCntLights; i++) {
		Light* light = &sgLight[i];
		if (light->enable) {
			float pos[4];
		 
			pos[0] = light->pos.x;
			pos[1] = light->pos.y;
			pos[2] = light->pos.z;
			pos[3] = 1.0f;

			glEnable(light->id);
			glLightfv(light->id, GL_POSITION, pos);
			glLightfv(light->id, GL_AMBIENT, gNone);

			setLightColor(light->id, light->color);

			if (light->spot) {
				glLightfv(light->id, GL_SPOT_DIRECTION, light->dir);
				glLightf(light->id, GL_SPOT_EXPONENT, light->exponent);
				glLightf(light->id, GL_SPOT_CUTOFF, light->cutoff);
			}
		} else {
			glDisable(light->id);
		}
	}
}
