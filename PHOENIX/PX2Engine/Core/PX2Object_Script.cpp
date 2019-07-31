// PX2Object_Script.cpp

#include "PX2Object.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void Object::SetUserScriptName(const std::string &scriptName)
{
	mUserScriptName = scriptName;
}
//----------------------------------------------------------------------------
const std::string &Object::GetUserScriptName() const
{
	return mUserScriptName;
}
//----------------------------------------------------------------------------
const std::string &Object::GetScriptName() const
{
	return mScriptName;
}
//----------------------------------------------------------------------------
bool Object::RegistToScriptSystem()
{
	if (IsRegistedToScriptSystem())
		return false;

	ScriptManager *sm = ScriptManager::GetSingletonPtr();
	if (!sm) return false;

	const std::string &rttiName = GetRttiType().GetName();
	const std::string spaceName = "PX2.";
	size_t nameLength = rttiName.size() - spaceName.length();
	std::string className = rttiName.substr(spaceName.length(), nameLength);

	if (!mUserScriptName.empty())
	{
		mScriptName = mUserScriptName;
	}
	else
	{
		mScriptName = className + "_ScriptID_"
			+ StringHelp::IntToString(sm->GetNextRegistObjectID());
	}

	PX2_SM.AddGlobalName(mScriptName, this);
	PX2_SM.SetUserTypePointer(mScriptName.c_str(), className.c_str(), this);

	return true;
}
//----------------------------------------------------------------------------
bool Object::UnRegistToScriptSystem()
{
	ScriptManager *sm = ScriptManager::GetSingletonPtr();
	if (!sm) return false;

	const std::string &rttiName = GetRttiType().GetName();
	const std::string spaceName = "PX2.";
	size_t nameLength = rttiName.size() - spaceName.length();
	std::string className = rttiName.substr(spaceName.length(), nameLength);

	PX2_SM.RemoveGlobalName(mScriptName);
	PX2_SM.SetUserTypePointer(mScriptName.c_str(), className.c_str(), 0);

	return true;
}
//----------------------------------------------------------------------------
bool Object::IsRegistedToScriptSystem()
{
	ScriptManager *sm = ScriptManager::GetSingletonPtr();
	if (!sm) return false;

	return PX2_SM.IsGlobalNameExist(mScriptName);
}
//----------------------------------------------------------------------------