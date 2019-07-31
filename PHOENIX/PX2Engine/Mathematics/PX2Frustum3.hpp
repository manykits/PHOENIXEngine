// PX2Frustum3.hpp

#ifndef PX2FRUSTUM3_HPP
#define PX2FRUSTUM3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 正交截头体模板类
	/** 
	* E是相机的原点，D是方向向量，U是上向量，R是右向量。让u( > 0 )，r（ > 0）作
	* 为U，R方向的扩展。让n和f作为D方向的扩展0 < n < f。截头体近裁剪面的四个角
	* 为E + n*D + s0*u*U + s1*r*R（|s0| = |s1| = 1）。截头体远裁剪面的四个角为
	* E + f*D + (f/n)*(s0*u*U + s1*r*R)（|s0| = |s1| = 1）。
	*/
	template <typename Real>
	class Frustum3
	{
	public:
		Frustum3 ();  //< 未初始化
		~Frustum3 ();

		Frustum3 (const Vector3<Real>& origin, const Vector3<Real>& dvector,
			const Vector3<Real>& uvector, const Vector3<Real>& rvector,
			Real dmin, Real dmax, Real ubound, Real rbound);

		/**
		* 当DMin, DMac, UBound, 或者 RBound发生任何变化时，Update()必须调用。
		* DRatio，MTwoUF和MTwoRF随着这些改变，所以在调用Update()后才调用
		* Get*()。
		*/
		void Update ();
		inline Real GetDRatio () const;
		inline Real GetMTwoUF () const;
		inline Real GetMTwoRF () const;

		void ComputeVertices (Vector3<Real> vertex[8]) const;

		Vector3<Real> Origin, DVector, UVector, RVector;
		Real DMin, DMax, UBound, RBound;

	protected:
		Real mDRatio, mMTwoUF, mMTwoRF;
	};

#include "PX2Frustum3.inl"

	typedef Frustum3<float> Frustum3f;
	typedef Frustum3<double> Frustum3d;

}

#endif