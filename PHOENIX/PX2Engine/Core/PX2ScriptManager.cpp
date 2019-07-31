// PX2ScriptManager.cpp

#include "PX2ScriptManager.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ScriptManager::ScriptManager() :
mNextRegistObjectID(0)
{
}
//----------------------------------------------------------------------------
ScriptManager::~ScriptManager()
{
}
//----------------------------------------------------------------------------
void ScriptManager::Ternimate(ScriptContext::ContextType type)
{
	if (type == ScriptContext::CT_LUA)
		mContextLua = 0;
}
//----------------------------------------------------------------------------
void ScriptManager::TernimateAll()
{
	mContextLua = 0;
}
//----------------------------------------------------------------------------
void ScriptManager::AddContext(ScriptContext *ctex)
{
	if (ScriptContext::CT_LUA == ctex->GetType())
	{
		mContextLua = ctex;
	}
}
//----------------------------------------------------------------------------
ScriptContext *ScriptManager::GetContext(ScriptContext::ContextType type)
{
	if (ScriptContext::CT_LUA == type)
	{
		return mContextLua;
	}

	return 0;
}
//----------------------------------------------------------------------------
bool ScriptManager::IsGlobalNameExist(const std::string &objectName)
{
	std::map<std::string, void*>::iterator it = mGlobals.find(objectName);
	if (it != mGlobals.end())
		return true;

	return false;
}
//----------------------------------------------------------------------------
bool ScriptManager::AddGlobalName(const std::string &objectName,
	void *object)
{
	mGlobals[objectName] = object;

	return true;
}
//----------------------------------------------------------------------------
bool ScriptManager::RemoveGlobalName(const std::string &objectName)
{
	std::map<std::string, void*>::iterator it = mGlobals.find(objectName);
	if (it != mGlobals.end())
	{
		mGlobals.erase(it);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
int ScriptManager::GetNextRegistObjectID()
{
	return ++mNextRegistObjectID;
}
//----------------------------------------------------------------------------
void ScriptManager::SetUserTypePointer(const std::string &luaName,
	const std::string &className, void *ptr)
{
	if (mContextLua)
	{
		mContextLua->SetUserTypePointer(luaName, className, ptr);
	}
}
//----------------------------------------------------------------------------