// PX2InterpCurveRotateCtrl.hpp

#ifndef PX2INTERPCURVEROTATECTRL_HPP
#define PX2INTERPCURVEROTATECTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveRotateController : public InterpCurveFloat3Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_NEW(InterpCurveRotateController);
		PX2_DECLARE_STREAM(InterpCurveRotateController);

	public:
		InterpCurveRotateController ();
		virtual ~InterpCurveRotateController ();

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
	};

	PX2_REGISTER_STREAM(InterpCurveRotateController);
	typedef Pointer0<InterpCurveRotateController> InterpCurveRotateControllerPtr;

}

#endif