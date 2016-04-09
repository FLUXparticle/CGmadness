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

#include "BigAlpha15.hpp"

#include "hw/varray.hpp"

float verticesBigAlpha15[332 * 3] = {
8.81042, 6.34375, 1,
8.79167, 6.125, 1,
8.81042, 6.34375, 0,
8.79167, 6.125, 0,
8.76042, 5.91042, 1,
8.76042, 5.91042, 0,
8.71667, 5.7, 1,
8.71667, 5.7, 0,
8.65521, 5.4901, 1,
8.65521, 5.4901, 0,
8.57083, 5.27708, 1,
8.57083, 5.27708, 0,
8.46354, 5.06094, 1,
8.46354, 5.06094, 0,
8.33333, 4.84167, 1,
8.33333, 4.84167, 0,
8.18281, 4.62813, 1,
8.18281, 4.62813, 0,
8.01458, 4.42917, 1,
8.01458, 4.42917, 0,
7.82865, 4.24479, 1,
7.82865, 4.24479, 0,
7.625, 4.075, 1,
7.625, 4.075, 0,
7.39479, 3.91979, 1,
7.39479, 3.91979, 0,
7.12917, 3.77917, 1,
7.12917, 3.77917, 0,
6.82812, 3.65313, 1,
6.82812, 3.65313, 0,
6.49167, 3.54167, 1,
6.49167, 3.54167, 0,
6.12448, 3.45052, 1,
6.12448, 3.45052, 0,
5.73125, 3.38542, 1,
5.73125, 3.38542, 0,
5.31198, 3.34635, 1,
5.31198, 3.34635, 0,
4.86667, 3.33333, 1,
4.86667, 3.33333, 0,
3.23333, 3.33333, 1,
3.23333, 3.33333, 0,
3.23333, 1.66667, 1,
3.23333, 1.66667, 0,
4.88333, 1.66667, 1,
4.88333, 1.66667, 0,
5.36458, 1.61458, 1,
5.36458, 1.61458, 0,
5.70833, 1.45833, 1,
5.70833, 1.45833, 0,
5.91458, 1.19792, 1,
5.91458, 1.19792, 0,
5.98333, 0.833333, 1,
5.98333, 0.833333, 0,
5.91458, 0.46875, 1,
5.91458, 0.46875, 0,
5.70833, 0.208333, 1,
5.70833, 0.208333, 0,
5.36458, 0.052083, 1,
5.36458, 0.052083, 0,
4.88333, 0, 1,
4.88333, 0, 0,
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
5.2, 9.71667, 1,
5.2, 9.71667, 0,
5.95625, 9.66146, 1,
5.95625, 9.66146, 0,
6.64167, 9.49583, 1,
6.64167, 9.49583, 0,
7.25625, 9.21979, 1,
7.25625, 9.21979, 0,
7.8, 8.83333, 1,
7.8, 8.83333, 0,
8.24479, 8.36042, 1,
8.24479, 8.36042, 0,
8.5625, 7.825, 1,
8.5625, 7.825, 0,
8.75313, 7.22708, 1,
8.75313, 7.22708, 0,
8.81667, 6.56667, 1,
8.81667, 6.56667, 0,
3.23333, 5, 1,
5.2, 5, 1,
3.23333, 5, 0,
5.2, 5, 0,
5.6099, 5.02552, 1,
5.6099, 5.02552, 0,
5.97292, 5.10208, 1,
5.97292, 5.10208, 0,
6.28906, 5.22969, 1,
6.28906, 5.22969, 0,
6.55833, 5.40833, 1,
6.55833, 5.40833, 0,
6.77344, 5.63073, 1,
6.77344, 5.63073, 0,
6.92708, 5.88958, 1,
6.92708, 5.88958, 0,
7.01927, 6.1849, 1,
7.01927, 6.1849, 0,
7.05, 6.51667, 1,
7.05, 6.51667, 0,
7.01927, 6.85573, 1,
7.01927, 6.85573, 0,
6.92708, 7.15625, 1,
6.92708, 7.15625, 0,
6.77344, 7.41823, 1,
6.77344, 7.41823, 0,
6.55833, 7.64167, 1,
6.55833, 7.64167, 0,
6.28906, 7.82031, 1,
6.28906, 7.82031, 0,
5.97292, 7.94792, 1,
5.97292, 7.94792, 0,
5.6099, 8.02448, 1,
5.6099, 8.02448, 0,
5.2, 8.05, 1,
5.2, 8.05, 0,
3.23333, 8.05, 1,
3.23333, 8.05, 0,
3.23333, 1.66667, 0,
4.88333, 0, 0,
4.88333, 1.66667, 0,
5.36458, 1.61458, 0,
5.36458, 0.052083, 0,
5.70833, 1.45833, 0,
5.70833, 0.208333, 0,
5.91458, 1.19792, 0,
5.91458, 0.46875, 0,
5.98333, 0.833333, 0,
5.31198, 3.34635, 0,
5.2, 5, 0,
4.86667, 3.33333, 0,
3.23333, 3.33333, 0,
3.23333, 5, 0,
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
7.82865, 4.24479, 0,
7.8, 8.83333, 0,
7.625, 4.075, 0,
7.39479, 3.91979, 0,
7.25625, 9.21979, 0,
7.12917, 3.77917, 0,
7.05, 6.51667, 0,
7.01927, 6.1849, 0,
6.55833, 5.40833, 0,
6.82812, 3.65313, 0,
6.77344, 5.63073, 0,
6.92708, 5.88958, 0,
5.73125, 3.38542, 0,
5.6099, 5.02552, 0,
5.97292, 5.10208, 0,
6.12448, 3.45052, 0,
6.28906, 5.22969, 0,
6.49167, 3.54167, 0,
8.81042, 6.34375, 0,
8.81667, 6.56667, 0,
8.79167, 6.125, 0,
8.76042, 5.91042, 0,
8.75313, 7.22708, 0,
8.71667, 5.7, 0,
8.65521, 5.4901, 0,
8.57083, 5.27708, 0,
8.5625, 7.825, 0,
8.46354, 5.06094, 0,
8.33333, 4.84167, 0,
8.24479, 8.36042, 0,
8.18281, 4.62813, 0,
8.01458, 4.42917, 0,
7.01927, 6.85573, 0,
6.92708, 7.15625, 0,
6.77344, 7.41823, 0,
6.64167, 9.49583, 0,
6.55833, 7.64167, 0,
6.28906, 7.82031, 0,
5.97292, 7.94792, 0,
5.95625, 9.66146, 0,
5.6099, 8.02448, 0,
5.2, 8.05, 0,
5.2, 9.71667, 0,
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
3.23333, 1.66667, 1,
4.88333, 0, 1,
4.88333, 1.66667, 1,
5.36458, 1.61458, 1,
5.36458, 0.052083, 1,
5.70833, 1.45833, 1,
5.70833, 0.208333, 1,
5.91458, 1.19792, 1,
5.91458, 0.46875, 1,
5.98333, 0.833333, 1,
5.31198, 3.34635, 1,
5.2, 5, 1,
4.86667, 3.33333, 1,
3.23333, 3.33333, 1,
3.23333, 5, 1,
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
7.82865, 4.24479, 1,
7.8, 8.83333, 1,
7.625, 4.075, 1,
7.39479, 3.91979, 1,
7.25625, 9.21979, 1,
7.12917, 3.77917, 1,
7.05, 6.51667, 1,
7.01927, 6.1849, 1,
6.55833, 5.40833, 1,
6.82812, 3.65313, 1,
6.77344, 5.63073, 1,
6.92708, 5.88958, 1,
5.73125, 3.38542, 1,
5.6099, 5.02552, 1,
5.97292, 5.10208, 1,
6.12448, 3.45052, 1,
6.28906, 5.22969, 1,
6.49167, 3.54167, 1,
8.81042, 6.34375, 1,
8.81667, 6.56667, 1,
8.79167, 6.125, 1,
8.76042, 5.91042, 1,
8.75313, 7.22708, 1,
8.71667, 5.7, 1,
8.65521, 5.4901, 1,
8.57083, 5.27708, 1,
8.5625, 7.825, 1,
8.46354, 5.06094, 1,
8.33333, 4.84167, 1,
8.24479, 8.36042, 1,
8.18281, 4.62813, 1,
8.01458, 4.42917, 1,
7.01927, 6.85573, 1,
6.92708, 7.15625, 1,
6.77344, 7.41823, 1,
6.64167, 9.49583, 1,
6.55833, 7.64167, 1,
6.28906, 7.82031, 1,
5.97292, 7.94792, 1,
5.95625, 9.66146, 1,
5.6099, 8.02448, 1,
5.2, 8.05, 1,
5.2, 9.71667, 1,
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
};

