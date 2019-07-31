// PX2Format.hpp

#ifndef PX2FORMAT_HPP
#define PX2FORMAT_HPP

#include "PX2CorePre.hpp"
#include "PX2Any.hpp"

namespace PX2
{

	///   %[<index>][<flags>][<width>][.<precision>][<modifier>]<type>
	/// Index, flags, width, precision 和 prefix 是可选的
	///   * b boolean (true = 1, false = 0)
	///   * c character
	///   * d signed decimal integer
	///   * i signed decimal integer
	///   * o unsigned octal integer
	///   * u unsigned decimal integer
	///   * x unsigned hexadecimal integer (lower case)
	///   * X unsigned hexadecimal integer (upper case)
	///   * e signed floating-point value in the form [-]d.dddde[<sign>]dd[d]
	///   * E signed floating-point value in the form [-]d.ddddE[<sign>]dd[d]
	///   * f signed floating-point value in the form [-]dddd.dddd
	///   * s std::string
	///   * z std::size_t
	/// Usage Examples:
	///     std::string s1 = Format("The answer to life, the universe, and everything is %d", 42);
	///     std::string s2 = Format("second: %[1]d, first: %[0]d", 1, 2);
	std::string Format(const std::string& fmt, const Any& value);
	std::string Format(const std::string& fmt, const Any& value1, const Any& value2);
	std::string Format(const std::string& fmt, const Any& value1, const Any& value2, const Any& value3);
	std::string Format(const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4);
	std::string Format(const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4, const Any& value5);
	std::string Format(const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4, const Any& value5, const Any& value6);


	void Format(std::string& result, const std::string& fmt, const Any& value);
	void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2);
	void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2, const Any& value3);
	void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4);
	void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4, const Any& value5);
	void Format(std::string& result, const std::string& fmt, const Any& value1, const Any& value2, const Any& value3, const Any& value4, const Any& value5, const Any& value6);


	void FormatVec(std::string& result, const std::string& fmt, const std::vector<Any>& values);

}

#endif