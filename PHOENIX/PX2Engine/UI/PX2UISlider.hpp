// PX2UISlider.hpp

#ifndef PX2UISLIDER_HPP
#define PX2UISLIDER_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIButton.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UISlider : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UISlider);
		PX2_DECLARE_STREAM(UISlider);

	public:
		UISlider();
		virtual ~UISlider();

		enum DirectionType
		{
			DT_HORIZONTAL,
			DT_VERTICALITY,
			GT_MAX_TYPE
		};
		void SetDirectionType(DirectionType dt);
		DirectionType GetDirectionType();

		UIFPicBox *GetFPicBoxBack();
		UIButton *GetButSlider();

		void SetSliderLength(float length);
		float GetSliderLength() const;

		void SetPercent(float percent, bool doCall=true);
		float GetPercent() const;

		void SetContentFrame(UIFrame *contentFrame);
		UIFrame *GetContentFrame();

		bool IsDraging() const;

		UIFText *CreateAddText(const std::string &text = "",
			const Float3& color = Float3::WHITE);
		UIFText *GetFText();
		UIText *GetText();

		virtual void OnBeAttached();
		virtual void OnBeDetach();
	
	protected:
		virtual void OnSizeNodePicked(const CanvasInputData &inputData);
		virtual void OnSizeNodeNotPicked(const CanvasInputData &inputData);

	protected:
		virtual void OnSizeChanged();
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _GenSlider();
		void _UpdateSliderLength();
		void _ReGenSliderLayout();
		void _UpdateSlider();
		void _SliderDrag(UIFrame *frame, UICallType type);
		void _SetDraging(bool draging);
		void _RelativeSizeChangeCallback(SizeNode *tellObject);

		bool mIsNeedReGenSliderLayout;
		bool mIsNeededUpdate;
		bool mIsDraging;

		DirectionType mDirectionType;
		UIFPicBoxPtr mFPicBoxBack;
		UIButtonPtr mButSlider;
		float mButSliderLength;
		float mPercent;

		UIFramePtr mContentFrame;

		UIFTextPtr mFText;

		int mPickTouchID;
	};

	PX2_REGISTER_STREAM(UISlider);
	typedef Pointer0<UISlider> UISliderPtr;
#include "PX2UISlider.inl"

}

#endif