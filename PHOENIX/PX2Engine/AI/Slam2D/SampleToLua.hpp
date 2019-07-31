// SampleToLua.hpp

#ifndef SAMPLETOLUA_HPP
#define SAMPLETOLUA_HPP

#include "PX2CorePre.hpp"
#include "Slam2DPlugin_Pre.hpp"
#include "PX2RobotExt.hpp"
using namespace PX2;

#define PX2_LUA

#if defined (PX2_LUA)

#if (defined(_WIN32) || defined(WIN32)) && defined(_DEBUG)
#pragma warning (disable:4800)
#pragma warning (disable:4505)
#endif

struct lua_State;
extern int tolua_Sample_open (lua_State* tolua_S);

#endif

#endif