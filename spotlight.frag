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
	/* Farbe erstmal auf Schwarz setzen */
	vec4 color = vec4(0);
	
	/* Richtung des Spotlights bestimmen */
	vec3 light = normalize(lightDir);

	/* diffuse Beleuchtung berechnen */
	float NdotL = max(dot(normalize(normal), light), 0.0);
	/* Winkel von der Hauptachse des Spotlights an dieser Stelle */
	float spotEffect = dot(normalize(gl_LightSource[2].spotDirection), -light);

	/* Cutoff des Spotlights */
	if (spotEffect > gl_LightSource[2].spotCosCutoff) {
		spotEffect = pow(spotEffect, gl_LightSource[2].spotExponent);
		color = spotEffect * diffuse * NdotL;

		/* Das grün in der Mitte rausfiltern, damit das gelbe Spotlight in der mitte rot wird. */
		color.g *= min((1.0 - spotEffect) * 2.0, 1.0);
	}

	/* Farbe zurückgeben */	
	gl_FragColor = color;
}
