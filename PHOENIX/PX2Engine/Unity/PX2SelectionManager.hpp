// PX2SelectionManager.hpp

#ifndef PX2SELECTIONMANAGER_HPP
#define PX2SELECTIONMANAGER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Selection.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM SelectionManager : public Singleton<SelectionManager>
	{
	public:
		SelectionManager();
		virtual ~SelectionManager();

		void Clear();

		Selection *CreateAddSelection(const std::string &name);
		void DestorySelecton(const std::string &name);
		Selection *GetSelecton(const std::string &name);

	protected:
		std::map<std::string, SelectionPtr> mSelectionMap;
	};

#define PX2_SELECTM SelectionManager::GetSingleton()
#define PX2_SELECTM_D SelectionManager::GetSingleton().GetSelecton("Default")
#define PX2_SELECTM_E SelectionManager::GetSingleton().GetSelecton("Editor")

}

#endif