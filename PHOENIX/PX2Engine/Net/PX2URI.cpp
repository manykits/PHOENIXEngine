// PX2URI.cpp

#include "PX2URI.hpp"
#include "PX2NumberFormatter.hpp"
#include "PX2Exception.hpp"
using namespace PX2;

const std::string URI::ILLEGAL = "%<>{}|\\\"^`!*'()$,[]";
//----------------------------------------------------------------------------
URI::URI()
{

}
//----------------------------------------------------------------------------
URI::~URI()
{

}
//----------------------------------------------------------------------------
void URI::encode(const std::string& str,
	const std::string& reserved, std::string& encodedStr)
{
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		char c = *it;
		if ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '-' || c == '_' ||
			c == '.' || c == '~')
		{
			encodedStr += c;
		}
		else if (c <= 0x20 || c >= 0x7F || ILLEGAL.find(c) != std::string::npos 
			|| reserved.find(c) != std::string::npos)
		{
			encodedStr += '%';
			encodedStr += NumberFormatter::formatHex(
				(unsigned)(unsigned char)c, 2);
		}
		else encodedStr += c;
	}
}
//----------------------------------------------------------------------------
void URI::decode(const std::string& str, std::string& decodedStr,
	bool plusAsSpace)
{
	bool inQuery = false;
	std::string::const_iterator it = str.begin();
	std::string::const_iterator end = str.end();
	while (it != end)
	{
		char c = *it++;
		if (c == '?') inQuery = true;
		// spaces may be encoded as plus signs in the query
		if (inQuery && plusAsSpace && c == '+') c = ' ';
		else if (c == '%')
		{
			if (it == end) throw URISyntaxException(
				"URI encoding: no hex digit following percent sign", str);
			char hi = *it++;
			if (it == end) throw URISyntaxException(
				"URI encoding: two hex digits must follow percent sign", str);
			char lo = *it++;
			if (hi >= '0' && hi <= '9')
				c = hi - '0';
			else if (hi >= 'A' && hi <= 'F')
				c = hi - 'A' + 10;
			else if (hi >= 'a' && hi <= 'f')
				c = hi - 'a' + 10;
			else throw URISyntaxException("URI encoding: not a hex digit");
			c *= 16;
			if (lo >= '0' && lo <= '9')
				c += lo - '0';
			else if (lo >= 'A' && lo <= 'F')
				c += lo - 'A' + 10;
			else if (lo >= 'a' && lo <= 'f')
				c += lo - 'a' + 10;
			else throw URISyntaxException("URI encoding: not a hex digit");
		}
		decodedStr += c;
	}
}
//----------------------------------------------------------------------------