// PX2LuaPlusContext.cpp

#include "PX2LuaPlusContext.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Assert.hpp"
#include "PX2Log.hpp"
#include "PX2LuaStackBackup.hpp"
#include "PX2LuaScriptController.hpp"
#include "LuaPlus.h"
#include "PX2System.hpp"
#include "PX2TimerManager.hpp"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "tolua++.h"
using namespace PX2;
using namespace LuaPlus;

namespace PX2
{
	//----------------------------------------------------------------------------
	class LuaPlusCallObject
	{
	public:
		LuaPlusCallObject() {}
		~LuaPlusCallObject() {}

		LuaPlus::LuaObject TheLuaObject;
		LuaPlus::LuaObject ObjectReset;
	};
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
class SleepTimer : public Timer
{
public:
	SleepTimer()
	{
		StartState = 0;
		RefNumCallState = LUA_NOREF;
		RefNumSelf = LUA_NOREF;
	}
	~SleepTimer()
	{

	}

	lua_State *StartState;
	int RefNumCallState;
	int RefNumSelf;

	virtual bool OnTimer(float appTime)
	{
		lua_getref(StartState, RefNumCallState);
		lua_unref(StartState, RefNumCallState);

		lua_getref(StartState, RefNumSelf);
		lua_setglobal(StartState, "self");

		lua_pcall(StartState, 0, 0, 0);

		lua_unref(StartState, RefNumSelf);

		RefNumCallState = 0;
		RefNumSelf = 0;

		return true;
	}

