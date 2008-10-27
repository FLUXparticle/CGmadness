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

#version 110

uniform samplerCube Environment;

uniform float reflection;

varying vec4 diffuse;
varying vec3 normal;
varying vec3 vViewVec;

void main() {
	vec3 lightDir = vec3(0.0, 0.0, 1.0);
	vec3 offset = gl_TexCoord[0].xyz;
	vec3 n = normal;

	if (length(offset) < 0.5) {
		n = n - 0.5 * gl_NormalMatrix * offset;
	}

	n = normalize(n);

	float NdotL = max(dot(n, normalize(lightDir)), 0.0);
	vec4 colDiffuse = diffuse * NdotL;

 	vec3 viewVec = normalize(vViewVec);
	vec3 reflVec = reflect(-viewVec, n);
	vec4 colReflection = textureCube(Environment, vec3(gl_TextureMatrix[0] * vec4(reflVec, 1)));

	float fog = exp(-gl_FogFragCoord * gl_Fog.density);
	gl_FragColor = vec4(vec3((1.0 - reflection) * colDiffuse + reflection * colReflection), diffuse.a) * fog;
}
