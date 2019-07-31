// PX2LanguageManager.hpp

#ifndef PX2LANGUAGEMANAGER_HPP
#define PX2LANGUAGEMANAGER_HPP

#include "PX2UnityPre.hpp"
#include "PX2HashTable.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2LanguagePackage.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LanguageManager : public Singleton<LanguageManager>
	{
	public:
		LanguageManager();
		virtual ~LanguageManager();

		LanguagePackage *GetPackageEngine();
		LanguagePackage *GetPackageEditor();
		LanguagePackage *GetPackageApp();
		LanguagePackage *GetPackageApp1();

	protected:
		LanguagePackagePtr mPackageEngine;
		LanguagePackagePtr mPackageEditor;
		LanguagePackagePtr mPackageApp;
		LanguagePackagePtr mPackageApp1;
	};
	
#define PX2_LM_ENGINE (*LanguageManager::GetSingleton().GetPackageEngine())
#define PX2_LM_EDITOR (*LanguageManager::GetSingleton().GetPackageEditor())
#define PX2_LM_APP (*LanguageManager::GetSingleton().GetPackageApp())
#define PX2_LM_APP1 (*LanguageManager::GetSingleton().GetPackageApp1())

}

#endif