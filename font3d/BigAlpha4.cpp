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

#include "BigAlpha4.hpp"

#include "hw/varray.hpp"

float verticesBigAlpha4[280 * 3] = {
7.55208, 3.02187, 1,
7.70833, 3.37083, 1,
7.55208, 3.02187, 0,
7.70833, 3.37083, 0,
7.96875, 3.58021, 1,
7.96875, 3.58021, 0,
8.33333, 3.65, 1,
8.33333, 3.65, 0,
8.69792, 3.58021, 1,
8.69792, 3.58021, 0,
8.95833, 3.37083, 1,
8.95833, 3.37083, 0,
9.11458, 3.02187, 1,
9.11458, 3.02187, 0,
9.16667, 2.53333, 1,
9.16667, 2.53333, 0,
9.16667, 0, 1,
9.16667, 0, 0,
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
8.8, 9.71667, 1,
8.8, 9.71667, 0,
8.8, 7.21667, 1,
8.8, 7.21667, 0,
8.74792, 6.72813, 1,
8.74792, 6.72813, 0,
8.59167, 6.37917, 1,
8.59167, 6.37917, 0,
8.33125, 6.16979, 1,
8.33125, 6.16979, 0,
7.96667, 6.1, 1,
7.96667, 6.1, 0,
7.60208, 6.16979, 1,
7.60208, 6.16979, 0,
7.34167, 6.37917, 1,
7.34167, 6.37917, 0,
7.18542, 6.72813, 1,
7.18542, 6.72813, 0,
7.13333, 7.21667, 1,
7.13333, 7.21667, 0,
7.13333, 8.05, 1,
7.13333, 8.05, 0,
3.23333, 8.05, 1,
3.23333, 8.05, 0,
3.23333, 5.7, 1,
3.23333, 5.7, 0,
4.65, 5.7, 1,
4.65, 5.7, 0,
4.65, 5.81667, 1,
4.65, 5.81667, 0,
4.70104, 6.30521, 1,
4.70104, 6.30521, 0,
4.85417, 6.65417, 1,
4.85417, 6.65417, 0,
5.10938, 6.86354, 1,
5.10938, 6.86354, 0,
5.46667, 6.93333, 1,
5.46667, 6.93333, 0,
5.83125, 6.86354, 1,
5.83125, 6.86354, 0,
6.09167, 6.65417, 1,
6.09167, 6.65417, 0,
6.24792, 6.30521, 1,
6.24792, 6.30521, 0,
6.3, 5.81667, 1,
6.3, 5.81667, 0,
6.3, 3.93333, 1,
6.3, 3.93333, 0,
6.24896, 3.45208, 1,
6.24896, 3.45208, 0,
6.09583, 3.10833, 1,
6.09583, 3.10833, 0,
5.84062, 2.90208, 1,
5.84062, 2.90208, 0,
5.48333, 2.83333, 1,
5.48333, 2.83333, 0,
5.11875, 2.90208, 1,
5.11875, 2.90208, 0,
4.85833, 3.10833, 1,
4.85833, 3.10833, 0,
4.70208, 3.45208, 1,
4.70208, 3.45208, 0,
4.65, 3.93333, 1,
4.65, 3.93333, 0,
4.65, 4.05, 1,
4.65, 4.05, 0,
3.23333, 4.05, 1,
3.23333, 4.05, 0,
3.23333, 1.66667, 1,
3.23333, 1.66667, 0,
7.5, 1.66667, 1,
7.5, 1.66667, 0,
7.5, 2.53333, 1,
7.5, 2.53333, 0,
9.16667, 0, 0,
1.56667, 1.66667, 0,
1.31667, 0, 0,
3.23333, 1.66667, 0,
7.5, 1.66667, 0,
7.55208, 3.02187, 0,
7.70833, 3.37083, 0,
7.96875, 3.58021, 0,
8.33333, 3.65, 0,
8.69792, 3.58021, 0,
8.95833, 3.37083, 0,
9.11458, 3.02187, 0,
9.16667, 2.53333, 0,
4.70208, 3.45208, 0,
4.70104, 6.30521, 0,
4.65, 3.93333, 0,
4.65, 4.05, 0,
4.65, 5.7, 0,
3.23333, 4.05, 0,
3.23333, 5.7, 0,
3.23333, 8.05, 0,
1.56667, 8.05, 0,
0.216667, 0.833333, 0,
0.285417, 0.46875, 0,
0.285417, 1.19792, 0,
0.491667, 1.45833, 0,
0.491667, 0.208333, 0,
0.835417, 1.61458, 0,
0.835417, 0.052083, 0,
1.31667, 1.66667, 0,
4.85833, 3.10833, 0,
4.85417, 6.65417, 0,
5.10938, 6.86354, 0,
5.11875, 2.90208, 0,
5.46667, 6.93333, 0,
5.48333, 2.83333, 0,
5.83125, 6.86354, 0,
5.84062, 2.90208, 0,
6.09167, 6.65417, 0,
6.09583, 3.10833, 0,
6.24792, 6.30521, 0,
6.24896, 3.45208, 0,
6.3, 5.81667, 0,
6.3, 3.93333, 0,
8.8, 9.71667, 0,
8.74792, 6.72813, 0,
8.8, 7.21667, 0,
8.59167, 6.37917, 0,
8.33125, 6.16979, 0,
7.96667, 6.1, 0,
7.60208, 6.16979, 0,
7.34167, 6.37917, 0,
7.18542, 6.72813, 0,
7.13333, 7.21667, 0,
7.13333, 8.05, 0,
1.31667, 8.05, 0,
1.31667, 9.71667, 0,
0.5, 9.54167, 0,
0.835417, 8.10208, 0,
0.657292, 9.61823, 0,
0.845833, 9.67292, 0,
1.06562, 9.70573, 0,
0.491667, 8.25833, 0,
0.2875, 9.28958, 0,
0.285417, 8.51875, 0,
0.376042, 9.4349, 0,
0.234375, 9.10573, 0,
0.216667, 8.88333, 0,
4.65, 5.81667, 0,
7.5, 2.53333, 0,
9.16667, 0, 1,
1.56667, 1.66667, 1,
1.31667, 0, 1,
3.23333, 1.66667, 1,
7.5, 1.66667, 1,
7.55208, 3.02187, 1,
7.70833, 3.37083, 1,
7.96875, 3.58021, 1,
8.33333, 3.65, 1,
8.69792, 3.58021, 1,
8.95833, 3.37083, 1,
9.11458, 3.02187, 1,
9.16667, 2.53333, 1,
4.70208, 3.45208, 1,
4.70104, 6.30521, 1,
4.65, 3.93333, 1,
4.65, 4.05, 1,
4.65, 5.7, 1,
3.23333, 4.05, 1,
3.23333, 5.7, 1,
3.23333, 8.05, 1,
1.56667, 8.05, 1,
0.216667, 0.833333, 1,
0.285417, 0.46875, 1,
0.285417, 1.19792, 1,
0.491667, 1.45833, 1,
0.491667, 0.208333, 1,
0.835417, 1.61458, 1,
0.835417, 0.052083, 1,
1.31667, 1.66667, 1,
4.85833, 3.10833, 1,
4.85417, 6.65417, 1,
5.10938, 6.86354, 1,
5.11875, 2.90208, 1,
5.46667, 6.93333, 1,
5.48333, 2.83333, 1,
5.83125, 6.86354, 1,
5.84062, 2.90208, 1,
6.09167, 6.65417, 1,
6.09583, 3.10833, 1,
6.24792, 6.30521, 1,
6.24896, 3.45208, 1,
6.3, 5.81667, 1,
6.3, 3.93333, 1,
8.8, 9.71667, 1,
8.74792, 6.72813, 1,
8.8, 7.21667, 1,
8.59167, 6.37917, 1,
8.33125, 6.16979, 1,
7.96667, 6.1, 1,
7.60208, 6.16979, 1,
7.34167, 6.37917, 1,
7.18542, 6.72813, 1,
7.13333, 7.21667, 1,
7.13333, 8.05, 1,
1.31667, 8.05, 1,
1.31667, 9.71667, 1,
0.5, 9.54167, 1,
0.835417, 8.10208, 1,
0.657292, 9.61823, 1,
0.845833, 9.67292, 1,
1.06562, 9.70573, 1,
0.491667, 8.25833, 1,
0.2875, 9.28958, 1,
0.285417, 8.51875, 1,
0.376042, 9.4349, 1,
0.234375, 9.10573, 1,
0.216667, 8.88333, 1,
4.65, 5.81667, 1,
7.5, 2.53333, 1,
};

