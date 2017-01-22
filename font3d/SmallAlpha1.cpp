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

#include "SmallAlpha1.hpp"

#include "hw/varray.hpp"

float verticesSmallAlpha1[348 * 3] = {
-0.064583, 9.93125, 1,
0.141667, 10.1917, 1,
-0.064583, 9.93125, 0,
0.141667, 10.1917, 0,
0.485417, 10.3479, 1,
0.485417, 10.3479, 0,
0.966667, 10.4, 1,
0.966667, 10.4, 0,
2.9, 10.4, 1,
2.9, 10.4, 0,
2.9, 6.68333, 1,
2.9, 6.68333, 0,
3.45, 7.04792, 1,
3.45, 7.04792, 0,
4.05, 7.30833, 1,
4.05, 7.30833, 0,
4.7, 7.46458, 1,
4.7, 7.46458, 0,
5.4, 7.51667, 1,
5.4, 7.51667, 0,
6.29375, 7.4474, 1,
6.29375, 7.4474, 0,
7.09167, 7.23958, 1,
7.09167, 7.23958, 0,
7.79375, 6.89323, 1,
7.79375, 6.89323, 0,
8.4, 6.40833, 1,
8.4, 6.40833, 0,
8.88854, 5.81927, 1,
8.88854, 5.81927, 0,
9.2375, 5.16042, 1,
9.2375, 5.16042, 0,
9.44687, 4.43177, 1,
9.44687, 4.43177, 0,
9.51667, 3.63333, 1,
9.51667, 3.63333, 0,
9.44583, 2.83542, 1,
9.44583, 2.83542, 0,
9.23333, 2.10833, 1,
9.23333, 2.10833, 0,
8.87917, 1.45208, 1,
8.87917, 1.45208, 0,
8.38333, 0.866667, 1,
8.38333, 0.866667, 0,
7.77292, 0.385417, 1,
7.77292, 0.385417, 0,
7.075, 0.041667, 1,
7.075, 0.041667, 0,
6.28958, -0.164583, 1,
6.28958, -0.164583, 0,
5.41667, -0.233333, 1,
5.41667, -0.233333, 0,
4.75938, -0.182292, 1,
4.75938, -0.182292, 0,
4.12083, -0.029167, 1,
4.12083, -0.029167, 0,
3.50104, 0.226042, 1,
3.50104, 0.226042, 0,
2.9, 0.583333, 1,
2.9, 0.583333, 0,
2.9, 0, 1,
2.9, 0, 0,
0.966667, 0, 1,
0.966667, 0, 0,
0.485417, 0.052083, 1,
0.485417, 0.052083, 0,
0.141667, 0.208333, 1,
0.141667, 0.208333, 0,
-0.064583, 0.46875, 1,
-0.064583, 0.46875, 0,
-0.133333, 0.833333, 1,
-0.133333, 0.833333, 0,
-0.115625, 1.05573, 1,
-0.115625, 1.05573, 0,
-0.0625, 1.23958, 1,
-0.0625, 1.23958, 0,
0.026042, 1.3849, 1,
0.026042, 1.3849, 0,
0.15, 1.49167, 1,
0.15, 1.49167, 0,
0.307292, 1.56823, 1,
0.307292, 1.56823, 0,
0.495833, 1.62292, 1,
0.495833, 1.62292, 0,
0.715625, 1.65573, 1,
0.715625, 1.65573, 0,
0.966667, 1.66667, 1,
0.966667, 1.66667, 0,
1.23333, 1.66667, 1,
1.23333, 1.66667, 0,
1.23333, 8.73333, 1,
1.23333, 8.73333, 0,
0.966667, 8.73333, 1,
0.966667, 8.73333, 0,
0.715625, 8.74427, 1,
0.715625, 8.74427, 0,
0.495833, 8.77708, 1,
0.495833, 8.77708, 0,
0.307292, 8.83177, 1,
0.307292, 8.83177, 0,
0.15, 8.90833, 1,
0.15, 8.90833, 0,
0.026042, 9.0151, 1,
0.026042, 9.0151, 0,
-0.0625, 9.16042, 1,
-0.0625, 9.16042, 0,
-0.115625, 9.34427, 1,
-0.115625, 9.34427, 0,
-0.133333, 9.56667, 1,
-0.133333, 9.56667, 0,
4.84531, 5.81094, 1,
4.36458, 5.69375, 1,
4.84531, 5.81094, 0,
4.36458, 5.69375, 0,
3.94115, 5.49844, 1,
3.94115, 5.49844, 0,
3.575, 5.225, 1,
3.575, 5.225, 0,
3.27969, 4.89323, 1,
3.27969, 4.89323, 0,
3.06875, 4.52292, 1,
3.06875, 4.52292, 0,
2.94219, 4.11406, 1,
2.94219, 4.11406, 0,
2.9, 3.66667, 1,
2.9, 3.66667, 0,
2.94323, 3.1974, 1,
2.94323, 3.1974, 0,
3.07292, 2.77292, 1,
3.07292, 2.77292, 0,
3.28906, 2.39323, 1,
3.28906, 2.39323, 0,
3.59167, 2.05833, 1,
3.59167, 2.05833, 0,
3.96302, 1.7849, 1,
3.96302, 1.7849, 0,
4.38542, 1.58958, 1,
4.38542, 1.58958, 0,
4.85885, 1.4724, 1,
4.85885, 1.4724, 0,
5.38333, 1.43333, 1,
5.38333, 1.43333, 0,
5.90052, 1.4724, 1,
5.90052, 1.4724, 0,
6.36875, 1.58958, 1,
6.36875, 1.58958, 0,
6.78802, 1.7849, 1,
6.78802, 1.7849, 0,
7.15833, 2.05833, 1,
7.15833, 2.05833, 0,
7.46094, 2.39219, 1,
7.46094, 2.39219, 0,
7.67708, 2.76875, 1,
7.67708, 2.76875, 0,
7.80677, 3.18802, 1,
7.80677, 3.18802, 0,
7.85, 3.65, 1,
7.85, 3.65, 0,
7.80729, 4.09792, 1,
7.80729, 4.09792, 0,
7.67917, 4.50833, 1,
7.67917, 4.50833, 0,
7.46562, 4.88125, 1,
7.46562, 4.88125, 0,
7.16667, 5.21667, 1,
7.16667, 5.21667, 0,
6.79896, 5.49375, 1,
6.79896, 5.49375, 0,
6.37917, 5.69167, 1,
6.37917, 5.69167, 0,
5.90729, 5.81042, 1,
5.90729, 5.81042, 0,
5.38333, 5.85, 1,
5.38333, 5.85, 0,
2.9, 10.4, 0,
2.9, 0, 0,
2.9, 6.68333, 0,
2.9, 3.66667, 0,
2.9, 0.583333, 0,
2.94323, 3.1974, 0,
3.50104, 0.226042, 0,
3.07292, 2.77292, 0,
3.28906, 2.39323, 0,
1.23333, 1.66667, 0,
1.23333, 8.73333, 0,
0.966667, 0, 0,
0.966667, 1.66667, 0,
0.15, 1.49167, 0,
0.485417, 0.052083, 0,
0.307292, 1.56823, 0,
0.495833, 1.62292, 0,
0.715625, 1.65573, 0,
0.141667, 0.208333, 0,
-0.0625, 1.23958, 0,
-0.064583, 0.46875, 0,
0.026042, 1.3849, 0,
-0.115625, 1.05573, 0,
-0.133333, 0.833333, 0,
7.80677, 3.18802, 0,
8.38333, 0.866667, 0,
7.85, 3.65, 0,
8.4, 6.40833, 0,
8.87917, 1.45208, 0,
8.88854, 5.81927, 0,
9.23333, 2.10833, 0,
9.2375, 5.16042, 0,
9.44583, 2.83542, 0,
9.44687, 4.43177, 0,
9.51667, 3.63333, 0,
7.77292, 0.385417, 0,
7.15833, 2.05833, 0,
7.075, 0.041667, 0,
7.46094, 2.39219, 0,
7.67708, 2.76875, 0,
4.85885, 1.4724, 0,
5.41667, -0.233333, 0,
5.38333, 1.43333, 0,
5.90052, 1.4724, 0,
6.28958, -0.164583, 0,
6.36875, 1.58958, 0,
6.78802, 1.7849, 0,
3.59167, 2.05833, 0,
4.12083, -0.029167, 0,
3.96302, 1.7849, 0,
4.38542, 1.58958, 0,
4.75938, -0.182292, 0,
7.80729, 4.09792, 0,
7.79375, 6.89323, 0,
7.67917, 4.50833, 0,
7.46562, 4.88125, 0,
7.16667, 5.21667, 0,
7.09167, 7.23958, 0,
6.79896, 5.49375, 0,
6.37917, 5.69167, 0,
6.29375, 7.4474, 0,
5.90729, 5.81042, 0,
5.4, 7.51667, 0,
5.38333, 5.85, 0,
4.84531, 5.81094, 0,
4.7, 7.46458, 0,
4.36458, 5.69375, 0,
4.05, 7.30833, 0,
3.94115, 5.49844, 0,
3.575, 5.225, 0,
3.45, 7.04792, 0,
3.27969, 4.89323, 0,
3.06875, 4.52292, 0,
2.94219, 4.11406, 0,
0.966667, 8.73333, 0,
0.966667, 10.4, 0,
0.715625, 8.74427, 0,
0.495833, 8.77708, 0,
0.485417, 10.3479, 0,
0.307292, 8.83177, 0,
0.15, 8.90833, 0,
0.141667, 10.1917, 0,
0.026042, 9.0151, 0,
-0.0625, 9.16042, 0,
-0.064583, 9.93125, 0,
-0.115625, 9.34427, 0,
-0.133333, 9.56667, 0,
2.9, 10.4, 1,
2.9, 0, 1,
2.9, 6.68333, 1,
2.9, 3.66667, 1,
2.9, 0.583333, 1,
2.94323, 3.1974, 1,
3.50104, 0.226042, 1,
3.07292, 2.77292, 1,
3.28906, 2.39323, 1,
1.23333, 1.66667, 1,
1.23333, 8.73333, 1,
0.966667, 0, 1,
0.966667, 1.66667, 1,
0.15, 1.49167, 1,
0.485417, 0.052083, 1,
0.307292, 1.56823, 1,
0.495833, 1.62292, 1,
0.715625, 1.65573, 1,
0.141667, 0.208333, 1,
-0.0625, 1.23958, 1,
-0.064583, 0.46875, 1,
0.026042, 1.3849, 1,
-0.115625, 1.05573, 1,
-0.133333, 0.833333, 1,
7.80677, 3.18802, 1,
8.38333, 0.866667, 1,
7.85, 3.65, 1,
8.4, 6.40833, 1,
8.87917, 1.45208, 1,
8.88854, 5.81927, 1,
9.23333, 2.10833, 1,
9.2375, 5.16042, 1,
9.44583, 2.83542, 1,
9.44687, 4.43177, 1,
9.51667, 3.63333, 1,
7.77292, 0.385417, 1,
7.15833, 2.05833, 1,
7.075, 0.041667, 1,
7.46094, 2.39219, 1,
7.67708, 2.76875, 1,
4.85885, 1.4724, 1,
5.41667, -0.233333, 1,
5.38333, 1.43333, 1,
5.90052, 1.4724, 1,
6.28958, -0.164583, 1,
6.36875, 1.58958, 1,
6.78802, 1.7849, 1,
3.59167, 2.05833, 1,
4.12083, -0.029167, 1,
3.96302, 1.7849, 1,
4.38542, 1.58958, 1,
4.75938, -0.182292, 1,
7.80729, 4.09792, 1,
7.79375, 6.89323, 1,
7.67917, 4.50833, 1,
7.46562, 4.88125, 1,
7.16667, 5.21667, 1,
7.09167, 7.23958, 1,
6.79896, 5.49375, 1,
6.37917, 5.69167, 1,
6.29375, 7.4474, 1,
5.90729, 5.81042, 1,
5.4, 7.51667, 1,
5.38333, 5.85, 1,
4.84531, 5.81094, 1,
4.7, 7.46458, 1,
4.36458, 5.69375, 1,
4.05, 7.30833, 1,
3.94115, 5.49844, 1,
3.575, 5.225, 1,
3.45, 7.04792, 1,
3.27969, 4.89323, 1,
3.06875, 4.52292, 1,
2.94219, 4.11406, 1,
0.966667, 8.73333, 1,
0.966667, 10.4, 1,
0.715625, 8.74427, 1,
0.495833, 8.77708, 1,
0.485417, 10.3479, 1,
0.307292, 8.83177, 1,
0.15, 8.90833, 1,
0.141667, 10.1917, 1,
0.026042, 9.0151, 1,
-0.0625, 9.16042, 1,
-0.064583, 9.93125, 1,
-0.115625, 9.34427, 1,
-0.133333, 9.56667, 1,
};

