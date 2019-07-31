// PX2InterpCurveTransCtrl.hpp

#ifndef PX2INTERPCURVESCALESCTRL_HPP
#define PX2INTERPCURVESCALESCTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveScaleController : public InterpCurveFloat3Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(InterpCurveScaleController);

	public:
		InterpCurveScaleController ();
		virtual ~InterpCurveScaleController ();

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
	};

	PX2_REGISTER_STREAM(InterpCurveScaleController);
	typedef Pointer0<InterpCurveScaleController> InterpCurveScaleControllerPtr;

}

#endif