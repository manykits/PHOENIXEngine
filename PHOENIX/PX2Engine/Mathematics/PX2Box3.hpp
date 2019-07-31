// PX2Box3.hpp

#ifndef PX2BOX3_HPP
#define PX2BOX3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 三元模板盒子类
	/** 
	* 盒子有一个中心C，轴向U[0]，U[1]和U[2]（所有的都是单位向量），e[0]，e[1]
	* ，e[2]（都非负）是各个方向的扩展。点X =  C+y[0]*U[0]+y[1]*U[1]+y[2]*U[2]
	* 在盒子内或者面上|y[i]| <= e[i]。
	*/
	template <typename Real>
	class Box3
	{
	public:
		Box3 ();  //< 未初始化
		~Box3 ();

		Box3 (const Vector3<Real>& center, const Vector3<Real> axis[3],
			const Real extent[3]);

		Box3 (const Vector3<Real>& center, const Vector3<Real>& axis0,
			const Vector3<Real>& axis1, const Vector3<Real>& axis2,
			const Real extent0, const Real extent1, const Real extent2);

		void ComputeVertices (Vector3<Real> vertex[8]) const;

		Vector3<Real> Center;
		Vector3<Real> Axis[3];
		Real Extent[3];
	};

#include "PX2Box3.inl"

	typedef Box3<float> Box3f;
	typedef Box3<double> Box3d;

}

#endif