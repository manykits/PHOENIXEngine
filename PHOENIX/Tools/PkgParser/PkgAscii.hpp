// PkgAscii.hpp

#ifndef PKGASCII_HPP
#define PKGASCII_HPP

#include "PkgParsePre.hpp"

namespace PkgParse
{

	class PkgAscii
	{
	public:
		enum CharacterProperties
		{
			ACP_CONTROL = 0x0001,
			ACP_SPACE = 0x0002,
			ACP_PUNCT = 0x0004,
			ACP_DIGIT = 0x0008,
			ACP_HEXDIGIT = 0x0010,
			ACP_ALPHA = 0x0020,
			ACP_LOWER = 0x0040,
			ACP_UPPER = 0x0080,
			ACP_GRAPH = 0x0100,
			ACP_PRINT = 0x0200
		};

		static int Properties(int ch);
		static bool HasSomeProperties(int ch, int properties);
		static bool HasProperties(int ch, int properties);
		static bool IsAscii(int ch);
		static bool IsSpace(int ch);
		static bool IsDigit(int ch);
		static bool IsHexDigit(int ch);
		static bool IsPunct(int ch);
		static bool IsAlpha(int ch);
		static bool IsAlphaNumeric(int ch);
		static bool IsLower(int ch);
		static bool IsUpper(int ch);

		static int ToLower(int ch);
		static int ToUpper(int ch);

	private:
		static const int CHARACTER_PROPERTIES[128];
	};

}

#endif