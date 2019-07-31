// PX2StringTokenizer.cpp

#include "PX2StringTokenizer.hpp"
#include "PX2Ascii.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
StringTokenizer::StringTokenizer(const std::string& str, 
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
			while (it1 != end && Ascii::IsSpace(*it1)) ++it1;
		}
		it2 = it1;
		while (it2 != end && separators.find(*it2) == std::string::npos) ++it2;
		it3 = it2;
		if (it3 != it1 && (options & TOK_TRIM))
		{
			--it3;
			while (it3 != it1 && Ascii::IsSpace(*it3)) --it3;
			if (!Ascii::IsSpace(*it3)) ++it3;
		}
		if (options & TOK_IGNORE_EMPTY)
		{
			if (it3 != it1)
				mTokens.push_back(std::string(it1, it3));
		}
		else
		{
			mTokens.push_back(std::string(it1, it3));
		}
		it1 = it2;
		if (it1 != end) ++it1;
	}
}
//----------------------------------------------------------------------------
StringTokenizer::~StringTokenizer()
{
}
//----------------------------------------------------------------------------
const std::string& StringTokenizer::operator [] (std::size_t index) 
	const
{
	if (index >= mTokens.size())
	{
		assertion(false, "index should be in right range.\n");
	}
	return mTokens[index];
}
//----------------------------------------------------------------------------
std::string StringTokenizer::GetAt(int index) const
{
	if (0 <= index && index <(int)mTokens.size())
	{
		return mTokens[index];
	}

	return "";
}
//----------------------------------------------------------------------------