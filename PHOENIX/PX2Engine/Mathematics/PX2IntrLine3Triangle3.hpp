// PX2IntrLine3Triangle3.hpp

#ifndef PX2INTRLINE3TRIANGLE3_HPP
#define PX2INTRLINE3TRIANGLE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Intersector.hpp"
#include "PX2Line3.hpp"
#include "PX2Triangle3.hpp"

namespace PX2
{

	template <typename Real>
	class PX2_ENGINE_ITEM IntrLine3Triangle3
		: public Intersector<Real,Vector3<Real> >
	{
	public:
		IntrLine3Triangle3 (const Line3<Real>& line,
			const Triangle3<Real>& triangle);

		// 物体访问
		const Line3<Real>& GetLine () const;
		const Triangle3<Real>& GetTriangle () const;

		// 检测查询
		virtual bool Test ();

		/// 寻找相交队列。
		/** 
		* 相交的点为P = origin + t*direction = b0*V0 + b1*V1 + b2*V2。
		*/
		virtual bool Find ();
		Real GetLineParameter () const;
		Real GetTriBary0 () const;
		Real GetTriBary1 () const;
		Real GetTriBary2 () const;

	private:
		using Intersector<Real,Vector3<Real> >::IT_EMPTY;
		using Intersector<Real,Vector3<Real> >::IT_POINT;
		using Intersector<Real,Vector3<Real> >::mIntersectionType;

		// 用来做检测的对象
		const Line3<Real>* mLine;
		const Triangle3<Real>* mTriangle;

		//相交集信息
		Real mLineParameter, mTriBary0, mTriBary1, mTriBary2;
	};

	typedef IntrLine3Triangle3<float> IntrLine3Triangle3f;
	typedef IntrLine3Triangle3<double> IntrLine3Triangle3d;

}

#endif