// PX2InterpCurve.hpp

#ifndef PX2INTERPCURVE_HPP
#define PX2INTERPCURVE_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Math.hpp"
#include "PX2Float1.hpp"
#include "PX2Float2.hpp"
#include "PX2Float3.hpp"
#include "PX2Float4.hpp"
#include "PX2AVector.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	enum InterpCurveMode
	{
		ICM_LINEAR,
		ICM_CONSTANT,
		ICM_CURVE_AUTO,
		ICM_CURVE_AUTOCLAMPED,
		ICM_CURVE_USER,
		ICM_CURVE_BREAK,
		ICM_MAX_MODE
	};
	std::string ICMToString (InterpCurveMode mode);
	InterpCurveMode StringToICM (std::string str);

	template <class Real>
	class InterpCurvePoint
	{
	public:
		float InVal;
		Real OutVal;
		Real ArriveTangent;
		Real LeaveTangent;
		InterpCurveMode InterpMode;

		InterpCurvePoint ();
		InterpCurvePoint (const float inVal, const Real &outVal);
		InterpCurvePoint (const float inVal, const Real &outVal,
			const Real &inArriveTangent, const Real &inLeaveTangent,
			InterpCurveMode interpMode);
		bool IsCurveKey () const;
		int GetStreamSize () const;
	};

	enum InterpMethodType
	{
		IMT_FIXED_TANGENT_EVAL_NEWAUTOTANGENTS,
		IMT_FIXED_TANGENT_EVAL,
		IMT_BROKEN_TANGENT_EVAL,
		IMT_MAX_TYPE
	};

	template <class Real>
	class InterpCurve
	{
	public:
		int AddPoint (const float inVal, const Real &outVal);
		int AddPoint (const float inVal, const Real &outVal,
			const Real &arriveTangent, const Real &leaveTangent, 
			InterpCurveMode interpMode);
		int MovePoint (int i, float newInVal);
		void RemovePoint (int i);
		int GetNumPoints () const;
		void Reset ();
		Real Eval (const float inVal, const Real &defVal, int *ptIndex=0) const;
		Real EvalDerivative (const float inVal, const Real &defVal,
			int *ptIndex=0) const;
		Real EvalSecondDerivative (const float inVal, const Real &defVal,
			int *ptIndex=0) const;
		void AutoSetTangents (float tension = 0.0f);
		bool IsUsingLegacyInterpMethod() const;
		void UpgradeInterpMethod();
		int GetStreamSize () const;

	public:
		std::vector<InterpCurvePoint<Real> > Points;
		InterpMethodType InterpMethod;
	};

	typedef InterpCurvePoint<float> InterpCurvePointFloat;
	typedef InterpCurvePoint<double> InterpCurvePointDouble;
	typedef InterpCurvePoint<Float1> InterpCurvePointFloat1;
	typedef InterpCurvePoint<Float2> InterpCurvePointFloat2;
	typedef InterpCurvePoint<Float3> InterpCurvePointFloat3;
	typedef InterpCurvePoint<Float4> InterpCurvePointFloat4;
	typedef InterpCurvePoint<AVector> InterpCurvePointAVector;
	typedef InterpCurvePoint<APoint> InterpCurvePointAPoint;

	typedef InterpCurve<float> InterpCurveFloat;
	typedef InterpCurve<double> InterpCurveDouble;
	typedef InterpCurve<Float1> InterpCurveFloat1;
	typedef InterpCurve<Float2> InterpCurveFloat2;
	typedef InterpCurve<Float3> InterpCurveFloat3;
	typedef InterpCurve<Float4> InterpCurveFloat4;
	typedef InterpCurve<AVector> InterpCurveAVector;
	typedef InterpCurve<APoint> InterpCurveAPoint;

#include "PX2InterpCurve.inl"

}

#endif