// PX2LanguagePackage.cpp

#include "PX2LanguagePackage.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2CSVParser.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
bool LanguagePackage::Load(const std::string &filename)
{
	CSVParser csv;

	if (csv.Load(filename))
	{
		mLanguages.clear();

		// language start form 2
		for (int i = 2; i < csv.GetNumLines(); i++)
		{
			std::string key = csv[i][0].String();

			std::vector<std::string> vals;
			for (int j = 1; j < csv.GetNumCols(); j++)
			{
				vals.push_back(csv[i][j].String());
			}

			mLanguages[key.c_str()] = vals;
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool LanguagePackage::Add(const std::string &filename)
{
	CSVParser csv;

	if (csv.Load(filename))
	{
		for (int i = 2; i < csv.GetNumLines(); i++)
		{
			std::string key = csv[i][0].String();

			std::vector<std::string> vals;
			for (int j = 1; j < csv.GetNumCols(); j++)
			{
				vals.push_back(csv[i][j].String());
			}

			mLanguages[key.c_str()] = vals;
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LanguagePackage::AddItem(const std::string &key, const std::string &langauge0,
	const std::string &langauge1)
{
	auto it = mLanguages.find(key.c_str());
	if (it != mLanguages.end())
	{
		it->second.clear();
		it->second.push_back(langauge0);
		it->second.push_back(langauge1);
	}
	else
	{
		std::vector<std::string> vals;
		vals.push_back(langauge0);
		vals.push_back(langauge1);
		mLanguages[key.c_str()] = vals;
	}
}
//----------------------------------------------------------------------------
void LanguagePackage::AddItem1(const std::string &key,
	const std::string &langauge0)
{
	auto it = mLanguages.find(key.c_str());
	if (it != mLanguages.end())
	{
		it->second.clear();
		it->second.push_back(langauge0);
		it->second.push_back(key);
	}
	else
	{
		std::vector<std::string> vals;
		vals.push_back(langauge0);
		vals.push_back(key);
		mLanguages[key.c_str()] = vals;
	}
}
//----------------------------------------------------------------------------
void LanguagePackage::SetLanguage(int index)
{
	mLanguageIndex = index;
}
//----------------------------------------------------------------------------
bool LanguagePackage::HasValue(const std::string &key) const
{
	auto it = mLanguages.find(key.c_str());
	return it != mLanguages.end();
}
//----------------------------------------------------------------------------
const std::string &LanguagePackage::GetValue(const std::string &key)
{
	auto it = mLanguages.find(key.c_str());
	if (it != mLanguages.end())
	{
		if (0 <= mLanguageIndex && mLanguageIndex < (int)it->second.size())
		{
			return it->second[mLanguageIndex];
		}
	}

	assertion(false, "language key %s does not exist.", key.c_str());
	PX2_LOG_ERROR("language key %s does not exist.", key.c_str());

	return mEmptyStr;
}
//----------------------------------------------------------------------------
const std::string &LanguagePackage::GetValueNoAssert(const std::string &key)
{
	auto it = mLanguages.find(key.c_str());
	if (it != mLanguages.end())
	{
		if (0 <= mLanguageIndex && mLanguageIndex < (int)it->second.size())
		{
			return it->second[mLanguageIndex];
		}
	}

	return mEmptyStr;
}
//----------------------------------------------------------------------------
const std::string &LanguagePackage::GetValue(const std::string &key, int index)
{
	auto it = mLanguages.find(key.c_str());
	if (it != mLanguages.end())
	{
		if (0 <= index && index < (int)it->second.size())
		{
			return it->second[index];
		}
	}

	assertion(false, "language key %s does not exist.", key.c_str());
	PX2_LOG_ERROR("language key %s does not exist.", key.c_str());

	return mEmptyStr;
}
//----------------------------------------------------------------------------
const std::string &LanguagePackage::V(const std::string &key)
{
	return GetValue(key);
}
//----------------------------------------------------------------------------
const std::string &LanguagePackage::V(const std::string &key, int index)
{
	return GetValue(key, index);
}
//----------------------------------------------------------------------------
LanguagePackage::LanguagePackage() :
mLanguageIndex(0)
{
}
//----------------------------------------------------------------------------
LanguagePackage::~LanguagePackage()
{
	Clear();
}
//----------------------------------------------------------------------------
void LanguagePackage::Clear()
{
	mLanguages.clear();
}
//----------------------------------------------------------------------------