// SlamGMappingPlugin_Pre.hpp

#ifndef SLAMGMAPPINGPLUGIN_PRE_HPP
#define SLAMGMAPPINGPLUGIN_PRE_HPP

#include "PX2CorePre.hpp"

#if defined(SLAMGMAPPING_DLL_EXPORT)
#define SLAMGMAPPING_DLL_ITEM __declspec(dllexport)
#elif defined(SLAMGMAPPING_DLL_IMPORT)
#define SLAMGMAPPING_DLL_ITEM __declspec(dllimport)
#else
#define SLAMGMAPPING_DLL_ITEM
#endif

//#define SLAM2D_DLL_ITEM PX2_ENGINE_ITEM

#endif