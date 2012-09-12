/*
 * cos.h
 * The basic idea is to exploit pade polinomials.
 * A lot of ideas were inspired by the cephes math library (by Stephen L. Moshier
 * moshier@na-net.ornl.gov) as well as actual code. 
 * The Cephes library can be found here:  http://www.netlib.org/cephes/
 * 
 *  Created on: Jun 23, 2012
 *      Author: Danilo Piparo, Thomas Hauth, Vincenzo Innocente
 */

/* 
 * VDT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIN_H_
#define SIN_H_

#include "vdtcore_common.h"
#include "sincos_common.h"
#include <cmath>
#include <limits>

namespace vdt{

using namespace sincos;

// Sin double precision --------------------------------------------------------

/// Double precision sine: just call sincos.
inline double fast_sin(double x){double s,c;sincos::fast_sincos(x,s,c);return s;}

//------------------------------------------------------------------------------

inline float fast_sinf(float x){float s,c;sincos::fast_sincosf(x,s,c);return s;}

//------------------------------------------------------------------------------
void sinv(const uint32_t size, double* __restrict__ iarray, double* __restrict__ oarray);
void fast_sinv(const uint32_t size, double* __restrict__ iarray, double* __restrict__ oarray);
void sinfv(const uint32_t size, float* __restrict__ iarray, float* __restrict__ oarray);
void fast_sinfv(const uint32_t size, float* __restrict__ iarray, float* __restrict__ oarray);


} //vdt namespace

#endif /* SIN_H_ */
