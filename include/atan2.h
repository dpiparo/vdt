/*
 * atan2.h
 * The basic idea is to exploit pade polinomials.
 * A lot of ideas were inspired by the cephes math library (by Stephen L. Moshier
 * moshier@na-net.ornl.gov) as well as actual code.
 * The Cephes library can be found here:  http://www.netlib.org/cephes/
 *
 *  Created on: Sept 20, 2012
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

#ifndef ATAN2_H_
#define ATAN2_H_

#include "vdtcore_common.h"

namespace vdt{

inline float fast_atan2f( float y, float x ) {
    // move in first octant
    float xx = std::fabs(x);
    float yy = std::fabs(y);
    float tmp =0.0f;
    if (yy>xx) {
      tmp = yy;
      yy=xx; xx=tmp;
    }

    float t=yy/xx;
    float z=t;
    if( t > 0.4142135623730950f ) // * tan pi/8
        z = (t-1.0f)/(t+1.0f);

    //printf("%e %e %e %e\n",yy,xx,t,z);
    float z2 = z * z;
    float ret =
      ((( 8.05374449538e-2f * z2
      - 1.38776856032E-1f) * z2
    + 1.99777106478E-1f) * z2
       - 3.33329491539E-1f) * z2 * z
      + z;

    // move back in place
    if (y==0) ret=0.0f;
    if( t > 0.4142135623730950f ) ret += details::PIO4F;
    if (tmp!=0) ret = details::PIO2F - ret;
    if (x<0) ret = details::PIF - ret;
    if (y<0) ret = -ret;

    return ret;

  }

} // end namespace vdt


#endif
