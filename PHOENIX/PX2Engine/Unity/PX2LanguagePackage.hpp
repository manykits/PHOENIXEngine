// PX2LanguagePackage.hpp

#ifndef PX2LANGUAGEPACKAGE_HPP
#define PX2LANGUAGEPACKAGE_HPP

#include "PX2UnityPre.hpp"
#include "PX2HashTable.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LanguagePackage
	{
	public:
		LanguagePackage();
		virtual ~LanguagePackage();

		bool Load(const std::string &filename);
		bool Add(const std::string &filename);
		void AddItem(const std::string &key, const std::string &langauge0,
			const std::string &langauge1);
		void AddItem1(const std::string &key, const std::string &langauge0);

		void SetLanguage(int index);
		bool HasValue(const std::string &key) const;
		const std::string &GetValue(const std::string &key);
		const std::string &GetValueNoAssert(const std::string &key);
		const std::string &GetValue(const std::string &key, int index);
		const std::string &V(const std::string &key);
		const std::string &V(const std::string &key, int index);
		void Clear();

	protected:
		int mLanguageIndex;
		typedef std::map<FString, std::vector<std::string> > LanguageTable;
		LanguageTable mLanguages;

		std::string mEmptyStr;
	};
	typedef Pointer0<LanguagePackage> LanguagePackagePtr;

}

#endif