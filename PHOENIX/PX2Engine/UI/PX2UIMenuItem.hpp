// PX2UIMenuItem.hpp

#ifndef PX2UIMENUITEM_HPP
#define PX2UIMENUITEM_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIText.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIMenuItem : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIMenuItem);

	public:
		UIMenuItem(const std::string name, const std::string &title,
			const std::string &scriptFun);
		virtual ~UIMenuItem();

		UIFText *GetText();

public_internal:
		void SetBeMainItem(bool isMainItem);
		bool IsMainItem() const;

	protected:
		bool mIsMainItem;
		std::string mTitle;
		std::string mScriptFun;
		UIButtonPtr mBut;
		UIFTextPtr mTitleText;

		std::map<std::string, Pointer0<UIMenuItem> > ChildItems;
	};

#include "PX2UIMenuItem.inl"
	PX2_REGISTER_STREAM(UIMenuItem);
	typedef Pointer0<UIMenuItem> UIMenuItemPtr;

}

#endif