	virtual void OnRemove()
	{
		if (LUA_NOREF != RefNumCallState)
		{
			lua_unref(StartState, RefNumCallState);
			RefNumCallState = LUA_NOREF;
		}

		if (LUA_NOREF != RefNumSelf)
		{
			lua_unref(StartState, RefNumSelf);
			RefNumSelf = LUA_NOREF;
		}
	}
};
//----------------------------------------------------------------------------
int sleep(lua_State *state)
{
	lua_settop(state, 2);
	int refState = lua_ref(state, LUA_REGISTRYINDEX);
	float timVal = luaL_checknumber(state, 1);

	SleepTimer *timer = TimerManager::GetSingleton().AddTimer<SleepTimer>(
		0.0f, timVal);
	timer->RefNumCallState = refState;

	lua_getglobal(state, "_StartLuaState");
	timer->StartState = (lua_State *)lua_touserdata(state, -1);
	lua_pop(state, 1);

	lua_getglobal(timer->StartState, "self");
	int refthis = lua_ref(state, LUA_REGISTRYINDEX);
	timer->RefNumSelf = refthis;

	return 0;
}
int sleepName(lua_State *state)
{
	lua_settop(state, 3);
	int refState = lua_ref(state, LUA_REGISTRYINDEX);
	float timVal = luaL_checknumber(state, 1);
	std::string strName = luaL_checkstring(state, 2);

	SleepTimer *timer = TimerManager::GetSingleton().AddTimer<SleepTimer>(
		strName, 0.0, timVal);
	timer->RefNumCallState = refState;

	lua_getglobal(state, "_StartLuaState");
	timer->StartState = (lua_State *)lua_touserdata(state, -1);
	lua_pop(state, 1);

	lua_getglobal(timer->StartState, "self");
	int refthis = lua_ref(state, LUA_REGISTRYINDEX);
	timer->RefNumSelf = refthis;

	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
LuaPlusContext *LuaPlusContext::mSingletonLuaPlusContext = 0;
//----------------------------------------------------------------------------
LuaPlusContext::LuaPlusContext() :
ScriptContext(ScriptContext::CT_LUA),
mLuaPlusState(0)
{
	mSingletonLuaPlusContext = this;

	mLuaPlusState = LuaPlus::LuaState::Create(true);

	mLuaPlusState->OpenLibs();
	lua_State *state = (lua_State*)mLuaPlusState;
	lua_pushlightuserdata(state, state);
	lua_setglobal(state, "_StartLuaState");

	mLuaPlusState->GetGlobals().RegisterDirect("CallFile", (*this), 
		&LuaPlusContext::CallFile1);
	mLuaPlusState->GetGlobals().RegisterDirect("CallString", (*this), 
		&LuaPlusContext::CallString1);
	mLuaPlusState->GetGlobals().RegisterDirect("RegistEventFunction", (*this),
		&LuaPlusContext::RegistEventFunction);
	mLuaPlusState->GetGlobals().RegisterDirect("RegistEventObjectFunction", (*this),
		&LuaPlusContext::RegistEventObjectFunction);
	mLuaPlusState->GetGlobals().RegisterDirect("UnRegistAllEventFunctions", (*this),
		&LuaPlusContext::UnRegistAllEventFunctions);
	mLuaPlusState->GetGlobals().RegisterDirect("ClearEventFunctions", (*this),
		&LuaPlusContext::ClearEventFunctions);

	//lua_pushcfunction(state, sleep);
	//lua_setglobal(state, "sleep");
	//lua_pushcfunction(state, sleepName);
	//lua_setglobal(state, "sleepName");

	mLuaPlusState->GetGlobals().Register("sleep", &sleep);
	mLuaPlusState->GetGlobals().Register("sleepName", &sleepName);

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
LuaPlusContext::~LuaPlusContext()
{
	ClearEventFunctions();

	LuaPlus::LuaState::Destroy(mLuaPlusState);
}
//----------------------------------------------------------------------------
LuaPlus::LuaState *LuaPlusContext::GetLuaPlusState()
{
	return mLuaPlusState;
}
//----------------------------------------------------------------------------
void *LuaPlusContext::GetLuaState()
{
	return mLuaPlusState->GetCState();
}
//----------------------------------------------------------------------------
void LuaPlusContext::Update(double appSeconds, double elapsedSeconds)
{
	PX2_UNUSED(appSeconds);
	PX2_UNUSED(elapsedSeconds);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallString1(const char *str)
{
	return CallString(str);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFile1(const char *filename)
{
	return CallFile(filename);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallString(const std::string &str)
{
	int result = 0;

	if (str.length() <= 1 || str[0] != '=')
	{
		result = mLuaPlusState->DoString(str.c_str());
		if (result != 0)
		{
			OnError(result);
			return false;
		}
	}
	// If the string starts with '=', wrap the statement in the print() function.
	else
	{
		std::string buffer("print(");
		buffer += (str.c_str() + 1);
		buffer += ")";
		result = mLuaPlusState->DoString(buffer.c_str());
		if (result != 0)
		{
			OnError(result);
			return false;
		}
	}

	return true;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFile(const std::string &filename)
{
	bool ret = false;

	int bufferSize = 0;
	char *buffer = 0;
	if (ResourceManager::GetSingleton().LoadBuffer(filename, bufferSize,
		buffer))
	{
		ret = CallBuffer(buffer, (unsigned long)bufferSize, filename);
	}

	return ret;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallBuffer(const char *buffer, unsigned long size, 
	const std::string &name)
{
	int result = mLuaPlusState->DoBuffer(buffer, size, name.c_str());
	if (result != 0)
	{
		OnError(result);
	}

	return true;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFileFunction(const std::string &filename,
	const std::string &funName)
{
	if (!filename.empty())
	{
		if (!CallFile(filename))
			return false;
	}

	LuaObject funcObj = mLuaPlusState->GetGlobal(funName.c_str());
	if (!funcObj.IsNil())
	{
		LuaCall call = funcObj;
		
		try
		{
			call << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFunction(const std::string &funName, Object *obj0)
{
	LuaObject funcObj = mLuaPlusState->GetGlobal(funName.c_str());
	if (!funcObj.IsNil())
	{
		LuaCall call = funcObj;

		try
		{
			LuaObject objdata0;
			objdata0.AssignLightUserdata(mLuaPlusState, obj0);

			call << objdata0 << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFunction(const std::string &funName,
	Object *obj0, int paramData1)
{
	LuaObject funcObj = mLuaPlusState->GetGlobal(funName.c_str());
	if (!funcObj.IsNil())
	{
		LuaCall call = funcObj;

		try
		{
			LuaObject objdata0;
			objdata0.AssignLightUserdata(mLuaPlusState, obj0);

			LuaObject objData1;
			objData1.AssignInteger(mLuaPlusState, paramData1);

			call << objdata0 << objData1 << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFunction(const std::string &funName,
	Object *obj0, const std::string &paramStr)
{
	LuaObject funcObj = mLuaPlusState->GetGlobal(funName.c_str());
	if (!funcObj.IsNil())
	{
		LuaCall call = funcObj;

		try
		{
			LuaObject objdata0;
			objdata0.AssignLightUserdata(mLuaPlusState, obj0);

			LuaObject objData1;
			objData1.AssignString(mLuaPlusState, paramStr.c_str(), 
				(int)paramStr.length());

			call << objdata0 << objData1 << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFunction(const std::string &funName,
	Object *obj0, int iData, const std::string &paramStr)
{
	LuaObject funcObj = mLuaPlusState->GetGlobal(funName.c_str());
	if (!funcObj.IsNil())
	{
		LuaCall call = funcObj;

		try
		{
			LuaObject objdata0;
			objdata0.AssignLightUserdata(mLuaPlusState, obj0);

			LuaObject objdata1;
			objdata1.AssignInteger(mLuaPlusState, iData);

			LuaObject objData2;
			objData2.AssignString(mLuaPlusState, paramStr.c_str(),
				(int)paramStr.length());

			call << objdata0 << objdata1 << objData2 << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LuaPlusContext::SetUserTypePointer(const std::string &luaName,
	const std::string &className, void *ptr)
{
	LuaStackBackup stackbackup((lua_State*)mLuaPlusState);
	tolua_pushusertype((lua_State*)mLuaPlusState, ptr, className.c_str());
	lua_setglobal((lua_State*)mLuaPlusState, luaName.c_str());
}
//----------------------------------------------------------------------------
void LuaPlusContext::SetUserFunction(const std::string &funName,
	const std::string &returnClassTypeName,
	ScriptGlobalFun globalFun)
{
	PX2_UNUSED(returnClassTypeName);

	mLuaPlusState->GetGlobals().RegisterDirect(funName.c_str(), globalFun);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallObjectFuntionValist(const std::string &funName,
	Object *paramObj, const std::string &format, va_list valist)
{
	lua_State *state = (lua_State*)mLuaPlusState;

	lua_getglobal(state, "this");
	lua_getglobal(state, paramObj->GetName().c_str());
	lua_setglobal(state, "this");

	const char *pfmt = format.c_str();
	int count = 0;
	static const char *args[] = { "arg0", "arg1", "arg2", "arg3" };
	while (pfmt[count])
	{
		if (*pfmt == 'i')
		{
			int value = va_arg(valist, int);
			lua_pushnumber(state, value);
		}
		else if (*pfmt == 'f')
		{
			float value = (float)(va_arg(valist, double));
			lua_pushnumber(state, value);
		}
		else if (*pfmt == 's')
		{
			char *str = va_arg(valist, char *);
			lua_pushstring(state, str);
		}
		else
		{
			assertion(false, "");
		}
		lua_setglobal(state, args[count++]);
	}

	{
		CallString(funName);
	}

	lua_setglobal(state, "this");

	return true;
}
//----------------------------------------------------------------------------
void LuaPlusContext::ClearEventFunctions()
{
	auto it = mEventFunObjects.begin();
	for (; it != mEventFunObjects.end(); it++)
	{
		std::vector<LuaPlusCallObject*> &objs = it->second;
		for (int i = 0; i < (int)objs.size(); i++)
		{
			LuaPlusCallObject *caller = objs[i];
			if (caller)
			{
				delete(caller);
			}
		}
	}
	mEventFunObjects.clear();
}
//----------------------------------------------------------------------------
bool LuaPlusContext::RegistEventFunction(const char *entName,
	LuaPlus::LuaObject callFunObject)
{
	std::string entNameStr(entName);
	if (!entNameStr.empty())
	{
		LuaPlusCallObject *callObj = new LuaPlusCallObject();
		callObj->TheLuaObject = callFunObject;

		mEventFunObjects[entNameStr].push_back(callObj);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LuaPlusContext::ClearEventObjectFunctions()
{
	auto it = mEventFunObjects.begin();
	for (; it != mEventFunObjects.end(); it++)
	{
		std::vector<LuaPlusCallObject*> &objs = it->second;
		auto it = objs.begin();
		for (; it != objs.end();)
		{
			LuaPlusCallObject *caller = *it;
			if (caller && caller->ObjectReset.IsValid())
			{
				delete(caller);
				it = objs.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}
//----------------------------------------------------------------------------
bool LuaPlusContext::RegistEventObjectFunction(const char *entName, 
	LuaPlus::LuaObject objReset, LuaPlus::LuaObject callFunObject)
{
	std::string entNameStr(entName);
	if (!entNameStr.empty())
	{
		LuaPlusCallObject *callObj = new LuaPlusCallObject();
		callObj->TheLuaObject = callFunObject;
		callObj->ObjectReset = objReset;

		mEventFunObjects[entNameStr].push_back(callObj);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LuaPlusContext::UnRegistAllEventFunctions(const char *entName)
{
	auto it = mEventFunObjects.find(std::string(entName));
	if (it != mEventFunObjects.end())
	{
		std::vector<LuaPlusCallObject*> &objs = it->second;
		for (int i = 0; i < (int)objs.size(); i++)
		{
			LuaPlusCallObject *caller = objs[i];
			if (caller)
			{
				delete(caller);
			}
		}

		mEventFunObjects.erase(it);
	}
}
//----------------------------------------------------------------------------
void LuaPlusContext::OnEvent(Event *ent)
{
	const std::string &entTypeStr = ent->GetEventTypeStr();
	const std::string &dataStr0 = ent->GetDataStr0();
	const std::string &dataStr1 = ent->GetDataStr1();
	const std::string &dataStr2 = ent->GetDataStr2();
	void *dataPointer0 = ent->GetDataPointer0();
	void *dataPointer1 = ent->GetDataPointer1();

	auto it = mEventFunObjects.begin();
	for (; it != mEventFunObjects.end(); it++)
	{
		const std::string &entName = it->first;
		if (entName == entTypeStr)
		{
			std::vector<LuaPlusCallObject*> &vecs = it->second;
			for (int i = 0; i < (int)vecs.size(); i++)
			{
				LuaPlus::LuaObject luaObj = vecs[i]->TheLuaObject;
				LuaPlus::LuaObject resetObj = vecs[i]->ObjectReset;

				if (!luaObj.IsNil())
				{
					LuaCall call = luaObj;

					if (!dataStr0.empty())
					{
						LuaObject luaObj0;
						luaObj0.AssignString(mLuaPlusState, dataStr0.c_str(), dataStr0.length());
						call << luaObj0;
					}
					if (!dataStr1.empty())
					{
						LuaObject luaObj1;
						luaObj1.AssignString(mLuaPlusState, dataStr1.c_str(), dataStr1.length());
						call << luaObj1;
					}
					if (!dataStr2.empty())
					{
						LuaObject luaObj2;
						luaObj2.AssignString(mLuaPlusState, dataStr2.c_str(), dataStr2.length());
						call << luaObj2;
					}

					if (!resetObj.IsNil())
					{
						call << resetObj;
					}

					if (dataPointer0)
					{
						LuaObject luaPointer0;
						luaPointer0.AssignLightUserdata(mLuaPlusState, dataPointer0);
						call << luaPointer0;
					}

					if (dataPointer1)
					{
						LuaObject luaPointer1;
						luaPointer1.AssignLightUserdata(mLuaPlusState, dataPointer1);
						call << luaPointer1;
					}

					call << LuaRun();
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void LuaPlusContext::OnError(int errorNum)
{
	PX2_UNUSED(errorNum);

	LuaPlus::LuaStackObject stackObj(mLuaPlusState, -1);
	const char* errStr = stackObj.GetString();
	if (errStr)
	{
		mLastError = errStr;
		ClearStack();
	}
	else
	{
		mLastError = "Unknown Lua parse error";
	}

	PX2_LOG_ERROR("LuaError:%s", mLastError.c_str());
	assertion(false, "LuaError:%s", mLastError.c_str());
}
//----------------------------------------------------------------------------
void LuaPlusContext::ClearStack(void)
{
	mLuaPlusState->SetTop(0);
}
//----------------------------------------------------------------------------