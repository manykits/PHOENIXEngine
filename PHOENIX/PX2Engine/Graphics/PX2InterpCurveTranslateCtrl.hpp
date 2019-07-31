// PX2InterpCurveTransCtrl.hpp

#ifndef PX2INTERPCURVETRANSLATECTRL_HPP
#define PX2INTERPCURVETRANSLATECTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveTranslateController : public InterpCurveFloat3Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(InterpCurveTranslateController);

	public:
		InterpCurveTranslateController ();
		virtual ~InterpCurveTranslateController ();

		void SetWorld(bool world);
		bool IsWorld() const;

		virtual void Reset ();

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update (double applicationTime, double elapsedTime);

		bool mIsWorld;
	};

	PX2_REGISTER_STREAM(InterpCurveTranslateController);
	typedef Pointer0<InterpCurveTranslateController> InterpCurveTranslateControllerPtr;

}

#endif