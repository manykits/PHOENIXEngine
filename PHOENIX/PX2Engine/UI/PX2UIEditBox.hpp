// PX2UIEditBox.hpp

#ifndef PX2UIEDITBOX_HPP
#define PX2UIEDITBOX_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIInputText.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2InterpCurveAlphaCtrl.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIEditBox : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UIEditBox);
		PX2_DECLARE_STREAM(UIEditBox);

	public:
		UIEditBox();
		virtual ~UIEditBox();

		virtual void Enable(bool enable);

		void SetPassword(bool isPassword);
		bool IsPassword() const;

		UIFPicBox *GetBackPicBox() const;

		void SetText(const std::string &text);
		const std::string &GetText() const;
		const std::string &GetRealText() const;
		UIInputText *GetInputText();

		void SetFixedWidth(float fixedWidth);
		float GetFixedWidth() const;

		virtual void OnSizeChanged();
		virtual void UpdateLeftBottomCornerOffset(Movable *parent);

		void OnAttachWithIME(const APoint &screenPos);
		void OnDetachWithIME();
		void OnTextChanged();
		void OnEnter(); // »Ø³µ
		void OnInsertArrow(int dirType);

		void OnEvent(Event *ent);

	protected:
		virtual void OnWidgetPicked(const CanvasInputData &data);
		virtual void OnSizeNodeNotPicked(const CanvasInputData &data);
		void _AttachIME(const APoint &attachWorldPos);
		void _DetachIME();
		int _CalAttachCharaIndex(const APoint &attachWorldPos);
		void _AdjustFadePicBoxPos(int characterIndex, bool isAttachInput);

		void SelectText(int fromCharaIndex, int toCharaIndex);
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		UIInputTextPtr mInputText;
		UIFPicBoxPtr mBackPicBox;
		UIFPicBoxPtr mFadePicBox;
		float mFadePicBoxSideWidth;
		float mFixedWidth;

		float mTextWidth;
		float mLeftTextWidth;
		UIFPicBoxPtr mSelectPicBox;

		bool mIsAttachedIME;
		APoint mAttachedLocalPos;
		float mAttachedOffsetX;
		int mAttachedSelectCharaIndex;
		float mAttachedTimeing;
		int mDragingSelectCharaIndex;
		bool mIsDragingSelect;

		bool mIsPressed;
	};

	PX2_REGISTER_STREAM(UIEditBox);
	typedef Pointer0<UIEditBox> UIEditBoxPtr;
#include "PX2UIEditBox.inl"

}

#endif