// PX2BSplineBasis.hpp

#ifndef PX2BSPLINEBASIS_HPP
#define PX2BSPLINEBASIS_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Math.hpp"

namespace PX2
{

	/// B样条曲线模板基类
	/**
	* 样条控制点数量为n+1，控制点满足0<=i<=n。样条的节点数组有n+d+2个元素。
	*/
	template <typename Real>
	class PX2_ENGINE_ITEM BSplineBasis
	{
	public:
		// Default constructor.  The number of control points is n+1 and the
		// indices i for the control points satisfy 0 <= i <= n.  The degree of
		// the curve is d.  The knot array has n+d+2 elements.  Whether uniform
		// or nonuniform knots, it is required that
		//   knot[i] = 0, 0 <= i <= d
		//   knot[i] = 1, n+1 <= i <= n+d+1
		// BSplineBasis enforces these conditions by not exposing SetKnot for the
		// relevant values of i.
		/**
		* 
		*/
		BSplineBasis ();

		// Open uniform or periodic uniform.  The knot array is internally
		// generated with equally spaced elements.  It is required that
		//   knot[i] = (i-d)/(n+1-d), d+1 <= i <= n
		// BSplineBasis enforces these conditions by not exposing SetKnot for the
		// relevant values of i.  GetKnot(j) will return knot[i] for i = j+d+1.
		BSplineBasis (int numCtrlPoints, int degree, bool open);
		void Create (int numCtrlPoints, int degree, bool open);

		// Open nonuniform.  The interiorKnot array must have n-d nondecreasing
		// elements in the interval [0,1].  The values are
		//   knot[i] = interiorKnot[j]
		// with 0 <= j <= n-d-1 and i = j+d+1, so d+1 <= i <= n.  The caller is
		// responsible for interiorKnot if it was dynamically allocated.  An
		// internal copy is made, so to dynamically change knots you must use
		// the SetKnot(j,*) function.
		BSplineBasis (int numCtrlPoints, int degree, const Real* interiorKnot);
		void Create (int numCtrlPoints, int degree, const Real* interiorKnot);

		virtual ~BSplineBasis ();

		int GetNumCtrlPoints () const;
		int GetDegree () const;
		bool IsOpen () const;
		bool IsUniform () const;

		// For a nonuniform spline, the knot[i] are modified by SetKnot(j,value)
		// for j = i+d+1.  That is, you specify j with 0 <= j <= n-d-1, i = j+d+1,
		// and knot[i] = value.  SetKnot(j,value) does nothing for indices outside
		// the j-range or for uniform splines.  GetKnot(j) returns knot[i]
		// regardless of whether the spline is uniform or nonuniform.
		void SetKnot (int j, Real knot);
		Real GetKnot (int j) const;

		// 导数
		Real GetD0 (int i) const;
		Real GetD1 (int i) const;
		Real GetD2 (int i) const;
		Real GetD3 (int i) const;

		// Evaluate basis functions and their derivatives.
		void Compute (Real t, unsigned int order, int& minIndex,
			int& maxIndex) const;

	protected:
		int Initialize (int numCtrlPoints, int degree, bool open);
		Real** Allocate () const;
		void Deallocate (Real** data);

		// Determine knot index i for which knot[i] <= rfTime < knot[i+1].
		int GetKey (Real& t) const;

		int mNumCtrlPoints;   // n+1
		int mDegree;          // d
		Real* mKnot;          // knot[n+d+2]
		bool mOpen, mUniform;

		// Storage for the basis functions and their derivatives first three
		// derivatives.  The basis array is always allocated by the constructor
		// calls.  A derivative basis array is allocated on the first call to a
		// derivative member function.
		Real** mBD0;          // bd0[d+1][n+d+1]
		mutable Real** mBD1;  // bd1[d+1][n+d+1]
		mutable Real** mBD2;  // bd2[d+1][n+d+1]
		mutable Real** mBD3;  // bd3[d+1][n+d+1]
	};

	typedef BSplineBasis<float> BSplineBasisf;
	typedef BSplineBasis<double> BSplineBasisd;

}

#endif
