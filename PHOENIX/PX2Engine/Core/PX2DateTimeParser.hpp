// PX2DateTimeParser.hpp

#ifndef PX2DATETIMEPARSER_HPP
#define PX2DATETIMEPARSER_HPP

#include "PX2DateTime.hpp"
#include "PX2DateTimeFormat.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM DateTimeParser
	{
	public:
		static void parse(const std::string& fmt, const std::string& str,
			DateTime& dateTime, int& timeZoneDifferential);
		static DateTime parse(const std::string& fmt, const std::string& str, 
			int& timeZoneDifferential);
		static bool tryParse(const std::string& fmt, const std::string& str,
			DateTime& dateTime, int& timeZoneDifferential);
		static void parse(const std::string& str, DateTime& dateTime,
			int& timeZoneDifferential);
		static DateTime parse(const std::string& str, int& timeZoneDifferential);
		static bool tryParse(const std::string& str, DateTime& dateTime,
			int& timeZoneDifferential);
		static int parseMonth(std::string::const_iterator& it, 
			const std::string::const_iterator& end);
		static int parseDayOfWeek(std::string::const_iterator& it, 
			const std::string::const_iterator& end);

	protected:
		static int parseTZD(std::string::const_iterator& it, 
			const std::string::const_iterator& end);
		static int parseAMPM(std::string::const_iterator& it,
			const std::string::const_iterator& end, int hour);
	};


}

#endif