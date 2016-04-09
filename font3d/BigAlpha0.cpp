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

#include "BigAlpha0.hpp"

#include "hw/varray.hpp"

float verticesBigAlpha0[264 * 3] = {
5.85104, 1.05573, 1,
5.90417, 1.23958, 1,
5.85104, 1.05573, 0,
5.90417, 1.23958, 0,
5.99271, 1.3849, 1,
5.99271, 1.3849, 0,
6.11667, 1.49167, 1,
6.11667, 1.49167, 0,
6.27396, 1.56823, 1,
6.27396, 1.56823, 0,
6.4625, 1.62292, 1,
6.4625, 1.62292, 0,
6.68229, 1.65573, 1,
6.68229, 1.65573, 0,
6.93333, 1.66667, 1,
6.93333, 1.66667, 0,
7.31667, 1.66667, 1,
7.31667, 1.66667, 0,
6.93333, 2.63333, 1,
6.93333, 2.63333, 0,
2.96667, 2.63333, 1,
2.96667, 2.63333, 0,
2.58333, 1.66667, 1,
2.58333, 1.66667, 0,
2.96667, 1.66667, 1,
2.96667, 1.66667, 0,
3.44792, 1.61458, 1,
3.44792, 1.61458, 0,
3.79167, 1.45833, 1,
3.79167, 1.45833, 0,
3.99792, 1.19792, 1,
3.99792, 1.19792, 0,
4.06667, 0.833333, 1,
4.06667, 0.833333, 0,
3.99792, 0.46875, 1,
3.99792, 0.46875, 0,
3.79167, 0.208333, 1,
3.79167, 0.208333, 0,
3.44792, 0.052083, 1,
3.44792, 0.052083, 0,
2.96667, 0, 1,
2.96667, 0, 0,
0.75, 0, 1,
0.75, 0, 0,
0.26875, 0.052083, 1,
0.26875, 0.052083, 0,
-0.075, 0.208333, 1,
-0.075, 0.208333, 0,
-0.28125, 0.46875, 1,
-0.28125, 0.46875, 0,
-0.35, 0.833333, 1,
-0.35, 0.833333, 0,
-0.331771, 1.0625, 1,
-0.331771, 1.0625, 0,
-0.277083, 1.25, 1,
-0.277083, 1.25, 0,
-0.185938, 1.39583, 1,
-0.185938, 1.39583, 0,
-0.058333, 1.5, 1,
-0.058333, 1.5, 0,
0.102604, 1.57292, 1,
0.102604, 1.57292, 0,
0.29375, 1.625, 1,
0.29375, 1.625, 0,
0.515104, 1.65625, 1,
0.515104, 1.65625, 0,
0.766667, 1.66667, 1,
0.766667, 1.66667, 0,
3.35, 8.05, 1,
3.35, 8.05, 0,
2.3, 8.05, 1,
2.3, 8.05, 0,
1.81146, 8.10208, 1,
1.81146, 8.10208, 0,
1.4625, 8.25833, 1,
1.4625, 8.25833, 0,
1.25312, 8.51875, 1,
1.25312, 8.51875, 0,
1.18333, 8.88333, 1,
1.18333, 8.88333, 0,
1.20156, 9.1125, 1,
1.20156, 9.1125, 0,
1.25625, 9.3, 1,
1.25625, 9.3, 0,
1.3474, 9.44583, 1,
1.3474, 9.44583, 0,
1.475, 9.55, 1,
1.475, 9.55, 0,
1.63594, 9.62292, 1,
1.63594, 9.62292, 0,
1.82708, 9.675, 1,
1.82708, 9.675, 0,
2.04844, 9.70625, 1,
2.04844, 9.70625, 0,
2.3, 9.71667, 1,
2.3, 9.71667, 0,
5.9, 9.71667, 1,
5.9, 9.71667, 0,
9.21667, 1.66667, 1,
9.21667, 1.66667, 0,
9.7125, 1.61458, 1,
9.7125, 1.61458, 0,
10.0667, 1.45833, 1,
10.0667, 1.45833, 0,
10.2792, 1.19792, 1,
10.2792, 1.19792, 0,
10.35, 0.833333, 1,
10.35, 0.833333, 0,
10.2812, 0.46875, 1,
10.2812, 0.46875, 0,
10.075, 0.208333, 1,
10.075, 0.208333, 0,
9.73125, 0.052083, 1,
9.73125, 0.052083, 0,
9.25, 0, 1,
9.25, 0, 0,
6.93333, 0, 1,
6.93333, 0, 0,
6.45208, 0.052083, 1,
6.45208, 0.052083, 0,
6.10833, 0.208333, 1,
6.10833, 0.208333, 0,
5.90208, 0.46875, 1,
5.90208, 0.46875, 0,
5.83333, 0.833333, 1,
5.83333, 0.833333, 0,
4.95, 7.5, 1,
3.63333, 4.3, 1,
4.95, 7.5, 0,
3.63333, 4.3, 0,
6.25, 4.3, 1,
6.25, 4.3, 0,
5.9, 9.71667, 0,
6.25, 4.3, 0,
9.21667, 1.66667, 0,
6.93333, 2.63333, 0,
3.63333, 4.3, 0,
2.96667, 2.63333, 0,
3.35, 8.05, 0,
2.58333, 1.66667, 0,
0.766667, 1.66667, 0,
2.96667, 0, 0,
0.75, 0, 0,
10.35, 0.833333, 0,
10.2792, 1.19792, 0,
10.2812, 0.46875, 0,
10.075, 0.208333, 0,
10.0667, 1.45833, 0,
9.73125, 0.052083, 0,
9.7125, 1.61458, 0,
9.25, 0, 0,
7.31667, 1.66667, 0,
6.93333, 0, 0,
6.4625, 1.62292, 0,
6.45208, 0.052083, 0,
6.68229, 1.65573, 0,
6.93333, 1.66667, 0,
5.90417, 1.23958, 0,
6.10833, 0.208333, 0,
5.99271, 1.3849, 0,
6.11667, 1.49167, 0,
6.27396, 1.56823, 0,
5.90208, 0.46875, 0,
5.85104, 1.05573, 0,
5.83333, 0.833333, 0,
4.95, 7.5, 0,
2.3, 8.05, 0,
2.3, 9.71667, 0,
1.475, 9.55, 0,
1.81146, 8.10208, 0,
1.63594, 9.62292, 0,
1.82708, 9.675, 0,
2.04844, 9.70625, 0,
1.4625, 8.25833, 0,
1.25625, 9.3, 0,
1.25312, 8.51875, 0,
1.3474, 9.44583, 0,
1.20156, 9.1125, 0,
1.18333, 8.88333, 0,
2.96667, 1.66667, 0,
3.44792, 1.61458, 0,
3.44792, 0.052083, 0,
3.79167, 1.45833, 0,
3.79167, 0.208333, 0,
3.99792, 1.19792, 0,
3.99792, 0.46875, 0,
4.06667, 0.833333, 0,
-0.058333, 1.5, 0,
0.26875, 0.052083, 0,
0.102604, 1.57292, 0,
0.29375, 1.625, 0,
0.515104, 1.65625, 0,
-0.075, 0.208333, 0,
-0.277083, 1.25, 0,
-0.28125, 0.46875, 0,
-0.185938, 1.39583, 0,
-0.331771, 1.0625, 0,
-0.35, 0.833333, 0,
5.9, 9.71667, 1,
6.25, 4.3, 1,
9.21667, 1.66667, 1,
6.93333, 2.63333, 1,
3.63333, 4.3, 1,
2.96667, 2.63333, 1,
3.35, 8.05, 1,
2.58333, 1.66667, 1,
0.766667, 1.66667, 1,
2.96667, 0, 1,
0.75, 0, 1,
10.35, 0.833333, 1,
10.2792, 1.19792, 1,
10.2812, 0.46875, 1,
10.075, 0.208333, 1,
10.0667, 1.45833, 1,
9.73125, 0.052083, 1,
9.7125, 1.61458, 1,
9.25, 0, 1,
7.31667, 1.66667, 1,
6.93333, 0, 1,
6.4625, 1.62292, 1,
6.45208, 0.052083, 1,
6.68229, 1.65573, 1,
6.93333, 1.66667, 1,
5.90417, 1.23958, 1,
6.10833, 0.208333, 1,
5.99271, 1.3849, 1,
6.11667, 1.49167, 1,
6.27396, 1.56823, 1,
5.90208, 0.46875, 1,
5.85104, 1.05573, 1,
5.83333, 0.833333, 1,
4.95, 7.5, 1,
2.3, 8.05, 1,
2.3, 9.71667, 1,
1.475, 9.55, 1,
1.81146, 8.10208, 1,
1.63594, 9.62292, 1,
1.82708, 9.675, 1,
2.04844, 9.70625, 1,
1.4625, 8.25833, 1,
1.25625, 9.3, 1,
1.25312, 8.51875, 1,
1.3474, 9.44583, 1,
1.20156, 9.1125, 1,
1.18333, 8.88333, 1,
2.96667, 1.66667, 1,
3.44792, 1.61458, 1,
3.44792, 0.052083, 1,
3.79167, 1.45833, 1,
3.79167, 0.208333, 1,
3.99792, 1.19792, 1,
3.99792, 0.46875, 1,
4.06667, 0.833333, 1,
-0.058333, 1.5, 1,
0.26875, 0.052083, 1,
0.102604, 1.57292, 1,
0.29375, 1.625, 1,
0.515104, 1.65625, 1,
-0.075, 0.208333, 1,
-0.277083, 1.25, 1,
-0.28125, 0.46875, 1,
-0.185938, 1.39583, 1,
-0.331771, 1.0625, 1,
-0.35, 0.833333, 1,
};

