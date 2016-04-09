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

#include "BigAlpha10.hpp"

#include "hw/varray.hpp"

float verticesBigAlpha10[348 * 3] = {
4.29792, 9.66458, 1,
4.64167, 9.50833, 1,
4.29792, 9.66458, 0,
4.64167, 9.50833, 0,
4.84792, 9.24792, 1,
4.84792, 9.24792, 0,
4.91667, 8.88333, 1,
4.91667, 8.88333, 0,
4.84792, 8.51875, 1,
4.84792, 8.51875, 0,
4.64167, 8.25833, 1,
4.64167, 8.25833, 0,
4.29792, 8.10208, 1,
4.29792, 8.10208, 0,
3.81667, 8.05, 1,
3.81667, 8.05, 0,
3.23333, 8.05, 1,
3.23333, 8.05, 0,
3.23333, 5.56667, 1,
3.23333, 5.56667, 0,
6.2, 8.11667, 1,
6.2, 8.11667, 0,
5.97396, 8.23854, 1,
5.97396, 8.23854, 0,
5.8125, 8.40417, 1,
5.8125, 8.40417, 0,
5.71562, 8.61354, 1,
5.71562, 8.61354, 0,
5.68333, 8.86667, 1,
5.68333, 8.86667, 0,
5.75208, 9.23854, 1,
5.75208, 9.23854, 0,
5.95833, 9.50417, 1,
5.95833, 9.50417, 0,
6.30208, 9.66354, 1,
6.30208, 9.66354, 0,
6.78333, 9.71667, 1,
6.78333, 9.71667, 0,
8.45, 9.71667, 1,
8.45, 9.71667, 0,
8.93125, 9.66458, 1,
8.93125, 9.66458, 0,
9.275, 9.50833, 1,
9.275, 9.50833, 0,
9.48125, 9.24792, 1,
9.48125, 9.24792, 0,
9.55, 8.88333, 1,
9.55, 8.88333, 0,
9.48542, 8.51875, 1,
9.48542, 8.51875, 0,
9.29167, 8.25833, 1,
9.29167, 8.25833, 0,
8.96875, 8.10208, 1,
8.96875, 8.10208, 0,
8.51667, 8.05, 1,
8.51667, 8.05, 0,
5.58333, 5.5, 1,
5.58333, 5.5, 0,
6.36562, 4.94167, 1,
6.36562, 4.94167, 0,
7.09583, 4.11667, 1,
7.09583, 4.11667, 0,
7.77396, 3.025, 1,
7.77396, 3.025, 0,
8.4, 1.66667, 1,
8.4, 1.66667, 0,
8.93333, 1.66667, 1,
8.93333, 1.66667, 0,
9.41458, 1.61458, 1,
9.41458, 1.61458, 0,
9.75833, 1.45833, 1,
9.75833, 1.45833, 0,
9.96458, 1.19792, 1,
9.96458, 1.19792, 0,
10.0333, 0.833333, 1,
10.0333, 0.833333, 0,
9.96458, 0.46875, 1,
9.96458, 0.46875, 0,
9.75833, 0.208333, 1,
9.75833, 0.208333, 0,
9.41458, 0.052083, 1,
9.41458, 0.052083, 0,
8.93333, 0, 1,
8.93333, 0, 0,
7.1, 0, 1,
7.1, 0, 0,
6.7974, 0.852083, 1,
6.7974, 0.852083, 0,
6.47292, 1.625, 1,
6.47292, 1.625, 0,
6.12656, 2.31875, 1,
6.12656, 2.31875, 0,
5.75833, 2.93333, 1,
5.75833, 2.93333, 0,
5.37865, 3.45104, 1,
5.37865, 3.45104, 0,
4.99792, 3.85417, 1,
4.99792, 3.85417, 0,
4.61615, 4.14271, 1,
4.61615, 4.14271, 0,
4.23333, 4.31667, 1,
4.23333, 4.31667, 0,
3.23333, 3.46667, 1,
3.23333, 3.46667, 0,
3.23333, 1.66667, 1,
3.23333, 1.66667, 0,
3.81667, 1.66667, 1,
3.81667, 1.66667, 0,
4.29792, 1.61458, 1,
4.29792, 1.61458, 0,
4.64167, 1.45833, 1,
4.64167, 1.45833, 0,
4.84792, 1.19792, 1,
4.84792, 1.19792, 0,
4.91667, 0.833333, 1,
4.91667, 0.833333, 0,
4.84792, 0.46875, 1,
4.84792, 0.46875, 0,
4.64167, 0.208333, 1,
4.64167, 0.208333, 0,
4.29792, 0.052083, 1,
4.29792, 0.052083, 0,
3.81667, 0, 1,
3.81667, 0, 0,
1.31667, 0, 1,
1.31667, 0, 0,
0.835417, 0.052083, 1,
0.835417, 0.052083, 0,
0.491667, 0.208333, 1,
0.491667, 0.208333, 0,
0.285417, 0.46875, 1,
0.285417, 0.46875, 0,
0.216667, 0.833333, 1,
0.216667, 0.833333, 0,
0.285417, 1.19792, 1,
0.285417, 1.19792, 0,
0.491667, 1.45833, 1,
0.491667, 1.45833, 0,
0.835417, 1.61458, 1,
0.835417, 1.61458, 0,
1.31667, 1.66667, 1,
1.31667, 1.66667, 0,
1.56667, 1.66667, 1,
1.56667, 1.66667, 0,
1.56667, 8.05, 1,
1.56667, 8.05, 0,
1.31667, 8.05, 1,
1.31667, 8.05, 0,
0.835417, 8.10208, 1,
0.835417, 8.10208, 0,
0.491667, 8.25833, 1,
0.491667, 8.25833, 0,
0.285417, 8.51875, 1,
0.285417, 8.51875, 0,
0.216667, 8.88333, 1,
0.216667, 8.88333, 0,
0.234375, 9.10573, 1,
0.234375, 9.10573, 0,
0.2875, 9.28958, 1,
0.2875, 9.28958, 0,
0.376042, 9.4349, 1,
0.376042, 9.4349, 0,
0.5, 9.54167, 1,
0.5, 9.54167, 0,
0.657292, 9.61823, 1,
0.657292, 9.61823, 0,
0.845833, 9.67292, 1,
0.845833, 9.67292, 0,
1.06562, 9.70573, 1,
1.06562, 9.70573, 0,
1.31667, 9.71667, 1,
1.31667, 9.71667, 0,
3.81667, 9.71667, 1,
3.81667, 9.71667, 0,
3.23333, 1.66667, 0,
3.81667, 0, 0,
3.81667, 1.66667, 0,
4.29792, 1.61458, 0,
4.29792, 0.052083, 0,
4.64167, 1.45833, 0,
4.64167, 0.208333, 0,
4.84792, 1.19792, 0,
4.84792, 0.46875, 0,
4.91667, 0.833333, 0,
4.61615, 4.14271, 0,
6.2, 8.11667, 0,
4.23333, 4.31667, 0,
3.23333, 3.46667, 0,
3.23333, 5.56667, 0,
3.23333, 8.05, 0,
1.56667, 1.66667, 0,
1.56667, 8.05, 0,
0.216667, 0.833333, 0,
0.285417, 0.46875, 0,
0.285417, 1.19792, 0,
0.491667, 1.45833, 0,
0.491667, 0.208333, 0,
0.835417, 1.61458, 0,
0.835417, 0.052083, 0,
1.31667, 1.66667, 0,
1.31667, 0, 0,
8.4, 1.66667, 0,
8.93333, 0, 0,
8.93333, 1.66667, 0,
9.41458, 1.61458, 0,
9.41458, 0.052083, 0,
9.75833, 1.45833, 0,
9.75833, 0.208333, 0,
9.96458, 1.19792, 0,
9.96458, 0.46875, 0,
10.0333, 0.833333, 0,
7.77396, 3.025, 0,
7.1, 0, 0,
7.09583, 4.11667, 0,
6.7974, 0.852083, 0,
6.47292, 1.625, 0,
6.36562, 4.94167, 0,
6.12656, 2.31875, 0,
5.75833, 2.93333, 0,
5.58333, 5.5, 0,
5.37865, 3.45104, 0,
4.99792, 3.85417, 0,
8.51667, 8.05, 0,
6.78333, 9.71667, 0,
8.45, 9.71667, 0,
8.93125, 9.66458, 0,
8.96875, 8.10208, 0,
9.275, 9.50833, 0,
9.29167, 8.25833, 0,
9.48125, 9.24792, 0,
9.48542, 8.51875, 0,
9.55, 8.88333, 0,
6.30208, 9.66354, 0,
5.97396, 8.23854, 0,
5.95833, 9.50417, 0,
5.8125, 8.40417, 0,
5.75208, 9.23854, 0,
5.71562, 8.61354, 0,
4.91667, 8.88333, 0,
4.84792, 9.24792, 0,
4.84792, 8.51875, 0,
4.64167, 8.25833, 0,
4.64167, 9.50833, 0,
4.29792, 8.10208, 0,
4.29792, 9.66458, 0,
3.81667, 8.05, 0,
3.81667, 9.71667, 0,
1.31667, 8.05, 0,
0.845833, 9.67292, 0,
0.835417, 8.10208, 0,
1.06562, 9.70573, 0,
1.31667, 9.71667, 0,
0.2875, 9.28958, 0,
0.491667, 8.25833, 0,
0.376042, 9.4349, 0,
0.5, 9.54167, 0,
0.657292, 9.61823, 0,
0.285417, 8.51875, 0,
0.234375, 9.10573, 0,
0.216667, 8.88333, 0,
5.68333, 8.86667, 0,
3.23333, 1.66667, 1,
3.81667, 0, 1,
3.81667, 1.66667, 1,
4.29792, 1.61458, 1,
4.29792, 0.052083, 1,
4.64167, 1.45833, 1,
4.64167, 0.208333, 1,
4.84792, 1.19792, 1,
4.84792, 0.46875, 1,
4.91667, 0.833333, 1,
4.61615, 4.14271, 1,
6.2, 8.11667, 1,
4.23333, 4.31667, 1,
3.23333, 3.46667, 1,
3.23333, 5.56667, 1,
3.23333, 8.05, 1,
1.56667, 1.66667, 1,
1.56667, 8.05, 1,
0.216667, 0.833333, 1,
0.285417, 0.46875, 1,
0.285417, 1.19792, 1,
0.491667, 1.45833, 1,
0.491667, 0.208333, 1,
0.835417, 1.61458, 1,
0.835417, 0.052083, 1,
1.31667, 1.66667, 1,
1.31667, 0, 1,
8.4, 1.66667, 1,
8.93333, 0, 1,
8.93333, 1.66667, 1,
9.41458, 1.61458, 1,
9.41458, 0.052083, 1,
9.75833, 1.45833, 1,
9.75833, 0.208333, 1,
9.96458, 1.19792, 1,
9.96458, 0.46875, 1,
10.0333, 0.833333, 1,
7.77396, 3.025, 1,
7.1, 0, 1,
7.09583, 4.11667, 1,
6.7974, 0.852083, 1,
6.47292, 1.625, 1,
6.36562, 4.94167, 1,
6.12656, 2.31875, 1,
5.75833, 2.93333, 1,
5.58333, 5.5, 1,
5.37865, 3.45104, 1,
4.99792, 3.85417, 1,
8.51667, 8.05, 1,
6.78333, 9.71667, 1,
8.45, 9.71667, 1,
8.93125, 9.66458, 1,
8.96875, 8.10208, 1,
9.275, 9.50833, 1,
9.29167, 8.25833, 1,
9.48125, 9.24792, 1,
9.48542, 8.51875, 1,
9.55, 8.88333, 1,
6.30208, 9.66354, 1,
5.97396, 8.23854, 1,
5.95833, 9.50417, 1,
5.8125, 8.40417, 1,
5.75208, 9.23854, 1,
5.71562, 8.61354, 1,
4.91667, 8.88333, 1,
4.84792, 9.24792, 1,
4.84792, 8.51875, 1,
4.64167, 8.25833, 1,
4.64167, 9.50833, 1,
4.29792, 8.10208, 1,
4.29792, 9.66458, 1,
3.81667, 8.05, 1,
3.81667, 9.71667, 1,
1.31667, 8.05, 1,
0.845833, 9.67292, 1,
0.835417, 8.10208, 1,
1.06562, 9.70573, 1,
1.31667, 9.71667, 1,
0.2875, 9.28958, 1,
0.491667, 8.25833, 1,
0.376042, 9.4349, 1,
0.5, 9.54167, 1,
0.657292, 9.61823, 1,
0.285417, 8.51875, 1,
0.234375, 9.10573, 1,
0.216667, 8.88333, 1,
5.68333, 8.86667, 1,
};

