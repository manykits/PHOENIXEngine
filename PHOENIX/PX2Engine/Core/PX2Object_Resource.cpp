// PX2Object_Resource.cpp

#include "PX2Object.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void Object::SetResourcePath(const std::string& name)
{
	mResourcePath = name;
	mResourcePathFString = mResourcePath.c_str();
}
//----------------------------------------------------------------------------
const std::string& Object::GetResourcePath() const
{
	return mResourcePath;
}
//----------------------------------------------------------------------------
const FString& Object::GetResourcePathFString() const
{
	return mResourcePathFString;
}
//----------------------------------------------------------------------------