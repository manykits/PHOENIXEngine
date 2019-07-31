// PkgClassInfo.cpp

#include "PkgClassInfo.hpp"
#include "PkgEnumInfo.hpp"
using namespace PkgParse;

//----------------------------------------------------------------------------
PkgClassInfo::PkgClassInfo() :
IsTemplate(false),
TemplateFloat(false),
TemplateDouble(false),
TemplateInt(false),
IsVirtual(false),
IsValueType(false),
ParentClassInfo(0),
mDestructorFunction(0),
IsRefObject(false),
IsUseRefIncrease(true)
{
}
//----------------------------------------------------------------------------
PkgClassInfo::~PkgClassInfo()
{
	for (int i = 0; i < (int)mFunctions.size(); i++)
	{
		delete mFunctions[i];
	}

	for (int i = 0; i < (int)MemberProps.size(); i++)
	{
		delete MemberProps[i];
	}
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsSingleton() const
{
	return !SingletonName.empty(); 
}
//----------------------------------------------------------------------------
std::string PkgClassInfo::GetClassNameAS()
{
	if (IsTemplate)
		return ClassName + "f";

	return ClassName;
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsContainEnumType(const std::string &type)
{
	for (int i = 0; i < (int)EnumInfos.size(); i++)
	{
		if (EnumInfos[i]->IsContainEnumType(type))
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void PkgClassInfo::AddFunInfo(PkgFunInfo *info)
{
	if (PkgFunInfo::FT_CLASS_CONSTRUCTOR == info->TheFunType)
	{
		mConstructorFunctions.push_back(info);
	}
	else if (PkgFunInfo::FT_CLASS_DESTRUCTOR == info->TheFunType)
	{
		mDestructorFunction = info;
	}

	info->ClassInfo = this;
	
	mFunctions.push_back(info);
}
//----------------------------------------------------------------------------
std::vector<PkgFunInfo*> &PkgClassInfo::GetFunctions()
{
	return mFunctions;
}
//----------------------------------------------------------------------------
std::vector<PkgFunInfo*> &PkgClassInfo::GetConstructorFunctions()
{
	return mConstructorFunctions;
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsHasConstruct() const
{
	return (int)mConstructorFunctions.size() > 0;
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsHasDestruct() const
{
	return 0 != mDestructorFunction;
}
//----------------------------------------------------------------------------
PkgFunInfo *PkgClassInfo::GetDestructorFunction()
{
	return mDestructorFunction;
}
//----------------------------------------------------------------------------
void PkgClassInfo::ClearRegistFun()
{
	mRegistedFuns.clear();
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsFunRegisted(const std::string &funStrAS) const
{
	return mRegistedFuns.find(funStrAS) != mRegistedFuns.end();
}
//----------------------------------------------------------------------------
void PkgClassInfo::AddRegistFun(const std::string &funStrAS)
{
	mRegistedFuns[funStrAS] = 1;
}
//----------------------------------------------------------------------------
void PkgClassInfo::ClearRegistParam()
{
	mRegistedParams.clear();
}
//----------------------------------------------------------------------------
bool PkgClassInfo::IsParamRegisted(const std::string &paramStrAS) const
{
	return mRegistedParams.find(paramStrAS) != mRegistedParams.end();
}
//----------------------------------------------------------------------------
void PkgClassInfo::AddRegistParam(const std::string &paramStrAS)
{
	mRegistedParams[paramStrAS] = 1;
}
//----------------------------------------------------------------------------