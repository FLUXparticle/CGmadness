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

#include "Digits6.hpp"

#include "hw/varray.hpp"

float verticesDigits6[464 * 3] = {
5.09531, -0.229687, 1,
4.69792, -0.16875, 1,
5.09531, -0.229687, 0,
4.69792, -0.16875, 0,
4.32448, -0.067187, 1,
4.32448, -0.067187, 0,
3.975, 0.075, 1,
3.975, 0.075, 0,
3.65417, 0.248438, 1,
3.65417, 0.248438, 0,
3.36667, 0.44375, 1,
3.36667, 0.44375, 0,
3.1125, 0.660937, 1,
3.1125, 0.660937, 0,
2.89167, 0.9, 1,
2.89167, 0.9, 0,
2.69635, 1.16146, 1,
2.69635, 1.16146, 0,
2.51875, 1.44583, 1,
2.51875, 1.44583, 0,
2.35885, 1.75312, 1,
2.35885, 1.75312, 0,
2.21667, 2.08333, 1,
2.21667, 2.08333, 0,
2.09375, 2.42292, 1,
2.09375, 2.42292, 0,
1.99167, 2.75833, 1,
1.99167, 2.75833, 0,
1.91042, 3.08958, 1,
1.91042, 3.08958, 0,
1.85, 3.41667, 1,
1.85, 3.41667, 0,
1.80625, 3.74375, 1,
1.80625, 3.74375, 0,
1.775, 4.075, 1,
1.775, 4.075, 0,
1.75625, 4.41042, 1,
1.75625, 4.41042, 0,
1.75, 4.75, 1,
1.75, 4.75, 0,
1.83021, 5.81354, 1,
1.83021, 5.81354, 0,
2.07083, 6.80417, 1,
2.07083, 6.80417, 0,
2.47187, 7.72188, 1,
2.47187, 7.72188, 0,
3.03333, 8.56667, 1,
3.03333, 8.56667, 0,
3.91563, 9.47083, 1,
3.91563, 9.47083, 0,
4.87917, 10.1167, 1,
4.87917, 10.1167, 0,
5.92396, 10.5042, 1,
5.92396, 10.5042, 0,
7.05, 10.6333, 1,
7.05, 10.6333, 0,
7.46979, 10.6146, 1,
7.46979, 10.6146, 0,
7.84583, 10.5583, 1,
7.84583, 10.5583, 0,
8.17813, 10.4646, 1,
8.17813, 10.4646, 0,
8.46667, 10.3333, 1,
8.46667, 10.3333, 0,
8.7, 10.1719, 1,
8.7, 10.1719, 0,
8.86667, 9.9875, 1,
8.86667, 9.9875, 0,
8.96667, 9.78021, 1,
8.96667, 9.78021, 0,
9, 9.55, 1,
9, 9.55, 0,
8.98438, 9.38125, 1,
8.98438, 9.38125, 0,
8.9375, 9.225, 1,
8.9375, 9.225, 0,
8.85938, 9.08125, 1,
8.85938, 9.08125, 0,
8.75, 8.95, 1,
8.75, 8.95, 0,
8.61875, 8.84062, 1,
8.61875, 8.84062, 0,
8.475, 8.7625, 1,
8.475, 8.7625, 0,
8.31875, 8.71562, 1,
8.31875, 8.71562, 0,
8.15, 8.7, 1,
8.15, 8.7, 0,
8.08281, 8.70677, 1,
8.08281, 8.70677, 0,
7.98125, 8.72708, 1,
7.98125, 8.72708, 0,
7.84531, 8.76094, 1,
7.84531, 8.76094, 0,
7.675, 8.80833, 1,
7.675, 8.80833, 0,
7.50156, 8.85573, 1,
7.50156, 8.85573, 0,
7.35625, 8.88958, 1,
7.35625, 8.88958, 0,
7.23906, 8.9099, 1,
7.23906, 8.9099, 0,
7.15, 8.91667, 1,
7.15, 8.91667, 0,
6.50885, 8.85469, 1,
6.50885, 8.85469, 0,
5.90208, 8.66875, 1,
5.90208, 8.66875, 0,
5.32969, 8.35885, 1,
5.32969, 8.35885, 0,
4.79167, 7.925, 1,
4.79167, 7.925, 0,
4.31823, 7.39531, 1,
4.31823, 7.39531, 0,
3.93958, 6.79792, 1,
3.93958, 6.79792, 0,
3.65573, 6.13281, 1,
3.65573, 6.13281, 0,
3.46667, 5.4, 1,
3.46667, 5.4, 0,
3.76198, 5.69375, 1,
3.76198, 5.69375, 0,
4.04792, 5.94167, 1,
4.04792, 5.94167, 0,
4.32448, 6.14375, 1,
4.32448, 6.14375, 0,
4.59167, 6.3, 1,
4.59167, 6.3, 0,
4.86094, 6.41667, 1,
4.86094, 6.41667, 0,
5.14375, 6.5, 1,
5.14375, 6.5, 0,
5.4401, 6.55, 1,
5.4401, 6.55, 0,
5.75, 6.56667, 1,
5.75, 6.56667, 0,
6.38385, 6.50417, 1,
6.38385, 6.50417, 0,
6.96875, 6.31667, 1,
6.96875, 6.31667, 0,
7.50469, 6.00417, 1,
7.50469, 6.00417, 0,
7.99167, 5.56667, 1,
7.99167, 5.56667, 0,
8.39635, 5.04063, 1,
8.39635, 5.04063, 0,
8.68542, 4.4625, 1,
8.68542, 4.4625, 0,
8.85885, 3.83229, 1,
8.85885, 3.83229, 0,
8.91667, 3.15, 1,
8.91667, 3.15, 0,
8.86042, 2.4276, 1,
8.86042, 2.4276, 0,
8.69167, 1.77708, 1,
8.69167, 1.77708, 0,
8.41042, 1.19844, 1,
8.41042, 1.19844, 0,
8.01667, 0.691667, 1,
8.01667, 0.691667, 0,
7.52292, 0.279687, 1,
7.52292, 0.279687, 0,
6.94167, -0.014583, 1,
6.94167, -0.014583, 0,
6.27292, -0.191146, 1,
6.27292, -0.191146, 0,
5.51667, -0.25, 1,
5.51667, -0.25, 0,
3.74271, 2.97865, 1,
3.8875, 2.54792, 1,
3.74271, 2.97865, 0,
3.8875, 2.54792, 0,
4.06771, 2.19115, 1,
4.06771, 2.19115, 0,
4.28333, 1.90833, 1,
4.28333, 1.90833, 0,
4.53854, 1.69323, 1,
4.53854, 1.69323, 0,
4.8375, 1.53958, 1,
4.8375, 1.53958, 0,
5.18021, 1.4474, 1,
5.18021, 1.4474, 0,
5.56667, 1.41667, 1,
5.56667, 1.41667, 0,
5.9375, 1.44583, 1,
5.9375, 1.44583, 0,
6.26667, 1.53333, 1,
6.26667, 1.53333, 0,
6.55417, 1.67917, 1,
6.55417, 1.67917, 0,
6.8, 1.88333, 1,
6.8, 1.88333, 0,
6.99687, 2.13854, 1,
6.99687, 2.13854, 0,
7.1375, 2.4375, 1,
7.1375, 2.4375, 0,
7.22188, 2.78021, 1,
7.22188, 2.78021, 0,
7.25, 3.16667, 1,
7.25, 3.16667, 0,
7.22083, 3.51198, 1,
7.22083, 3.51198, 0,
7.13333, 3.83125, 1,
7.13333, 3.83125, 0,
6.9875, 4.12448, 1,
6.9875, 4.12448, 0,
6.78333, 4.39167, 1,
6.78333, 4.39167, 0,
6.53646, 4.61406, 1,
6.53646, 4.61406, 0,
6.2625, 4.77292, 1,
6.2625, 4.77292, 0,
5.96146, 4.86823, 1,
5.96146, 4.86823, 0,
5.63333, 4.9, 1,
5.63333, 4.9, 0,
5.36458, 4.875, 1,
5.36458, 4.875, 0,
5.09167, 4.8, 1,
5.09167, 4.8, 0,
4.81458, 4.675, 1,
4.81458, 4.675, 0,
4.53333, 4.5, 1,
4.53333, 4.5, 0,
4.26458, 4.28646, 1,
4.26458, 4.28646, 0,
4.025, 4.04583, 1,
4.025, 4.04583, 0,
3.81458, 3.77813, 1,
3.81458, 3.77813, 0,
3.63333, 3.48333, 1,
3.63333, 3.48333, 0,
8.91667, 3.15, 0,
8.85885, 3.83229, 0,
8.86042, 2.4276, 0,
8.69167, 1.77708, 0,
8.68542, 4.4625, 0,
8.41042, 1.19844, 0,
8.39635, 5.04063, 0,
8.01667, 0.691667, 0,
7.99167, 5.56667, 0,
7.52292, 0.279687, 0,
7.50469, 6.00417, 0,
7.25, 3.16667, 0,
7.22083, 3.51198, 0,
6.99687, 2.13854, 0,
6.94167, -0.014583, 0,
7.1375, 2.4375, 0,
7.22188, 2.78021, 0,
5.9375, 1.44583, 0,
6.27292, -0.191146, 0,
6.26667, 1.53333, 0,
6.55417, 1.67917, 0,
6.8, 1.88333, 0,
4.06771, 2.19115, 0,
4.32448, -0.067187, 0,
4.28333, 1.90833, 0,
4.53854, 1.69323, 0,
4.69792, -0.16875, 0,
4.8375, 1.53958, 0,
5.09531, -0.229687, 0,
5.18021, 1.4474, 0,
5.51667, -0.25, 0,
5.56667, 1.41667, 0,
3.975, 0.075, 0,
3.74271, 2.97865, 0,
3.65417, 0.248438, 0,
3.8875, 2.54792, 0,
3.63333, 3.48333, 0,
3.46667, 5.4, 0,
3.76198, 5.69375, 0,
3.81458, 3.77813, 0,
3.36667, 0.44375, 0,
3.1125, 0.660937, 0,
3.03333, 8.56667, 0,
3.91563, 9.47083, 0,
3.65573, 6.13281, 0,
7.13333, 3.83125, 0,
6.9875, 4.12448, 0,
6.96875, 6.31667, 0,
6.78333, 4.39167, 0,
6.53646, 4.61406, 0,
6.38385, 6.50417, 0,
6.2625, 4.77292, 0,
5.96146, 4.86823, 0,
5.75, 6.56667, 0,
4.025, 4.04583, 0,
4.26458, 4.28646, 0,
4.04792, 5.94167, 0,
4.32448, 6.14375, 0,
4.53333, 4.5, 0,
4.59167, 6.3, 0,
4.81458, 4.675, 0,
4.86094, 6.41667, 0,
5.09167, 4.8, 0,
5.14375, 6.5, 0,
5.36458, 4.875, 0,
5.4401, 6.55, 0,
5.63333, 4.9, 0,
9, 9.55, 0,
8.96667, 9.78021, 0,
8.98438, 9.38125, 0,
8.9375, 9.225, 0,
8.86667, 9.9875, 0,
8.85938, 9.08125, 0,
8.75, 8.95, 0,
8.7, 10.1719, 0,
8.61875, 8.84062, 0,
8.475, 8.7625, 0,
8.46667, 10.3333, 0,
8.31875, 8.71562, 0,
8.17813, 10.4646, 0,
8.15, 8.7, 0,
8.08281, 8.70677, 0,
7.98125, 8.72708, 0,
7.84583, 10.5583, 0,
7.84531, 8.76094, 0,
7.675, 8.80833, 0,
7.50156, 8.85573, 0,
7.46979, 10.6146, 0,
7.35625, 8.88958, 0,
7.23906, 8.9099, 0,
7.15, 8.91667, 0,
7.05, 10.6333, 0,
6.50885, 8.85469, 0,
5.92396, 10.5042, 0,
5.90208, 8.66875, 0,
5.32969, 8.35885, 0,
4.87917, 10.1167, 0,
4.79167, 7.925, 0,
4.31823, 7.39531, 0,
3.93958, 6.79792, 0,
2.89167, 0.9, 0,
2.69635, 1.16146, 0,
2.51875, 1.44583, 0,
2.47187, 7.72188, 0,
2.35885, 1.75312, 0,
2.21667, 2.08333, 0,
2.09375, 2.42292, 0,
2.07083, 6.80417, 0,
1.99167, 2.75833, 0,
1.91042, 3.08958, 0,
1.85, 3.41667, 0,
1.83021, 5.81354, 0,
1.80625, 3.74375, 0,
1.775, 4.075, 0,
1.75625, 4.41042, 0,
1.75, 4.75, 0,
8.91667, 3.15, 1,
8.85885, 3.83229, 1,
8.86042, 2.4276, 1,
8.69167, 1.77708, 1,
8.68542, 4.4625, 1,
8.41042, 1.19844, 1,
8.39635, 5.04063, 1,
8.01667, 0.691667, 1,
7.99167, 5.56667, 1,
7.52292, 0.279687, 1,
7.50469, 6.00417, 1,
7.25, 3.16667, 1,
7.22083, 3.51198, 1,
6.99687, 2.13854, 1,
6.94167, -0.014583, 1,
7.1375, 2.4375, 1,
7.22188, 2.78021, 1,
5.9375, 1.44583, 1,
6.27292, -0.191146, 1,
6.26667, 1.53333, 1,
6.55417, 1.67917, 1,
6.8, 1.88333, 1,
4.06771, 2.19115, 1,
4.32448, -0.067187, 1,
4.28333, 1.90833, 1,
4.53854, 1.69323, 1,
4.69792, -0.16875, 1,
4.8375, 1.53958, 1,
5.09531, -0.229687, 1,
5.18021, 1.4474, 1,
5.51667, -0.25, 1,
5.56667, 1.41667, 1,
3.975, 0.075, 1,
3.74271, 2.97865, 1,
3.65417, 0.248438, 1,
3.8875, 2.54792, 1,
3.63333, 3.48333, 1,
3.46667, 5.4, 1,
3.76198, 5.69375, 1,
3.81458, 3.77813, 1,
3.36667, 0.44375, 1,
3.1125, 0.660937, 1,
3.03333, 8.56667, 1,
3.91563, 9.47083, 1,
3.65573, 6.13281, 1,
7.13333, 3.83125, 1,
6.9875, 4.12448, 1,
6.96875, 6.31667, 1,
6.78333, 4.39167, 1,
6.53646, 4.61406, 1,
6.38385, 6.50417, 1,
6.2625, 4.77292, 1,
5.96146, 4.86823, 1,
5.75, 6.56667, 1,
4.025, 4.04583, 1,
4.26458, 4.28646, 1,
4.04792, 5.94167, 1,
4.32448, 6.14375, 1,
4.53333, 4.5, 1,
4.59167, 6.3, 1,
4.81458, 4.675, 1,
4.86094, 6.41667, 1,
5.09167, 4.8, 1,
5.14375, 6.5, 1,
5.36458, 4.875, 1,
5.4401, 6.55, 1,
5.63333, 4.9, 1,
9, 9.55, 1,
8.96667, 9.78021, 1,
8.98438, 9.38125, 1,
8.9375, 9.225, 1,
8.86667, 9.9875, 1,
8.85938, 9.08125, 1,
8.75, 8.95, 1,
8.7, 10.1719, 1,
8.61875, 8.84062, 1,
8.475, 8.7625, 1,
8.46667, 10.3333, 1,
8.31875, 8.71562, 1,
8.17813, 10.4646, 1,
8.15, 8.7, 1,
8.08281, 8.70677, 1,
7.98125, 8.72708, 1,
7.84583, 10.5583, 1,
7.84531, 8.76094, 1,
7.675, 8.80833, 1,
7.50156, 8.85573, 1,
7.46979, 10.6146, 1,
7.35625, 8.88958, 1,
7.23906, 8.9099, 1,
7.15, 8.91667, 1,
7.05, 10.6333, 1,
6.50885, 8.85469, 1,
5.92396, 10.5042, 1,
5.90208, 8.66875, 1,
5.32969, 8.35885, 1,
4.87917, 10.1167, 1,
4.79167, 7.925, 1,
4.31823, 7.39531, 1,
3.93958, 6.79792, 1,
2.89167, 0.9, 1,
2.69635, 1.16146, 1,
2.51875, 1.44583, 1,
2.47187, 7.72188, 1,
2.35885, 1.75312, 1,
2.21667, 2.08333, 1,
2.09375, 2.42292, 1,
2.07083, 6.80417, 1,
1.99167, 2.75833, 1,
1.91042, 3.08958, 1,
1.85, 3.41667, 1,
1.83021, 5.81354, 1,
1.80625, 3.74375, 1,
1.775, 4.075, 1,
1.75625, 4.41042, 1,
1.75, 4.75, 1,
};

