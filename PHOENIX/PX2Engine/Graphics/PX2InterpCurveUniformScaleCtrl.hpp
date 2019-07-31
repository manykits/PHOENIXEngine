// PX2InterpCurveUniformScaleCtrl.hpp

#ifndef PX2INTERPCURVEUNIFORMSCALESCTRL_HPP
#define PX2INTERPCURVEUNIFORMSCALESCTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloatController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveUniformScaleController : public InterpCurveFloatController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_NEW(InterpCurveUniformScaleController);
		PX2_DECLARE_STREAM(InterpCurveUniformScaleController);

	public:
		InterpCurveUniformScaleController ();
		virtual ~InterpCurveUniformScaleController ();

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
	};

	PX2_REGISTER_STREAM(InterpCurveUniformScaleController);
	typedef Pointer0<InterpCurveUniformScaleController> InterpCurveUniformScaleControllerPtr;

}

#endif