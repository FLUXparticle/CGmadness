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
