// PX2InterpCurveAnchorParamCtrl.hpp

#ifndef PX2INTERPCURVEANCHORPARAMCTRL_HPP
#define PX2INTERPCURVEANCHORPARAMCTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveAnchorParamCtrl : public InterpCurveFloat3Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_NEW(InterpCurveAnchorParamCtrl);
		PX2_DECLARE_STREAM(InterpCurveAnchorParamCtrl);

	public:
		InterpCurveAnchorParamCtrl(bool isHor = true);
		virtual ~InterpCurveAnchorParamCtrl();

		void SetHor(bool isHor);
		bool IsHor() const;

		void AddPoint(float inVal, float anchorParam0, float anchorParam1, 
			InterpCurveMode mode = ICM_LINEAR);

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);

		bool mIsHor;
	};

	PX2_REGISTER_STREAM(InterpCurveAnchorParamCtrl);
	typedef Pointer0<InterpCurveAnchorParamCtrl> InterpCurveAnchorParamCtrlPtr;

}

#endif