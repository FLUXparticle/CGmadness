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

varying vec4 diffuse, ambient;
varying vec3 lightDir, normal;
varying vec3 vViewVec;

void main() {	
	/* Berechnen aus welcher Richtung das Licht kommt */
	vec4 ecPos = gl_ModelViewMatrix * gl_Vertex;
	lightDir = vec3(gl_LightSource[1].position - ecPos);

	/* Die Entefernung für den Nebel übergeben */
	gl_FogFragCoord = -ecPos.z;

	/* Texture-Koordinaten durchreichen */	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	/* Ambient- und Diffuse-Anteil berechnen */
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	
	/* Richtung der Kamera berechnen */
	vViewVec = -ecPos.xyz;

	/* Die Normale berechnen */
 	normal = normalize(gl_NormalMatrix * vec3(gl_Normal));

	/* Den aktuerllen Vertex projezieren */
	gl_Position = ftransform();
} 
