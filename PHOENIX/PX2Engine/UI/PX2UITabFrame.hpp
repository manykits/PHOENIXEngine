// PX2UITabFrame.hpp

#ifndef PX2UITABFRAME_HPP
#define PX2UITABFRAME_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIButton.hpp"

namespace PX2
{

	class UIAuiBlockFrame;
	
	class PX2_ENGINE_ITEM UITabFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UITabFrame);
		PX2_NEW(UITabFrame);

	public:
		UITabFrame();
		virtual ~UITabFrame();

		enum LayoutPosType
		{
			LPT_TOP,
			LPT_BOTTOM,
			LPT_LEFT,
			LPT_RIGHT,
			LPT_MAX_TYPE
		};
		void SetLayoutPos(LayoutPosType posType);
		LayoutPosType GetLayoutPos() const;

		void SetTabBarHeight(float height);
		float GetTabBarHeight() const;

		void SetTabWidth(float width);
		float GetTabWidth() const;

		void SetTabHeight(float height);
		float GetTabHeight() const;

		// font
		void SetFontSize(int size);
		int GetFontSize() const;

		void SetFontScale(float scale);
		float GetFontScale() const;

		void SetFontColor(const Float3 &fontColor);
		const Float3 &GetFontColor() const;		

		// lay
		enum TabLayoutType
		{
			TLT_FIX,
			TLT_SCALE,
			TLT_MAX_TYPE
		};
		void SetTabLayoutType(TabLayoutType tlt);
		TabLayoutType GetTabLayoutType() const;

		virtual void OnSizeChanged();

		void AddTab(const std::string &name, const std::string &title,
			UIFrame *tabContentFrame);
		bool IsHasTab(const std::string &name);
		UIFrame *GetTab(const std::string &name);
		void RemoveTab(const std::string &name);
		void RemoveAllTabs();
		int GetNumTabs() const;
		void SetActiveTab(const std::string &name);
		std::string GetActiveTab() const;
		UIButton *GetTabButton(const std::string &name);
		UIFrame *GetTitleBarFrame();
		void OnSetActive();

		void OnDragBegin();
		
		virtual void PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas);
		virtual void OnWidgetPicked(const CanvasInputData &inputData);
		virtual void OnSizeNodeNotPicked(const CanvasInputData &inputData);

	public_internal:
		void _SetActiveTab(const std::string &name);

	protected:
		void _TabButCallback(UIFrame *frame, UICallType type);
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _CalTabs();

		LayoutPosType mLayoutPosType;

		int mFontSize;
		float mFontScale;
		Float3 mFontColor;

		bool mIsTabsNeedReCal;
		TabLayoutType mTabLayoutType;

		UIFramePtr mFrame_TitleBar;
		std::vector<UIButtonPtr> mTabButs;
		float mTabBarHeight;
		float mTabWidth;
		float mTabHeight;

		UIFramePtr mFrame_Content;
		std::map<std::string, UIFramePtr> mTabContentFrames;

		UIButtonPtr mActiveTabBut;
		UIFramePtr mActiveTabFrame;
		bool mIsDragingTab;
		std::string mPressingTabName;

public_internal:
		void SetAuiBlockFrame(UIAuiBlockFrame *auiBlockFrame);
		UIAuiBlockFrame *GetAuiBlockFrame();

	protected:
		UIAuiBlockFrame *mAuiBlockFrame;
	};

#include "PX2UITabFrame.inl"
	PX2_REGISTER_STREAM(UITabFrame);
	typedef Pointer0<UITabFrame> UITabFramePtr;

}

#endif