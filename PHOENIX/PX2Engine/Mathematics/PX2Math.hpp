// PX2Math.hpp

#ifndef PX2MATH_HPP
#define PX2MATH_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Assert.hpp"
#include <cfloat>
#include <cmath>
#include <cstdlib>

namespace PX2
{

	template <typename Real>
	class Math
	{
	public:
		static Real ACos (Real value);
		static Real ASin (Real value);
		static Real ATan (Real value);
		static Real ATan2 (Real y, Real x);
		static Real Ceil (Real value);
		static Real Cos (Real value);
		static Real Exp (Real value);
		static Real FAbs (Real value);
		static Real Floor (Real value);
		static Real FMod (Real x, Real y);
		static Real InvSqrt (Real value);
		static Real Log (Real value);
		static Real Log2 (Real value);
		static Real Log10 (Real value);
		static Real Pow (Real base, Real exponent);
		static Real Sin (Real value);
		static Real Sqr (Real value);
		static Real Sqrt (Real value);
		static Real Tan (Real value);
		static Real Max (Real a, Real b);
		static Real Min (Real a, Real b);

		//  如果输入是负值，返回-1；输入的是0，返回0；如果输入的为正值，返回+1。
		static Real Sign (Real value);

		/// 产生一个在区间[0,1]的随机数。（输入的种子不要小于0）
		static Real UnitRandom (unsigned int seed = 0);

		/// 生成一个在区间[-1,1]的随机数。（输入的种子不要小于0）
		static Real SymmetricRandom (unsigned int seed = 0);

		/// 生成一个在区间[min,max]的随机数。（输入的种子不要小于0）
		static Real IntervalRandom (Real min, Real max, unsigned int seed = 0);

		/// 生成一个在区间[min, max)的随机数。
		static int IntRandom (int min, int max, unsigned int seed = 0);

		// 将输入的值夹取到[min,max].
		static Real Clamp (Real value, Real minValue, Real maxValue);

		// 将输入夹取到区间[0,1].
		static Real Saturate (Real value);

		// 常量
		PX2_ENGINE_ITEM static const Real EPSILON;
		PX2_ENGINE_ITEM static const Real ZERO_TOLERANCE;
		PX2_ENGINE_ITEM static const Real MAX_REAL;
		PX2_ENGINE_ITEM static const Real PI;
		PX2_ENGINE_ITEM static const Real TWO_PI;
		PX2_ENGINE_ITEM static const Real HALF_PI;
		PX2_ENGINE_ITEM static const Real INV_PI;
		PX2_ENGINE_ITEM static const Real INV_TWO_PI;
		PX2_ENGINE_ITEM static const Real DEG_TO_RAD;
		PX2_ENGINE_ITEM static const Real RAD_TO_DEG;
		PX2_ENGINE_ITEM static const Real LN_2;
		PX2_ENGINE_ITEM static const Real LN_10;
		PX2_ENGINE_ITEM static const Real INV_LN_2;
		PX2_ENGINE_ITEM static const Real INV_LN_10;
		PX2_ENGINE_ITEM static const Real SQRT_2;
		PX2_ENGINE_ITEM static const Real INV_SQRT_2;
		PX2_ENGINE_ITEM static const Real SQRT_3;
		PX2_ENGINE_ITEM static const Real INV_SQRT_3;
	};
	
	template <class Real, class U>
	Real Lerp ( const Real &a, const Real &b, const U &alpha)
	{
		return (Real)(a + (b-a)*alpha);
	}

	template <class Real, class U>
	Real CubicInterp ( const Real &p0, const Real &t0, const Real &p1, const Real &t1,
		const U &alpha)
	{
		const float A2 = alpha  * alpha;
		const float A3 = A2 * alpha;

		return (Real)(p0*((2*A3)-(3*A2)+1)) + t0*((A3-(2*A2)+alpha))
			+ (t1*(A3-A2)) + (p1*((-2*A3)+(3*A2)));
	}

	template <class Real, class U>
	Real CubicInterpDerivative (
		const Real &p0, const Real &t0, const Real &p1, const Real &t1,
		const U &alpha )
	{
		Real a = 6.f*p0 + 3.f*t0 + 3.f*t1 - 6.f*p1;
		Real b = -6.f*p0 - 4.f*t0 - 2.f*t1 + 6.f*p1;
		Real c = t0;

		const float A2 = alpha  * alpha;

		return (a * A2) + (b * alpha) + c;
	}

	template <class Real, class U> 
	Real CubicInterpSecondDerivative (
		const Real &p0, const Real &t0, const Real &p1, const Real &t1,
		const U &alpha)
	{
		Real a = 12.f*p0 + 6.f*t0 + 6.f*t1 - 12.f*p1;
		Real b = -6.f*p0 - 4.f*t0 - 2.f*t1 + 6.f*p1;

		return (a * alpha) + b;
	}

	template< class Real, class U > void LegacyAutoCalcTangent( 
		const Real& prevP, const Real& p, const Real& nextP,
		const U& tension, Real& outTan )
	{
		outTan = 0.5f * (1.f - tension) * ( (p - prevP) + (nextP - p) );
	}

	template <class Real, class U>
	void AutoCalcTangent (const Real &prevP, const Real &p, const Real &nextP,
		const U &tension, Real &outTan)
	{
		outTan = (1.0f - tension) * ((p - prevP) + (nextP - p));
	}

	template <class Real>
	void ComputeCurveTangent( float prevTime, const Real& prevPoint,
		float curTime, const Real& curPoint,
		float nextTime, const Real& nextPoint,
		float tension,
		bool wantClamping,
		Real& outTangent )
	{
		AutoCalcTangent( prevPoint, curPoint, nextPoint, tension, outTangent );

		float diff = (nextTime - prevTime);
		const float prevToNextTimeDiff = diff>0.0001f ? diff : 0.0001f;

		outTangent /= prevToNextTimeDiff;
	}

#include "PX2Math.inl"

	typedef Math<float> Mathf;
	typedef Math<double> Mathd;

}

#endif