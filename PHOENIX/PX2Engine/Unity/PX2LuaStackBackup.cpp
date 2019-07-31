// PX2LuaContext.cpp

#include "PX2LuaStackBackup.hpp"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "tolua++.h"
using namespace PX2;

//----------------------------------------------------------------------------
// LuaStackBackup
//----------------------------------------------------------------------------
LuaStackBackup::LuaStackBackup(lua_State *luaState)
:
mState(luaState)
{
	mTop = lua_gettop(mState);
}
//----------------------------------------------------------------------------
LuaStackBackup::~LuaStackBackup()
{
	lua_settop(mState, mTop);
}
//----------------------------------------------------------------------------