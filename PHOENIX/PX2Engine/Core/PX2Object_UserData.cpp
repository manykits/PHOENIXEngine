// PX2Object_UserData.cpp

#include "PX2Object.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void Object::SetUserDataFloat(const std::string &name, float val)
{
	mUserDataFloats[name] = val;
}
//----------------------------------------------------------------------------
float Object::GetUserDataFloat(const std::string &name, bool *isValied) const
{
	std::map<std::string, float>::const_iterator it = mUserDataFloats.find(name);
	if (it != mUserDataFloats.end())
	{
		if (isValied)
		{
			*isValied = true;
		}
		return it->second;
	}

	*isValied = false;
	return 0.0f;
}
//----------------------------------------------------------------------------
void Object::SetUserDataString(const std::string &name, const std::string &val)
{
	mUserDataStrings[name] = val;
}
//----------------------------------------------------------------------------
std::string Object::GetUserDataString(const std::string &name, 
	bool *isValied) const
{
	std::map<std::string, std::string>::const_iterator it =
		mUserDataStrings.find(name);
	if (it != mUserDataStrings.end())
	{
		if (isValied)
		{
			*isValied = true;
		}
		return it->second;
	}

	*isValied = false;
	return "";
}
//----------------------------------------------------------------------------
void Object::SetUserData(const std::string &name, const Any &any)
{
	mUserDatas[name] = any;
}
//----------------------------------------------------------------------------
Any Object::GetUserDataAny(const std::string &name, bool *isValied) const
{
	std::map<std::string, Any>::const_iterator it = mUserDatas.find(name);
	if (it != mUserDatas.end())
	{
		if (isValied)
		{
			*isValied = true;
		}
		return it->second;
	}

	*isValied = false;
	return Any();
}
//----------------------------------------------------------------------------
void Object::SetUserDataPointer(const std::string &name, void *p)
{
	mUserDataPointers[name] = p;
}
//----------------------------------------------------------------------------
void *Object::GetUserDataPointer(const std::string &name, bool *isValied)
const
{
	std::map<std::string, void*>::const_iterator it =
		mUserDataPointers.find(name);
	if (it != mUserDataPointers.end())
	{
		if (isValied)
		{
			*isValied = true;
		}
		return it->second;
	}

	if (isValied)
		*isValied = false;

	return 0;
}
//----------------------------------------------------------------------------