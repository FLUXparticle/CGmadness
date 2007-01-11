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

varying vec4 diffuse;
varying vec3 normal, lightDir;
	
void main()
{	
	/* Richtung des Spotlights berechnen */
	vec3 ecPos = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[2].position) - ecPos;

	/* diffuser Anteil */	
	diffuse = gl_LightSource[2].diffuse * gl_Color;
	
	/* Normale berechnen */
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	/* aktuellen Vertex transformieren */
	gl_Position = ftransform();
} 
