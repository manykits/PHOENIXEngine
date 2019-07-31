// PkgFunInfo.cpp

#include "PkgFunctionInfo.hpp"
#include "PkgClassInfo.hpp"
#include "PkgInfoManager.hpp"
using namespace PkgParse;

//----------------------------------------------------------------------------
ParamInfo::ParamInfo() :
IsReturn(false),
TheParamType(PT_MAX_TYPE),
IsPointer(false),
IsRef(false),
IsConst(false),
FunInfo(0),
IsStatic(false)
{
}
//----------------------------------------------------------------------------
ParamInfo::~ParamInfo()
{
}
//----------------------------------------------------------------------------
std::string ParamInfo::GetTypeStrAS() const
{
	std::string typeStr = TypeStr;

	std::string strRep = "std::";
	std::string::size_type st = typeStr.find(strRep);
	if (st != std::string::npos)
	{
		typeStr.replace(st, strlen(strRep.c_str()), "");
	}

	strRep = "unsigned short";
	st = typeStr.find(strRep);
	if (st != std::string::npos)
	{
		typeStr.replace(st, strlen(strRep.c_str()), "uint16");
	}

	strRep = "unsigned char";
	st = typeStr.find(strRep);
	if (st != std::string::npos)
	{
		typeStr.replace(st, strlen(strRep.c_str()), "uint8");
	}

	strRep = "char";
	st = typeStr.find(strRep);
	if (st != std::string::npos)
	{
		typeStr.replace(st, strlen(strRep.c_str()), "int8");
	}

	strRep = "unsigned long";
	st = typeStr.find(strRep);
	if (st != std::string::npos)
	{
		typeStr.replace(st, strlen(strRep.c_str()), "uint64");
	}

	strRep = "unsigned int";
	st = typeStr.find(strRep);
	if (st != std::string::npos)
	{
		typeStr.replace(st, strlen(strRep.c_str()), "uint32");
	}

	strRep = "::";
	st = typeStr.find(strRep);
	if (st != std::string::npos)
	{
		typeStr = typeStr.substr(st+2, typeStr.length());
	}

	strRep = "<T>";
	st = typeStr.find(strRep);
	if (st != std::string::npos)
	{
		typeStr.replace(st, strlen(strRep.c_str()), "f");
	}

	if ("T" == typeStr)
		typeStr = "float";

	if (typeStr.empty())
	{
		typeStr = "void";
	}

	return typeStr;
}
//----------------------------------------------------------------------------
bool ParamInfo::IsInternalEnumType() const
{
	return FunInfo->ClassInfo->IsContainEnumType(TypeStr);
}
//----------------------------------------------------------------------------
std::string ParamInfo::GetParamStr_FunPtr() const
{
	std::string str = TypeStr;

	if (IsInternalEnumType())
	{
		str =  FunInfo->ClassInfo->ClassName + "::" + str;
	}

	std::string strRep = "<T>";
	std::string::size_type st = str.find(strRep);
	if (st != std::string::npos)
	{
		str.replace(st, strlen(strRep.c_str()), "f");
	}

	if ("T" == str)
		str = "float";

	if (IsConst)
		str = "const " + str;

	if (IsPointer)
		str += " * ";

	if (IsRef)
		str += " & ";

	return str;
}
//----------------------------------------------------------------------------
std::string ParamInfo::GetASStr() const
{
	std::string str;

	if (IsConst)
		str += "const ";

	std::string typeStr = GetTypeStrAS();
	str += typeStr;
	str += " ";

	if (IsPointer)
	{
		PkgClassInfo *classInfo = PkgInfoManager::GetSingleton().GetClassInfo(typeStr);
		if (classInfo)
		{
			if (classInfo->IsValueType)
			{
				str += "& ";
			}
			else if (classInfo->IsRefObject)
			{
				str += "@+ ";
			}
			else
			{
				str += "@ ";
			}
		}
		else
		{
			str += "& ";
		}
	}

	if (IsRef)
		str += "& ";

	str += ValueNameStr;
	
	return str;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
PkgFunInfo::PkgFunInfo() :
IsVirtual(false),
IsPureVirtual(false),
IsInline(false),
IsConst(false),
TheFunType(PT_MAX_TYPE),
IsOperator(false),
ClassInfo(0),
ReturnVal(0)
{
}
//----------------------------------------------------------------------------
PkgFunInfo::~PkgFunInfo()
{
	delete ReturnVal;

	for (int i = 0; i < (int)Params.size(); i++)
	{
		delete Params[i];
	};
}
//----------------------------------------------------------------------------
std::string PkgFunInfo::GetASFunStr() const
{
	std::string str;

	if (ReturnVal)
	{
		if (ReturnVal->IsConst)
			str += "const ";

		std::string typeStr = ReturnVal->GetTypeStrAS();
		str += typeStr;
		str += " ";

		if (ReturnVal->IsPointer)
		{
			PkgClassInfo *classInfo = PkgInfoManager::GetSingleton().GetClassInfo(typeStr);
			if (classInfo && classInfo->IsRefObject)
			{
				str += "@+ ";
			}
			else
			{
				str += "@ ";
			}
		}

		if (ReturnVal->IsRef)
			str += "& ";
	}
	else
	{
		str += "void ";
	}

	if (FT_CLASS_CONSTRUCTOR == TheFunType)
	{
		str += "f";
	}
	else if (IsOperator)
	{
		if ("=" == FunName)
		{
			str += "opAssign";
		}
		else if ("+=" == FunName)
		{
			str += "opAddAssign ";
		}
		else if ("-= " == FunName)
		{
			str += "opSubAssign";
		}
		else if ("*=" == FunName)
		{
			str += "opMulAssign";
		}
	}
	else
	{
		str += FunName;
	}

	str += "(";

	str += GetASParamStr();

	str += ")";

	if (IsConst)
		str += " const";

	return str;
}
//----------------------------------------------------------------------------
std::string PkgFunInfo::GetASFactoryFunStr() const
{
	std::string str;

	if (ClassInfo->IsRefObject)
		str += FunName + " @ ";
	else
		str += FunName + " @ ";

	str += "f";

	str += "(";

	str += GetASParamStr();

	str += ")";

	return str;
}
//----------------------------------------------------------------------------
std::string PkgFunInfo::GetASParamStr() const
{
	std::string str;

	for (int i = 0; i < (int)Params.size(); i++)
	{
		if (0 != i)
			str += ", ";

		const ParamInfo *paramInfo = Params[i];

		str += paramInfo->GetASStr();
	}

	return str;
}
//----------------------------------------------------------------------------
std::string PkgFunInfo::GetASParamStr_C_NoValue() const
{
	std::string str;

	for (int i = 0; i < (int)Params.size(); i++)
	{
		if (0 != i)
			str += ", ";

		const ParamInfo *paramInfo = Params[i];

		if (paramInfo->IsConst)
			str += "const ";

		if (paramInfo->IsInternalEnumType())
			str += ClassInfo->ClassName + "::" + paramInfo->GetTypeStrAS();
		else
			str += paramInfo->GetTypeStrAS();
		str += " ";

		if (paramInfo->IsPointer)
			str += "* ";

		if (paramInfo->IsRef)
			str += "& ";

		str += paramInfo->ValueNameStr;
	}

	return str;
}
//----------------------------------------------------------------------------
std::string PkgFunInfo::GetASValueStr() const
{
	std::string str;

	for (int i = 0; i < (int)Params.size(); i++)
	{
		if (0 != i)
			str += ", ";

		const ParamInfo *paramInfo = Params[i];
		str += paramInfo->ValueNameStr;
	}

	return str;
}
//----------------------------------------------------------------------------