float normalsBigAlpha4[280 * 3] = {
-0.912685, 0.408665, 0,
-0.626594, 0.779346, 0,
-0.912685, 0.408665, 0,
-0.626594, 0.779346, 0,
-0.188015, 0.982166, 0,
-0.188015, 0.982166, 0,
0.188015, 0.982166, 0,
0.188015, 0.982166, 0,
0.626594, 0.779346, 0,
0.626594, 0.779346, 0,
0.912685, 0.408665, 0,
0.912685, 0.408665, 0,
0.994365, 0.106009, 0,
0.994365, 0.106009, 0,
1, 0, 0,
1, 0, 0,
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
1, 0, 0,
1, 0, 0,
0.994365, -0.106009, 0,
0.994365, -0.106009, 0,
0.912685, -0.408665, 0,
0.912685, -0.408665, 0,
0.626594, -0.779346, 0,
0.626594, -0.779346, 0,
0.188015, -0.982166, 0,
0.188015, -0.982166, 0,
-0.188015, -0.982166, 0,
-0.188015, -0.982166, 0,
-0.626594, -0.779346, 0,
-0.626594, -0.779346, 0,
-0.912685, -0.408665, 0,
-0.912685, -0.408665, 0,
-0.994365, -0.106009, 0,
-0.994365, -0.106009, 0,
-1, 0, 0,
-1, 0, 0,
-0, -1, 0,
-0, -1, 0,
1, 0, 0,
1, 0, 0,
-0, 1, 0,
-0, 1, 0,
-1, 0, 0,
-1, 0, 0,
-0.994586, 0.103912, 0,
-0.994586, 0.103912, 0,
-0.915718, 0.401822, 0,
-0.915718, 0.401822, 0,
-0.634268, 0.773113, 0,
-0.634268, 0.773113, 0,
-0.191712, 0.981451, 0,
-0.191712, 0.981451, 0,
0.188015, 0.982166, 0,
0.188015, 0.982166, 0,
0.626594, 0.779346, 0,
0.626594, 0.779346, 0,
0.912685, 0.408665, 0,
0.912685, 0.408665, 0,
0.994365, 0.106009, 0,
0.994365, 0.106009, 0,
1, 0, 0,
1, 0, 0,
0.994423, -0.105469, 0,
0.994423, -0.105469, 0,
0.913469, -0.406909, 0,
0.913469, -0.406909, 0,
0.628559, -0.777762, 0,
0.628559, -0.777762, 0,
0.188954, -0.981986, 0,
0.188954, -0.981986, 0,
-0.185306, -0.982681, 0,
-0.185306, -0.982681, 0,
-0.620864, -0.783919, 0,
-0.620864, -0.783919, 0,
-0.910366, -0.413803, 0,
-0.910366, -0.413803, 0,
-0.994195, -0.107597, 0,
-0.994195, -0.107597, 0,
-1, 0, 0,
-1, 0, 0,
-0, -1, 0,
-0, -1, 0,
1, 0, 0,
1, 0, 0,
-0, 1, 0,
-0, 1, 0,
-1, 0, 0,
-1, 0, 0,
-0.994365, 0.106009, 0,
-0.994365, 0.106009, 0,
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
};

