// PX2UIButtonBase.hpp

#ifndef PX2UIBUTTONBASE_HPP
#define PX2UIBUTTONBASE_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIFText.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIButtonBase : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UIButtonBase);

	public:
		virtual ~UIButtonBase();

		virtual void OnPivotChanged();

		enum ButType
		{
			BT_COLOR,
			BT_PICBOXSWAP,
			BT_MAX_TYPE
		};
		void SetButType(ButType butType);
		ButType GetButType() const;

		enum ButtonState
		{
			BS_NORMAL,
			BS_HOVERED,
			BS_PRESSED,
			BS_DISABLED,
			BS_MAX_STATE
		};
		virtual void SetButtonState(ButtonState state);
		ButtonState GetButtonState() const;

		void SetPicBox(ButtonState state, UIPicBox *pic);
		UIPicBox *GetPicBoxAtState(ButtonState state);

		void SetPicBoxSizeSameWithButton(bool same);
		bool IsPicBoxSizeSameWithButton() const;

		virtual void AutoMakeSizeFixable(float scale=1.0f);

		void SetStateColor(ButtonState state, const Float3 &color);
		const Float3 &GetStateColor(ButtonState state) const;
		void SetStateAlpha(ButtonState state, float alpha);
		float GetStateAlpha(ButtonState state) const;
		void SetStateBrightness(ButtonState state, float brightness);
		float GetStateBrightness(ButtonState state) const;

		void SetStateColorDefaultWhite();

		virtual void SetActivate(bool act);

		UIFText *CreateAddText(const std::string &text="", 
			const Float3& color=Float3::WHITE);
		UIFText *GetFText();
		UIText *GetText();

		virtual void OnSizeChanged();

	protected:
		UIButtonBase();
		const Float3 &_GetStateColorWithActivated(ButtonState state) const;
		float _GetStateAlphaWithActivated(ButtonState state) const;
		float _GetStateBrightnessWithActivated(ButtonState state) const;

		ButType mButType;

		Float3 mNormalColor;
		Float3 mHoveredColor;
		Float3 mPressedColor;
		Float3 mDisableColor;
		float mNormalAlpha;
		float mHoveredAlpha;
		float mPressedAlpha;
		float mDisableAlpha;
		float mNormalBrightness;
		float mHoveredBrightness;
		float mPressedBrightness;
		float mDisableBrightness;

		UIPicBoxPtr mPicBoxNormal;
		UIPicBoxPtr mPicBoxOver;
		UIPicBoxPtr mPicBoxDown;
		UIPicBoxPtr mPicBoxDisabled;

		ButtonState mButtonState;

		UIFTextPtr mFText;

		bool mIsPicBoxSizeSameWithButton;

		int mPressedTouchID;
	};

	PX2_REGISTER_STREAM(UIButtonBase);
	typedef Pointer0<UIButtonBase> UIButtonBasePtr;
#include "PX2UIButtonBase.inl"

}

#endif