float normalsBigAlpha10[348 * 3] = {
0.413803, 0.910366, 0,
0.783919, 0.620864, 0,
0.413803, 0.910366, 0,
0.783919, 0.620864, 0,
0.982681, 0.185306, 0,
0.982681, 0.185306, 0,
0.982681, -0.185306, 0,
0.982681, -0.185306, 0,
0.783919, -0.620864, 0,
0.783919, -0.620864, 0,
0.413803, -0.910366, 0,
0.413803, -0.910366, 0,
0.107597, -0.994195, 0,
0.107597, -0.994195, 0,
-0, -1, 0,
-0, -1, 0,
1, 0, 0,
1, 0, 0,
-0.651843, 0.758354, 0,
-0.651843, 0.758354, 0,
-0.474584, -0.88021, 0,
-0.474584, -0.88021, 0,
-0.716056, -0.698042, 0,
-0.716056, -0.698042, 0,
-0.907563, -0.419917, 0,
-0.907563, -0.419917, 0,
-0.991961, -0.126546, 0,
-0.991961, -0.126546, 0,
-0.983337, 0.181793, 0,
-0.983337, 0.181793, 0,
-0.789852, 0.613297, 0,
-0.789852, 0.613297, 0,
-0.420627, 0.907234, 0,
-0.420627, 0.907234, 0,
-0.109723, 0.993962, 0,
-0.109723, 0.993962, 0,
-0, 1, 0,
-0, 1, 0,
0.107597, 0.994195, 0,
0.107597, 0.994195, 0,
0.413803, 0.910366, 0,
0.413803, 0.910366, 0,
0.783919, 0.620864, 0,
0.783919, 0.620864, 0,
0.982681, 0.185306, 0,
0.982681, 0.185306, 0,
0.98467, -0.174427, 0,
0.98467, -0.174427, 0,
0.802305, -0.596915, 0,
0.802305, -0.596915, 0,
0.435561, -0.900159, 0,
0.435561, -0.900159, 0,
0.11445, -0.993429, 0,
0.11445, -0.993429, 0,
0.656073, -0.754698, 0,
0.656073, -0.754698, 0,
0.580931, 0.813953, 0,
0.580931, 0.813953, 0,
0.748816, 0.662778, 0,
0.748816, 0.662778, 0,
0.849452, 0.527665, 0,
0.849452, 0.527665, 0,
0.908183, 0.418572, 0,
0.908183, 0.418572, 0,
-0, 1, 0,
-0, 1, 0,
0.107597, 0.994195, 0,
0.107597, 0.994195, 0,
0.413803, 0.910366, 0,
0.413803, 0.910366, 0,
0.783919, 0.620864, 0,
0.783919, 0.620864, 0,
0.982681, 0.185306, 0,
0.982681, 0.185306, 0,
0.982681, -0.185306, 0,
0.982681, -0.185306, 0,
0.783919, -0.620864, 0,
0.783919, -0.620864, 0,
0.413803, -0.910366, 0,
0.413803, -0.910366, 0,
0.107597, -0.994195, 0,
0.107597, -0.994195, 0,
-0, -1, 0,
-0, -1, 0,
-0.94234, -0.334657, 0,
-0.94234, -0.334657, 0,
-0.922044, -0.387085, 0,
-0.922044, -0.387085, 0,
-0.894696, -0.446676, 0,
-0.894696, -0.446676, 0,
-0.857813, -0.513961, 0,
-0.857813, -0.513961, 0,
-0.806379, -0.591399, 0,
-0.806379, -0.591399, 0,
-0.727013, -0.686624, 0,
-0.727013, -0.686624, 0,
-0.602956, -0.797774, 0,
-0.602956, -0.797774, 0,
-0.41371, -0.910409, 0,
-0.41371, -0.910409, 0,
0.647648, -0.761939, 0,
0.647648, -0.761939, 0,
1, 0, 0,
1, 0, 0,
-0, 1, 0,
-0, 1, 0,
0.107597, 0.994195, 0,
0.107597, 0.994195, 0,
0.413803, 0.910366, 0,
0.413803, 0.910366, 0,
0.783919, 0.620864, 0,
0.783919, 0.620864, 0,
0.982681, 0.185306, 0,
0.982681, 0.185306, 0,
0.982681, -0.185306, 0,
0.982681, -0.185306, 0,
0.783919, -0.620864, 0,
0.783919, -0.620864, 0,
0.413803, -0.910366, 0,
0.413803, -0.910366, 0,
0.107597, -0.994195, 0,
0.107597, -0.994195, 0,
-0, -1, 0,
-0, -1, 0,
-0.107597, -0.994195, 0,
-0.107597, -0.994195, 0,
-0.413803, -0.910366, 0,
-0.413803, -0.910366, 0,
-0.783919, -0.620864, 0,
-0.783919, -0.620864, 0,
-0.982681, -0.185306, 0,
-0.982681, -0.185306, 0,
-0.982681, 0.185306, 0,
-0.982681, 0.185306, 0,
-0.783919, 0.620864, 0,
-0.783919, 0.620864, 0,
-0.413803, 0.910366, 0,
-0.413803, 0.910366, 0,
-0.107597, 0.994195, 0,
-0.107597, 0.994195, 0,
-0, 1, 0,
-0, 1, 0,
-1, 0, 0,
-1, 0, 0,
-0, -1, 0,
-0, -1, 0,
-0.107597, -0.994195, 0,
-0.107597, -0.994195, 0,
-0.413803, -0.910366, 0,
-0.413803, -0.910366, 0,
-0.783919, -0.620864, 0,
-0.783919, -0.620864, 0,
-0.982681, -0.185306, 0,
-0.982681, -0.185306, 0,
-0.996845, 0.079374, 0,
-0.996845, 0.079374, 0,
-0.960698, 0.277595, 0,
-0.960698, 0.277595, 0,
-0.853962, 0.520335, 0,
-0.853962, 0.520335, 0,
-0.652625, 0.757681, 0,
-0.652625, 0.757681, 0,
-0.437661, 0.89914, 0,
-0.437661, 0.89914, 0,
-0.278573, 0.960415, 0,
-0.278573, 0.960415, 0,
-0.147653, 0.989039, 0,
-0.147653, 0.989039, 0,
-0.043527, 0.999052, 0,
-0.043527, 0.999052, 0,
-0, 1, 0,
-0, 1, 0,
0.107597, 0.994195, 0,
0.107597, 0.994195, 0,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
};

