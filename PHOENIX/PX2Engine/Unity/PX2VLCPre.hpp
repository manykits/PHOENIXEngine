// PX2VLCPre.hpp

#ifndef PX2VLCMEMPRE_HPP
#define PX2VLCMEMPRE_HPP

#include "PX2UnityPre.hpp"

#if defined (_WIN32) || defined (WIN32)

#if defined(_WIN64) || defined(WIN64)
#else
#define PX2_USE_VLC 1
#endif

#endif

#endif