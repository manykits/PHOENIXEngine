// ProjectPre.hpp

#ifndef PROJECTPRE_HPP
#define PROJECTPRE_HPP

#include "PX2CorePre.hpp"

#if defined(PROJECT_DLL_EXPORT)
#define PROJECT_DLL_ITEM __declspec(dllexport)
#elif defined(PROJECT_DLL_IMPORT)
#define PROJECT_DLL_ITEM __declspec(dllimport)
#else
#define PROJECT_DLL_ITEM
#endif

#endif