// PX2UIInputText.hpp

#ifndef PX2UIINPUTTEXT_HPP
#define PX2UIINPUTTEXT_HPP

#include "PX2UIText.hpp"
#include "PX2IMEDelegate.hpp"

namespace PX2
{

	class UIInputText;

	enum UIInputTextCallType
	{
		UIITCT_ATTACHWITHIME,
		UIITCT_DETACHWITHWITHIME,
		UIITCT_INSERTTEXT,
		UIITCT_DELETEBACKWARD,
		UIITCT_DELETE,
		UIITCT_ENTER,
		UIITCT_ARROW,
		UIITCT_MAX_TYPE
	};
	typedef void(*UIInputTextCallback) (UIInputText *inputText, UIInputTextCallType type);

	class PX2_ENGINE_ITEM UIInputText : public UIText, public IMEDelegate
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIInputText);

	public:
		UIInputText();
		virtual ~UIInputText();

		virtual bool AttachWithIME(const APoint &screenPos);
		virtual bool DetachWithIME();

		void SetCallback(UIInputTextCallback callback);
		UIInputTextCallback GetCallback() const;

		void SetRealText(const std::string &realText);
		const std::string &GetRealText() const;

		virtual const char *GetContentText() const;

		void SetPassword(bool isPassword);
		bool IsPassword() const;

		void SetFixedWidth(float fixedWidth);
		float GetFixedWidth() const;

		void SetInputTextCharacterIndex(int charaIndex);
		int GetInputTextCharacterIndex() const;

		virtual float GetTextWidth() const;
		virtual float GetTextWidth(const std::string &text) const;

	protected:
		virtual void InsertText(const char *text, int length);
		virtual void DeleteBackward();
		virtual void Delete();
		virtual void InsertArrow(int dirType);
		void _DeleteSelectedCharas();

		std::string mInputString;
		UIInputTextCallback mCallback;

		bool mIsPassword;
		std::string mRealText;

		float mFixedWidth;

		int mInputTextCharacterIndex;
	};

#include "PX2UIInputText.inl"
	PX2_REGISTER_STREAM(UIInputText);
	typedef Pointer0<UIInputText> UIInputTextPtr;

}

#endif