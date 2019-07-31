// PX2NumberFormatter.inl

//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(int value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(int value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format0(int value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::formatHex(int value, bool prefix)
{
	std::string result;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, prefix);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::formatHex(int value, int width, bool prefix)
{
	std::string result;
	uIntToStr(static_cast<unsigned int>(value), 0x10, result, prefix, width, '0');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(unsigned value)
{
	std::string result;
	uIntToStr(value, 10, result);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(unsigned value, int width)
{
	std::string result;
	uIntToStr(value, 10, result, false, width, ' ');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format0(unsigned int value, int width)
{
	std::string result;
	uIntToStr(value, 10, result, false, width, '0');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::formatHex(unsigned value, bool prefix)
{
	std::string result;
	uIntToStr(value, 0x10, result, prefix);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::formatHex(unsigned value, int width, bool prefix)
{
	std::string result;
	uIntToStr(value, 0x10, result, prefix, width, '0');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(long value)
{
	std::string result;
	intToStr(value, 10, result);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, ' ');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format0(long value, int width)
{
	std::string result;
	intToStr(value, 10, result, false, width, '0');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::formatHex(long value, bool prefix)
{
	std::string result;
	uIntToStr(static_cast<unsigned long>(value), 0x10, result, prefix);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::formatHex(long value, int width, bool prefix)
{
	std::string result;
	uIntToStr(static_cast<unsigned long>(value), 0x10, result, prefix, width, '0');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(unsigned long value)
{
	std::string result;
	uIntToStr(value, 10, result);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(unsigned long value, int width)
{
	std::string result;
	uIntToStr(value, 10, result, false, width, ' ');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format0(unsigned long value, int width)
{
	std::string result;
	uIntToStr(value, 10, result, false, width, '0');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::formatHex(unsigned long value, bool prefix)
{
	std::string result;
	uIntToStr(value, 0x10, result, prefix);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::formatHex(unsigned long value, int width, bool prefix)
{
	std::string result;
	uIntToStr(value, 0x10, result, prefix, width, '0');
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(float value)
{
	char buffer[PX2_MAX_FLT_STRING_LEN];
	floatToStr(buffer, PX2_MAX_FLT_STRING_LEN, value);
	return std::string(buffer);
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(float value, int precision)
{
	char buffer[PX2_MAX_FLT_STRING_LEN];
	floatToFixedStr(buffer, PX2_MAX_FLT_STRING_LEN, value, precision);
	return std::string(buffer);
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(float value, int width, int precision)
{
	std::string result;
	floatToFixedStr(result, value, precision, width);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(double value)
{
	char buffer[PX2_MAX_FLT_STRING_LEN];
	doubleToStr(buffer, PX2_MAX_FLT_STRING_LEN, value);
	return std::string(buffer);
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(double value, int precision)
{
	char buffer[PX2_MAX_FLT_STRING_LEN];
	doubleToFixedStr(buffer, PX2_MAX_FLT_STRING_LEN, value, precision);
	return std::string(buffer);
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(double value, int width, int precision)
{
	std::string result;
	doubleToFixedStr(result, value, precision, width);
	return result;
}
//----------------------------------------------------------------------------
inline std::string NumberFormatter::format(const void* ptr)
{
	std::string result;
	append(result, ptr);
	return result;
}
//----------------------------------------------------------------------------