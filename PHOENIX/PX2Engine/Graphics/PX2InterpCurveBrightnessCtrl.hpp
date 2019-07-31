// PX2InterpCurveBrightnessCtrl.hpp

#ifndef PX2INTERPCURVEBRIGHTNESSCTRL_HPP
#define PX2INTERPCURVEBRIGHTNESSCTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloatController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveBrightnessController : public InterpCurveFloatController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(InterpCurveBrightnessController);

	public:
		InterpCurveBrightnessController();
		virtual ~InterpCurveBrightnessController();

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
	};

	PX2_REGISTER_STREAM(InterpCurveBrightnessController);
	typedef Pointer0<InterpCurveBrightnessController> InterpCurveBrightnessControllerPtr;

}

#endif