unsigned int indicesBigAlpha10[1032] = {
0, 1, 2,
1, 3, 2,
1, 4, 3,
4, 5, 3,
4, 6, 5,
6, 7, 5,
6, 8, 7,
8, 9, 7,
8, 10, 9,
10, 11, 9,
10, 12, 11,
12, 13, 11,
12, 14, 13,
14, 15, 13,
14, 16, 15,
16, 17, 15,
16, 18, 17,
18, 19, 17,
18, 20, 19,
20, 21, 19,
20, 22, 21,
22, 23, 21,
22, 24, 23,
24, 25, 23,
24, 26, 25,
26, 27, 25,
26, 28, 27,
28, 29, 27,
28, 30, 29,
30, 31, 29,
30, 32, 31,
32, 33, 31,
32, 34, 33,
34, 35, 33,
34, 36, 35,
36, 37, 35,
36, 38, 37,
38, 39, 37,
38, 40, 39,
40, 41, 39,
40, 42, 41,
42, 43, 41,
42, 44, 43,
44, 45, 43,
44, 46, 45,
46, 47, 45,
46, 48, 47,
48, 49, 47,
48, 50, 49,
50, 51, 49,
50, 52, 51,
52, 53, 51,
52, 54, 53,
54, 55, 53,
54, 56, 55,
56, 57, 55,
56, 58, 57,
58, 59, 57,
58, 60, 59,
60, 61, 59,
60, 62, 61,
62, 63, 61,
62, 64, 63,
64, 65, 63,
64, 66, 65,
66, 67, 65,
66, 68, 67,
68, 69, 67,
68, 70, 69,
70, 71, 69,
70, 72, 71,
72, 73, 71,
72, 74, 73,
74, 75, 73,
74, 76, 75,
76, 77, 75,
76, 78, 77,
78, 79, 77,
78, 80, 79,
80, 81, 79,
80, 82, 81,
82, 83, 81,
82, 84, 83,
84, 85, 83,
84, 86, 85,
86, 87, 85,
86, 88, 87,
88, 89, 87,
88, 90, 89,
90, 91, 89,
90, 92, 91,
92, 93, 91,
92, 94, 93,
94, 95, 93,
94, 96, 95,
96, 97, 95,
96, 98, 97,
98, 99, 97,
98, 100, 99,
100, 101, 99,
100, 102, 101,
102, 103, 101,
102, 104, 103,
104, 105, 103,
104, 106, 105,
106, 107, 105,
106, 108, 107,
108, 109, 107,
108, 110, 109,
110, 111, 109,
110, 112, 111,
112, 113, 111,
112, 114, 113,
114, 115, 113,
114, 116, 115,
116, 117, 115,
116, 118, 117,
118, 119, 117,
118, 120, 119,
120, 121, 119,
120, 122, 121,
122, 123, 121,
122, 124, 123,
124, 125, 123,
124, 126, 125,
126, 127, 125,
126, 128, 127,
128, 129, 127,
128, 130, 129,
130, 131, 129,
130, 132, 131,
132, 133, 131,
132, 134, 133,
134, 135, 133,
134, 136, 135,
136, 137, 135,
136, 138, 137,
138, 139, 137,
138, 140, 139,
140, 141, 139,
140, 142, 141,
142, 143, 141,
142, 144, 143,
144, 145, 143,
144, 146, 145,
146, 147, 145,
146, 148, 147,
148, 149, 147,
148, 150, 149,
150, 151, 149,
150, 152, 151,
152, 153, 151,
152, 154, 153,
154, 155, 153,
154, 156, 155,
156, 157, 155,
156, 158, 157,
158, 159, 157,
158, 160, 159,
160, 161, 159,
160, 162, 161,
162, 163, 161,
162, 164, 163,
164, 165, 163,
164, 166, 165,
166, 167, 165,
166, 168, 167,
168, 169, 167,
168, 170, 169,
170, 171, 169,
170, 172, 171,
172, 173, 171,
172, 0, 173,
0, 2, 173,
174, 175, 176,
175, 177, 176,
175, 178, 177,
178, 179, 177,
178, 180, 179,
180, 181, 179,
180, 182, 181,
182, 183, 181,
184, 185, 186,
185, 187, 186,
185, 188, 187,
188, 174, 187,
188, 189, 174,
189, 190, 174,
189, 191, 190,
192, 193, 194,
193, 195, 194,
193, 196, 195,
196, 197, 195,
196, 198, 197,
198, 199, 197,
198, 200, 199,
200, 190, 199,
200, 175, 190,
175, 174, 190,
201, 202, 203,
202, 204, 203,
202, 205, 204,
205, 206, 204,
205, 207, 206,
207, 208, 206,
207, 209, 208,
209, 210, 208,
201, 211, 202,
211, 212, 202,
211, 213, 212,
213, 214, 212,
213, 215, 214,
216, 215, 213,
216, 217, 215,
216, 218, 217,
216, 219, 218,
219, 220, 218,
219, 221, 220,
219, 185, 221,
219, 222, 185,
223, 222, 224,
222, 225, 224,
222, 226, 225,
226, 227, 225,
226, 228, 227,
228, 229, 227,
228, 230, 229,
230, 231, 229,
232, 222, 223,
232, 185, 222,
232, 233, 185,
232, 234, 233,
235, 236, 237,
235, 234, 236,
235, 233, 234,
238, 239, 240,
239, 241, 240,
239, 242, 241,
242, 243, 241,
242, 244, 243,
244, 245, 243,
244, 246, 245,
246, 189, 245,
246, 191, 189,
247, 248, 249,
247, 250, 248,
247, 251, 250,
247, 246, 251,
247, 191, 246,
252, 253, 254,
253, 255, 254,
253, 249, 255,
249, 256, 255,
249, 248, 256,
257, 258, 259,
257, 252, 258,
257, 253, 252,
236, 260, 237,
184, 221, 185,
261, 262, 263,
262, 264, 263,
262, 265, 264,
265, 266, 264,
265, 267, 266,
267, 268, 266,
267, 269, 268,
269, 270, 268,
271, 272, 273,
272, 274, 273,
272, 275, 274,
275, 261, 274,
275, 276, 261,
276, 277, 261,
276, 278, 277,
279, 280, 281,
280, 282, 281,
280, 283, 282,
283, 284, 282,
283, 285, 284,
285, 286, 284,
285, 287, 286,
287, 277, 286,
287, 262, 277,
262, 261, 277,
288, 289, 290,
289, 291, 290,
289, 292, 291,
292, 293, 291,
292, 294, 293,
294, 295, 293,
294, 296, 295,
296, 297, 295,
288, 298, 289,
298, 299, 289,
298, 300, 299,
300, 301, 299,
300, 302, 301,
303, 302, 300,
303, 304, 302,
303, 305, 304,
303, 306, 305,
306, 307, 305,
306, 308, 307,
306, 272, 308,
306, 309, 272,
310, 309, 311,
309, 312, 311,
309, 313, 312,
313, 314, 312,
313, 315, 314,
315, 316, 314,
315, 317, 316,
317, 318, 316,
319, 309, 310,
319, 272, 309,
319, 320, 272,
319, 321, 320,
322, 323, 324,
322, 321, 323,
322, 320, 321,
325, 326, 327,
326, 328, 327,
326, 329, 328,
329, 330, 328,
329, 331, 330,
331, 332, 330,
331, 333, 332,
333, 276, 332,
333, 278, 276,
334, 335, 336,
334, 337, 335,
334, 338, 337,
334, 333, 338,
334, 278, 333,
339, 340, 341,
340, 342, 341,
340, 336, 342,
336, 343, 342,
336, 335, 343,
344, 345, 346,
344, 339, 345,
344, 340, 339,
323, 347, 324,
271, 308, 272,
};

void drawBigAlpha10()
{
drawTrianglesVerticesNormalsIndices(1032, verticesBigAlpha10, normalsBigAlpha10, indicesBigAlpha10);
}
