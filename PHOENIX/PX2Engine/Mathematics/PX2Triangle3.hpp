// PX2Triangle3.hpp

#ifndef PX2TRIANGLE3_HPP
#define PX2TRIANGLE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 三维模板三角形类
	/** 
	* 三角形由一个顶点数组代表，V0,V1,V2。
	*/
	template <typename Real>
	class Triangle3
	{
	public:
		Triangle3 ();  //< 未初始化
		~Triangle3 ();

		Triangle3 (const Vector3<Real>& v0, const Vector3<Real>& v1,
			const Vector3<Real>& v2);

		Triangle3 (const Vector3<Real> vertex[3]);

		/// 点Q到三角形的距离
		Real DistanceTo (const Vector3<Real>& q) const;

		Vector3<Real> V[3];
	};

#include "PX2Triangle3.inl"

	typedef Triangle3<float> Triangle3f;
	typedef Triangle3<double> Triangle3d;

}

#endif