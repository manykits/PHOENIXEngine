// PX2Circle3.hpp

#ifndef PX2CIRCLE3_HPP
#define PX2CIRCLE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 3d圆类
	/**
	* 包含圆的平面为Dot(N,X-C) = 0，X为平面上的一点。U，V，N是右手坐标系下的正
	* 交坐标系。在这个平面上的圆由函数X = C + R*(cos(t)*U + sin(t)*V)表示，t的
	* 范围为[-pi,pi)。
	*/
	template <typename Real>
	class Circle3
	{
	public:
		Circle3 ();  //< 未初始化
		~Circle3 ();

		/**
		* U = direction0，V = direction1，N = normal
		*/
		Circle3 (const Vector3<Real>& center, const Vector3<Real>& direction0,
			const Vector3<Real>& direction1, const Vector3<Real>& normal,
			const Real radius);

		Vector3<Real> Center, Direction0, Direction1, Normal;
		Real Radius;
	};

#include "PX2Circle3.inl"

	typedef Circle3<float> Circle3f;
	typedef Circle3<double> Circle3d;

}

#endif