float normalsBigAlpha15[332 * 3] = {
0.996347, -0.085401, 0,
0.989562, -0.144111, 0,
0.996347, -0.085401, 0,
0.989562, -0.144111, 0,
0.979061, -0.203567, 0,
0.979061, -0.203567, 0,
0.959706, -0.281006, 0,
0.959706, -0.281006, 0,
0.929726, -0.368253, 0,
0.929726, -0.368253, 0,
0.895718, -0.444622, 0,
0.895718, -0.444622, 0,
0.859827, -0.510586, 0,
0.859827, -0.510586, 0,
0.817354, -0.576135, 0,
0.817354, -0.576135, 0,
0.763614, -0.645673, 0,
0.763614, -0.645673, 0,
0.704117, -0.710084, 0,
0.704117, -0.710084, 0,
0.640378, -0.76806, 0,
0.640378, -0.76806, 0,
0.559022, -0.829153, 0,
0.559022, -0.829153, 0,
0.467888, -0.883788, 0,
0.467888, -0.883788, 0,
0.386201, -0.922415, 0,
0.386201, -0.922415, 0,
0.314464, -0.949269, 0,
0.314464, -0.949269, 0,
0.240916, -0.970546, 0,
0.240916, -0.970546, 0,
0.163339, -0.98657, 0,
0.163339, -0.98657, 0,
0.092766, -0.995688, 0,
0.092766, -0.995688, 0,
0.029227, -0.999573, 0,
0.029227, -0.999573, 0,
-0, -1, 0,
-0, -1, 0,
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
0.072809, 0.997346, 0,
0.072809, 0.997346, 0,
0.234881, 0.972024, 0,
0.234881, 0.972024, 0,
0.409722, 0.912211, 0,
0.409722, 0.912211, 0,
0.579316, 0.815103, 0,
0.579316, 0.815103, 0,
0.728435, 0.685114, 0,
0.728435, 0.685114, 0,
0.859992, 0.510307, 0,
0.859992, 0.510307, 0,
0.952751, 0.303752, 0,
0.952751, 0.303752, 0,
0.995403, 0.095772, 0,
0.995403, 0.095772, 0,
0.999607, -0.028026, 0,
0.999607, -0.028026, 0,
-0, 1, 0,
-0.062141, 0.998067, 0,
-0, 1, 0,
-0.062141, 0.998067, 0,
-0.206364, 0.978475, 0,
-0.206364, 0.978475, 0,
-0.374286, 0.927313, 0,
-0.374286, 0.927313, 0,
-0.552839, 0.833288, 0,
-0.552839, 0.833288, 0,
-0.718792, 0.695225, 0,
-0.718792, 0.695225, 0,
-0.859926, 0.510419, 0,
-0.859926, 0.510419, 0,
-0.95457, 0.297987, 0,
-0.95457, 0.297987, 0,
-0.995738, 0.092227, 0,
-0.995738, 0.092227, 0,
-0.995918, -0.09026, 0,
-0.995918, -0.09026, 0,
-0.956029, -0.293271, 0,
-0.956029, -0.293271, 0,
-0.862595, -0.505896, 0,
-0.862595, -0.505896, 0,
-0.720413, -0.693545, 0,
-0.720413, -0.693545, 0,
-0.552839, -0.833288, 0,
-0.552839, -0.833288, 0,
-0.374286, -0.927313, 0,
-0.374286, -0.927313, 0,
-0.206364, -0.978475, 0,
-0.206364, -0.978475, 0,
-0.062141, -0.998067, 0,
-0.062141, -0.998067, 0,
-0, -1, 0,
-0, -1, 0,
1, 0, 0,
1, 0, 0,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
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
};

