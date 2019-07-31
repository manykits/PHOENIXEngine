/* Flexiport
 *
 * Header file for the timeout object.
 *
 * Copyright 2008-2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of Flexiport.
 *
 * Flexiport is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * Flexiport is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Flexiport. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef __TIMEOUT_H
#define __TIMEOUT_H

#if defined (WIN32)
    #if defined (FLEXIPORT_STATIC)
        #define FLEXIPORT_EXPORT
    #elif defined (flexiport_EXPORTS)
        #define FLEXIPORT_EXPORT    __declspec (dllexport)
    #else
        #define FLEXIPORT_EXPORT    __declspec (dllimport)
    #endif
#define has_timespec
    #if !defined (has_timespec)
        // No timespec on Windows
        typedef struct timespec
        {
            int tv_sec;
            int tv_nsec;
        } timespec;
    #endif
#else
    #define FLEXIPORT_EXPORT
#endif

struct timeval;
struct timespec;

/** @ingroup library_flexiport
@{
*/

namespace flexiport
{

/** @brief An object used to represent timeouts. */
class FLEXIPORT_EXPORT Timeout
{
    public:
        Timeout (int sec, int usec) : _sec (sec), _usec (usec) {}
        Timeout (const Timeout &rhs) : _sec (rhs._sec), _usec (rhs._usec) {}

        void AsTimeval (struct timeval &dest) const;
        void FromTimeval (const struct timeval &src);
        void AsTimespec (struct timespec &dest) const;
        void FromTimespec (const struct timespec &src);

        Timeout& operator= (const Timeout &rhs);
        Timeout& operator= (const struct timeval &rhs);
        Timeout& operator= (const struct timespec &rhs);

        int _sec;
        int _usec;
};

} // namespace flexiport

/** @} */

#endif // __TIMEOUT_H

