// PX2Line3.hpp

#ifndef PX2LINE3_HPP
#define PX2LINE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 三元模板线
	/** 
	* 线的表示形式为P+t*D，P是线的原点，D是单位长度的方向向量。用户必须确保
	* 向量满足这些条件。
	*/
	template <typename Real>
	class Line3
	{
	public:
		Line3 ();  //< 未初始化
		~Line3 ();

		Line3 (const Vector3<Real>& origin, const Vector3<Real>& direction);

		Vector3<Real> Origin, Direction;
	};

#include "PX2Line3.inl"

	typedef Line3<float> Line3f;
	typedef Line3<double> Line3d;

}

#endif