float normalsBigAlpha0[264 * 3] = {
-0.960698, 0.277595, 0,
-0.853962, 0.520335, 0,
-0.960698, 0.277595, 0,
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
-0.929578, -0.368626, 0,
-0.929578, -0.368626, 0,
-0, -1, 0,
-0, -1, 0,
0.929578, -0.368626, 0,
0.929578, -0.368626, 0,
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
-0.996851, 0.079295, 0,
-0.996851, 0.079295, 0,
-0.96, 0.28, 0,
-0.96, 0.28, 0,
-0.847998, 0.529999, 0,
-0.847998, 0.529999, 0,
-0.632376, 0.774661, 0,
-0.632376, 0.774661, 0,
-0.412692, 0.910871, 0,
-0.412692, 0.910871, 0,
-0.262895, 0.964824, 0,
-0.262895, 0.964824, 0,
-0.13979, 0.990181, 0,
-0.13979, 0.990181, 0,
-0.041372, 0.999144, 0,
-0.041372, 0.999144, 0,
-0.926967, 0.375143, 0,
-0.926967, 0.375143, 0,
-0, -1, 0,
-0, -1, 0,
-0.106009, -0.994365, 0,
-0.106009, -0.994365, 0,
-0.408665, -0.912685, 0,
-0.408665, -0.912685, 0,
-0.779346, -0.626594, 0,
-0.779346, -0.626594, 0,
-0.982166, -0.188015, 0,
-0.982166, -0.188015, 0,
-0.996851, 0.079295, 0,
-0.996851, 0.079295, 0,
-0.96, 0.28, 0,
-0.96, 0.28, 0,
-0.847998, 0.529999, 0,
-0.847998, 0.529999, 0,
-0.632376, 0.774661, 0,
-0.632376, 0.774661, 0,
-0.412692, 0.910871, 0,
-0.412692, 0.910871, 0,
-0.262895, 0.964824, 0,
-0.262895, 0.964824, 0,
-0.13979, 0.990181, 0,
-0.13979, 0.990181, 0,
-0.041372, 0.999144, 0,
-0.041372, 0.999144, 0,
-0, 1, 0,
-0, 1, 0,
0.924599, 0.380942, 0,
0.924599, 0.380942, 0,
0.104467, 0.994528, 0,
0.104467, 0.994528, 0,
0.40364, 0.914918, 0,
0.40364, 0.914918, 0,
0.774785, 0.632225, 0,
0.774785, 0.632225, 0,
0.981645, 0.19072, 0,
0.981645, 0.19072, 0,
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
-0.996845, 0.079374, 0,
-0.996845, 0.079374, 0,
0.924778, -0.380508, 0,
-0, 1, 0,
0.924778, -0.380508, 0,
-0, 1, 0,
-0.926467, -0.376377, 0,
-0.926467, -0.376377, 0,
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
};

