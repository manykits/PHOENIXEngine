// PX2InterpCurveFloatController.hpp

#ifndef PX2INTERPCURVEFLOATCONTROLLER_HPP
#define PX2INTERPCURVEFLOATCONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveFloatController : public InterpCurveController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(InterpCurveFloatController);

	public:
		InterpCurveFloatController (float initValue=0.0f);
		virtual ~InterpCurveFloatController ();

		void SetInitValue (float val);
		const float &GetInitValue () const;

		const float &GetCurValueRaw () const;
		const float &GetCurValueRelatived () const;

		void Clear();
		void AddPoint(float inVal, float outVal, InterpCurveMode mode);

	public:
		InterpCurveFloat mValues;

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);

		float mInitValue;
		float mCurValueRaw; // 不处理Relative关系的值
		float mCurValueRelatived; // Update更新得到的当前值,不需要保存
	};

#include "PX2InterpCurveFloatController.inl"

	PX2_REGISTER_STREAM(InterpCurveFloatController);
	typedef Pointer0<InterpCurveFloatController> InterpCurveFloatControllerPtr;

}

#endif