float normalsSmallAlpha1[348 * 3] = {
-0.783919, 0.620864, 0,
-0.413803, 0.910366, 0,
-0.783919, 0.620864, 0,
-0.413803, 0.910366, 0,
-0.107597, 0.994195, 0,
-0.107597, 0.994195, 0,
-0, 1, 0,
-0, 1, 0,
1, 0, 0,
1, 0, 0,
-0.552513, 0.833505, 0,
-0.552513, 0.833505, 0,
-0.398144, 0.917323, 0,
-0.398144, 0.917323, 0,
-0.233727, 0.972302, 0,
-0.233727, 0.972302, 0,
-0.0742, 0.997243, 0,
-0.0742, 0.997243, 0,
0.077274, 0.99701, 0,
0.077274, 0.99701, 0,
0.252036, 0.967718, 0,
0.252036, 0.967718, 0,
0.442417, 0.896809, 0,
0.442417, 0.896809, 0,
0.624613, 0.780934, 0,
0.624613, 0.780934, 0,
0.769725, 0.638375, 0,
0.769725, 0.638375, 0,
0.883703, 0.468048, 0,
0.883703, 0.468048, 0,
0.961108, 0.276173, 0,
0.961108, 0.276173, 0,
0.996201, 0.087078, 0,
0.996201, 0.087078, 0,
0.996083, -0.088425, 0,
0.996083, -0.088425, 0,
0.959846, -0.280528, 0,
0.959846, -0.280528, 0,
0.880022, -0.474933, 0,
0.880022, -0.474933, 0,
0.763077, -0.646307, 0,
0.763077, -0.646307, 0,
0.619123, -0.785294, 0,
0.619123, -0.785294, 0,
0.44185, -0.897089, 0,
0.44185, -0.897089, 0,
0.253988, -0.967207, 0,
0.253988, -0.967207, 0,
0.078516, -0.996913, 0,
0.078516, -0.996913, 0,
-0.077421, -0.996998, 0,
-0.077421, -0.996998, 0,
-0.233193, -0.97243, 0,
-0.233193, -0.97243, 0,
-0.38075, -0.924678, 0,
-0.38075, -0.924678, 0,
-0.510986, -0.859589, 0,
-0.510986, -0.859589, 0,
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
-0.982681, 0.185306, 0,
-0.982681, 0.185306, 0,
0.236835, -0.97155, 0,
0.418846, -0.908057, 0,
0.236835, -0.97155, 0,
0.418846, -0.908057, 0,
0.598358, -0.801229, 0,
0.598358, -0.801229, 0,
0.746956, -0.664873, 0,
0.746956, -0.664873, 0,
0.868919, -0.494954, 0,
0.868919, -0.494954, 0,
0.955278, -0.29571, 0,
0.955278, -0.29571, 0,
0.995584, -0.093879, 0,
0.995584, -0.093879, 0,
0.995784, 0.091731, 0,
0.995784, 0.091731, 0,
0.956361, 0.292189, 0,
0.956361, 0.292189, 0,
0.869049, 0.494726, 0,
0.869049, 0.494726, 0,
0.741973, 0.67043, 0,
0.741973, 0.67043, 0,
0.592929, 0.805254, 0,
0.592929, 0.805254, 0,
0.419697, 0.907664, 0,
0.419697, 0.907664, 0,
0.240274, 0.970705, 0,
0.240274, 0.970705, 0,
0.074273, 0.997238, 0,
0.074273, 0.997238, 0,
-0.075314, 0.99716, 0,
-0.075314, 0.99716, 0,
-0.24279, 0.970079, 0,
-0.24279, 0.970079, 0,
-0.422269, 0.906471, 0,
-0.422269, 0.906471, 0,
-0.594009, 0.804458, 0,
-0.594009, 0.804458, 0,
-0.740933, 0.671579, 0,
-0.740933, 0.671579, 0,
-0.867282, 0.497817, 0,
-0.867282, 0.497817, 0,
-0.955342, 0.295503, 0,
-0.955342, 0.295503, 0,
-0.995651, 0.093167, 0,
-0.995651, 0.093167, 0,
-0.995485, -0.094918, 0,
-0.995485, -0.094918, 0,
-0.954566, -0.297999, 0,
-0.954566, -0.297999, 0,
-0.867795, -0.496922, 0,
-0.867795, -0.496922, 0,
-0.746513, -0.665371, 0,
-0.746513, -0.665371, 0,
-0.601808, -0.79864, 0,
-0.601808, -0.79864, 0,
-0.426445, -0.904513, 0,
-0.426445, -0.904513, 0,
-0.244046, -0.969764, 0,
-0.244046, -0.969764, 0,
-0.075332, -0.997159, 0,
-0.075332, -0.997159, 0,
0.072413, -0.997375, 0,
0.072413, -0.997375, 0,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
0, 0, -1,
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

unsigned int indicesSmallAlpha1[1044] = {
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
175, 177, 176,
175, 178, 177,
178, 179, 177,
178, 180, 179,
180, 181, 179,
180, 182, 181,
183, 174, 184,
183, 175, 174,
183, 185, 175,
183, 186, 185,
187, 188, 189,
188, 190, 189,
188, 185, 190,
185, 191, 190,
185, 186, 191,
192, 193, 194,
192, 195, 193,
192, 187, 195,
192, 188, 187,
194, 196, 197,
194, 193, 196,
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
209, 212, 210,
209, 213, 212,
209, 198, 213,
209, 199, 198,
214, 215, 216,
215, 217, 216,
215, 218, 217,
218, 219, 217,
218, 211, 219,
211, 220, 219,
211, 210, 220,
221, 222, 223,
222, 224, 223,
222, 225, 224,
225, 214, 224,
225, 215, 214,
180, 221, 182,
180, 222, 221,
226, 227, 228,
226, 201, 227,
226, 200, 201,
228, 227, 229,
227, 230, 229,
227, 231, 230,
231, 232, 230,
231, 233, 232,
231, 234, 233,
234, 235, 233,
234, 236, 235,
236, 237, 235,
236, 238, 237,
236, 239, 238,
239, 240, 238,
239, 241, 240,
241, 242, 240,
241, 243, 242,
244, 243, 241,
244, 245, 243,
244, 246, 245,
244, 247, 246,
244, 176, 247,
248, 249, 250,
248, 174, 249,
248, 184, 174,
251, 252, 253,
251, 249, 252,
251, 250, 249,
254, 255, 256,
254, 252, 255,
254, 253, 252,
257, 258, 259,
257, 255, 258,
257, 256, 255,
258, 260, 259,
176, 177, 247,
261, 262, 263,
262, 264, 263,
262, 265, 264,
265, 266, 264,
265, 267, 266,
267, 268, 266,
267, 269, 268,
270, 261, 271,
270, 262, 261,
270, 272, 262,
270, 273, 272,
274, 275, 276,
275, 277, 276,
275, 272, 277,
272, 278, 277,
272, 273, 278,
279, 280, 281,
279, 282, 280,
279, 274, 282,
279, 275, 274,
281, 283, 284,
281, 280, 283,
285, 286, 287,
286, 288, 287,
286, 289, 288,
289, 290, 288,
289, 291, 290,
291, 292, 290,
291, 293, 292,
293, 294, 292,
293, 295, 294,
296, 297, 298,
296, 299, 297,
296, 300, 299,
296, 285, 300,
296, 286, 285,
301, 302, 303,
302, 304, 303,
302, 305, 304,
305, 306, 304,
305, 298, 306,
298, 307, 306,
298, 297, 307,
308, 309, 310,
309, 311, 310,
309, 312, 311,
312, 301, 311,
312, 302, 301,
267, 308, 269,
267, 309, 308,
313, 314, 315,
313, 288, 314,
313, 287, 288,
315, 314, 316,
314, 317, 316,
314, 318, 317,
318, 319, 317,
318, 320, 319,
318, 321, 320,
321, 322, 320,
321, 323, 322,
323, 324, 322,
323, 325, 324,
323, 326, 325,
326, 327, 325,
326, 328, 327,
328, 329, 327,
328, 330, 329,
331, 330, 328,
331, 332, 330,
331, 333, 332,
331, 334, 333,
331, 263, 334,
335, 336, 337,
335, 261, 336,
335, 271, 261,
338, 339, 340,
338, 336, 339,
338, 337, 336,
341, 342, 343,
341, 339, 342,
341, 340, 339,
344, 345, 346,
344, 342, 345,
344, 343, 342,
345, 347, 346,
263, 264, 334,
};

void drawSmallAlpha1()
{
drawTrianglesVerticesNormalsIndices(1044, verticesSmallAlpha1, normalsSmallAlpha1, indicesSmallAlpha1);
}