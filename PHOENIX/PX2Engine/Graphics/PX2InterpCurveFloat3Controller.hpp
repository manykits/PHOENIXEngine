// PX2InterpCurveFloat3Controller.hpp

#ifndef PX2INTERPCURVEFLOAT3CONTROLLER_HPP
#define PX2INTERPCURVEFLOAT3CONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM InterpCurveFloat3Controller : public InterpCurveController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(InterpCurveFloat3Controller);

	public:
		InterpCurveFloat3Controller (const Float3 &initValue=Float3::ZERO);
		virtual ~InterpCurveFloat3Controller ();

		void SetInitValue (const Float3 &val);
		const Float3 &GetInitValue () const;

		const Float3 &GetCurValueRaw () const;
		const Float3 &GetCurValueRelatived () const;

		void Clear();
		void AddPoint(float inVal, const Float3 &outVal, InterpCurveMode mode);
		void AddPoint(float inVal, const AVector &outVal, InterpCurveMode mode);

	public:
		InterpCurveFloat3 mValues;

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);

		Float3 mInitValue;
		Float3 mCurValueRaw; // 不处理Relative关系的值
		Float3 mCurValueRelatived; // Update更新得到的当前值,不需要保存
	};

#include "PX2InterpCurveFloat3Controller.inl"

	PX2_REGISTER_STREAM(InterpCurveFloat3Controller);
	typedef Pointer0<InterpCurveFloat3Controller> InterpCurveFloat3ControllerPtr;

}

#endif