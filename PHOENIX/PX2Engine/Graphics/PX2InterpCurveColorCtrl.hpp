// PX2InterpCurveColorCtrl.hpp

#ifndef PX2INTERPCURVECOLORCTRL_HPP
#define PX2INTERPCURVECOLORCTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveColorController : public InterpCurveFloat3Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(InterpCurveColorController);

	public:
		InterpCurveColorController();
		virtual ~InterpCurveColorController();

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
	};

	PX2_REGISTER_STREAM(InterpCurveColorController);
	typedef Pointer0<InterpCurveColorController> InterpCurveColorControllerPtr;

}

#endif