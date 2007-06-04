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

#include "skyplane.h"
#include "features.h"

#include "debug.h"

#include <math.h>

typedef struct tVertex {
	float x;
	float y;
	float z;
	
	float alpha;
	
	float u;
	float v;	
} tVertex;

int numOfVerts;
int numOfIndices;
int texID;

tVertex  *PlaneVerts = NULL;
int 	   *Indices = NULL;

void initSkyplane(float PlanetRadius, float AtmosphereRadius, float hTile, float vTile, float alpha, int divs, int exponentialFadeout) {
	float planeSize;
	float delta;
	float tex_delta;
	float xdist;
	float ydist;
	float xheight;
	float yheight;
	float height;
	float alpharadius_squared;
	float planetradius_squared;
	int i;
	int j;
	tVertex SV;
	int index = 0;
	
	texID = loadTexture("data/clouds.tga", 1);
    
  if (divs < 1) {
		divs = 1;
	}
		
	if (alpha > 1.0f) {
		alpha = 1.0f;
	}
	
	numOfVerts = (divs + 1) * (divs + 1);
	numOfIndices = divs * divs * 2 * 3;
	
	MALLOC(PlaneVerts, numOfVerts * sizeof(tVertex));
	MALLOC(Indices,    numOfIndices * sizeof(int));
	
	planeSize =  sqrt(((2.0 * PlanetRadius) * (2.0 * PlanetRadius)) * 0.5);
	delta     =  planeSize / divs;
	tex_delta =  1.0f / divs;
	
	alpharadius_squared  = (planeSize * alpha * 0.5f) * (planeSize * alpha * 0.5f);
	planetradius_squared = PlanetRadius*PlanetRadius;
	
	for (i = 0; i <= divs; i++) {
		for (j = 0; j <= divs; j++) {
			xdist = ( -0.5f * planeSize) + (j * delta);
			ydist = ( -0.5f * planeSize) + (i * delta);
			
			xheight = xdist * xdist;
			yheight = ydist * ydist;
			height = (xheight + yheight) / planetradius_squared;

			SV.x = xdist;
			SV.y = ydist;
			SV.z = (1.f - height) * AtmosphereRadius;
 			SV.alpha = (xheight + yheight) / alpharadius_squared;

			if (exponentialFadeout) {
	      SV.alpha = SV.alpha * SV.alpha * SV.alpha;
			}
            
			SV.alpha = 1.0f - SV.alpha;
            
			if (SV.alpha < 0.0f) {
	      SV.alpha = 0.0f;
			}

			/* Calculate the texture coordinates */
			SV.u = hTile * ((float) j * tex_delta);
			SV.v = vTile * ((float) i * tex_delta);
			
			PlaneVerts[i * (divs + 1) + j] = SV;
		}
	}
	
	for (i = 0; i < divs; i++) {
		for(j = 0; j < divs; j++) {
			int startVert;
			
			startVert  = i * (divs + 1) + j; 
			
			Indices[index++] = startVert;
			Indices[index++] = startVert + divs + 1;
			Indices[index++] = startVert + 1;
			
			Indices[index++] = startVert + 1;
			Indices[index++] = startVert + divs + 1;
			Indices[index++] = startVert + divs + 2;
		}
	}
}
	
void drawSkyplane(void) {
	int i;
	
	glPushMatrix();

  	glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texID);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_TRIANGLES);
			for (i = 0; i< numOfIndices; i++) {
				glColor4f(1.0f, 1.0f, 1.0f, PlaneVerts[Indices[i]].alpha);
				glTexCoord2f(PlaneVerts[Indices[i]].u, PlaneVerts[Indices[i]].v);
				glVertex3f(PlaneVerts[Indices[i]].x, PlaneVerts[Indices[i]].y, PlaneVerts[Indices[i]].z);
			}
		glEnd();
		
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

	glPopMatrix(); 
}

void destroySkyPlane(void){
	FREE(PlaneVerts);
	FREE(Indices);
}
