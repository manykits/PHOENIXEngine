// PkgTokenizer.cpp

#include "PkgTokenizer.hpp"
#include "PkgAscii.hpp"
using namespace PkgParse;
using namespace std;

//----------------------------------------------------------------------------
PkgStringTokenizer::PkgStringTokenizer(const std::string& str,
	const std::string& separators, int options)
{
	std::string::const_iterator it1 = str.begin();
	std::string::const_iterator it2;
	std::string::const_iterator it3;
	std::string::const_iterator end = str.end();

	while (it1 != end)
	{
		if (options & TOK_TRIM)
		{
			while (it1 != end && PkgAscii::IsSpace(*it1)) ++it1;
		}
		it2 = it1;
		while (it2 != end && separators.find(*it2) == std::string::npos) ++it2;
		it3 = it2;
		if (it3 != it1 && (options & TOK_TRIM))
		{
			--it3;
			while (it3 != it1 && PkgAscii::IsSpace(*it3)) --it3;
			if (!PkgAscii::IsSpace(*it3)) ++it3;
		}
		if (options & TOK_IGNORE_EMPTY)
		{
			if (it3 != it1)
				_tokens.push_back(std::string(it1, it3));
		}
		else
		{
			_tokens.push_back(std::string(it1, it3));
		}
		it1 = it2;
		if (it1 != end) ++it1;
	}
}
//----------------------------------------------------------------------------
PkgStringTokenizer::~PkgStringTokenizer()
{
}
//----------------------------------------------------------------------------
const std::string& PkgStringTokenizer::operator [] (std::size_t index)
const
{
	if (index >= _tokens.size())
	{
		assert(false);
	}
	return _tokens[index];
}
//----------------------------------------------------------------------------
PkgStringTokenizer::Iterator PkgStringTokenizer::Begin() const
{
	return _tokens.begin();
}
//----------------------------------------------------------------------------
const std::vector<std::string> &PkgStringTokenizer::GetTokens() const
{
	return _tokens;
}
//----------------------------------------------------------------------------
PkgStringTokenizer::Iterator PkgStringTokenizer::End() const
{
	return _tokens.end();
}
//----------------------------------------------------------------------------
std::size_t PkgStringTokenizer::Count() const
{
	return _tokens.size();
}
//----------------------------------------------------------------------------