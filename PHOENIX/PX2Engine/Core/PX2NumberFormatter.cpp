// PX2NumberFormatter.cpp

#include "PX2NumberFormatter.hpp"
using namespace PX2;

#if defined(_MSC_VER) || defined(__MINGW32__)
#define I64_FMT "I64"
#elif defined(__APPLE__) 
#define I64_FMT "q"
#else
#define I64_FMT "ll"
#endif


std::string NumberFormatter::format(bool value, BoolFormat format)
{
	switch (format)
	{
	default:
	case FMT_TRUE_FALSE:
		if (value == true)
			return "true";
		return "false";
	case FMT_YES_NO:
		if (value == true)
			return "yes";
		return "no";
	case FMT_ON_OFF:
		if (value == true)
			return "on";
		return "off";
	}
}


void NumberFormatter::append(std::string& str, int value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, int value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, unsigned int value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width);
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	intToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned long>(value), 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(static_cast<unsigned long>(value), 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::append(std::string& str, unsigned long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::append0(std::string& str, unsigned long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 10, result, sz, false, width, '0');
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz);
	str.append(result, sz);
}


void NumberFormatter::appendHex(std::string& str, unsigned long value, int width)
{
	char result[NF_MAX_INT_STRING_LEN];
	std::size_t sz = NF_MAX_INT_STRING_LEN;
	uIntToStr(value, 0x10, result, sz, false, width, '0');
	str.append(result, sz);
}

void NumberFormatter::append(std::string& str, float value)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	floatToStr(buffer, PX2_MAX_FLT_STRING_LEN, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, float value, int precision)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	floatToFixedStr(buffer, PX2_MAX_FLT_STRING_LEN, value, precision);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, float value, int width, int precision)
{
	std::string result;
	str.append(floatToFixedStr(result, value, precision, width));
}


void NumberFormatter::append(std::string& str, double value)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	doubleToStr(buffer, PX2_MAX_FLT_STRING_LEN, value);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int precision)
{
	char buffer[NF_MAX_FLT_STRING_LEN];
	doubleToFixedStr(buffer, PX2_MAX_FLT_STRING_LEN, value, precision);
	str.append(buffer);
}


void NumberFormatter::append(std::string& str, double value, int width, int precision)
{
	std::string result;
	str.append(doubleToFixedStr(result, value, precision, width));
}


void NumberFormatter::append(std::string& str, const void* ptr)
{
	char buffer[24];
#if defined(PX2_PTR_IS_64_BIT)
#if defined(PX2_LONG_IS_64_BIT)
	std::sprintf(buffer, "%016lX", (uint64_t)ptr);
#else
	std::sprintf(buffer, "%016" I64_FMT "X", (uint64_t)ptr);
#endif
#else
	std::sprintf(buffer, "%08lX", (unsigned long)ptr);
#endif
	str.append(buffer);
}