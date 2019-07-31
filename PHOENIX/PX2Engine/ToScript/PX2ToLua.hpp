// PX2ToLua.hpp

#ifndef PX2TOLUA_HPP
#define PX2TOLUA_HPP

#include "PX2Core.hpp"
#include "PX2Mathematics.hpp"
#include "PX2Graphics.hpp"
#include "PX2Unity.hpp"
#include "PX2Net.hpp"
#include "PX2Terrains.hpp"
#include "PX2Effect.hpp"
#include "PX2UIs.hpp"
#include "PX2Logics.hpp"
#include "PX2Voxels.hpp"
#include "PX2AIs.hpp"
#include "PX2Applications.hpp"
#include "PX2LuaScriptController.hpp"
using namespace PX2;

#define PX2_LUA

#if defined (PX2_LUA)

#if (defined(_WIN32) || defined(WIN32)) && defined(_DEBUG)
#pragma warning (disable:4800)
#pragma warning (disable:4505)
#endif

struct lua_State;
PX2_ENGINE_ITEM int tolua_PX2_open(lua_State* tolua_S);

#endif

#endif