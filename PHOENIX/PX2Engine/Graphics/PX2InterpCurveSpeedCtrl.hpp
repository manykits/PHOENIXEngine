// PX2InterpCurveSpeedCtrl.hpp

#ifndef PX2INTERPCURVESPEEDCTRL_HPP
#define PX2INTERPCURVESPEEDCTRL_HPP

#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveSpeedController : public InterpCurveFloat3Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(InterpCurveSpeedController);

	public:
		InterpCurveSpeedController ();
		virtual ~InterpCurveSpeedController ();

		virtual void Reset ();

		void SetMaxLength (float length);
		float GetMaxLength ();

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update (double applicationTime, double elapsedTime);

		APoint mInitPos;
		float mMaxLength;
		float mAllLength;
	};

#include "PX2InterpCurveSpeedCtrl.inl"

	PX2_REGISTER_STREAM(InterpCurveSpeedController);
	typedef Pointer0<InterpCurveSpeedController> InterpCurveSpeedControllerPtr;

}

#endif