unsigned int indicesBigAlpha4[828] = {
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
138, 0, 139,
0, 2, 139,
140, 141, 142,
140, 143, 141,
140, 144, 143,
140, 145, 144,
140, 146, 145,
140, 147, 146,
140, 148, 147,
140, 149, 148,
140, 150, 149,
140, 151, 150,
140, 152, 151,
153, 154, 155,
154, 156, 155,
154, 157, 156,
157, 158, 156,
157, 159, 158,
159, 143, 158,
159, 160, 143,
160, 141, 143,
160, 161, 141,
162, 163, 164,
163, 165, 164,
163, 166, 165,
166, 167, 165,
166, 168, 167,
168, 169, 167,
168, 142, 169,
142, 141, 169,
153, 170, 171,
170, 172, 171,
170, 173, 172,
173, 174, 172,
173, 175, 174,
175, 176, 174,
175, 177, 176,
177, 178, 176,
177, 179, 178,
179, 180, 178,
179, 181, 180,
181, 182, 180,
181, 183, 182,
184, 185, 186,
184, 187, 185,
184, 188, 187,
184, 189, 188,
184, 190, 189,
184, 191, 190,
184, 192, 191,
184, 193, 192,
184, 194, 193,
184, 160, 194,
184, 161, 160,
184, 195, 161,
184, 196, 195,
197, 198, 199,
198, 200, 199,
198, 195, 200,
195, 201, 200,
195, 196, 201,
202, 203, 204,
202, 205, 203,
202, 197, 205,
202, 198, 197,
204, 206, 207,
204, 203, 206,
154, 208, 157,
153, 171, 154,
145, 209, 144,
210, 211, 212,
210, 213, 211,
210, 214, 213,
210, 215, 214,
210, 216, 215,
210, 217, 216,
210, 218, 217,
210, 219, 218,
210, 220, 219,
210, 221, 220,
210, 222, 221,
223, 224, 225,
224, 226, 225,
224, 227, 226,
227, 228, 226,
227, 229, 228,
229, 213, 228,
229, 230, 213,
230, 211, 213,
230, 231, 211,
232, 233, 234,
233, 235, 234,
233, 236, 235,
236, 237, 235,
236, 238, 237,
238, 239, 237,
238, 212, 239,
212, 211, 239,
223, 240, 241,
240, 242, 241,
240, 243, 242,
243, 244, 242,
243, 245, 244,
245, 246, 244,
245, 247, 246,
247, 248, 246,
247, 249, 248,
249, 250, 248,
249, 251, 250,
251, 252, 250,
251, 253, 252,
254, 255, 256,
254, 257, 255,
254, 258, 257,
254, 259, 258,
254, 260, 259,
254, 261, 260,
254, 262, 261,
254, 263, 262,
254, 264, 263,
254, 230, 264,
254, 231, 230,
254, 265, 231,
254, 266, 265,
267, 268, 269,
268, 270, 269,
268, 265, 270,
265, 271, 270,
265, 266, 271,
272, 273, 274,
272, 275, 273,
272, 267, 275,
272, 268, 267,
274, 276, 277,
274, 273, 276,
224, 278, 227,
223, 241, 224,
215, 279, 214,
};

void drawBigAlpha4()
{
drawTrianglesVerticesNormalsIndices(828, verticesBigAlpha4, normalsBigAlpha4, indicesBigAlpha4);
}