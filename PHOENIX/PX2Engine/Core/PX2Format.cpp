// PX2Format.cpp

#include "PX2Format.hpp"
#include "PX2Assert.hpp"
#include "PX2Ascii.hpp"

namespace PX2
{

//----------------------------------------------------------------------------
void ParseFlags(std::ostream& str, std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt)
{
	bool isFlag = true;
	while (isFlag && itFmt != endFmt)
	{
		switch (*itFmt)
		{
		case '-': str.setf(std::ios::left); ++itFmt; break;
		case '+': str.setf(std::ios::showpos); ++itFmt; break;
		case '0': str.fill('0'); str.setf(std::ios::internal); ++itFmt; break;
		case '#': str.setf(std::ios::showpoint | std::ios::showbase); ++itFmt; break;
		default:  isFlag = false; break;
		}
	}
}
//----------------------------------------------------------------------------
void ParseWidth(std::ostream& str, std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt)
{
	int width = 0;
	while (itFmt != endFmt && Ascii::IsDigit(*itFmt))
	{
		width = 10*width + *itFmt - '0';
		++itFmt;
	}
	if (width != 0) str.width(width);
}
//----------------------------------------------------------------------------
void ParsePrec(std::ostream& str, std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt)
{
	if (itFmt != endFmt && *itFmt == '.')
	{
		++itFmt;
		int prec = 0;
		while (itFmt != endFmt && Ascii::IsDigit(*itFmt))
		{
			prec = 10*prec + *itFmt - '0';
			++itFmt;
		}
		if (prec >= 0) str.precision(prec);
	}
}
//----------------------------------------------------------------------------
char ParseMod(std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt)
{
	char mod = 0;
	if (itFmt != endFmt)
	{
		switch (*itFmt)
		{
		case 'l':
		case 'h':
		case 'L': 
		case '?': mod = *itFmt++; break;
		}
	}
	return mod;
}
//----------------------------------------------------------------------------
std::size_t ParseIndex(std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt)
{
	int index = 0;
	while (itFmt != endFmt && Ascii::IsDigit(*itFmt))
	{
		index = 10*index + *itFmt - '0';
		++itFmt;
	}
	if (itFmt != endFmt && *itFmt == ']') ++itFmt;
	return index;
}
//----------------------------------------------------------------------------
void PrepareFormat(std::ostream& str, char type)
{
	switch (type)
	{
	case 'd':
	case 'i': str << std::dec; break;
	case 'o': str << std::oct; break;
	case 'x': str << std::hex; break;
	case 'X': str << std::hex << std::uppercase; break;
	case 'e': str << std::scientific; break;
	case 'E': str << std::scientific << std::uppercase; break;
	case 'f': str << std::fixed; break;
	}
}
//----------------------------------------------------------------------------
void WriteAnyInt(std::ostream& str, const Any& any)
{
	if (any.type() == typeid(char))
		str << static_cast<int>(PX2_ANY_AS(any, char));
	else if (any.type() == typeid(signed char))
		str << static_cast<int>(PX2_ANY_AS(any, signed char));
	else if (any.type() == typeid(unsigned char))
		str << static_cast<unsigned>(PX2_ANY_AS(any, unsigned char));
	else if (any.type() == typeid(short))
		str << PX2_ANY_AS(any, short);
	else if (any.type() == typeid(unsigned short))
		str << PX2_ANY_AS(any, unsigned short);
	else if (any.type() == typeid(int))
		str << PX2_ANY_AS(any, int);
	else if (any.type() == typeid(unsigned int))
		str << PX2_ANY_AS(any, unsigned int);
	else if (any.type() == typeid(long))
		str << PX2_ANY_AS(any, long);
	else if (any.type() == typeid(unsigned long))
		str << PX2_ANY_AS(any, unsigned long);
	else if (any.type() == typeid(int64_t))
		str << PX2_ANY_AS(any, int64_t);
	else if (any.type() == typeid(uint64_t))
		str << PX2_ANY_AS(any, uint64_t);
	else if (any.type() == typeid(bool))
		str << PX2_ANY_AS(any, bool);
}
//----------------------------------------------------------------------------
void FormatOne(std::string& result, std::string::const_iterator& itFmt, const std::string::const_iterator& endFmt, std::vector<Any>::const_iterator& itVal)
{
	std::ostringstream str;
	str.imbue(std::locale::classic());

	ParseFlags(str, itFmt, endFmt);
	ParseWidth(str, itFmt, endFmt);
	ParsePrec(str, itFmt, endFmt);
	char mod = ParseMod(itFmt, endFmt);
	if (itFmt != endFmt)
	{
		char type = *itFmt++;
		PrepareFormat(str, type);
		switch (type)
		{
		case 'b':
			str << PX2_ANY_AS(*itVal++, bool);
			break;
		case 'c':
			str << PX2_ANY_AS(*itVal++, char);
			break;
		case 'd':
		case 'i':
			switch (mod)
			{
			case 'l': str << PX2_ANY_AS(*itVal++, long); break;
			case 'L': str << PX2_ANY_AS(*itVal++, int64_t); break;
			case 'h': str << PX2_ANY_AS(*itVal++, short); break;
			case '?': WriteAnyInt(str, *itVal++); break;
			default:  str << PX2_ANY_AS(*itVal++, int); break;
			}
			break;
		case 'o':
		case 'u':
		case 'x':
		case 'X':
			switch (mod)
			{
			case 'l': str << PX2_ANY_AS(*itVal++, unsigned long); break;
			case 'L': str << PX2_ANY_AS(*itVal++, uint64_t); break;
			case 'h': str << PX2_ANY_AS(*itVal++, unsigned short); break;
			case '?': WriteAnyInt(str, *itVal++); break;
			default:  str << PX2_ANY_AS(*itVal++, unsigned); break;
			}
			break;
		case 'e':
		case 'E':
		case 'f':
			switch (mod)
			{
			case 'l': str << PX2_ANY_AS(*itVal++, long double); break;
			case 'L': str << PX2_ANY_AS(*itVal++, long double); break;
			case 'h': str << PX2_ANY_AS(*itVal++, float); break;
			default:  str << PX2_ANY_AS(*itVal++, double); break;
			}
			break;
		case 's':
			str << PX2_ANY_AS(*itVal++, std::string);
			break;
		case 'z':
			str << PX2_ANY_AS(*itVal++, std::size_t); 
			break;
		case 'I':
		case 'D':
		default:
			str << type;
		}
	}

	result.append(str.str());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Formats
//----------------------------------------------------------------------------
std::string Format(const std::string& fmt, const Any& value)
{
	std::string result;
	Format(result, fmt, value);
	return result;
}
//----------------------------------------------------------------------------
std::string Format(const std::string& fmt, const Any& value1, const Any& value2)
{
	std::string result;
	Format(result, fmt, value1, value2);
	return result;
}
//----------------------------------------------------------------------------
std::string Format(const std::string& fmt, const Any& value1, const Any& value2, const Any& value3)
{
	std::string result;
	Format(result, fmt, value1, value2, value3);
	return result;
}
//----------------------------------------------------------------------------
std::string Format(const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4)
{
	std::string result;
	Format(result, fmt, value1, value2, value3, value4);
	return result;
}
//----------------------------------------------------------------------------
std::string Format(const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4, const Any& value5)
{
	std::string result;
	Format(result, fmt, value1, value2, value3, value4, value5);
	return result;
}
//----------------------------------------------------------------------------
std::string Format(const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4, const Any& value5, const Any& value6)
{
	std::string result;
	Format(result, fmt, value1, value2, value3, value4, value5, value6);
	return result;
}
//----------------------------------------------------------------------------
void Format(std::string& result, const std::string& fmt, const Any& value)
{
	std::vector<Any> args;
	args.push_back(value);
	FormatVec(result, fmt, args);
}
//----------------------------------------------------------------------------
void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2)
{
	std::vector<Any> args;
	args.push_back(value1);
	args.push_back(value2);
	FormatVec(result, fmt, args);
}
//----------------------------------------------------------------------------
void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2, const Any& value3)
{
	std::vector<Any> args;
	args.push_back(value1);
	args.push_back(value2);
	args.push_back(value3);
	FormatVec(result, fmt, args);
}
//----------------------------------------------------------------------------
void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4)
{
	std::vector<Any> args;
	args.push_back(value1);
	args.push_back(value2);
	args.push_back(value3);
	args.push_back(value4);
	FormatVec(result, fmt, args);
}
//----------------------------------------------------------------------------
void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4, const Any& value5)
{
	std::vector<Any> args;
	args.push_back(value1);
	args.push_back(value2);
	args.push_back(value3);
	args.push_back(value4);
	args.push_back(value5);
	FormatVec(result, fmt, args);
}
//----------------------------------------------------------------------------
void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4, const Any& value5, const Any& value6)
{
	std::vector<Any> args;
	args.push_back(value1);
	args.push_back(value2);
	args.push_back(value3);
	args.push_back(value4);
	args.push_back(value5);
	args.push_back(value6);
	FormatVec(result, fmt, args);
}
//----------------------------------------------------------------------------
void FormatVec(std::string& result, const std::string& fmt, const std::vector<Any>& values)
{
	std::string::const_iterator itFmt  = fmt.begin();
	std::string::const_iterator endFmt = fmt.end();
	std::vector<Any>::const_iterator itVal  = values.begin();
	std::vector<Any>::const_iterator endVal = values.end(); 
	while (itFmt != endFmt)
	{
		switch (*itFmt)
		{
		case '%':
			++itFmt;
			if (itFmt != endFmt && itVal != endVal)
			{
				if (*itFmt == '[')
				{
					++itFmt;
					std::size_t index = ParseIndex(itFmt, endFmt);
					if (index < values.size())
					{
						std::vector<Any>::const_iterator it = values.begin() + index;
						FormatOne(result, itFmt, endFmt, it);
					}
					else 
					{
						assertion(false, "Format argument index out of range");
					}
				}
				else
				{
					FormatOne(result, itFmt, endFmt, itVal);
				}
			}
			else if (itFmt != endFmt)
			{
				result += *itFmt++;
			}
			break;
		default:
			result += *itFmt;
			++itFmt;
		}
	}
}
//----------------------------------------------------------------------------

}