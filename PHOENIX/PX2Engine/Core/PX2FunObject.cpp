// PX2FunObject.cpp

#include "PX2FunObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// FunParam
//----------------------------------------------------------------------------
FunParam::FunParam() :
Type(FPT_INT),
IsEnum(false),
IsEnumItem(false)
{
}
//----------------------------------------------------------------------------
FunParam::~FunParam()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FunObject
//----------------------------------------------------------------------------
FunObject::FunObject() :
IsClassCatalogue(false),
mParamType(PT_NONE),
IsEnum(false),
IsStatic(false),
ParentFunObject(0)
{
}
//----------------------------------------------------------------------------
FunObject::~FunObject()
{
}
//----------------------------------------------------------------------------
void FunObject::Save(XMLNode node)
{
	node.SetAttributeString("Name", Name);
	node.SetAttributeBool("IsClassCatalogue", IsClassCatalogue);
	node.SetAttributeString("ParentClassName", ParentClassName);
	node.SetAttributeBool("IsStatic", IsStatic);
	node.SetAttributeString("ClassName", ClassName);
	node.SetAttributeString("SigletonName", SigletonName);
	node.SetAttributeInt("ParamType", mParamType);

	int numInParam = (int)mInParams.size();
	int numOutParam = (int)mOutParams.size();

	node.SetAttributeInt("NumInParam", numInParam);
	node.SetAttributeInt("NumOutParam", numOutParam);

	XMLNode paramNodeIn = node.NewChild("ParamsIn");
	for (int i = 0; i < numInParam; i++)
	{
		XMLNode paramNode = paramNodeIn.NewChild("param");

		FunParam &funParam = mInParams[i];
		paramNode.SetAttributeString("name", funParam.Name);
		paramNode.SetAttributeInt("type", funParam.Type);
	}

	XMLNode paramNodeOut = node.NewChild("ParamsOut");
	for (int i = 0; i < numOutParam; i++)
	{
		XMLNode paramNode = paramNodeOut.NewChild("param");

		FunParam &funParam = mOutParams[i];
		paramNode.SetAttributeString("name", funParam.Name);
		paramNode.SetAttributeInt("type", funParam.Type);
	}
}
//----------------------------------------------------------------------------
void FunObject::Clear()
{
	mInParams.clear();
	mOutParams.clear();
}
//----------------------------------------------------------------------------
void FunObject::AddInput(const std::string &paramName,
	FunParamType type, const std::string &tName, const Any &paramValue,
	bool isEnumItem, bool isEnum)
{
	FunParam funParam;
	funParam.Name = paramName;
	funParam.Type = type;
	funParam.TypeName = tName;
	funParam.IsEnum = isEnum;
	funParam.IsEnumItem = isEnumItem;
	funParam.Value = paramValue;
	mInParams.push_back(funParam);
}
//----------------------------------------------------------------------------
void FunObject::RemoveInput(const std::string &paramName)
{
	auto it = mInParams.begin();
	for (; it != mInParams.end(); it++)
	{
		if (paramName == it->Name)
		{
			mInParams.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void FunObject::AddOutput(const std::string &paramName, FunParamType type,
	const std::string &tName,
	const Any &paramValue, bool isEnum)
{
	FunParam funParam;
	funParam.Name = paramName;
	funParam.Type = type;
	funParam.TypeName = tName;
	funParam.Value = paramValue;
	funParam.IsEnum = isEnum;
	mOutParams.push_back(funParam);
}
//----------------------------------------------------------------------------
const std::vector<FunParam> &FunObject::GetInParams() const
{
	return mInParams;
}
//----------------------------------------------------------------------------
int FunObject::GetNumInParams() const
{
	return (int)mInParams.size();
}
//----------------------------------------------------------------------------
const FunParam &FunObject::GetInParam(int i) const
{
	return mInParams[i];
}
//----------------------------------------------------------------------------
void FunObject::ClearInParams()
{
	mInParams.clear();
}
//----------------------------------------------------------------------------
const std::vector<FunParam> &FunObject::GetOutParams() const
{
	return mOutParams;
}
//----------------------------------------------------------------------------
int FunObject::GetNumOutParams() const
{
	return (int)mOutParams.size();
}
//----------------------------------------------------------------------------
const FunParam &FunObject::GetOutParam(int i) const
{
	return mOutParams[i];
}
//----------------------------------------------------------------------------
void FunObject::ClearOutParams()
{
	mOutParams.clear();
}
//----------------------------------------------------------------------------
void FunObject::SetParamType(ParamType pt)
{
	mParamType = pt;
}
//----------------------------------------------------------------------------
FunObject::ParamType FunObject::GetParamType() const
{
	return mParamType;
}
//----------------------------------------------------------------------------
FunObject *FunObject::GetClass(const std::string &className)
{
	if (IsClassCatalogue && ClassName == className)
		return this;

	std::vector<Pointer0<FunObject> >::iterator it = mChildFunObjectVec_Class.begin();
	for (; it != mChildFunObjectVec_Class.end(); it++)
	{
		FunObject *funObj = (*it)->GetClass(className);
		if (funObj)
		{
			return funObj;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *FunObject::AddClass(const std::string &className)
{
	FunObjectPtr funObj = new0 FunObject();
	funObj->IsClassCatalogue = true;
	funObj->ClassName = className;

	AddFunObject(funObj);

	return funObj;
}
//----------------------------------------------------------------------------
FunObject *FunObject::GetEnum(const std::string &enumName)
{
	if (IsEnum && Name == enumName)
		return this;

	std::vector<Pointer0<FunObject> >::iterator it = mChildFunObjectVec_Enum.begin();
	for (; it != mChildFunObjectVec_Enum.end(); it++)
	{
		FunObject *funObj = (*it)->GetEnum(enumName);
		if (funObj)
		{
			return funObj;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *FunObject::AddEnum(const std::string &enumName)
{
	FunObjectPtr funObj = new0 FunObject();
	funObj->IsEnum = true;
	funObj->Name = enumName;

	AddFunObject(funObj);

	return funObj;
}
//----------------------------------------------------------------------------
FunObject *FunObject::GetFunObject(const std::string &className,
	const std::string &funName)
{
	if (!IsClassCatalogue && ClassName == className && Name == funName)
		return this;

	std::vector<Pointer0<FunObject> >::iterator it = mChildFunObjectVec.begin();
	for (; it != mChildFunObjectVec.end(); it++)
	{
		FunObject *funObj = (*it)->GetFunObject(className, funName);
		if (funObj)
		{
			return funObj;
		}
	}

	std::vector<Pointer0<FunObject> >::iterator itEnum = mChildFunObjectVec_Enum.begin();
	for (; itEnum != mChildFunObjectVec_Enum.end(); itEnum++)
	{
		FunObject *funObj = (*itEnum)->GetFunObject(className, funName);
		if (funObj)
		{
			return funObj;
		}
	}

	std::vector<Pointer0<FunObject> >::iterator it1 = mChildFunObjectVec_Class.begin();
	for (; it1 != mChildFunObjectVec_Class.end(); it1++)
	{
		FunObject *funObj = (*it1)->GetFunObject(className, funName);
		if (funObj)
		{
			return funObj;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
void FunObject::AddFunObject(FunObject *funObj)
{
	if (IsHasFunObject(funObj))
		return;

	if (funObj->IsClassCatalogue)
	{
		mChildFunObjectVec_Class.push_back(funObj);
		funObj->ParentFunObject = this;
	}
	else if (funObj->IsEnum)
	{
		mChildFunObjectVec_Enum.push_back(funObj);
		funObj->ParentFunObject = this;
	}
	else
	{
		funObj->ClassName = ClassName;
		mChildFunObjectVec.push_back(funObj);
	}
}
//----------------------------------------------------------------------------
bool FunObject::IsHasFunObject(FunObject *funObj)
{
	for (int i = 0; i < (int)mChildFunObjectVec_Class.size(); i++)
	{
		if (funObj == mChildFunObjectVec_Class[i])
			return true;
	}

	for (int i = 0; i < (int)mChildFunObjectVec_Enum.size(); i++)
	{
		if (funObj == mChildFunObjectVec_Enum[i])
			return true;
	}

	for (int i = 0; i<(int)mChildFunObjectVec.size(); i++)
	{
		if (funObj == mChildFunObjectVec[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
std::vector<FunObject*> FunObject::GetParentAndMeFunObjectList()
{
	std::vector<FunObject*> funList;
	funList.push_back(this);

	FunObject *parent = ParentFunObject;
	while (parent)
	{
		funList.push_back(parent);

		parent = parent->ParentFunObject;
	}

	return funList;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FunObjectManager
//----------------------------------------------------------------------------
FunObjectManager::FunObjectManager()
{
}
//----------------------------------------------------------------------------
FunObjectManager::~FunObjectManager()
{
}
//----------------------------------------------------------------------------
bool FunObjectManager::Initlize()
{
	return true;
}
//----------------------------------------------------------------------------
bool FunObjectManager::Terminate()
{
	mObjectFunObject = 0;
	mFunMap.clear();

	return true;
}
//----------------------------------------------------------------------------
void FunObjectManager::SetObjectFunObject(FunObject *funObject)
{
	mObjectFunObject = funObject;
}
//----------------------------------------------------------------------------
FunObject *FunObjectManager::GetObjectFunObject()
{
	return mObjectFunObject;
}
//----------------------------------------------------------------------------
std::map<std::string, FunObjectPtr> FunObjectManager::GetFunMap()
{
	return mFunMap;
}
//----------------------------------------------------------------------------
FunObject *FunObjectManager::GetAddFunMap(const std::string &name)
{
	std::map<std::string, FunObjectPtr>::iterator it = mFunMap.find(name);
	if (it != mFunMap.end())
	{
		return it->second;
	}

	FunObject *funObj = new0 FunObject();
	mFunMap[name] = funObj;

	return funObj;
}
//----------------------------------------------------------------------------