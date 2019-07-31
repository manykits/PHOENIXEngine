// PkgTokenizer.hpp

#ifndef PKGTOKENIZER_HPP
#define PKGTOKENIZER_HPP

#include "PkgParsePre.hpp"

namespace PkgParse
{

	/// 将字符串进行分割
	/**
	* TOK_IGNORE_EMPTY: 空 tokens 被忽略
	* TOK_TRIM:收尾的字符被忽略，例如
	*		StringTokenizer(",ab,cd,", ","); 产生两个token "ab"和"cd"
	*/
	class PkgStringTokenizer
	{
	public:
		enum Options
		{
			TOK_IGNORE_EMPTY = 1, /// ignore empty tokens
			TOK_TRIM = 2  /// remove leading and trailing whitespace from tokens
		};

		typedef std::vector<std::string>::const_iterator Iterator;

		PkgStringTokenizer(const std::string& str, const std::string& separators, int options = 0);
		~PkgStringTokenizer();

		Iterator Begin() const;
		Iterator End() const;

		const std::vector<std::string> &GetTokens() const;
		const std::string& operator [] (std::size_t index) const;
		std::size_t Count() const;
		/// Returns the number of tokens.

	private:
		PkgStringTokenizer(const PkgStringTokenizer&);
		PkgStringTokenizer& operator = (const PkgStringTokenizer&);

		std::vector<std::string> _tokens;
	};

}

#endif