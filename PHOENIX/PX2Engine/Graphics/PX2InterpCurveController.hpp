// PX2InterpCurveController.hpp

#ifndef PX2INTERPCURVECONTROLLER_HPP
#define PX2INTERPCURVECONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurve.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(InterpCurveController);

	public:
		InterpCurveController ();
		virtual ~InterpCurveController ();

		// attach detach
		void SetAttachUpdateInit (bool doUpdate);
		bool IsAttachUpdateInit () const;

		void SetDetachResetInit (bool doReset);
		bool IsDetachResetInit () const;

		// other options
		void SetRelatively (bool r);
		bool IsRelatively () const;

	protected:		
		bool mIsRelatively;
		bool mIsDoAttachUpdate;
		bool mIsDoDetachReset;
	};

#include "PX2InterpCurveController.inl"

	PX2_REGISTER_STREAM(InterpCurveController);
	typedef Pointer0<InterpCurveController> InterpCurveControllerPtr;

}

#endif