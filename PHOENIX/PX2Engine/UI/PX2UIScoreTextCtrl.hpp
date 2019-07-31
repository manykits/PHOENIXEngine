// PX2UIScoreTextCtrl.hpp

#ifndef PX2UISCORESTATICTEXTCTRL_HPP
#define PX2UISCORESTATICTEXTCTRL_HPP

#include "PX2UIPre.hpp"
#include "PX2InterpCurveFloatController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIScoreStaticTextCtrl : public InterpCurveFloatController
	{
		PX2_DECLARE_RTTI; 
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIScoreStaticTextCtrl);

	public:
		UIScoreStaticTextCtrl();
		virtual ~UIScoreStaticTextCtrl();

		void SetSpeed(float speed);
		float GetSpeed() const;

		void SetNumInit(int num);
		void SetNum(int num);
		int GetNum() const;

	protected:
		virtual bool Update(double applicationTime, double elapsedTime);
		virtual void _Update(double applicationTime, double elapsedTime);

		float mSpeed;
		int mNum;
	};

#include "PX2UIScoreTextCtrl.inl"

	PX2_REGISTER_STREAM(UIScoreStaticTextCtrl);
	typedef Pointer0<UIScoreStaticTextCtrl> UIScoreStaticTextCtrlPtr;

}

#endif