unsigned int indicesBigAlpha15[996] = {
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
126, 0, 127,
0, 2, 127,
128, 129, 130,
129, 131, 130,
129, 132, 131,
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
164, 128, 165,
128, 130, 165,
166, 167, 168,
167, 169, 168,
167, 170, 169,
170, 171, 169,
170, 172, 171,
172, 173, 171,
172, 174, 173,
174, 175, 173,
176, 177, 178,
177, 179, 178,
177, 180, 179,
180, 166, 179,
180, 181, 166,
181, 182, 166,
181, 183, 182,
184, 185, 186,
185, 187, 186,
185, 188, 187,
188, 189, 187,
188, 190, 189,
190, 191, 189,
190, 192, 191,
192, 182, 191,
192, 167, 182,
167, 166, 182,
193, 194, 195,
194, 196, 195,
194, 197, 196,
197, 198, 196,
197, 199, 198,
199, 200, 198,
201, 202, 203,
202, 204, 203,
202, 198, 204,
198, 200, 204,
176, 205, 206,
205, 207, 206,
205, 208, 207,
208, 209, 207,
208, 210, 209,
210, 201, 209,
210, 202, 201,
211, 212, 213,
212, 214, 213,
212, 215, 214,
215, 216, 214,
215, 217, 216,
218, 219, 220,
218, 215, 219,
218, 217, 215,
221, 222, 223,
221, 219, 222,
221, 220, 219,
222, 224, 223,
222, 193, 224,
222, 194, 193,
197, 225, 199,
197, 226, 225,
197, 227, 226,
197, 228, 227,
228, 229, 227,
228, 230, 229,
228, 231, 230,
228, 232, 231,
231, 232, 233,
232, 234, 233,
232, 235, 234,
235, 181, 234,
235, 183, 181,
236, 237, 238,
236, 239, 237,
236, 240, 239,
236, 235, 240,
236, 183, 235,
241, 242, 243,
242, 244, 243,
242, 238, 244,
238, 245, 244,
238, 237, 245,
246, 247, 248,
246, 241, 247,
246, 242, 241,
176, 206, 177,
249, 250, 251,
250, 252, 251,
250, 253, 252,
253, 254, 252,
253, 255, 254,
255, 256, 254,
255, 257, 256,
257, 258, 256,
259, 260, 261,
260, 262, 261,
260, 263, 262,
263, 249, 262,
263, 264, 249,
264, 265, 249,
264, 266, 265,
267, 268, 269,
268, 270, 269,
268, 271, 270,
271, 272, 270,
271, 273, 272,
273, 274, 272,
273, 275, 274,
275, 265, 274,
275, 250, 265,
250, 249, 265,
276, 277, 278,
277, 279, 278,
277, 280, 279,
280, 281, 279,
280, 282, 281,
282, 283, 281,
284, 285, 286,
285, 287, 286,
285, 281, 287,
281, 283, 287,
259, 288, 289,
288, 290, 289,
288, 291, 290,
291, 292, 290,
291, 293, 292,
293, 284, 292,
293, 285, 284,
294, 295, 296,
295, 297, 296,
295, 298, 297,
298, 299, 297,
298, 300, 299,
301, 302, 303,
301, 298, 302,
301, 300, 298,
304, 305, 306,
304, 302, 305,
304, 303, 302,
305, 307, 306,
305, 276, 307,
305, 277, 276,
280, 308, 282,
280, 309, 308,
280, 310, 309,
280, 311, 310,
311, 312, 310,
311, 313, 312,
311, 314, 313,
311, 315, 314,
314, 315, 316,
315, 317, 316,
315, 318, 317,
318, 264, 317,
318, 266, 264,
319, 320, 321,
319, 322, 320,
319, 323, 322,
319, 318, 323,
319, 266, 318,
324, 325, 326,
325, 327, 326,
325, 321, 327,
321, 328, 327,
321, 320, 328,
329, 330, 331,
329, 324, 330,
329, 325, 324,
259, 289, 260,
};

void drawBigAlpha15()
{
drawTrianglesVerticesNormalsIndices(996, verticesBigAlpha15, normalsBigAlpha15, indicesBigAlpha15);
}
