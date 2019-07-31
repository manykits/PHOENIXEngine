// PkgEnumInfo.cpp

#include "PkgEnumInfo.hpp"
using namespace PkgParse;

//----------------------------------------------------------------------------
PkgEnumInfo::PkgEnumInfo() :
ClassInfo(0)
{
}
//----------------------------------------------------------------------------
PkgEnumInfo::~PkgEnumInfo()
{
}
//----------------------------------------------------------------------------
bool PkgEnumInfo::IsContainEnumType(const std::string &type)
{
	return type == EnumName;
}
//----------------------------------------------------------------------------