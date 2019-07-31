// PX2NumberParser.cpp

#include "PX2NumberParser.hpp"
#include "PX2StringHelp.hpp"
#include "PX2NumericString.hpp"
#include "PX2Exception.hpp"
using namespace PX2;

#if defined(PX2_LONG_IS_64_BIT)
#define I64_FMT "l"
#elif defined(_MSC_VER) || defined(__MINGW32__)
#define I64_FMT "I64"
#elif defined(__APPLE__) 
#define I64_FMT "q"
#else
#define I64_FMT "ll"
#endif

//----------------------------------------------------------------------------
int NumberParser::parse(const std::string& s, char thSep)
{
	int result;
	if (tryParse(s, result, thSep))
		return result;
	else
		throw SyntaxException("Not a valid integer", s);
}
//----------------------------------------------------------------------------
bool NumberParser::tryParse(const std::string& s, int& value, char thSep)
{
	return strToInt(s.c_str(), value, NUM_BASE_DEC, thSep);
}
//----------------------------------------------------------------------------
unsigned NumberParser::parseUnsigned(const std::string& s, char thSep)
{
	unsigned result;
	if (tryParseUnsigned(s, result, thSep))
		return result;
	else
		throw SyntaxException("Not a valid unsigned integer", s);
}
//----------------------------------------------------------------------------
bool NumberParser::tryParseUnsigned(const std::string& s, unsigned& value, char thSep)
{
	return strToInt(s.c_str(), value, NUM_BASE_DEC, thSep);
}
//----------------------------------------------------------------------------
unsigned NumberParser::parseHex(const std::string& s)
{
	unsigned result;
	if (tryParseHex(s, result))
		return result;
	else
		throw SyntaxException("Not a valid hexadecimal integer", s);
}
//----------------------------------------------------------------------------
bool NumberParser::tryParseHex(const std::string& s, unsigned& value)
{
	int offset = 0;
	if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) offset = 2;
	return strToInt(s.c_str() + offset, value, NUM_BASE_HEX);
}
//----------------------------------------------------------------------------
unsigned NumberParser::parseOct(const std::string& s)
{
	unsigned result;
	if (tryParseOct(s, result))
		return result;
	else
		throw SyntaxException("Not a valid hexadecimal integer", s);
}
//----------------------------------------------------------------------------
bool NumberParser::tryParseOct(const std::string& s, unsigned& value)
{
	return strToInt(s.c_str(), value, NUM_BASE_OCT);
}
//----------------------------------------------------------------------------
int64_t NumberParser::parse64(const std::string& s, char thSep)
{
	int64_t result;
	if (tryParse64(s, result, thSep))
		return result;
	else
		throw SyntaxException("Not a valid integer", s);
}
//----------------------------------------------------------------------------
bool NumberParser::tryParse64(const std::string& s, int64_t& value, char thSep)
{
	return strToInt(s.c_str(), value, NUM_BASE_DEC, thSep);
}
//----------------------------------------------------------------------------
double NumberParser::parseFloat(const std::string& s, char decSep, char thSep)
{
	double result;
	if (tryParseFloat(s, result, decSep, thSep))
		return result;
	else
		throw SyntaxException("Not a valid floating-point number", s);
}
//----------------------------------------------------------------------------
bool NumberParser::tryParseFloat(const std::string& s, double& value, char decSep, char thSep)
{
	return strToDouble(s.c_str(), value, decSep, thSep);
}
//----------------------------------------------------------------------------
bool NumberParser::parseBool(const std::string& s)
{
	bool result;
	if (tryParseBool(s, result))
		return result;
	else
		throw SyntaxException("Not a valid bool number", s);
}
//----------------------------------------------------------------------------
bool NumberParser::tryParseBool(const std::string& s, bool& value)
{
	int n;
	if (NumberParser::tryParse(s, n))
	{
		value = (n != 0);
		return true;
	}

	if (Icompare(s, "true") == 0)
	{
		value = true;
		return true;
	}
	else if (Icompare(s, "yes") == 0)
	{
		value = true;
		return true;
	}
	else if (Icompare(s, "on") == 0)
	{
		value = true;
		return true;
	}

	if (Icompare(s, "false") == 0)
	{
		value = false;
		return true;
	}
	else if (Icompare(s, "no") == 0)
	{
		value = false;
		return true;
	}
	else if (Icompare(s, "off") == 0)
	{
		value = false;
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------