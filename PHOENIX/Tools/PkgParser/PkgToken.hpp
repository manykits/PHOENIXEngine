// PkgToken.hpp

#ifndef PKGTOKEN_HPP
#define PKGTOKEN_HPP

#include "PkgParsePre.hpp"

namespace PkgParse
{

	class PkgToken
	{
	public:
		PkgToken();
		~PkgToken();

		enum TokenType
		{
			RealNumber = 1,
			Equals, Comma, OpenParen, CloseParen, OpenBrace, CloseBrace, Quote,
			Comment, Include, File,
			// ¹Ø¼ü×Ö
			Class, Public, Wave, Enum,
			String,
			Unknown
		};

		TokenType TheTokenType;
		static TokenType DetermineTokenType(const std::string &str);
	};

}

#endif