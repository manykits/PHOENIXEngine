// PX2Curve3.hpp

#ifndef PX2CURVE3_HPP
#define PX2CURVE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 样条曲线抽象基类
	template <typename Real>
	class PX2_ENGINE_ITEM Curve3
	{
	public:
		Curve3 (Real tmin, Real tmax);
		virtual ~Curve3 ();

		// Interval on which curve parameter is defined.  If you are interested
		// in only a subinterval of the actual domain of the curve, you may set
		// that subinterval with SetTimeInterval.  This function requires that
		// tmin < tmax.
		Real GetMinTime () const;
		Real GetMaxTime () const;
		void SetTimeInterval (Real tmin, Real tmax);

		// 位置和倒数
		virtual Vector3<Real> GetPosition (Real t) const = 0;
		virtual Vector3<Real> GetFirstDerivative (Real t) const = 0;
		virtual Vector3<Real> GetSecondDerivative (Real t) const = 0;
		virtual Vector3<Real> GetThirdDerivative (Real t) const = 0;

		// Differential geometric quantities.
		Real GetSpeed (Real t) const;
		virtual Real GetLength (Real t0, Real t1) const = 0;
		Real GetTotalLength () const;
		Vector3<Real> GetTangent (Real t) const;
		Vector3<Real> GetNormal (Real t) const;
		Vector3<Real> GetBinormal (Real t) const;
		void GetFrame (Real t, Vector3<Real>& position, Vector3<Real>& tangent,
			Vector3<Real>& normal, Vector3<Real>& binormal) const;
		Real GetCurvature (Real t) const;
		Real GetTorsion (Real t) const;

		// Inverse mapping of s = Length(t) given by t = Length^{-1}(s).
		virtual Real GetTime (Real length, int iterations = 32,
			Real tolerance = (Real)1e-06) const = 0;

		// 细分
		void SubdivideByTime (int numPoints, Vector3<Real>*& points) const;
		void SubdivideByLength (int numPoints, Vector3<Real>*& points) const;

	protected:
		// Curve parameter is t where tmin <= t <= tmax.
		Real mTMin, mTMax;
	};

	typedef Curve3<float> Curve3f;
	typedef Curve3<double> Curve3d;

}

#endif