// PX2InterpCurveAlphaCtrl.hpp

#ifndef PX2INTERPCURVEALPHACTRL_HPP
#define PX2INTERPCURVEALPHACTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloatController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveAlphaController : public InterpCurveFloatController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(InterpCurveAlphaController);

	public:
		InterpCurveAlphaController ();
		virtual ~InterpCurveAlphaController ();

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update (double applicationTime, double elapsedTime);
	};

	PX2_REGISTER_STREAM(InterpCurveAlphaController);
	typedef Pointer0<InterpCurveAlphaController> InterpCurveAlphaControllerPtr;

}

#endif