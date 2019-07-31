// PX2Ray3.hpp

#ifndef PX2RAY3_HPP
#define PX2RAY3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 射线类
	/**
	* 射线的表现形式为P+t*D，P为射线的原点，D为单位长度的方向向量，并且t>=0。用
	* 户必须确保D为单位长度。
	*/
	template <typename Real>
	class Ray3
	{
	public:
		Ray3 ();  //< 未初始化
		~Ray3 ();

		Ray3 (const Vector3<Real>& origin, const Vector3<Real>& direction);

		Vector3<Real> Origin, Direction;
	};

#include "PX2Ray3.inl"

	typedef Ray3<float> Ray3f;
	typedef Ray3<double> Ray3d;

}

#endif