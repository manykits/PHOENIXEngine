// PX2UICollapseItem.hpp

#ifndef PX2UICOLLAPSEITEM_HPP
#define PX2UICOLLAPSEITEM_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2UICheckButton.hpp"

namespace PX2
{

	class UICollapsePanel;

	class PX2_ENGINE_ITEM UICollapseItem : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UICollapseItem);

	public:
		UICollapseItem();
		virtual ~UICollapseItem();

		void SetFont(const std::string &font);
		void SetTitle(const std::string &title);

		void Expand(bool expand);
		bool IsExpand() const;

		void SetExpandBarHeight(float height);
		float GetExpandBarHeight() const;

		void SetContentHeight(float cntHeight);
		float GetAllHeight() const;

		UIButton *GetButton();
		UIFrame *GetContentFrame();

	public_internal:
		void SetCollapsedPanel(UICollapsePanel *panel);
		void _UpdateButText();

	protected:
		void _ButCallback(UIFrame *frame, UICallType type);
		void _MarkPanelRecal();

		UICollapsePanel *mUICollapsePanel;
		std::string mFont;
		std::string mTitle;
		bool mIsExpand;
		float mExpandBarHeight;
		float mContentHeight;
		UIButtonPtr mExpandBut;
		UIFramePtr mContentFrame;

		UICheckButtonPtr mCheckBut;
	};

	PX2_REGISTER_STREAM(UICollapseItem);
	typedef Pointer0<UICollapseItem> UICollapseItemPtr;

}

#endif