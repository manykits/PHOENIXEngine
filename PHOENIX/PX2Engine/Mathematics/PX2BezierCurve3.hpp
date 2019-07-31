// PX2BezierCurve3.hpp

#ifndef PX2BEZIERCURVE3_HPP
#define PX2BEZIERCURVE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2SingleCurve3.hpp"

namespace PX2
{

	template <typename Real>
	class PX2_ENGINE_ITEM BezierCurve3 : public SingleCurve3<Real>
	{
	public:
		// Construction and destruction.  BezierCurve3 accepts responsibility for
		// deleting the input arrays.
		BezierCurve3 (int degree, Vector3<Real>* ctrlPoint);
		virtual ~BezierCurve3 ();

		int GetDegree () const;
		const Vector3<Real>* GetControlPoints () const;

		virtual Vector3<Real> GetPosition (Real t) const;
		virtual Vector3<Real> GetFirstDerivative (Real t) const;
		virtual Vector3<Real> GetSecondDerivative (Real t) const;
		virtual Vector3<Real> GetThirdDerivative (Real t) const;

	protected:
		int mDegree;
		int mNumCtrlPoints;
		Vector3<Real>* mCtrlPoint;
		Vector3<Real>* mDer1CtrlPoint;
		Vector3<Real>* mDer2CtrlPoint;
		Vector3<Real>* mDer3CtrlPoint;
		Real** mChoose;
	};

	typedef BezierCurve3<float> BezierCurve3f;
	typedef BezierCurve3<double> BezierCurve3d;

}

#endif
