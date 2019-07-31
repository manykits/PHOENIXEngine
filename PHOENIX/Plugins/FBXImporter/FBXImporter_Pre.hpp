// FBxImporter_Pre.hpp

#ifndef FBXIMPORTER_PRE_HPP
#define FBXIMPORTER_PRE_HPP

#include "PX2CorePre.hpp"

#if defined(FBXIMPORTER_DLL_EXPORT)
#define FBXIMPORTER_DLL_ITEM __declspec(dllexport)
#elif defined(FBXIMPORTER_DLL_IMPORT)
#define FBXIMPORTER_DLL_ITEM __declspec(dllimport)
#else
#define FBXIMPORTER_DLL_ITEM
#endif

#endif