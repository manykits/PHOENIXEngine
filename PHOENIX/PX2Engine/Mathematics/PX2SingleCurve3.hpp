// PX2SingleCurve3.hpp

#ifndef PX2SINGLECURVE3_HPP
#define PX2SINGLECURVE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Curve3.hpp"

namespace PX2
{

	template <typename Real>
	class PX2_ENGINE_ITEM SingleCurve3 : public Curve3<Real>
	{
	public:
		// ³éÏó»ùÀà
		SingleCurve3 (Real tmin, Real tmax);

		// Length-from-time and time-from-length.
		virtual Real GetLength (Real t0, Real t1) const;
		virtual Real GetTime (Real length, int iterations = 32,
			Real tolerance = (Real)1e-06) const;

	protected:
		using Curve3<Real>::mTMin;
		using Curve3<Real>::mTMax;
		using Curve3<Real>::GetSpeed;
		using Curve3<Real>::GetTotalLength;

		static Real GetSpeedWithData (Real t, void* data);
	};

	typedef SingleCurve3<float> SingleCurve3f;
	typedef SingleCurve3<double> SingleCurve3d;

}

#endif
