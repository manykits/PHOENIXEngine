// FBxImporter_Pre.hpp

#ifndef FBXIMPORTER_PRE_HPP
#define FBXIMPORTER_PRE_HPP

#include "PX2CorePre.hpp"

#if defined(SAMPLE_DLL_EXPORT)
#define SAMPLE_DLL_ITEM __declspec(dllexport)
#elif defined(SAMPLE_DLL_IMPORT)
#define SAMPLE_DLL_ITEM __declspec(dllimport)
#else
#define SAMPLE_DLL_ITEM
#endif

#endif