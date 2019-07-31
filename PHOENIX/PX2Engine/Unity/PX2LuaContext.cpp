// PX2LuaContext.cpp

#include "PX2LuaContext.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Assert.hpp"
#include "PX2LuaStackBackup.hpp"
#include "PX2Log.hpp"

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "tolua++.h"
using namespace PX2;

//----------------------------------------------------------------------------
static int GetGlobal (lua_State *mState)
{
	if (lua_isstring(mState,1) == false)
	{
		lua_pushnil(mState);
	}
	else 
	{
		lua_getglobal(mState, lua_tostring(mState,1));
	}

	return 1;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// LuaContext
//----------------------------------------------------------------------------
LuaContext::LuaContext () :
ScriptContext(ScriptContext::CT_LUA),
mState(0)
{
	mState = lua_open();
	luaL_openlibs(mState);
	lua_register(mState, "GetGlobal", GetGlobal);
}
//----------------------------------------------------------------------------
LuaContext::~LuaContext ()
{
	if(mState) 
	{
		lua_close(mState);
	}
}
//----------------------------------------------------------------------------
void LuaContext::SetLuaState (lua_State *state)
{
	mState = state;
	lua_register(mState, "GetGlobal", GetGlobal);
}
//----------------------------------------------------------------------------
bool LuaContext::CallString(const std::string &str)
{
	LuaStackBackup stackbackup(mState);

	int status = luaL_dostring(mState, str.c_str());

	if (status != 0)
	{
		const char *err = lua_tostring(mState, -1);

		//if (!PX2_GR.IsInEditor())
		{

#if defined(_WIN32) || defined(WIN32)
			::MessageBox(0, err, "CallString::luaL_dostring error.\n", MB_OK);
#endif
			assertion(false, "CallString::luaL_dostring %s error:%s.\n", str.c_str(), err);
		}

		PX2_LOG_ERROR("CallString::luaL_dostring %s error:%s.\n", str.c_str(), err);

		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool LuaContext::CallFile (const std::string &filename)
{
	bool ret = false;

	int bufferSize = 0;
	char *buffer = 0;
	if (ResourceManager::GetSingleton().LoadBuffer(filename, bufferSize,
		buffer))
	{
		ret = CallBuffer(buffer, (unsigned long)bufferSize, 
			filename);
	}

	return ret;
}
//----------------------------------------------------------------------------
bool LuaContext::CallBuffer(const char *buffer, unsigned long size, 
	const std::string &name)
{
	PX2_UNUSED(name);

	if (luaL_loadbuffer(
		mState, buffer, (size_t)size, lua_tostring(mState, -1)) != 0)
	{
		const char *err = lua_tostring(mState, -1);

#if defined(_WIN32) || defined(WIN32)
		::MessageBox(0, err, "CallBuffer::luaL_loadbuffer error.\n", MB_OK);
#endif
		PX2_LOG_ERROR("CallBuffer::luaL_loadbuffer error:%s.\n", err);

		return false;
	}

	if (lua_pcall(mState, 0, 0, 0) != 0)
	{
		const char *err = lua_tostring(mState, -1);

#if defined(_WIN32) || defined(WIN32)
		::MessageBox(0, err, "CallBuffer::lua_pcall error.\n", MB_OK);
#endif
		PX2_LOG_ERROR("CallBuffer::lua_pcall error:%s.\n", err);

		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
void LuaContext::SetUserTypePointer(const std::string &luaName,
	const std::string &className, void *ptr)
{
	LuaStackBackup stackbackup(mState);

	tolua_pushusertype(mState, ptr, className.c_str());

	lua_setglobal(mState, luaName.c_str());
}
//----------------------------------------------------------------------------
void LuaContext::SetUserFunction(const std::string &funName,
	const std::string &returnClassTypeName,
	ScriptGlobalFun globalFun)
{
	PX2_UNUSED(funName);
	PX2_UNUSED(returnClassTypeName);
	PX2_UNUSED(globalFun);
	assertion(false, "");
}
//----------------------------------------------------------------------------
bool LuaContext::CallFileFunction(const std::string &filename,
	const std::string &funName)
{
	PX2_UNUSED(funName);
	return CallFile(filename);
}
//----------------------------------------------------------------------------
bool LuaContext::CallObjectFuntionValist(const std::string &funName,
	Object *paramObj, const std::string &format, va_list valist)
{
	lua_getglobal(mState, "this");
	lua_getglobal(mState, paramObj->GetName().c_str());
	lua_setglobal(mState, "this");

	const char *pfmt = format.c_str();
	int count = 0;
	static const char *args[] = {"arg0", "arg1", "arg2", "arg3"};
	while (pfmt[count])
	{
		if(*pfmt == 'i')
		{
			int value = va_arg(valist, int);
			lua_pushnumber(mState, value);
		}
		else if(*pfmt == 'f')
		{
			float value = (float)(va_arg(valist, double));
			lua_pushnumber(mState, value);
		}
		else if(*pfmt == 's')
		{
			char *str = va_arg(valist, char *);
			lua_pushstring(mState, str);
		}
		else
		{
			assertion(false, "");
		}
		lua_setglobal(mState, args[count++]);
	}

	{
		CallString(funName);
	}

	lua_setglobal(mState, "this");

	return true;
}
//----------------------------------------------------------------------------