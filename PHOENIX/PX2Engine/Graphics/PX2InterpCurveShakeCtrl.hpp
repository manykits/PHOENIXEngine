// PX2InterpCurveShakeCtrl.hpp

#ifndef PX2INTERPCURVESHAKECTRL_HPP
#define PX2INTERPCURVESHAKECTRL_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloatController.hpp"

namespace PX2
{

	// 一个简单的震动控件,绑定时记录控制体方位,接触绑定时还原控制体方位
	// 通常控制曲线的第一个和最后一个点的大小都为0
	class PX2_ENGINE_ITEM InterpCurveShakeController : public InterpCurveFloatController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(InterpCurveShakeController);

	public:
		InterpCurveShakeController ();
		virtual ~InterpCurveShakeController ();

		enum ShakeRangeType
		{
			SRT_X,
			SRT_Y,
			SRT_Z,
			SRT_XY,
			SRT_YZ,
			SRT_XZ,
			SRT_XYZ,
			SRT_MAX_TYPE
		};
		void SetShakeRangeType (ShakeRangeType type);
		ShakeRangeType GetShakeRangeType ();

		virtual bool Update (double applicationTime, double elapsedTime);

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
		void ReCalcCurve ();

		ShakeRangeType mShakeRangeType;
		APoint mInitPos;
		InterpCurveFloat3 mPoses;
	};

#include "PX2InterpCurveShakeCtrl.inl"

	PX2_REGISTER_STREAM(InterpCurveShakeController);
	typedef Pointer0<InterpCurveShakeController> InterpCurveShakeControllerPtr;

}

#endif