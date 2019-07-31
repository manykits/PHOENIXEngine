// PkgParser.cpp

#include "PkgParser.hpp"
#include "PkgTokenizer.hpp"
#include "PkgToken.hpp"
#include "PkgInfoManager.hpp"
#include "PkgAscii.hpp"
using namespace PkgParse;

//----------------------------------------------------------------------------
PkgParser::PkgParser() :
mCurPkgClassInfo(0),
mFunState(FS_NONE),
mEnumState(ES_NONE),
mCurEnumInfo(0)
{
}
//----------------------------------------------------------------------------
PkgParser::~PkgParser()
{
}
//----------------------------------------------------------------------------
bool PkgParser::ParseFile(const std::string &filename)
{
	std::ifstream fin(filename, std::ios::in);
	char line[1024] = { 0 };
	
	while (fin.getline(line, sizeof(line)))
	{
		std::string lineStr(line);
		std::string retStr;
		bool addSpace = false;
		for (int i = 0; i < (int)lineStr.length(); i++)
		{
			if (PkgAscii::IsSpace(lineStr[i]))
			{
				if (addSpace)
					retStr.push_back(lineStr[i]);
			}
			else
			{
				addSpace = true;
				retStr.push_back(lineStr[i]);
			}
		}
		
		if (!retStr.empty())
		{
			ParseLine(retStr);
		}
	}

	fin.clear();
	fin.close();

	return true;
}
//----------------------------------------------------------------------------
void PkgParser::ParseLine(const std::string &lineStr)
{
	std::string line = lineStr;
	if (line.empty())
		return;

	PkgStringTokenizer token(line, " ",
		PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);

	int count = token.Count();
	if (0 == count) return;

	const std::string tokenStr0 = token[0];
	std::string tokenStr1;
	std::string tokenStr2;
	if (count > 1) tokenStr1 = token[1];
	if (count > 2) tokenStr2 = token[2];

	const PkgToken::TokenType tt = PkgToken::DetermineTokenType(tokenStr0);

	// 注释，忽略
	if (tt == PkgToken::Comment)
	{
		if (lineStr.find("VALUE") != std::string::npos)
		{
			if (mCurPkgClassInfo)
			{
				mCurPkgClassInfo->IsValueType = true;
			}
		}
		if (lineStr.find("NO_INCREASEREF") != std::string::npos)
		{
			if (mCurPkgClassInfo)
			{
				mCurPkgClassInfo->IsUseRefIncrease = false;
			}
		}
		if (lineStr.find("SINGLETON") != std::string::npos)
		{
			if (mCurPkgClassInfo)
			{
				mCurPkgClassInfo->SingletonName = tokenStr2;
			}
		}

		return;
	}

	// 包含文件，忽略
	if (tt == PkgToken::Include) return;

	if (tt == PkgToken::File)
	{
		std::string filename = StringToFilename(token[1]);

		ParseFile(filename);

		return;
	}

	if (tt == PkgToken::Class)
	{
		std::string className = tokenStr1;

		PkgClassInfo *info = new PkgClassInfo();
		mCurPkgClassInfo = info;

		info->ClassName = className;

		if (token.Count() >= 5)
		{
			std::string parentName = token[4];
			info->ParentClassName = parentName;
		}

		PkgInfoManager::GetSingleton().AddClassInfo(className, info);

		return;
	}

	if (tt == PkgToken::Enum)
	{
		std::string enumName = tokenStr1;

		PkgEnumInfo *info = new PkgEnumInfo();
		mCurEnumInfo = info;

		info->EnumName = enumName;

		PkgInfoManager::GetSingleton().AddEnumInfo(enumName, info);

		mEnumState = ES_BEGIN;

		if (mCurPkgClassInfo)
		{
			info->ClassInfo = mCurPkgClassInfo;
			mCurPkgClassInfo->EnumInfos.push_back(info);
		}

		return;
	}

	if (tt == PkgToken::OpenBrace)
	{
		if (ES_BEGIN == mEnumState)
			mEnumState = ES_BEGIN_ITEMS;

		return;
	}

	if (tt == PkgToken::CloseBrace)
	{
		if (mCurEnumInfo)
		{
			mCurEnumInfo = 0;
			mEnumState = ES_END;
			mEnumState = ES_NONE;
		}
		else
		{
			mCurPkgClassInfo = 0;
		}

		return;
	}

	if (lineStr.find("TOLUA_TEMPLATE_BIND") != std::string::npos)
	{
		PraseClassTemplate(lineStr);
	}

	if (tt == PkgToken::String || tt == PkgToken::Wave)
	{
		if (0 != mCurPkgClassInfo)
		{
			// 普通函数
			if (FS_NONE == mFunState)
			{
				if (line.find("(") == std::string::npos &&
					line.find(";") != std::string::npos)
				{
					mParamStr = line;
					PraseParam(mParamStr);
				}
				else if (line.find("(") != std::string::npos &&
					line.find(";") != std::string::npos)
				{
					mFunState = FS_END;
					mFunStr = line;
				}
				else if (line.find("(") != std::string::npos)
				{
					mFunState = FS_BEGIN;
					mFunStr = line;
					return;
				}
			}
			else if (FS_BEGIN == mFunState)
			{
				if (line.find(";") == std::string::npos)
				{
					mFunStr += line;
				}
				else
				{
					mFunStr += line;
					mFunState = FS_END;
				}
			}
			
			if (FS_END == mFunState)
			{
				PraseFunction(mFunStr);
				mFunStr.clear();
				mFunState = FS_NONE;
			}
		}

		if (ES_BEGIN_ITEMS == mEnumState)
		{
			PraseEnumItem(tokenStr0);
		}
	}
}
//----------------------------------------------------------------------------
std::string PkgParser::StringToFilename(const std::string &str)
{
	std::string strRet;
	strRet = str.substr(1, str.length() - 2);

	return strRet;
}
//----------------------------------------------------------------------------
void PkgParser::PraseEnumItem(const std::string &itemStr)
{
	PkgStringTokenizer tokenFun(itemStr, ",",
		PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);

	std::string strToken0 = tokenFun[0];

	mCurEnumInfo->EnumItemStrs.push_back(strToken0);
}
//----------------------------------------------------------------------------
void PkgParser::PraseFunction(const std::string &functionStrWithSemicolon)
{
	std::string functionStr = functionStrWithSemicolon;
	std::string::size_type st = functionStrWithSemicolon.find(";");
	if (st != std::string::npos)
		functionStr = functionStrWithSemicolon.substr(0, st);

	PkgFunInfo *funInfo = new PkgFunInfo();	

	PkgStringTokenizer tokenFun(functionStr, "(",
		PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);

	std::string strToken0 = tokenFun[0];
	if (1 == tokenFun.Count())
	{
		delete(funInfo);
		return;
	}
	std::string strToken1 = tokenFun[1];

	funInfo->FunStr = strToken0 + "(" + strToken1;

	PkgStringTokenizer tokenFunName(strToken0, " ",
		PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);

	std::string functionName;

	// 构造函数，析构函数
	if (1 == tokenFunName.Count())
	{
		if (tokenFunName[0] == mCurPkgClassInfo->ClassName)
		{
			funInfo->TheFunType = PkgFunInfo::FT_CLASS_CONSTRUCTOR;
			functionName = mCurPkgClassInfo->ClassName;

			funInfo->FunName = functionName;
		}
		else if (tokenFunName[0] == "~" + mCurPkgClassInfo->ClassName)
		{
			funInfo->TheFunType = PkgFunInfo::FT_CLASS_DESTRUCTOR;
			functionName = mCurPkgClassInfo->ClassName;

			funInfo->FunName = functionName;
		}
	}
	else if (2 == tokenFunName.Count() && "~" == tokenFunName[0])
	{
		funInfo->TheFunType = PkgFunInfo::FT_CLASS_DESTRUCTOR;
		functionName = mCurPkgClassInfo->ClassName;

		funInfo->FunName = functionName;
	}
	else if (2 == tokenFunName.Count() && "virtual" == tokenFunName[0] && '~' == tokenFunName[1][0])
	{
		funInfo->TheFunType = PkgFunInfo::FT_CLASS_DESTRUCTOR;
		funInfo->IsVirtual = true;
		functionName = mCurPkgClassInfo->ClassName;
		mCurPkgClassInfo->IsVirtual = true;

		funInfo->FunName = functionName;
	}
	else
	{
		int paramIndex = 0;
		if ("static" == tokenFunName[0])
		{
			funInfo->TheFunType = PkgFunInfo::PT_CLASS_STATIC;
			paramIndex += 1;
		}
		else
		{
			if ("virtual" == tokenFunName[0])
			{
				funInfo->IsVirtual = true;
				paramIndex += 1;
			}
			else if ("inline" == tokenFunName[0])
			{
				funInfo->IsInline = true;
				paramIndex += 1;
			}

			funInfo->TheFunType = PkgFunInfo::FT_CLASS_MEMBER;
		}

		std::string returnStr;
		for (int i = paramIndex; i < (int)tokenFunName.Count() - 1; i++)
		{
			returnStr += tokenFunName[i] + " ";
		}
		std::string funName = tokenFunName[tokenFunName.Count() - 1];
		if ('*' == funName[0])
		{
			functionName = funName.substr(1, funName.length() - 1);
			returnStr += "*";
		}
		else if ('&' == funName[0])
		{
			functionName = funName.substr(1, funName.length() - 1);
			returnStr += "&";
		}
		else
		{
			functionName = funName;
		}

		std::string::size_type st = functionName.find("operator");
		if (st != functionName.npos)
		{
			funInfo->IsOperator = true;
			functionName = functionName.substr(8, functionName.length());
		}

		funInfo->FunName = functionName;

		ParamInfo *pi = PraseFunctionParam(returnStr);
		pi->IsReturn = true;

		funInfo->ReturnVal = pi;
		pi->FunInfo = funInfo;
	}

	if (strToken1.length() > 4)
	{
		std::string lastStr = strToken1.substr(strToken1.length() - 4, 4);
		if (" = 0" == lastStr)
		{
			funInfo->IsPureVirtual = true;
			strToken1 = strToken1.substr(0, strToken1.length() - 4);
		}
	}
	if (strToken1.length() > 5)
	{
		std::string lastStr = strToken1.substr(strToken1.length() - 5, 5);
		if ("const" == lastStr)
		{
			funInfo->IsConst = true;
			strToken1 = strToken1.substr(0, strToken1.length() - 5);
		}
	}

	PkgStringTokenizer tokenParam(strToken1, ")",
		PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);

	if (tokenParam.Count() >= 1)
	{
		std::string strTokenParam = tokenParam[0];

		PkgStringTokenizer tokenParams(strTokenParam, ",",
			PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);
		std::vector<std::string> params = tokenParams.GetTokens();

		for (int i = 0; i < (int)params.size(); i++)
		{
			const std::string &param = params[i];
			ParamInfo *pi = PraseFunctionParam(param);
			pi->FunInfo = funInfo;

			funInfo->Params.push_back(pi);
		}
	}

	mCurPkgClassInfo->AddFunInfo(funInfo);
}
//----------------------------------------------------------------------------
ParamInfo *PkgParser::PraseFunctionParam(const std::string &paramStr)
{
	ParamInfo *info = new ParamInfo();
	info->ParamStr = paramStr;

	bool findStar = (paramStr.find("*") != std::string::npos);
	bool findRef = (paramStr.find("&") != std::string::npos);

	std::string operatorFrontStr = paramStr;

	PkgStringTokenizer tokenFunOperator(paramStr, "=",
		PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);

	// value
	if (tokenFunOperator.Count() > 1)
	{
		operatorFrontStr = tokenFunOperator[0];
		info->DefaultValStr = tokenFunOperator[1];
	}

	PkgStringTokenizer tokenFun(operatorFrontStr, " ",
		PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);

	if (tokenFun.Count() > 0)
	{
		std::string typeStr;
		std::string valStr;

		if (!findStar && !findRef)
		{
			int typeIndex = 0;
			if ("const" == tokenFun[0])
			{
				info->IsConst = true;
				typeIndex += 1;
			}

			bool findUnsigned = paramStr.find("unsigned") != std::string::npos;
			if (findUnsigned)
			{
				typeStr = tokenFun[typeIndex + 0] + " " + tokenFun[typeIndex+1];

				if ((typeIndex+3) == (int)tokenFun.Count())
				{
					valStr = tokenFun[typeIndex+2];
				}
			}
			else
			{
				typeStr = tokenFun[typeIndex+0];

				if ((typeIndex+2) == (int)tokenFun.Count())
				{
					valStr = tokenFun[typeIndex+1];
				}
				else if ((typeIndex + 4) == (int)tokenFun.Count())
				{
					valStr = tokenFun[typeIndex + 2];
				}
			}
		}
		else
		{
			if (findStar)
			{
				info->IsPointer = true;
			}
			if (findRef)
			{
				info->IsRef = true;
			}

			int typeIndex = 0;
			if ("const" == tokenFun[0])
			{
				info->IsConst = true;
				typeIndex += 1;
			}

			std::string strTken;
			for (int i = typeIndex; i < (int)tokenFun.Count(); i++)
			{
				strTken  += tokenFun[i];
			}

			bool addTypeStr = true;
			bool addValueStr = false;
			for (int i = 0; i < (int)strTken.size(); i++)
			{
				char chara = strTken[i];
				if ('*' == chara || '&' == chara)
				{
					addTypeStr = false;
				}
				else
				{
					if (false == addTypeStr)
					{
						addValueStr = true;
					}
				}

				if (addTypeStr)
				{
					typeStr += chara;
				}
				if (addValueStr)
				{
					valStr += chara;
				}
			}
		}

		std::string::size_type st = valStr.find("=");
		if (std::string::npos != st)
		{
			valStr = valStr.substr(0, st);
		}

		info->TypeStr = typeStr;
		info->ValueNameStr = valStr;
	}

	return info;
}
//----------------------------------------------------------------------------
void PkgParser::PraseClassTemplate(const std::string &str)
{
	mCurPkgClassInfo->IsTemplate = true;

	std::string strSub;

	std::string::size_type st = str.find("(");
	if (st != std::string::npos)
	{
		strSub = str.substr(st+1, str.length() - st);

		std::string::size_type st1 = strSub.find(")");
		if (st1 != std::string::npos)
		{
			strSub = strSub.substr(0, st1);
		}
	}

	PkgStringTokenizer tokenFun(strSub, ",",
		PkgStringTokenizer::TOK_IGNORE_EMPTY | PkgStringTokenizer::TOK_TRIM);
	if (tokenFun.Count() > 0)
	{
		for (int i = 1; i < (int)tokenFun.Count(); i++)
		{
			std::string token = tokenFun[i];
			if ("float" == token)
			{
				mCurPkgClassInfo->TemplateFloat = true;
			}
			else if ("double" == token)
			{
				mCurPkgClassInfo->TemplateDouble = true;
			}
			else if ("int" == token)
			{
				mCurPkgClassInfo->TemplateInt = true;
			}
		}
	}
}
//----------------------------------------------------------------------------
void PkgParser::PraseParam(const std::string &paramStrSrc)
{
	std::string paramStr = paramStrSrc;
	std::string::size_type st = paramStr.find(";");
	if (st != std::string::npos)
		paramStr = paramStr.substr(0, st);

	bool findStatic = false;
	std::string::size_type stStatic = paramStr.find("static");
	if (stStatic != std::string::npos)
	{
		findStatic = true;
		paramStr = paramStr.substr(stStatic + 6, paramStr.length()-6);
	}

	ParamInfo *info = PraseFunctionParam(paramStr);
	if (info)
	{
		if (findStatic)
		{
			info->IsStatic = true;
		}

		mCurPkgClassInfo->MemberProps.push_back(info);
	}
}
//----------------------------------------------------------------------------