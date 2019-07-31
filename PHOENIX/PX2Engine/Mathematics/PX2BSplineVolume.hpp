// PX2BSplineVolume.hpp

#ifndef PX2BSPLINEVOLUME_HPP
#define PX2BSPLINEVOLUME_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2BSplineBasis.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	template <typename Real>
	class PX2_ENGINE_ITEM BSplineVolume
	{
	public:
		// Construction and destruction of an open uniform B-spline volume.  The
		// class will allocate space for the control points.  The caller is
		// responsible for setting the values with the member function
		// ControlPoint.

		BSplineVolume (int numUCtrlPoints, int numVCtrlPoints,
			int numWCtrlPoints, int uDegree, int vDegree, int wDegree);

		~BSplineVolume ();

		int GetNumCtrlPoints (int dim) const;
		int GetDegree (int dim) const;

		// Control points may be changed at any time.  If any input index is
		// invalid, the returned point is a vector whose components are all
		// MAX_REAL.
		void SetControlPoint (int uIndex, int vIndex, int wIndex,
			const Vector3<Real>& ctrlPoint);
		Vector3<Real> GetControlPoint (int uIndex, int vIndex, int wIndex) const;

		// The spline is defined for 0 <= u <= 1, 0 <= v <= 1, and 0 <= w <= 1.
		// The input values should be in this domain.  Any inputs smaller than 0
		// are clamped to 0.  Any inputs larger than 1 are clamped to 1.
		Vector3<Real> GetPosition (Real u, Real v, Real w) const;
		Vector3<Real> GetDerivativeU (Real u, Real v, Real w) const;
		Vector3<Real> GetDerivativeV (Real u, Real v, Real w) const;
		Vector3<Real> GetDerivativeW (Real u, Real v, Real w) const;

		// for array indexing:  i = 0 for u, i = 1 for v, i = 2 for w
		Vector3<Real> GetPosition (Real pos[3]) const;
		Vector3<Real> GetDerivative (int i, Real pos[3]) const;

	private:
		Vector3<Real>*** mCtrlPoint;  // ctrl[unum][vnum][wnum]
		BSplineBasis<Real> mBasis[3];
	};

	typedef BSplineVolume<float> BSplineVolumef;
	typedef BSplineVolume<double> BSplineVolumed;

}

#endif