float normalsDigits6[464 * 3] = {
-0.15157, -0.988446, 0,
-0.262434, -0.96495, 0,
-0.15157, -0.988446, 0,
-0.262434, -0.96495, 0,
-0.376858, -0.926271, 0,
-0.376858, -0.926271, 0,
-0.475547, -0.87969, 0,
-0.475547, -0.87969, 0,
-0.561941, -0.827177, 0,
-0.561941, -0.827177, 0,
-0.649636, -0.760245, 0,
-0.649636, -0.760245, 0,
-0.734558, -0.678546, 0,
-0.734558, -0.678546, 0,
-0.801148, -0.598467, 0,
-0.801148, -0.598467, 0,
-0.848173, -0.52972, 0,
-0.848173, -0.52972, 0,
-0.887094, -0.461589, 0,
-0.887094, -0.461589, 0,
-0.918469, -0.395492, 0,
-0.918469, -0.395492, 0,
-0.940298, -0.340353, 0,
-0.940298, -0.340353, 0,
-0.956674, -0.291162, 0,
-0.956674, -0.291162, 0,
-0.971211, -0.238222, 0,
-0.971211, -0.238222, 0,
-0.983365, -0.181641, 0,
-0.983365, -0.181641, 0,
-0.991173, -0.132577, 0,
-0.991173, -0.132577, 0,
-0.99558, -0.093923, 0,
-0.99558, -0.093923, 0,
-0.998441, -0.055813, 0,
-0.998441, -0.055813, 0,
-0.999831, -0.018402, 0,
-0.999831, -0.018402, 0,
-0.997168, 0.075203, 0,
-0.997168, 0.075203, 0,
-0.971744, 0.236039, 0,
-0.971744, 0.236039, 0,
-0.916324, 0.400437, 0,
-0.916324, 0.400437, 0,
-0.832839, 0.553515, 0,
-0.832839, 0.553515, 0,
-0.715712, 0.698396, 0,
-0.715712, 0.698396, 0,
-0.556771, 0.830666, 0,
-0.556771, 0.830666, 0,
-0.347741, 0.937591, 0,
-0.347741, 0.937591, 0,
-0.113961, 0.993485, 0,
-0.113961, 0.993485, 0,
0.044621, 0.999004, 0,
0.044621, 0.999004, 0,
0.147939, 0.988997, 0,
0.147939, 0.988997, 0,
0.271532, 0.962429, 0,
0.271532, 0.962429, 0,
0.414051, 0.910254, 0,
0.414051, 0.910254, 0,
0.569019, 0.822324, 0,
0.569019, 0.822324, 0,
0.741834, 0.670584, 0,
0.741834, 0.670584, 0,
0.900674, 0.434496, 0,
0.900674, 0.434496, 0,
0.989679, 0.143302, 0,
0.989679, 0.143302, 0,
0.995741, -0.092198, 0,
0.995741, -0.092198, 0,
0.957826, -0.287348, 0,
0.957826, -0.287348, 0,
0.878625, -0.477513, 0,
0.878625, -0.477513, 0,
0.768221, -0.640184, 0,
0.768221, -0.640184, 0,
0.640184, -0.768221, 0,
0.640184, -0.768221, 0,
0.477513, -0.878625, 0,
0.477513, -0.878625, 0,
0.287348, -0.957826, 0,
0.287348, -0.957826, 0,
0.092198, -0.995741, 0,
0.092198, -0.995741, 0,
-0.100267, -0.994961, 0,
-0.100267, -0.994961, 0,
-0.196116, -0.980581, 0,
-0.196116, -0.980581, 0,
-0.241661, -0.970361, 0,
-0.241661, -0.970361, 0,
-0.2681, -0.963391, 0,
-0.2681, -0.963391, 0,
-0.263608, -0.96463, 0,
-0.263608, -0.96463, 0,
-0.226899, -0.973918, 0,
-0.226899, -0.973918, 0,
-0.170787, -0.985308, 0,
-0.170787, -0.985308, 0,
-0.075805, -0.997123, 0,
-0.075805, -0.997123, 0,
0.096221, -0.99536, 0,
0.096221, -0.99536, 0,
0.29299, -0.956116, 0,
0.29299, -0.956116, 0,
0.476103, -0.87939, 0,
0.476103, -0.87939, 0,
0.627723, -0.778437, 0,
0.627723, -0.778437, 0,
0.745586, -0.666409, 0,
0.745586, -0.666409, 0,
0.84463, -0.53535, 0,
0.84463, -0.53535, 0,
0.91974, -0.392528, 0,
0.91974, -0.392528, 0,
0.968293, -0.249816, 0,
0.968293, -0.249816, 0,
-0.705229, 0.70898, 0,
-0.705229, 0.70898, 0,
-0.655088, 0.755553, 0,
-0.655088, 0.755553, 0,
-0.589978, 0.807419, 0,
-0.589978, 0.807419, 0,
-0.504812, 0.863229, 0,
-0.504812, 0.863229, 0,
-0.397558, 0.917577, 0,
-0.397558, 0.917577, 0,
-0.282644, 0.959225, 0,
-0.282644, 0.959225, 0,
-0.166366, 0.986064, 0,
-0.166366, 0.986064, 0,
-0.053704, 0.998557, 0,
-0.053704, 0.998557, 0,
0.098127, 0.995174, 0,
0.098127, 0.995174, 0,
0.305268, 0.952266, 0,
0.305268, 0.952266, 0,
0.503714, 0.86387, 0,
0.503714, 0.86387, 0,
0.668305, 0.743887, 0,
0.668305, 0.743887, 0,
0.792595, 0.609749, 0,
0.792595, 0.609749, 0,
0.894427, 0.447214, 0,
0.894427, 0.447214, 0,
0.964154, 0.265342, 0,
0.964154, 0.265342, 0,
0.996429, 0.08443, 0,
0.996429, 0.08443, 0,
0.996982, -0.077631, 0,
0.996982, -0.077631, 0,
0.967962, -0.251097, 0,
0.967962, -0.251097, 0,
0.89939, -0.437147, 0,
0.89939, -0.437147, 0,
0.789658, -0.613547, 0,
0.789658, -0.613547, 0,
0.640662, -0.767823, 0,
0.640662, -0.767823, 0,
0.451685, -0.892178, 0,
0.451685, -0.892178, 0,
0.255272, -0.966869, 0,
0.255272, -0.966869, 0,
0.077589, -0.996985, 0,
0.077589, -0.996985, 0,
-0.048152, -0.99884, 0,
-0.048152, -0.99884, 0,
0.947878, 0.318634, 0,
0.892595, 0.450858, 0,
0.947878, 0.318634, 0,
0.892595, 0.450858, 0,
0.79523, 0.606308, 0,
0.79523, 0.606308, 0,
0.644472, 0.764628, 0,
0.644472, 0.764628, 0,
0.457103, 0.889414, 0,
0.457103, 0.889414, 0,
0.259763, 0.965672, 0,
0.259763, 0.965672, 0,
0.079265, 0.996854, 0,
0.079265, 0.996854, 0,
-0.07841, 0.996921, 0,
-0.07841, 0.996921, 0,
-0.256901, 0.966438, 0,
-0.256901, 0.966438, 0,
-0.452376, 0.891827, 0,
-0.452376, 0.891827, 0,
-0.638901, 0.769289, 0,
-0.638901, 0.769289, 0,
-0.791782, 0.610803, 0,
-0.791782, 0.610803, 0,
-0.90489, 0.425645, 0,
-0.90489, 0.425645, 0,
-0.971004, 0.239062, 0,
-0.971004, 0.239062, 0,
-0.997362, 0.072584, 0,
-0.997362, 0.072584, 0,
-0.996452, -0.084165, 0,
-0.996452, -0.084165, 0,
-0.964436, -0.264315, 0,
-0.964436, -0.264315, 0,
-0.895379, -0.445304, 0,
-0.895379, -0.445304, 0,
-0.794578, -0.607163, 0,
-0.794578, -0.607163, 0,
-0.669311, -0.742982, 0,
-0.669311, -0.742982, 0,
-0.50162, -0.865088, 0,
-0.50162, -0.865088, 0,
-0.301842, -0.953358, 0,
-0.301842, -0.953358, 0,
-0.096375, -0.995345, 0,
-0.096375, -0.995345, 0,
0.092623, -0.995701, 0,
0.092623, -0.995701, 0,
0.264985, -0.964252, 0,
0.264985, -0.964252, 0,
0.411219, -0.911536, 0,
0.411219, -0.911536, 0,
0.528302, -0.849057, 0,
0.528302, -0.849057, 0,
0.622101, -0.782937, 0,
0.622101, -0.782937, 0,
0.708639, -0.705571, 0,
0.708639, -0.705571, 0,
0.786212, -0.617957, 0,
0.786212, -0.617957, 0,
0.851865, -0.523761, 0,
0.851865, -0.523761, 0,
0.977313, 0.211802, 0,
0.977313, 0.211802, 0,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
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
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
0, 0, 1,
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

unsigned int indicesDigits6[1392] = {
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
166, 0, 167,
0, 2, 167,
168, 169, 170,
169, 171, 170,
169, 172, 171,
172, 173, 171,
172, 174, 173,
174, 175, 173,
174, 176, 175,
176, 177, 175,
176, 178, 177,
178, 179, 177,
178, 180, 179,
180, 181, 179,
180, 182, 181,
182, 183, 181,
182, 184, 183,
184, 185, 183,
184, 186, 185,
186, 187, 185,
186, 188, 187,
188, 189, 187,
188, 190, 189,
190, 191, 189,
190, 192, 191,
192, 193, 191,
192, 194, 193,
194, 195, 193,
194, 196, 195,
196, 197, 195,
196, 198, 197,
198, 199, 197,
198, 200, 199,
200, 201, 199,
200, 202, 201,
202, 203, 201,
202, 204, 203,
204, 205, 203,
204, 206, 205,
206, 207, 205,
206, 208, 207,
208, 209, 207,
208, 210, 209,
210, 211, 209,
210, 212, 211,
212, 213, 211,
212, 214, 213,
214, 215, 213,
214, 216, 215,
216, 217, 215,
216, 218, 217,
218, 219, 217,
218, 220, 219,
220, 221, 219,
220, 222, 221,
222, 223, 221,
222, 224, 223,
224, 225, 223,
224, 226, 225,
226, 227, 225,
226, 228, 227,
228, 229, 227,
228, 230, 229,
230, 231, 229,
230, 168, 231,
168, 170, 231,
232, 233, 234,
233, 235, 234,
233, 236, 235,
236, 237, 235,
236, 238, 237,
238, 239, 237,
238, 240, 239,
240, 241, 239,
240, 242, 241,
242, 243, 241,
242, 244, 243,
241, 245, 246,
241, 247, 245,
241, 248, 247,
241, 243, 248,
249, 250, 251,
250, 252, 251,
250, 246, 252,
246, 253, 252,
246, 245, 253,
254, 255, 256,
255, 257, 256,
255, 258, 257,
258, 259, 257,
258, 260, 259,
260, 261, 259,
260, 262, 261,
262, 263, 261,
262, 250, 263,
250, 249, 263,
264, 265, 266,
264, 267, 265,
264, 254, 267,
264, 255, 254,
268, 266, 265,
268, 269, 266,
268, 270, 269,
268, 271, 270,
269, 272, 266,
269, 273, 272,
269, 274, 273,
269, 275, 274,
269, 276, 275,
244, 242, 277,
242, 278, 277,
242, 279, 278,
279, 280, 278,
279, 281, 280,
282, 281, 279,
282, 283, 281,
282, 284, 283,
282, 285, 284,
286, 287, 288,
287, 289, 288,
287, 290, 289,
290, 291, 289,
290, 292, 291,
292, 293, 291,
292, 294, 293,
294, 295, 293,
294, 296, 295,
296, 297, 295,
296, 298, 297,
298, 285, 297,
298, 284, 285,
271, 288, 270,
271, 286, 288,
299, 300, 301,
300, 302, 301,
300, 303, 302,
303, 304, 302,
303, 305, 304,
306, 305, 303,
306, 307, 305,
306, 308, 307,
306, 309, 308,
310, 311, 312,
310, 309, 311,
310, 308, 309,
312, 311, 313,
311, 314, 313,
311, 315, 314,
315, 316, 314,
315, 317, 316,
318, 319, 320,
318, 315, 319,
318, 317, 315,
320, 319, 321,
319, 322, 321,
319, 323, 322,
323, 324, 322,
323, 325, 324,
325, 326, 324,
325, 327, 326,
328, 327, 325,
328, 329, 327,
328, 330, 329,
328, 331, 330,
328, 275, 331,
274, 332, 273,
274, 333, 332,
274, 334, 333,
274, 335, 334,
335, 336, 334,
335, 337, 336,
335, 338, 337,
335, 339, 338,
339, 340, 338,
339, 341, 340,
339, 342, 341,
339, 343, 342,
343, 344, 342,
343, 345, 344,
343, 346, 345,
343, 347, 346,
276, 331, 275,
348, 349, 350,
349, 351, 350,
349, 352, 351,
352, 353, 351,
352, 354, 353,
354, 355, 353,
354, 356, 355,
356, 357, 355,
356, 358, 357,
358, 359, 357,
358, 360, 359,
357, 361, 362,
357, 363, 361,
357, 364, 363,
357, 359, 364,
365, 366, 367,
366, 368, 367,
366, 362, 368,
362, 369, 368,
362, 361, 369,
370, 371, 372,
371, 373, 372,
371, 374, 373,
374, 375, 373,
374, 376, 375,
376, 377, 375,
376, 378, 377,
378, 379, 377,
378, 366, 379,
366, 365, 379,
380, 381, 382,
380, 383, 381,
380, 370, 383,
380, 371, 370,
384, 382, 381,
384, 385, 382,
384, 386, 385,
384, 387, 386,
385, 388, 382,
385, 389, 388,
385, 390, 389,
385, 391, 390,
385, 392, 391,
360, 358, 393,
358, 394, 393,
358, 395, 394,
395, 396, 394,
395, 397, 396,
398, 397, 395,
398, 399, 397,
398, 400, 399,
398, 401, 400,
402, 403, 404,
403, 405, 404,
403, 406, 405,
406, 407, 405,
406, 408, 407,
408, 409, 407,
408, 410, 409,
410, 411, 409,
410, 412, 411,
412, 413, 411,
412, 414, 413,
414, 401, 413,
414, 400, 401,
387, 404, 386,
387, 402, 404,
415, 416, 417,
416, 418, 417,
416, 419, 418,
419, 420, 418,
419, 421, 420,
422, 421, 419,
422, 423, 421,
422, 424, 423,
422, 425, 424,
426, 427, 428,
426, 425, 427,
426, 424, 425,
428, 427, 429,
427, 430, 429,
427, 431, 430,
431, 432, 430,
431, 433, 432,
434, 435, 436,
434, 431, 435,
434, 433, 431,
436, 435, 437,
435, 438, 437,
435, 439, 438,
439, 440, 438,
439, 441, 440,
441, 442, 440,
441, 443, 442,
444, 443, 441,
444, 445, 443,
444, 446, 445,
444, 447, 446,
444, 391, 447,
390, 448, 389,
390, 449, 448,
390, 450, 449,
390, 451, 450,
451, 452, 450,
451, 453, 452,
451, 454, 453,
451, 455, 454,
455, 456, 454,
455, 457, 456,
455, 458, 457,
455, 459, 458,
459, 460, 458,
459, 461, 460,
459, 462, 461,
459, 463, 462,
392, 447, 391,
};

void drawDigits6()
{
drawTrianglesVerticesNormalsIndices(1392, verticesDigits6, normalsDigits6, indicesDigits6);
}
