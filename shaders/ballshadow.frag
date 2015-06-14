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

varying vec3 normal;
varying vec3 vertex;

uniform vec3 ball;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	vec3 d = ball - vertex;

	float r = length(d);

	float d1 = dot(normal, normalize(d));

	float light = 1.0 - max(0.0, (d1 / (1.0 + ((r * r) / (0.2 * 0.2)))));

	vec4 shadow = vec4(light);

	vec4 texColor0 = texture2D(tex0, vec2(gl_TexCoord[0]));
	vec4 texColor1 = texture2D(tex1, vec2(gl_TexCoord[1]));

	gl_FragColor = gl_Color * texColor0 * texColor1 * shadow;
}
