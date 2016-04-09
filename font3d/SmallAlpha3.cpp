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

#include "SmallAlpha3.hpp"

#include "hw/varray.hpp"

float verticesSmallAlpha3[348 * 3] = {
9.59792, 1.61458, 1,
9.94167, 1.45833, 1,
9.59792, 1.61458, 0,
9.94167, 1.45833, 0,
10.1479, 1.19792, 1,
10.1479, 1.19792, 0,
10.2167, 0.833333, 1,
10.2167, 0.833333, 0,
10.1479, 0.46875, 1,
10.1479, 0.46875, 0,
9.94167, 0.208333, 1,
9.94167, 0.208333, 0,
9.59792, 0.052083, 1,
9.59792, 0.052083, 0,
9.11667, 0, 1,
9.11667, 0, 0,
7.18333, 0, 1,
7.18333, 0, 0,
7.18333, 0.583333, 1,
7.18333, 0.583333, 0,
6.57604, 0.226042, 1,
6.57604, 0.226042, 0,
5.95417, -0.029167, 1,
5.95417, -0.029167, 0,
5.31771, -0.182292, 1,
5.31771, -0.182292, 0,
4.66667, -0.233333, 1,
4.66667, -0.233333, 0,
3.77969, -0.163542, 1,
3.77969, -0.163542, 0,
2.98542, 0.045833, 1,
2.98542, 0.045833, 0,
2.28385, 0.394792, 1,
2.28385, 0.394792, 0,
1.675, 0.883333, 1,
1.675, 0.883333, 0,
1.18281, 1.475, 1,
1.18281, 1.475, 0,
0.83125, 2.13333, 1,
0.83125, 2.13333, 0,
0.620313, 2.85833, 1,
0.620313, 2.85833, 0,
0.55, 3.65, 1,
0.55, 3.65, 0,
0.620313, 4.44115, 1,
0.620313, 4.44115, 0,
0.83125, 5.16458, 1,
0.83125, 5.16458, 0,
1.18281, 5.82031, 1,
1.18281, 5.82031, 0,
1.675, 6.40833, 1,
1.675, 6.40833, 0,
2.2849, 6.89323, 1,
2.2849, 6.89323, 0,
2.98958, 7.23958, 1,
2.98958, 7.23958, 0,
3.78906, 7.4474, 1,
3.78906, 7.4474, 0,
4.68333, 7.51667, 1,
4.68333, 7.51667, 0,
5.38333, 7.46458, 1,
5.38333, 7.46458, 0,
6.03333, 7.30833, 1,
6.03333, 7.30833, 0,
6.63333, 7.04792, 1,
6.63333, 7.04792, 0,
7.18333, 6.68333, 1,
7.18333, 6.68333, 0,
7.18333, 8.75, 1,
7.18333, 8.75, 0,
6.91667, 8.75, 1,
6.91667, 8.75, 0,
6.66563, 8.76094, 1,
6.66563, 8.76094, 0,
6.44583, 8.79375, 1,
6.44583, 8.79375, 0,
6.25729, 8.84844, 1,
6.25729, 8.84844, 0,
6.1, 8.925, 1,
6.1, 8.925, 0,
5.97604, 9.03177, 1,
5.97604, 9.03177, 0,
5.8875, 9.17708, 1,
5.8875, 9.17708, 0,
5.83437, 9.36094, 1,
5.83437, 9.36094, 0,
5.81667, 9.58333, 1,
5.81667, 9.58333, 0,
5.83437, 9.79844, 1,
5.83437, 9.79844, 0,
5.8875, 9.97708, 1,
5.8875, 9.97708, 0,
5.97604, 10.1193, 1,
5.97604, 10.1193, 0,
6.1, 10.225, 1,
6.1, 10.225, 0,
6.25729, 10.3016, 1,
6.25729, 10.3016, 0,
6.44583, 10.3562, 1,
6.44583, 10.3562, 0,
6.66563, 10.3891, 1,
6.66563, 10.3891, 0,
6.91667, 10.4, 1,
6.91667, 10.4, 0,
8.85, 10.4, 1,
8.85, 10.4, 0,
8.85, 1.66667, 1,
8.85, 1.66667, 0,
9.11667, 1.66667, 1,
9.11667, 1.66667, 0,
7.14062, 4.09271, 1,
7.0125, 4.52083, 1,
7.14062, 4.09271, 0,
7.0125, 4.52083, 0,
6.79896, 4.90104, 1,
6.79896, 4.90104, 0,
6.5, 5.23333, 1,
6.5, 5.23333, 0,
6.13021, 5.50313, 1,
6.13021, 5.50313, 0,
5.70417, 5.69583, 1,
5.70417, 5.69583, 0,
5.22188, 5.81146, 1,
5.22188, 5.81146, 0,
4.68333, 5.85, 1,
4.68333, 5.85, 0,
4.1526, 5.8099, 1,
4.1526, 5.8099, 0,
3.67708, 5.68958, 1,
3.67708, 5.68958, 0,
3.25677, 5.48906, 1,
3.25677, 5.48906, 0,
2.89167, 5.20833, 1,
2.89167, 5.20833, 0,
2.59635, 4.87031, 1,
2.59635, 4.87031, 0,
2.38542, 4.49792, 1,
2.38542, 4.49792, 0,
2.25885, 4.09115, 1,
2.25885, 4.09115, 0,
2.21667, 3.65, 1,
2.21667, 3.65, 0,
2.2599, 3.19479, 1,
2.2599, 3.19479, 0,
2.38958, 2.77917, 1,
2.38958, 2.77917, 0,
2.60573, 2.40313, 1,
2.60573, 2.40313, 0,
2.90833, 2.06667, 1,
2.90833, 2.06667, 0,
3.27969, 1.78958, 1,
3.27969, 1.78958, 0,
3.70208, 1.59167, 1,
3.70208, 1.59167, 0,
4.17552, 1.47292, 1,
4.17552, 1.47292, 0,
4.7, 1.43333, 1,
4.7, 1.43333, 0,
5.22448, 1.4724, 1,
5.22448, 1.4724, 0,
5.69792, 1.58958, 1,
5.69792, 1.58958, 0,
6.12031, 1.7849, 1,
6.12031, 1.7849, 0,
6.49167, 2.05833, 1,
6.49167, 2.05833, 0,
6.79427, 2.3901, 1,
6.79427, 2.3901, 0,
7.01042, 2.76042, 1,
7.01042, 2.76042, 0,
7.1401, 3.16927, 1,
7.1401, 3.16927, 0,
7.18333, 3.61667, 1,
7.18333, 3.61667, 0,
7.18333, 6.68333, 0,
8.85, 10.4, 0,
7.18333, 8.75, 0,
7.18333, 3.61667, 0,
7.14062, 4.09271, 0,
7.0125, 4.52083, 0,
6.79896, 4.90104, 0,
6.63333, 7.04792, 0,
6.5, 5.23333, 0,
6.13021, 5.50313, 0,
6.03333, 7.30833, 0,
5.70417, 5.69583, 0,
5.38333, 7.46458, 0,
5.22188, 5.81146, 0,
4.68333, 5.85, 0,
4.68333, 7.51667, 0,
4.1526, 5.8099, 0,
3.78906, 7.4474, 0,
3.67708, 5.68958, 0,
3.25677, 5.48906, 0,
2.98958, 7.23958, 0,
2.89167, 5.20833, 0,
2.59635, 4.87031, 0,
2.38542, 4.49792, 0,
2.2849, 6.89323, 0,
2.25885, 4.09115, 0,
2.21667, 3.65, 0,
1.675, 0.883333, 0,
1.675, 6.40833, 0,
0.55, 3.65, 0,
0.620313, 2.85833, 0,
0.620313, 4.44115, 0,
0.83125, 5.16458, 0,
0.83125, 2.13333, 0,
1.18281, 5.82031, 0,
1.18281, 1.475, 0,
7.1401, 3.16927, 0,
7.18333, 0.583333, 0,
7.18333, 0, 0,
8.85, 1.66667, 0,
9.11667, 0, 0,
9.11667, 1.66667, 0,
9.59792, 1.61458, 0,
6.12031, 1.7849, 0,
6.57604, 0.226042, 0,
6.49167, 2.05833, 0,
6.79427, 2.3901, 0,
7.01042, 2.76042, 0,
4.7, 1.43333, 0,
5.31771, -0.182292, 0,
5.22448, 1.4724, 0,
5.69792, 1.58958, 0,
5.95417, -0.029167, 0,
3.27969, 1.78958, 0,
3.77969, -0.163542, 0,
3.70208, 1.59167, 0,
4.17552, 1.47292, 0,
4.66667, -0.233333, 0,
2.98542, 0.045833, 0,
2.38958, 2.77917, 0,
2.28385, 0.394792, 0,
2.60573, 2.40313, 0,
2.90833, 2.06667, 0,
2.2599, 3.19479, 0,
9.59792, 0.052083, 0,
9.94167, 1.45833, 0,
9.94167, 0.208333, 0,
10.1479, 1.19792, 0,
10.1479, 0.46875, 0,
10.2167, 0.833333, 0,
5.81667, 9.58333, 0,
5.83437, 9.36094, 0,
5.83437, 9.79844, 0,
5.8875, 9.97708, 0,
5.8875, 9.17708, 0,
5.97604, 10.1193, 0,
5.97604, 9.03177, 0,
6.1, 10.225, 0,
6.1, 8.925, 0,
6.25729, 10.3016, 0,
6.25729, 8.84844, 0,
6.44583, 10.3562, 0,
6.44583, 8.79375, 0,
6.66563, 10.3891, 0,
6.66563, 8.76094, 0,
6.91667, 10.4, 0,
6.91667, 8.75, 0,
7.18333, 6.68333, 1,
8.85, 10.4, 1,
7.18333, 8.75, 1,
7.18333, 3.61667, 1,
7.14062, 4.09271, 1,
7.0125, 4.52083, 1,
6.79896, 4.90104, 1,
6.63333, 7.04792, 1,
6.5, 5.23333, 1,
6.13021, 5.50313, 1,
6.03333, 7.30833, 1,
5.70417, 5.69583, 1,
5.38333, 7.46458, 1,
5.22188, 5.81146, 1,
4.68333, 5.85, 1,
4.68333, 7.51667, 1,
4.1526, 5.8099, 1,
3.78906, 7.4474, 1,
3.67708, 5.68958, 1,
3.25677, 5.48906, 1,
2.98958, 7.23958, 1,
2.89167, 5.20833, 1,
2.59635, 4.87031, 1,
2.38542, 4.49792, 1,
2.2849, 6.89323, 1,
2.25885, 4.09115, 1,
2.21667, 3.65, 1,
1.675, 0.883333, 1,
1.675, 6.40833, 1,
0.55, 3.65, 1,
0.620313, 2.85833, 1,
0.620313, 4.44115, 1,
0.83125, 5.16458, 1,
0.83125, 2.13333, 1,
1.18281, 5.82031, 1,
1.18281, 1.475, 1,
7.1401, 3.16927, 1,
7.18333, 0.583333, 1,
7.18333, 0, 1,
8.85, 1.66667, 1,
9.11667, 0, 1,
9.11667, 1.66667, 1,
9.59792, 1.61458, 1,
6.12031, 1.7849, 1,
6.57604, 0.226042, 1,
6.49167, 2.05833, 1,
6.79427, 2.3901, 1,
7.01042, 2.76042, 1,
4.7, 1.43333, 1,
5.31771, -0.182292, 1,
5.22448, 1.4724, 1,
5.69792, 1.58958, 1,
5.95417, -0.029167, 1,
3.27969, 1.78958, 1,
3.77969, -0.163542, 1,
3.70208, 1.59167, 1,
4.17552, 1.47292, 1,
4.66667, -0.233333, 1,
2.98542, 0.045833, 1,
2.38958, 2.77917, 1,
2.28385, 0.394792, 1,
2.60573, 2.40313, 1,
2.90833, 2.06667, 1,
2.2599, 3.19479, 1,
9.59792, 0.052083, 1,
9.94167, 1.45833, 1,
9.94167, 0.208333, 1,
10.1479, 1.19792, 1,
10.1479, 0.46875, 1,
10.2167, 0.833333, 1,
5.81667, 9.58333, 1,
5.83437, 9.36094, 1,
5.83437, 9.79844, 1,
5.8875, 9.97708, 1,
5.8875, 9.17708, 1,
5.97604, 10.1193, 1,
5.97604, 9.03177, 1,
6.1, 10.225, 1,
6.1, 8.925, 1,
6.25729, 10.3016, 1,
6.25729, 8.84844, 1,
6.44583, 10.3562, 1,
6.44583, 8.79375, 1,
6.66563, 10.3891, 1,
6.66563, 8.76094, 1,
6.91667, 10.4, 1,
6.91667, 8.75, 1,
};

