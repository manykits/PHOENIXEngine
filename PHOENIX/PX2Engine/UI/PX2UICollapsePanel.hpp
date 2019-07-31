// PX2UICollapsePanel.hpp

#ifndef PX2UICOLLAPSEPANEL_HPP
#define PX2UICOLLAPSEPANEL_HPP

#include "PX2UIFrame.hpp"
#include "PX2UICollapseItem.hpp"
#include "PX2UISlider.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UICollapsePanel : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UICollapsePanel);

	public:
		UICollapsePanel();
		virtual ~UICollapsePanel();

		void SetBarPicBox(UIPicBox *picBox);

		void SetFont(const std::string &font);

		void SetExpandBarHeight(float height);
		float GetExpandBarHeight() const;

		void SetSliderWidth(float size);
		UISlider *GetSlider();

		void RemoveAllItems();
		UICollapseItem *AddItem(const std::string &name, const std::string &title);
		void ShowItem(const std::string &name, bool show);

		virtual void OnSizeChanged();

	public_internal:
		void _MarkCollpaseRecal();

	protected:
		virtual void OnSizeNodePicked(const CanvasInputData &inputData);
		void _SliderCallback(UIFrame *frame, UICallType type);
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _CalCollpase();
		void _UpdateContentPos();

		bool mIsNeedUpdateContentPos;

		std::string mFont;
		UIPicBoxPtr mBarPicBox;
		float mExpandBarHeight;
		UIFramePtr mContentFrame;
		std::vector<UICollapseItemPtr> mCollpaseFrames;
		std::map<std::string, UICollapseItemPtr> mCollpaseFrameMap;
		UISliderPtr mSlider;

		bool mIsNeedRecalCollpase;
	};

	PX2_REGISTER_STREAM(UICollapsePanel);
	typedef Pointer0<UICollapsePanel> UICollapsePanelPtr;

}

#endif