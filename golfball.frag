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

#version 110

uniform samplerCube Environment;

uniform float useFog;

uniform float reflection;

varying vec4 diffuse, ambient;
varying vec3 lightDir, normal;
varying vec3 vViewVec;

void main() {
	/* Entfernung vom nächsten Loch */
	vec3 offset = gl_TexCoord[0].xyz;

	/* Normale */
	vec3 n = normal;

	/* Wenn dieser Pixel in einem Loch liegt, wird die Normale verändert */
	if (length(offset) < 0.5) {
		n = n - 0.5 * gl_NormalMatrix * offset;
	}
	
	/* Und zum Schluss die Normale nochmal normieren. Dadurch wirkt der Ball auch runder. */
	n = normalize(n);

	/* diffuse Beleuchtung berechnen */
	float NdotL = max(dot(n, normalize(lightDir)), 0.0);
	vec4 colDiffuse = diffuse * NdotL;

	/* Richtung der Reflektion berechnen und entsprechenden Punkt in der CubeMap nachschauen. */
 	vec3 viewVec = normalize(vViewVec);
	vec3 reflVec = reflect(-viewVec, n);
	vec4 colReflection = textureCube(Environment, vec3(gl_TextureMatrix[0] * vec4(reflVec, 1)));

	/* Nebel berechnen */
	float fog = (useFog * exp(-gl_FogFragCoord * gl_Fog.density));

	/* Alles zusammensetzen */
	gl_FragColor = vec4(vec3((1.0 - reflection) * colDiffuse + reflection * colReflection), 1.0) * fog;
}