unsigned int indicesBigAlpha0[792] = {
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
124, 0, 125,
0, 2, 125,
126, 127, 128,
127, 129, 128,
127, 130, 129,
130, 131, 129,
130, 126, 131,
126, 128, 131,
132, 133, 134,
133, 135, 134,
133, 136, 135,
136, 137, 135,
136, 138, 137,
138, 139, 137,
138, 140, 139,
140, 141, 139,
140, 142, 141,
143, 144, 145,
144, 146, 145,
144, 147, 146,
147, 148, 146,
147, 149, 148,
149, 150, 148,
149, 134, 150,
134, 151, 150,
134, 135, 151,
152, 153, 154,
152, 155, 153,
152, 156, 155,
152, 151, 156,
152, 150, 151,
157, 158, 159,
158, 160, 159,
158, 154, 160,
154, 161, 160,
154, 153, 161,
162, 163, 164,
162, 157, 163,
162, 158, 157,
132, 165, 133,
132, 138, 165,
132, 166, 138,
132, 167, 166,
168, 169, 170,
169, 171, 170,
169, 166, 171,
166, 172, 171,
166, 167, 172,
173, 174, 175,
173, 176, 174,
173, 168, 176,
173, 169, 168,
175, 177, 178,
175, 174, 177,
139, 141, 179,
141, 180, 179,
141, 181, 180,
181, 182, 180,
181, 183, 182,
183, 184, 182,
183, 185, 184,
185, 186, 184,
187, 188, 189,
188, 190, 189,
188, 142, 190,
142, 191, 190,
142, 140, 191,
192, 193, 194,
192, 195, 193,
192, 187, 195,
192, 188, 187,
194, 196, 197,
194, 193, 196,
138, 136, 165,
198, 199, 200,
199, 201, 200,
199, 202, 201,
202, 203, 201,
202, 204, 203,
204, 205, 203,
204, 206, 205,
206, 207, 205,
206, 208, 207,
209, 210, 211,
210, 212, 211,
210, 213, 212,
213, 214, 212,
213, 215, 214,
215, 216, 214,
215, 200, 216,
200, 217, 216,
200, 201, 217,
218, 219, 220,
218, 221, 219,
218, 222, 221,
218, 217, 222,
218, 216, 217,
223, 224, 225,
224, 226, 225,
224, 220, 226,
220, 227, 226,
220, 219, 227,
228, 229, 230,
228, 223, 229,
228, 224, 223,
198, 231, 199,
198, 204, 231,
198, 232, 204,
198, 233, 232,
234, 235, 236,
235, 237, 236,
235, 232, 237,
232, 238, 237,
232, 233, 238,
239, 240, 241,
239, 242, 240,
239, 234, 242,
239, 235, 234,
241, 243, 244,
241, 240, 243,
205, 207, 245,
207, 246, 245,
207, 247, 246,
247, 248, 246,
247, 249, 248,
249, 250, 248,
249, 251, 250,
251, 252, 250,
253, 254, 255,
254, 256, 255,
254, 208, 256,
208, 257, 256,
208, 206, 257,
258, 259, 260,
258, 261, 259,
258, 253, 261,
258, 254, 253,
260, 262, 263,
260, 259, 262,
204, 202, 231,
};

void drawBigAlpha0()
{
drawTrianglesVerticesNormalsIndices(792, verticesBigAlpha0, normalsBigAlpha0, indicesBigAlpha0);
}
