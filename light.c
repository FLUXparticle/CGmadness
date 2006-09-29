#include "light.h"

#include "types.h"

#include "debug.h"

#include <GL/gl.h>

#include <string.h>

#define MAX_LIGHTS length(gLightIDs)

float gNone[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float gSome[] = { 0.2f, 0.2f, 0.2f, 1.0f };
int gLightIDs[] = { GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };

Light sgLight[MAX_LIGHTS];
int gCntLights = 0;

void setLightColor(int id, float* color) {
	glLightfv(id, GL_DIFFUSE, color);
	glLightfv(id, GL_SPECULAR, color);
}
	
/* Lichtquelle ein oder ausschalten */
void toggleLight(int index) {
	Light* light = &sgLight[index];

	light->enable = !light->enable;

	if (light->enable) {
		setLightColor(light->id, light->color);
	} else {
		setLightColor(light->id, gSome);
	}
}

/* Lichtquelle hinzufuegen */
int addLight(float x, float y, float z) {
	int light = gCntLights++;
	Light* newLight = &sgLight[light];
	newLight->id = gLightIDs[light];
	newLight->enable = 1;
	
	glGetFloatv(GL_CURRENT_COLOR, newLight->color);

	newLight->pos[0] = x;
	newLight->pos[1] = y;
	newLight->pos[2] = z;
	newLight->pos[3] = 1.0f;

	newLight->spot = 0;

	glEnable(newLight->id);

	return light;
}

/* Punktlichtquelle erzeugen */
int addPointLight(float x, float y, float z) {
	return addLight(x, y, z);
}

/* Spotlight erzeugen */
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

/*
 * Setzt alle Lichtquellen, damit diese am selben Ort bleiben, auch wenn sich die Kamera bewegt.
 */
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
			glEnable(light->id);
			glLightfv(light->id, GL_POSITION, light->pos);
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
