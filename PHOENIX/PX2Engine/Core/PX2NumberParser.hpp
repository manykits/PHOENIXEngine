// PX2NumberParser.hpp

#ifndef PX2NUMPARSER_HPP
#define PX2NUMPARSER_HPP

#include "PX2CorePre.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM NumberParser
	{
	public:
		static const unsigned short NUM_BASE_OCT = 010;
		static const unsigned short NUM_BASE_DEC = 10;
		static const unsigned short NUM_BASE_HEX = 0x10;

		static int parse(const std::string& s, char thousandSeparator = ',');
		static bool tryParse(const std::string& s, int& value, char thousandSeparator = ',');

		static unsigned parseUnsigned(const std::string& s, char thousandSeparator = ',');
		static bool tryParseUnsigned(const std::string& s, unsigned& value, char thousandSeparator = ',');

		static unsigned parseHex(const std::string& s);
		static bool tryParseHex(const std::string& s, unsigned& value);

		static unsigned parseOct(const std::string& s);
		static bool tryParseOct(const std::string& s, unsigned& value);

		static int64_t parse64(const std::string& s, char thousandSeparator = ',');
		static bool tryParse64(const std::string& s, int64_t& value, char thousandSeparator = ',');

		static double parseFloat(const std::string& s, char decimalSeparator = '.', char thousandSeparator = ',');
		static bool tryParseFloat(const std::string& s, double& value, char decimalSeparator = '.', char thousandSeparator = ',');

		static bool parseBool(const std::string& s);
		static bool tryParseBool(const std::string& s, bool& value);
	};

}

#endif