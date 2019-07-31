// PX2StringTokenizer.hpp

#ifndef PX2STRINGTOKENIZER_HPP
#define PX2STRINGTOKENIZER_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	/// 将字符串进行分割
	/**
	* TOK_IGNORE_EMPTY: 空 tokens 被忽略
	* TOK_TRIM:收尾的字符被忽略，例如
	*		StringTokenizer(",ab,cd,", ","); 产生两个token "ab"和"cd"
	*/
	class PX2_ENGINE_ITEM StringTokenizer
	{
	public:
		enum Options
		{
			TOK_IGNORE_EMPTY = 1, /// ignore empty tokens
			TOK_TRIM         = 2  /// remove leading and trailing whitespace from tokens
		};

		typedef std::vector<std::string>::const_iterator Iterator;

		StringTokenizer(const std::string& str, const std::string& separators, int options = 0);
		~StringTokenizer();

		Iterator Begin() const;
		Iterator End() const;

		const std::string& operator [] (std::size_t index) const;
		
		std::string GetAt(int index) const;
		int Count();

	private:
		StringTokenizer(const StringTokenizer&);
		StringTokenizer& operator = (const StringTokenizer&);

		std::vector<std::string> mTokens;
	};
#include "PX2StringTokenizer.inl"

}

#endif