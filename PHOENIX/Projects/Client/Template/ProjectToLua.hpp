// ProjectToLua.hpp

#ifndef PROJECTTOLUA_HPP
#define PROJECTTOLUA_HPP

#include "PX2CorePre.hpp"
#include "ProjectPre.hpp"

#define PX2_LUA

#if defined (PX2_LUA)

#if (defined(_WIN32) || defined(WIN32)) && defined(_DEBUG)
#pragma warning (disable:4800)
#pragma warning (disable:4505)
#endif

struct lua_State;
extern int tolua_Project_open (lua_State* tolua_S);

#endif

#endif