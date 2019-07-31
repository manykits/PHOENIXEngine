// PkgInfoManager.cpp

#include "PkgInfoManager.hpp"
#include "PkgXMLData.hpp"
using namespace PkgParse;
using namespace std;

//----------------------------------------------------------------------------
std::string gFunTypeStrs[PkgFunInfo::FunType::PT_MAX_TYPE] =
{
	"class_constructor",
	"class_destructor",
	"class_member",
	"class_static",
	"static"
};
//----------------------------------------------------------------------------
PkgInfoManager &PkgInfoManager::GetSingleton()
{
	static PkgInfoManager inst;
	return inst;
}
//----------------------------------------------------------------------------
PkgInfoManager *PkgInfoManager::GetSingletonPtr()
{
	return &GetSingleton();
}
//----------------------------------------------------------------------------
void PkgInfoManager::AddClassInfo(const std::string &name, PkgClassInfo *classInfo)
{
	mClassInfos.push_back(classInfo);
	mClassInfoMap[name] = classInfo;
}
//----------------------------------------------------------------------------
PkgClassInfo *PkgInfoManager::GetClassInfo(const std::string &className)
{
	std::map<std::string, PkgClassInfo*>::iterator it = 
		mClassInfoMap.find(className);

	if (it != mClassInfoMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void PkgInfoManager::AddEnumInfo(const std::string &name, PkgEnumInfo *enumInfo)
{
	mEnumInfos.push_back(enumInfo);
	mEnumInfoMap[name] = enumInfo;
}
//----------------------------------------------------------------------------
PkgEnumInfo *PkgInfoManager::GetEnumInfo(const std::string &enumName)
{
	std::map<std::string, PkgEnumInfo*>::iterator it =
		mEnumInfoMap.find(enumName);

	if (it != mEnumInfoMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void PkgInfoManager::Ternimate()
{
	for (int i = 0; i < (int)mClassInfos.size(); i++)
	{
		PkgClassInfo* pkgInfo = mClassInfos[i];
		delete(pkgInfo);
	}

	mClassInfos.clear();
	mClassInfoMap.clear();

	for (int i = 0; i < (int)mEnumInfos.size(); i++)
	{
		PkgEnumInfo *enumInfo = mEnumInfos[i];
		delete(enumInfo);
	}
	mEnumInfos.clear();
}	
//----------------------------------------------------------------------------
void PkgInfoManager::SaveParam(XMLNode &paramNode, ParamInfo *paramInfo)
{
	paramNode.SetAttributeString("paramstr", paramInfo->ParamStr);
	paramNode.SetAttributeBool("is_retrun", paramInfo->IsReturn);
	paramNode.SetAttributeString("type_str", paramInfo->TypeStr);
	paramNode.SetAttributeString("valuename_str", paramInfo->ValueNameStr);
	paramNode.SetAttributeString("defaultvalue_Str", paramInfo->DefaultValStr);
	paramNode.SetAttributeBool("is_pointer", paramInfo->IsPointer);
	paramNode.SetAttributeBool("is_ref", paramInfo->IsRef);
	paramNode.SetAttributeBool("is_const", paramInfo->IsConst);
}
//----------------------------------------------------------------------------
void PkgInfoManager::CalInherit()
{
	for (int i = 0; i < (int)mClassInfos.size(); i++)
	{
		PkgClassInfo *classInfo = mClassInfos[i];
		if (!classInfo->ParentClassName.empty())
		{
			PkgClassInfo *parentClassInfo = GetClassInfo(classInfo->ParentClassName);
			if (parentClassInfo)
			{
				classInfo->ParentClassInfo = parentClassInfo;
				parentClassInfo->ChildClassInfos.push_back(classInfo);
			}
		}
	}

	PkgClassInfo *classInfo = GetClassInfo("RefObject");
	if (classInfo)
	{
		SetRefObject(classInfo);
	}
}
//----------------------------------------------------------------------------
void PkgInfoManager::SetRefObject(PkgClassInfo *classInfo)
{
	classInfo->IsRefObject = true;

	for (int i = 0; i < (int)classInfo->ChildClassInfos.size(); i++)
	{
		PkgClassInfo *childClassInfo = classInfo->ChildClassInfos[i];
		if (childClassInfo)
		{
			SetRefObject(childClassInfo);
		}
	}
}
//----------------------------------------------------------------------------
bool PkgInfoManager::Save(const std::string &filename)
{
	XMLData data;
	data.Create();

	XMLNode rootNode = data.NewChild("rootnode");

	XMLNode enumsNode = rootNode.NewChild("enums");
	enumsNode.SetAttributeInt("num", (int)mEnumInfos.size());
	for (int i = 0; i < (int)mEnumInfos.size(); i++)
	{
		PkgEnumInfo *enumInfo = mEnumInfos[i];
		if (!enumInfo->ClassInfo)
		{
			SaveEnumInfo(enumsNode, enumInfo);
		}
	}

	XMLNode classesNode = rootNode.NewChild("classes");
	classesNode.SetAttributeInt("num", (int)mClassInfos.size());
	for (int i = 0; i < (int)mClassInfos.size(); i++)
	{
		PkgClassInfo *classInfo = mClassInfos[i];

		const std::string &className = classInfo->ClassName;		
		XMLNode classNode = classesNode.NewChild("class");

		for (int k = 0; k < (int)classInfo->EnumInfos.size(); k++)
		{
			PkgEnumInfo *enumInfo = classInfo->EnumInfos[k];
			SaveEnumInfo(classNode, enumInfo);
		}

		classNode.SetAttributeString("name", className.c_str());
		classNode.SetAttributeString("parentname", classInfo->ParentClassName.c_str());
		classNode.SetAttributeString("singletonname", classInfo->SingletonName.c_str());

		for (int i = 0; i < (int)classInfo->GetFunctions().size(); i++)
		{
			PkgFunInfo *info = classInfo->GetFunctions()[i];
			XMLNode classFunctionNode = classNode.NewChild("function");
			classFunctionNode.SetAttributeString("name", info->FunName);
			classFunctionNode.SetAttributeString("funtype", gFunTypeStrs[info->TheFunType]);
			classFunctionNode.SetAttributeBool("is_operator", info->IsOperator);
			classFunctionNode.SetAttributeBool("is_purevirtual", info->IsPureVirtual);
			classFunctionNode.SetAttributeBool("is_virtual", info->IsVirtual);
			classFunctionNode.SetAttributeBool("is_inline", info->IsInline);
			classFunctionNode.SetAttributeBool("is_const", info->IsConst);

			std::vector<ParamInfo*> params = info->Params;
			params.insert(params.begin(), info->ReturnVal);

			for (int i = 0; i < (int)params.size(); i++)
			{
				ParamInfo *paramInfo = params[i];
				if (paramInfo)
				{
					XMLNode paramNodeReturn = classFunctionNode.NewChild("param");
					SaveParam(paramNodeReturn, paramInfo);
				}
			}
		}
	}

	data.SaveFile(filename);

	return true;
}
//----------------------------------------------------------------------------
void PkgInfoManager::SaveEnumInfo(XMLNode &parentNode, PkgEnumInfo *enumInfo)
{
	const std::string &enumName = enumInfo->EnumName;

	XMLNode enumNode = parentNode.NewChild("enum");
	enumNode.SetAttributeString("name", enumName.c_str());

	for (int i = 0; i < (int)enumInfo->EnumItemStrs.size(); i++)
	{
		const std::string &enumItemStr = enumInfo->EnumItemStrs[i];
		XMLNode enumItemNode = enumNode.NewChild("enum_item");
		enumItemNode.SetAttributeString("itemstr", enumItemStr);
	}
}
//----------------------------------------------------------------------------
void SplitBaseFilename(const std::string &baseFileName,
	std::string &outBaseName,
	std::string &outExtention)
{
	size_t i = baseFileName.find_last_of(".");

	if (i == std::string::npos)
	{
		outExtention.clear();
		outBaseName = baseFileName;
	}
	else
	{
		outExtention = baseFileName.substr(i + 1);
		outBaseName = baseFileName.substr(0, i);
	}
}
//----------------------------------------------------------------------------
bool PkgInfoManager::GenToASFile(const std::string &filename)
{
	std::ofstream fout(filename, std::ios::out);
	fout.clear();

	fout << "// " << filename << endl << endl;

	std::string headerCpp;
	std::string baseFileName;
	std::string ext;
	SplitBaseFilename(filename, baseFileName, ext);
	std::string headFilename = baseFileName + ".hpp";

	fout << "#include <" << headFilename << ">" << endl;
	fout << "#include \"angelscript.h\"" << endl;
	fout << "using namespace std;" << endl << endl;

	//----------------------------------------------------------------------------
	fout << "void FakeAddRef(void* ptr) {}" << endl;
	fout << "void FakeReleaseRef(void* ptr) {}" << endl << endl;
	fout << "template<class A, class B>" << endl;
	fout << "B* RefCast(A* a)" << endl;
	fout << "{" << endl;
	fout << "	if( !a )" << endl;
	fout << "		return 0;" << endl;
	fout << "	B* b = dynamic_cast<B*>(a);" << endl;
	fout << "	return b;" << endl;
	fout << "}" << endl << endl;

	for (int i = 0; i < (int)mClassInfos.size(); i++)
	{
		PkgClassInfo *pkgClassInfo = mClassInfos[i];

		std::string className = pkgClassInfo->ClassName;

		std::vector<PkgFunInfo*> constrFunInfos = pkgClassInfo->GetConstructorFunctions();
		for (int j = 0; j < (int)constrFunInfos.size(); j++)
		{
			const PkgFunInfo *funInfo = constrFunInfos[j];
			if (pkgClassInfo->IsValueType)
			{
				std::string paramStr = funInfo->GetASParamStr_C_NoValue();
				if (!paramStr.empty())
					paramStr += ", ";

				std::string paramValStr = funInfo->GetASValueStr();
				fout << "static void Ctor_" << funInfo->FunName << j << "(" << paramStr << pkgClassInfo->GetClassNameAS() << "* _pointer)" << endl;
				fout << "{" << endl;
				fout << "	new(_pointer) " << pkgClassInfo->GetClassNameAS() << "(" << paramValStr << ");" << endl;
				fout << "}" << endl;
			}
			else
			{
				std::string paramStr = funInfo->GetASParamStr_C_NoValue();
				std::string paramValStr = funInfo->GetASValueStr();
				std::string classNameAs = pkgClassInfo->GetClassNameAS();
				fout << "static " << funInfo->FunName << " * " << "f" << funInfo->FunName << j << "(" << paramStr << ")" << endl;
				fout << "{" << endl;
				fout << "	" << classNameAs << "* pointer = new0 " << classNameAs << "(" << paramValStr << ");" << endl;
				if (!pkgClassInfo->IsValueType)
					fout << "	" << "pointer->IncrementReferences();" << endl;
				fout << "	" << "return pointer;" << endl;
				fout << "}" << endl;
			}
		}

		PkgFunInfo *destFunInfo = pkgClassInfo->GetDestructorFunction();
		if (destFunInfo)
		{
			if (pkgClassInfo->IsValueType)
			{
				fout << "static void DCtor_" << destFunInfo->FunName << "(" << pkgClassInfo->GetClassNameAS() << "* _pointer)" << endl;
				fout << "{" << endl;
				fout << "	_pointer->~" << pkgClassInfo->GetClassNameAS() << "();" << endl;
				fout << "}" << endl;
			}
		}
	}

	fout << "int " << baseFileName <<"(asIScriptEngine* asEngine)" << endl;

	fout << "{" << endl;

	// content

	// regist global enums
	int numTable = 0;
	fout << "	" << "int r = 0;" << endl;
	for (int i = 0; i < (int)mEnumInfos.size(); i++)
	{
		PkgEnumInfo *enumInfo = mEnumInfos[i];
		if (!enumInfo->ClassInfo)
		{
			numTable = 1;

			GenASFileEnum(fout, enumInfo, numTable);
		}
	}

	// regist object type
	for (int i = 0; i < (int)mClassInfos.size(); i++)
	{
		numTable = 1;

		PkgClassInfo *pkgClassInfo = mClassInfos[i];

		std::string className = pkgClassInfo->GetClassNameAS();

		if (pkgClassInfo->IsValueType)
		{
			for (int n = 0; n < numTable; n++) fout << "	";
			fout << "r = asEngine->RegisterObjectType(\"" << className << "\", sizeof(" << className << "), asOBJ_VALUE | asOBJ_APP_CLASS);" << endl;
		}
		else
		{
			if (!pkgClassInfo->IsRefObject)
			{
				for (int n = 0; n < numTable; n++) fout << "	";
				fout << "r = asEngine->RegisterObjectType(\"" << className << "\", 0,  asOBJ_REF | asOBJ_NOCOUNT);" << endl;
			}
			else
			{
				for (int n = 0; n < numTable; n++) fout << "	";
				fout << "r = asEngine->RegisterObjectType(\"" << className << "\", 0, asOBJ_REF); " << endl;
			}
		}

		for (int n = 0; n < numTable; n++) fout << "	";
		fout << "assert(r >= 0);" << endl;
	}

	// regist class Info
	for (int i = 0; i < (int)mClassInfos.size(); i++)
	{
		numTable = 1;

		PkgClassInfo *pkgClassInfo = mClassInfos[i];

		std::string className = pkgClassInfo->GetClassNameAS();

		for (int n = 0; n < numTable; n++) fout << "	";
		fout << "// Class: " << className << endl;

		// enums
		for (int n = 0; n < (int)pkgClassInfo->EnumInfos.size(); n++)
		{
			PkgEnumInfo *enumInfo = pkgClassInfo->EnumInfos[n];
			GenASFileEnum(fout, enumInfo, numTable);
		}

		// constructors
		std::vector<PkgFunInfo*> constrFunInfos = pkgClassInfo->GetConstructorFunctions();
		for (int j = 0; j < (int)constrFunInfos.size(); j++)
		{
			PkgFunInfo *funInfo = constrFunInfos[j];

			if (pkgClassInfo->IsValueType)
			{
				for (int n = 0; n < numTable; n++) fout << "	";
				fout << "r = asEngine->RegisterObjectBehaviour(\"" << className <<
					"\", asBEHAVE_CONSTRUCT, \"" + funInfo->GetASFunStr() + "\", asFUNCTION(Ctor_"
					<< funInfo->FunName << j << "), asCALL_CDECL_OBJLAST);" << endl;

				for (int n = 0; n < numTable; n++) fout << "	";
				fout << "assert(r >= 0);" << endl;
			}
			else
			{
				for (int n = 0; n < numTable; n++) fout << "	";
				fout << "r = asEngine->RegisterObjectBehaviour(\"" << className <<
					"\", asBEHAVE_FACTORY, \"" + funInfo->GetASFactoryFunStr() + "\", asFUNCTION(f"
					<< funInfo->FunName << j << "), asCALL_CDECL);" << endl;

				for (int n = 0; n < numTable; n++) fout << "	";
				fout << "assert(r >= 0);" << endl;
			}
		}

		// destructors
		PkgFunInfo *destFunInfo = pkgClassInfo->GetDestructorFunction();
		if (destFunInfo)
		{
			if (pkgClassInfo->IsValueType)
			{
				for (int n = 0; n < numTable; n++) fout << "	";
				fout << "r = asEngine->RegisterObjectBehaviour(\"" << className <<
					"\", asBEHAVE_DESTRUCT, \"void f()\", asFUNCTION(DCtor_"
					<< destFunInfo->FunName << "), asCALL_CDECL_OBJLAST);" << endl;

				for (int n = 0; n < numTable; n++) fout << "	";
				fout << "assert(r >= 0);" << endl;
			}
			else
			{
				if (pkgClassInfo->IsRefObject)
				{
					if (pkgClassInfo->IsUseRefIncrease)
					{
						for (int n = 0; n < numTable; n++) fout << "	";
						fout << "r = asEngine->RegisterObjectBehaviour(\"" << className <<
							"\", asBEHAVE_ADDREF, \"void f()\", asMETHOD(" << className << ", IncrementReferences), asCALL_THISCALL);" << endl;

						for (int n = 0; n < numTable; n++) fout << "	";
						fout << "assert(r >= 0);" << endl;

						for (int n = 0; n < numTable; n++) fout << "	";
						fout << "r = asEngine->RegisterObjectBehaviour(\"" << className <<
							"\", asBEHAVE_RELEASE, \"void f()\", asMETHOD(" << className << ", DecrementReferences), asCALL_THISCALL); " << endl;

						for (int n = 0; n < numTable; n++) fout << "	";
						fout << "assert(r >= 0);" << endl;
					}
					else
					{
						for (int n = 0; n < numTable; n++) fout << "	";
						fout << "r = asEngine->RegisterObjectBehaviour(\"" << className <<
							"\", asBEHAVE_ADDREF, \"void f()\", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);" << endl;
						for (int n = 0; n < numTable; n++) fout << "	";
						fout << "assert(r >= 0);" << endl;

						for (int n = 0; n < numTable; n++) fout << "	";
						fout << "r = asEngine->RegisterObjectBehaviour(\"" << className <<
							"\", asBEHAVE_RELEASE, \"void f()\", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);" << endl;
						for (int n = 0; n < numTable; n++) fout << "	";
						fout << "assert(r >= 0);" << endl;
					}
				}
			}
		}

		// member params
		pkgClassInfo->ClearRegistFun();
		GenASClassMemberFuns(fout, pkgClassInfo, numTable, pkgClassInfo);

		pkgClassInfo->ClearRegistParam();
		GenASClassMemberParams(fout, pkgClassInfo, numTable, pkgClassInfo);
	}

	fout << endl;

	fout << "	return 0;" << endl;

	fout << "}";

	fout.close();

	return true;
}
//----------------------------------------------------------------------------
void PkgInfoManager::GenASFileEnum(std::ofstream &fout, PkgEnumInfo *enumInfo, int numTable)
{	
	std::string classNameAppend; // keep empty
	std::string classNameAppend1;
	if (enumInfo->ClassInfo)
	{
		classNameAppend1 = enumInfo->ClassInfo->ClassName + "::";
	}

	std::string &enumName = enumInfo->EnumName;
	for (int n = 0; n < numTable; n++) fout << "	";
	fout << "// Enum: " << classNameAppend << enumName << endl;

	for (int n = 0; n < numTable; n++) fout << "	";
	fout << "r = asEngine->RegisterEnum(\"" << classNameAppend << enumName << "\");" << endl;
	for (int n = 0; n < numTable; n++) fout << "	";
	fout << "assert(r >= 0);" << endl;
	for (int j = 0; j < (int)enumInfo->EnumItemStrs.size(); j++)
	{
		const std::string &enumItemStr = enumInfo->EnumItemStrs[j];

		for (int n = 0; n < numTable; n++) fout << "	";
		std::string valStr;
		fout << "r = asEngine->RegisterEnumValue(\"" << classNameAppend << enumName << "\", " << "\"" << enumItemStr << "\", " <<
			classNameAppend1 << enumName << "::" << enumItemStr << ");" << endl;
		for (int n = 0; n < numTable; n++) fout << "	";
		fout << "assert(r >= 0);" << endl;
	}
}
//----------------------------------------------------------------------------
void PkgInfoManager::GenASMemberParam(std::ofstream &fout,
	const ParamInfo *paramInfo, int numTable, PkgClassInfo *classInfoTo)
{
	std::string className = classInfoTo->GetClassNameAS();

	std::string asParamStr = paramInfo->GetASStr();
	if (!classInfoTo->IsParamRegisted(asParamStr))
	{
		for (int n = 0; n < numTable; n++) fout << "	";
		fout << "r = asEngine->RegisterObjectProperty(\"" << className << "\", \"" <<
			paramInfo->GetASStr() << "\", " << "asOFFSET(" << className << ", " <<
			paramInfo->ValueNameStr << "));" << endl;

		for (int n = 0; n < numTable; n++) fout << "	";
		fout << "assert(r >= 0);" << endl;

		classInfoTo->AddRegistParam(asParamStr);
	}
}
//----------------------------------------------------------------------------
void PkgInfoManager::GenASMemberParamStatic(std::ofstream &fout, const ParamInfo *paramInfo,
	int numTable, PkgClassInfo *classInfoTo)
{
	std::string className = classInfoTo->GetClassNameAS();

	std::string asParamStr = paramInfo->GetASStr();
	std::string typeStrAS = paramInfo->GetTypeStrAS();
	std::string valueStr = paramInfo->ValueNameStr;
	if (!classInfoTo->IsParamRegisted(asParamStr))
	{
		for (int n = 0; n < numTable; n++) fout << "	";

		fout << "r = asEngine->RegisterGlobalProperty(\"" << typeStrAS << " " +
			className + "_" + valueStr << "\", (void*)&" << className << "::" << valueStr << "); " << endl;

		for (int n = 0; n < numTable; n++) fout << "	";
		fout << "assert(r >= 0);" << endl;

		classInfoTo->AddRegistParam(asParamStr);
	}
}
//----------------------------------------------------------------------------
void PkgInfoManager::GenASClassMemberParams(std::ofstream &fout, 
	PkgClassInfo *pkgClassInfo, int numTable, PkgClassInfo *classInfoTo)
{
	if (pkgClassInfo->ParentClassInfo)
		GenASClassMemberParams(fout, pkgClassInfo->ParentClassInfo, numTable, classInfoTo);

	for (int n = 0; n < (int)pkgClassInfo->MemberProps.size(); n++)
	{
		ParamInfo *paramInfo = pkgClassInfo->MemberProps[n];
		if (!paramInfo->IsStatic)
		{
			GenASMemberParam(fout, paramInfo, numTable, classInfoTo);
		}
		else
		{
			GenASMemberParamStatic(fout, paramInfo, numTable, classInfoTo);
		}
	}
}
//----------------------------------------------------------------------------
void PkgInfoManager::GenASClassMemberFuns(std::ofstream &fout,
	PkgClassInfo *pkgClassInfo, int numTable, 
	PkgClassInfo *classInfoTo)
{
	if (pkgClassInfo->ParentClassInfo)
	{
		if (!pkgClassInfo->IsValueType)
		{
			std::string classStrTo = classInfoTo->GetClassNameAS();
			std::string classStr = pkgClassInfo->GetClassNameAS();
			std::string parentClassStr = pkgClassInfo->ParentClassInfo->GetClassNameAS();

			std::string asFunStrToParent = parentClassStr;
			if (pkgClassInfo->ParentClassInfo->IsRefObject)
			{
				asFunStrToParent += "@+ ";
			}
			else
			{
				asFunStrToParent += "@ ";
			}
			asFunStrToParent += "opImplCast()";

			for (int n = 0; n < numTable; n++) fout << "	";
			fout << "r = asEngine->RegisterObjectMethod(\"" << classStrTo << "\", \"" << asFunStrToParent << "\", " <<
				"asFUNCTION((RefCast<" << classStrTo << ", " << parentClassStr << ">)), " << "asCALL_CDECL_OBJLAST);" << endl;
			for (int n = 0; n < numTable; n++) fout << "	";
			fout << "assert(r >= 0);" << endl;

			std::string asFunStrToChild = classStrTo;
			if (classInfoTo->IsRefObject)
			{
				asFunStrToChild += "@+ ";
			}
			else
			{
				asFunStrToChild += "@ ";
			}
			asFunStrToChild += "opCast()";

			for (int n = 0; n < numTable; n++) fout << "	";
			fout << "r = asEngine->RegisterObjectMethod(\"" << parentClassStr << "\", \"" << asFunStrToChild << "\", " <<
				"asFUNCTION((RefCast<" << parentClassStr << ", " << classStrTo << ">)), " << "asCALL_CDECL_OBJLAST);" << endl;
			for (int n = 0; n < numTable; n++) fout << "	";
			fout << "assert(r >= 0);" << endl;
		}

		GenASClassMemberFuns(fout, pkgClassInfo->ParentClassInfo, numTable, classInfoTo);
	}

	for (int j = 0; j < (int)pkgClassInfo->GetFunctions().size(); j++)
	{
		const PkgFunInfo *funInfo = pkgClassInfo->GetFunctions()[j];
		if (PkgFunInfo::FT_CLASS_MEMBER == funInfo->TheFunType)
		{
			if (!funInfo->IsOperator)
			{
				GenASMemberFun(fout, funInfo, numTable, classInfoTo);
			}
		}
		else if (PkgFunInfo::PT_CLASS_STATIC == funInfo->TheFunType && !funInfo->IsOperator)
		{
			GetASMemberFunStatic(fout, funInfo, numTable, classInfoTo);
		}
	}
}
//----------------------------------------------------------------------------
void PkgInfoManager::GenASMemberFun(std::ofstream &fout, const PkgFunInfo *funInfo,
	int numTable, PkgClassInfo *classInfoTo)
{
	for (int n = 0; n < numTable; n++) fout << "	";
	std::string funName = funInfo->FunName;
	if (funInfo->IsOperator)
	{
		funName = "operator" + funName;
	}

	std::string returnStr = funInfo->ReturnVal->GetParamStr_FunPtr();

	std::string paramStr = "(";
	for (int i = 0; i < (int)funInfo->Params.size(); i++)
	{
		if (0 != i)
			paramStr += ", ";

		const ParamInfo *paramInfo = funInfo->Params[i];

		paramStr += paramInfo->GetParamStr_FunPtr();
	}
	paramStr += ")";
	if (funInfo->IsConst)
	{
		paramStr += " const";
	}

	std::string className = classInfoTo->GetClassNameAS();

	std::string asFunStr = funInfo->GetASFunStr();
	if (!classInfoTo->IsFunRegisted(asFunStr))
	{
		fout << "r = asEngine->RegisterObjectMethod(\"" << className << "\", \"" << asFunStr << "\", " <<
			"asMETHODPR(" << className << ", " << funName << ", " << paramStr << ", " << returnStr << "), " << "asCALL_THISCALL);" << endl;

		for (int n = 0; n < numTable; n++) fout << "	";
		fout << "assert(r >= 0);" << endl;

		classInfoTo->AddRegistFun(asFunStr);
	}
}
//----------------------------------------------------------------------------
void PkgInfoManager::GetASMemberFunStatic(std::ofstream &fout,
	const PkgFunInfo *funInfo, int numTable, PkgClassInfo *classInfoTo)
{
	for (int n = 0; n < numTable; n++) fout << "	";
	std::string funName = funInfo->FunName;
	if (funInfo->IsOperator)
	{
		funName = "operator" + funName;
	}

	std::string returnStr = funInfo->ReturnVal->GetParamStr_FunPtr();

	std::string paramStr = "(";
	for (int i = 0; i < (int)funInfo->Params.size(); i++)
	{
		if (0 != i)
			paramStr += ", ";

		const ParamInfo *paramInfo = funInfo->Params[i];

		paramStr += paramInfo->GetParamStr_FunPtr();
	}
	paramStr += ")";
	if (funInfo->IsConst)
	{
		paramStr += " const";
	}

	std::string className = classInfoTo->GetClassNameAS();

	std::string asFunStr = funInfo->GetASFunStr();
	if (!classInfoTo->IsFunRegisted(asFunStr))
	{
		fout << "r = asEngine->RegisterObjectMethod(\"" << className << "\", \"" << asFunStr << "\", " <<
			"asFUNCTIONPR(" << className << "::" << funName << ", " << paramStr << ", " << returnStr << "), " << "asCALL_CDECL_OBJLAST);" << endl;

		for (int n = 0; n < numTable; n++) fout << "	";
		fout << "assert(r >= 0);" << endl;

		classInfoTo->AddRegistFun(asFunStr);
	}
}
//----------------------------------------------------------------------------
PkgInfoManager::PkgInfoManager()
{
}
//----------------------------------------------------------------------------
PkgInfoManager::~PkgInfoManager()
{
}
//----------------------------------------------------------------------------