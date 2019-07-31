// PX2UIMenu.hpp

#ifndef PX2UIMENU_HPP
#define PX2UIMENU_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIMenuItem.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIMenu : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UIMenu);

	public:
		enum MenuType
		{
			MT_HOR,
			MT_VER,
			MT_MAX_TYPE
		};
		UIMenu(MenuType mt = MT_HOR);
		virtual ~UIMenu();

		MenuType GetMenuType() const;

		void SetMainItemSize(const Sizef &mainItemSize);
		const Sizef &GetMainItemSize() const;

		void SetMenuItemFontSize(int fontSize);

		UIMenuItem *AddMainItem(const std::string &name, const std::string &title, 
			const std::string &scriptFun);
		int GetNunMainItems() const;

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		MenuType mMenuType;
		std::map<std::string, UIMenuItemPtr> mMainItemsMap;
		std::vector<UIMenuItem*> mMainItemsVec;

		Sizef mMainItemSize;
		int mItemFontSize;
	};

#include "PX2UIMenu.inl"
	PX2_REGISTER_STREAM(UIMenu);
	typedef Pointer0<UIMenu> UIMenuPtr;

}

#endif