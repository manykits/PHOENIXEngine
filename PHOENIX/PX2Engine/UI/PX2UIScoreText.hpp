// PX2UIScoreText.hpp

#ifndef PX2UISCORETEXT_HPP
#define PX2UISCORETEXT_HPP

#include "PX2UIPre.hpp"
#include "PX2UIText.hpp"
#include "PX2UIScoreTextCtrl.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIScoreText : public UIText
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(UIScoreText);

	public:
		UIScoreText();
		virtual ~UIScoreText();

		void SetNumAnimInit(int num);
		void SetNumAnim(int num);

		void SetNum(int num);
		int GetNum() const;

		UIScoreStaticTextCtrl *GetScoreStaticTextCtrl();

	protected:
		int mNum;
		UIScoreStaticTextCtrlPtr mSSTCtrl;
	};

	PX2_REGISTER_STREAM(UIScoreText);
	typedef Pointer0<UIScoreText> UIScoreStaticTextPtr;
#include "PX2UIScoreText.inl"

}

#endif