float normalsSmallAlpha3[348 * 3] = {
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
-1, 0, 0,
-1, 0, 0,
0.507085, -0.861896, 0,
0.507085, -0.861896, 0,
0.379658, -0.925127, 0,
0.379658, -0.925127, 0,
0.233915, -0.972257, 0,
0.233915, -0.972257, 0,
0.07816, -0.996941, 0,
0.07816, -0.996941, 0,
-0.078442, -0.996919, 0,
-0.078442, -0.996919, 0,
-0.254899, -0.966968, 0,
-0.254899, -0.966968, 0,
-0.445351, -0.895356, 0,
-0.445351, -0.895356, 0,
-0.625834, -0.779957, 0,
-0.625834, -0.779957, 0,
-0.768776, -0.639519, 0,
-0.768776, -0.639519, 0,
-0.882102, -0.471059, 0,
-0.882102, -0.471059, 0,
-0.960185, -0.279364, 0,
-0.960185, -0.279364, 0,
-0.996079, -0.088468, 0,
-0.996079, -0.088468, 0,
-0.996074, 0.088525, 0,
-0.996074, 0.088525, 0,
-0.960023, 0.27992, 0,
-0.960023, 0.27992, 0,
-0.881324, 0.472513, 0,
-0.881324, 0.472513, 0,
-0.766828, 0.641853, 0,
-0.766828, 0.641853, 0,
-0.622328, 0.782756, 0,
-0.622328, 0.782756, 0,
-0.441101, 0.897458, 0,
-0.441101, 0.897458, 0,
-0.251575, 0.967838, 0,
-0.251575, 0.967838, 0,
-0.077229, 0.997013, 0,
-0.077229, 0.997013, 0,
0.0742, 0.997243, 0,
0.0742, 0.997243, 0,
0.233727, 0.972302, 0,
0.233727, 0.972302, 0,
0.398144, 0.917323, 0,
0.398144, 0.917323, 0,
0.552513, 0.833505, 0,
0.552513, 0.833505, 0,
-1, 0, 0,
-1, 0, 0,
-0, -1, 0,
-0, -1, 0,
-0.043527, -0.999052, 0,
-0.043527, -0.999052, 0,
-0.147653, -0.989039, 0,
-0.147653, -0.989039, 0,
-0.278573, -0.960415, 0,
-0.278573, -0.960415, 0,
-0.437661, -0.89914, 0,
-0.437661, -0.89914, 0,
-0.652625, -0.757681, 0,
-0.652625, -0.757681, 0,
-0.853962, -0.520335, 0,
-0.853962, -0.520335, 0,
-0.960698, -0.277595, 0,
-0.960698, -0.277595, 0,
-0.996845, -0.079374, 0,
-0.996845, -0.079374, 0,
-0.996628, 0.082047, 0,
-0.996628, 0.082047, 0,
-0.958516, 0.28504, 0,
-0.958516, 0.28504, 0,
-0.848871, 0.528601, 0,
-0.848871, 0.528601, 0,
-0.648947, 0.760834, 0,
-0.648947, 0.760834, 0,
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
-0, 1, 0,
-0, 1, 0,
0.107597, 0.994195, 0,
0.107597, 0.994195, 0,
-0.958019, -0.286706, 0,
-0.871894, -0.489694, 0,
-0.958019, -0.286706, 0,
-0.871894, -0.489694, 0,
-0.74341, -0.668836, 0,
-0.74341, -0.668836, 0,
-0.589389, -0.80785, 0,
-0.589389, -0.80785, 0,
-0.412124, -0.911128, 0,
-0.412124, -0.911128, 0,
-0.233135, -0.972444, 0,
-0.233135, -0.972444, 0,
-0.071384, -0.997449, 0,
-0.071384, -0.997449, 0,
0.075349, -0.997157, 0,
0.075349, -0.997157, 0,
0.245283, -0.969452, 0,
0.245283, -0.969452, 0,
0.430585, -0.90255, 0,
0.430585, -0.90255, 0,
0.609547, -0.79275, 0,
0.609547, -0.79275, 0,
0.75308, -0.657929, 0,
0.75308, -0.657929, 0,
0.870109, -0.492859, 0,
0.870109, -0.492859, 0,
0.954849, -0.297091, 0,
0.954849, -0.297091, 0,
0.995458, -0.095197, 0,
0.995458, -0.095197, 0,
0.995521, 0.09454, 0,
0.995521, 0.09454, 0,
0.954608, 0.297866, 0,
0.954608, 0.297866, 0,
0.866984, 0.498336, 0,
0.866984, 0.498336, 0,
0.743523, 0.66871, 0,
0.743523, 0.66871, 0,
0.59802, 0.801481, 0,
0.59802, 0.801481, 0,
0.424291, 0.905526, 0,
0.424291, 0.905526, 0,
0.243289, 0.969954, 0,
0.243289, 0.969954, 0,
0.075258, 0.997164, 0,
0.075258, 0.997164, 0,
-0.074273, 0.997238, 0,
-0.074273, 0.997238, 0,
-0.240274, 0.970705, 0,
-0.240274, 0.970705, 0,
-0.419697, 0.907664, 0,
-0.419697, 0.907664, 0,
-0.592929, 0.805254, 0,
-0.592929, 0.805254, 0,
-0.738837, 0.673884, 0,
-0.738837, 0.673884, 0,
-0.863647, 0.504098, 0,
-0.863647, 0.504098, 0,
-0.953197, 0.302352, 0,
-0.953197, 0.302352, 0,
-0.995364, 0.096176, 0,
-0.995364, 0.096176, 0,
-0.996, -0.089357, 0,
-0.996, -0.089357, 0,
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

unsigned int indicesSmallAlpha3[1044] = {
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
108, 0, 109,
0, 2, 109,
110, 111, 112,
111, 113, 112,
111, 114, 113,
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
172, 110, 173,
110, 112, 173,
174, 175, 176,
174, 177, 175,
174, 178, 177,
174, 179, 178,
174, 180, 179,
174, 181, 180,
180, 181, 182,
181, 183, 182,
181, 184, 183,
184, 185, 183,
184, 186, 185,
186, 187, 185,
186, 188, 187,
186, 189, 188,
189, 190, 188,
189, 191, 190,
191, 192, 190,
191, 193, 192,
194, 193, 191,
194, 195, 193,
194, 196, 195,
194, 197, 196,
194, 198, 197,
198, 199, 197,
198, 200, 199,
198, 201, 200,
198, 202, 201,
203, 204, 205,
204, 206, 205,
204, 207, 206,
207, 208, 206,
207, 209, 208,
209, 202, 208,
209, 201, 202,
210, 211, 177,
211, 175, 177,
211, 212, 175,
212, 213, 175,
212, 214, 213,
214, 215, 213,
214, 216, 215,
217, 218, 219,
218, 220, 219,
218, 211, 220,
211, 221, 220,
211, 210, 221,
222, 223, 224,
223, 225, 224,
223, 226, 225,
226, 217, 225,
226, 218, 217,
227, 228, 229,
228, 230, 229,
228, 231, 230,
231, 222, 230,
231, 223, 222,
232, 233, 234,
232, 235, 233,
232, 236, 235,
232, 227, 236,
232, 228, 227,
234, 200, 201,
234, 237, 200,
234, 233, 237,
214, 238, 216,
238, 239, 216,
238, 240, 239,
240, 241, 239,
240, 242, 241,
242, 243, 241,
244, 245, 246,
245, 247, 246,
245, 248, 247,
248, 249, 247,
248, 250, 249,
250, 251, 249,
250, 252, 251,
252, 253, 251,
252, 254, 253,
254, 255, 253,
254, 256, 255,
256, 257, 255,
256, 258, 257,
258, 259, 257,
258, 260, 259,
260, 175, 259,
260, 176, 175,
261, 262, 263,
261, 264, 262,
261, 265, 264,
261, 266, 265,
261, 267, 266,
261, 268, 267,
267, 268, 269,
268, 270, 269,
268, 271, 270,
271, 272, 270,
271, 273, 272,
273, 274, 272,
273, 275, 274,
273, 276, 275,
276, 277, 275,
276, 278, 277,
278, 279, 277,
278, 280, 279,
281, 280, 278,
281, 282, 280,
281, 283, 282,
281, 284, 283,
281, 285, 284,
285, 286, 284,
285, 287, 286,
285, 288, 287,
285, 289, 288,
290, 291, 292,
291, 293, 292,
291, 294, 293,
294, 295, 293,
294, 296, 295,
296, 289, 295,
296, 288, 289,
297, 298, 264,
298, 262, 264,
298, 299, 262,
299, 300, 262,
299, 301, 300,
301, 302, 300,
301, 303, 302,
304, 305, 306,
305, 307, 306,
305, 298, 307,
298, 308, 307,
298, 297, 308,
309, 310, 311,
310, 312, 311,
310, 313, 312,
313, 304, 312,
313, 305, 304,
314, 315, 316,
315, 317, 316,
315, 318, 317,
318, 309, 317,
318, 310, 309,
319, 320, 321,
319, 322, 320,
319, 323, 322,
319, 314, 323,
319, 315, 314,
321, 287, 288,
321, 324, 287,
321, 320, 324,
301, 325, 303,
325, 326, 303,
325, 327, 326,
327, 328, 326,
327, 329, 328,
329, 330, 328,
331, 332, 333,
332, 334, 333,
332, 335, 334,
335, 336, 334,
335, 337, 336,
337, 338, 336,
337, 339, 338,
339, 340, 338,
339, 341, 340,
341, 342, 340,
341, 343, 342,
343, 344, 342,
343, 345, 344,
345, 346, 344,
345, 347, 346,
347, 262, 346,
347, 263, 262,
};

void drawSmallAlpha3()
{
drawTrianglesVerticesNormalsIndices(1044, verticesSmallAlpha3, normalsSmallAlpha3, indicesSmallAlpha3);
}
