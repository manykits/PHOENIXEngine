// PX2UIColorSelect.hpp

#ifndef PX2UICOLORSELECTFRAME_HPP
#define PX2UICOLORSELECTFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIEditBox.hpp"
#include "PX2UIFText.hpp"
#include "PX2UIButton.hpp"
#include "PX2UISlider.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIColorSelector : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UIColorSelector);
		PX2_DECLARE_STREAM(UIColorSelector);

	public:
		UIColorSelector();
		virtual ~UIColorSelector();

		void SetInitColor(const Float3 &color);
		void SetSelectColor(const Float3 &color);
		const Float3 &GetSelectColor() const;

		void _UICallback(UIFrame *frame, UICallType type);

		virtual void OnWidgetPicked(const CanvasInputData &inputData);
		virtual void OnSizeNodeNotPicked(const CanvasInputData &inputData);

	protected:
		Float2 _GetUV(const Float2 &pickPos, const Rectf &worldRect);
		Float3 _GetColor(const Float2 &uv, float percent);
		void _UpdateColor();

		UISliderPtr mSliderBrightness;
		UIFPicBoxPtr mColorPattle;
		UIFPicBoxPtr mColorPattleCursor;

		UIEditBoxPtr mEditBoxRed;
		UIEditBoxPtr mEditBoxGreen;
		UIEditBoxPtr mEditBoxBlue;
		UIFTextPtr mEBTextRed;
		UIFTextPtr mEBTextGreen;
		UIFTextPtr mEBTextBlue;
		UIButtonPtr mButClose;

		Float3 mSelectColor;

		bool mIsPickedPattle;
		Float2 mCurUV;
		float mCurPercent;
	};

	PX2_REGISTER_STREAM(UIColorSelector);
	typedef Pointer0<UIColorSelector> UIColorSelectorPtr;

}

#endif