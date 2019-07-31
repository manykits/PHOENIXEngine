// PX2UIGridFrame.hpp

#ifndef PX2UIGRIDFRAME_HPP
#define PX2UIGRIDFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UISlider.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIGridFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIGridFrame);
		PX2_NEW(UIGridFrame);

	public:
		UIGridFrame();
		virtual ~UIGridFrame();

		void AddItem(UIFrame *item);
		void RemoveAllItems();

		void SetSliderPercent(float percent);

		UIFrame *GetContentFrame();

		virtual void OnSizeChanged();

	protected:
		virtual void OnSizeNodePicked(const CanvasInputData &inputData);
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _UpdateContentPos();
		void _SliderCallback(UIFrame *frame, UICallType type);

		bool mIsUpdateSliderVisible;
		bool mIsUpdateContentPos;

		float mSliderSize;
		float mItemHeight;
		UIFramePtr mMaskFrame;
		UIFramePtr mContentFrame;
		std::vector<UIFramePtr> mItems;
		UISliderPtr mSlider;

		std::vector<UIFramePtr> mSelectedItems;
		int mSelectedIndex;
	};

	PX2_REGISTER_STREAM(UIGridFrame);
	typedef Pointer0<UIGridFrame> UIGridFramePtr;

}

#endif