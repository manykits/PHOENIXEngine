// PX2NumberFormatter.hpp

#ifndef PX2NUMBERFORMATTER_HPP
#define PX2NUMBERFORMATTER_HPP

#include "PX2CorePre.hpp"
#include "PX2NumericString.hpp"


namespace PX2 
{

	class PX2_ENGINE_ITEM NumberFormatter
	{
	public:
		enum BoolFormat
		{
			FMT_TRUE_FALSE,
			FMT_YES_NO,
			FMT_ON_OFF
		};

		static const unsigned NF_MAX_INT_STRING_LEN = 32; // increase for 64-bit binary formatting support
		static const unsigned NF_MAX_FLT_STRING_LEN = PX2_MAX_FLT_STRING_LEN;

		static std::string format(int value);
		static std::string format(int value, int width);
		static std::string format0(int value, int width);
		static std::string formatHex(int value, bool prefix = false);
		static std::string formatHex(int value, int width, bool prefix = false);
		static std::string format(unsigned value);
		static std::string format(unsigned value, int width);
		static std::string format0(unsigned int value, int width);
		static std::string formatHex(unsigned value, bool prefix = false);
		static std::string formatHex(unsigned value, int width, bool prefix = false);
		static std::string format(long value);
		static std::string format(long value, int width);
		static std::string format0(long value, int width);
		static std::string formatHex(long value, bool prefix = false);
		static std::string formatHex(long value, int width, bool prefix = false);
		static std::string format(unsigned long value);
		static std::string format(unsigned long value, int width);
		static std::string format0(unsigned long value, int width);
		static std::string formatHex(unsigned long value, bool prefix = false);
		static std::string formatHex(unsigned long value, int width, bool prefix = false);
		static std::string format(float value);
		static std::string format(float value, int precision);
		static std::string format(float value, int width, int precision);
		static std::string format(double value);
		static std::string format(double value, int precision);
		static std::string format(double value, int width, int precision);
		static std::string format(const void* ptr);
		static std::string format(bool value, BoolFormat format = FMT_TRUE_FALSE);
		static void append(std::string& str, int value);
		static void append(std::string& str, int value, int width);
		static void append0(std::string& str, int value, int width);
		static void appendHex(std::string& str, int value);
		static void appendHex(std::string& str, int value, int width);
		static void append(std::string& str, unsigned value);
		static void append(std::string& str, unsigned value, int width);
		static void append0(std::string& str, unsigned int value, int width);
		static void appendHex(std::string& str, unsigned value);
		static void appendHex(std::string& str, unsigned value, int width);
		static void append(std::string& str, long value);
		static void append(std::string& str, long value, int width);
		static void append0(std::string& str, long value, int width);
		static void appendHex(std::string& str, long value);
		static void appendHex(std::string& str, long value, int width);
		static void append(std::string& str, unsigned long value);
		static void append(std::string& str, unsigned long value, int width);
		static void append0(std::string& str, unsigned long value, int width);
		static void appendHex(std::string& str, unsigned long value);
		static void appendHex(std::string& str, unsigned long value, int width);
		static void append(std::string& str, float value);
		static void append(std::string& str, float value, int precision);
		static void append(std::string& str, float value, int width, int precision);
		static void append(std::string& str, double value);
		static void append(std::string& str, double value, int precision);
		static void append(std::string& str, double value, int width, int precision);
		static void append(std::string& str, const void* ptr);
	};

#include "PX2NumberFormatter.inl"

} 

#endif