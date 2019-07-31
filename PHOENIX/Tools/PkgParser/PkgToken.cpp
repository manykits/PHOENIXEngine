// PkgToken.cpp

#include "PkgToken.hpp"
using namespace PkgParse;

//----------------------------------------------------------------------------
PkgToken::PkgToken()
{
}
//----------------------------------------------------------------------------
PkgToken::~PkgToken()
{
}
//----------------------------------------------------------------------------
PkgToken::TokenType PkgToken::DetermineTokenType(const std::string &tokenStr)
{
	TokenType type = Unknown;

	int tokenStrSize = (int)tokenStr.size();

	if (tokenStrSize >= 2)
	{
		if (tokenStr[0] == '/' && tokenStr[1] == '/')
		{
			type = Comment;
			return type;
		}
		if (tokenStr[0] == '/' && tokenStr[1] == '*')
		{
			type = Comment;
			return type;
		}
		if (tokenStr[0] == '*')
		{
			type = Comment;
			return type;
		}
		if ("$#include" == tokenStr)
		{
			type = Include;
			return type;
		}
		if ("$pfile" == tokenStr)
		{
			type = File;
			return type;
		}
		if ("class" == tokenStr)
		{
			type = Class;
			return type;
		}
		if ("public" == tokenStr || "public:" == tokenStr)
		{
			type = Public;
			return type;
		}
		if ("enum" == tokenStr)
		{
			type = Enum;
			return type;
		}
	}

	if ("~" == tokenStr)
	{
		type = Wave;
		return type;
	}

	if (tokenStr[0] == '*')
	{
		type = Comment;
		return type;
	}

	if (isdigit(tokenStr[0]) || tokenStr[0] == '-') { type = RealNumber; return type; }
	if (tokenStr[0] == '=')   { type = Equals; return type; }
	if (tokenStr[0] == ',')   { type = Comma; return type; }
	if (tokenStr[0] == '(')   { type = OpenParen; return type; }
	if (tokenStr[0] == ')')   { type = CloseParen; return type; }
	if (tokenStr[0] == '{')   { type = OpenBrace; return type; }
	if (tokenStr[0] == '}')   { type = CloseBrace; return type; }
	if (tokenStr[0] == '\"')  { type = Quote; return type; }

	type = String;
	return type;
}
//----------------------------------------------------------------------------