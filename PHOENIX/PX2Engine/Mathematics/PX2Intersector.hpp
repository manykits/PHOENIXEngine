// PX2Intersector.hpp

#ifndef PX2INTERSECTOR_HPP
#define PX2INTERSECTOR_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector2.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	template <typename Real, typename TVector>
	class PX2_ENGINE_ITEM Intersector
	{
	public:
		virtual ~Intersector ();

		/// 静态检测是否相交
		/** 
		* 缺省实现返回'false'。
		*/
		virtual bool Test ();

		/// 静态产生相交集
		/** 
		* 产生一个相交集。派生类对获得相交集负责，因为相交检测的物体产生相交集。
		*/
		virtual bool Find ();

		/// 动态检测是否相交
		/** 缺省实现返回'false'。*/
		virtual bool Test (Real tmax, const TVector& velocity0,
			const TVector& velocity1);

		/// 动态产生相交集
		/** 派生类实现此函数，提供各自相交集的实现方法。*/
		virtual bool Find (Real tmax, const TVector& velocity0,
			const TVector& velocity1);

		/// 返回动态相交检测的物体第一次相交的时间（在动态相交检测中）
		Real GetContactTime () const;

		/// 相交集类型信息
		enum
		{
			IT_EMPTY,
			IT_POINT,
			IT_SEGMENT,
			IT_RAY,
			IT_LINE,
			IT_POLYGON,
			IT_PLANE,
			IT_POLYHEDRON,
			IT_OTHER
		};
		int GetIntersectionType () const;

	protected:
		Intersector ();

		int mIntersectionType;
		Real mContactTime;
	};

	typedef Intersector<float, Vector2<float> > Intersector2f;
	typedef Intersector<float, Vector3<float> > Intersector3f;
	typedef Intersector<double, Vector2<double> > Intersector2d;
	typedef Intersector<double, Vector3<double> > Intersector3d;

}

#endif