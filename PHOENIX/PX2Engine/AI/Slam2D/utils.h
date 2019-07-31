/* HokuyoAIST
 *
 * Header file for various utilities.
 *
 * Copyright 2008-2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *   RT-Synthesis Research Group
 *   Intelligent Systems Research Institute,
 *   National Institute of Advanced Industrial Science and Technology (AIST),
 *   Japan
 *   All rights reserved.
 *
 * This file is part of HokuyoAIST.
 *
 * HokuyoAIST is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * HokuyoAIST is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with HokuyoAIST. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_HOKUYOAIST_UTILS_H__
#define INCLUDE_HOKUYOAIST_UTILS_H__

#include <algorithm>
#include <cassert>
#include "port.h"
#include <iostream>
#include <string>
#include <vector>

#if defined(WIN32)
  typedef unsigned char       uint8_t;
  typedef unsigned int      uint32_t;
  #if defined(HOKUYOAIST_STATIC)
    #define HOKUYOAIST_EXPORT
  #elif defined(hokuyoaist_EXPORTS)
    #define HOKUYOAIST_EXPORT     __declspec(dllexport)
  #else
    #define HOKUYOAIST_EXPORT     __declspec(dllimport)
  #endif
#else
  #include <stdint.h>
  #define HOKUYOAIST_EXPORT
#endif

/** @ingroup library_hokuyoaist
@{
*/

namespace hokuyoaist {

#ifndef M_PI
  double const M_PI = 3.14159265358979323846;
#endif
// Convert radians to degrees
#ifndef RTOD
  inline double RTOD(double rad)
  {
    return rad * 180.0 / M_PI;
  }
#endif
// Convert degrees to radians
#ifndef DTOR
  inline double DTOR(double deg)
  {
    return deg * M_PI / 180.0;
  }
#endif


/// Find the median value of a std::vector.
template<typename T>
inline T median(std::vector<T>& v)
{
  typename std::vector<T>::iterator first(v.begin());
  typename std::vector<T>::iterator median(first + (v.end() - first) / 2);
  std::nth_element(first, median, v.end());
  return *median;
}

} // namespace hokuyoaist

/** @} */

#endif // INCLUDE_HOKUYOAIST_UTILS_H__
