// PX2Bound.hpp

#ifndef PX2BOUND_HPP
#define PX2BOUND_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2HPlane.hpp"
#include "PX2Transform.hpp"

namespace PX2
{
	
	/// 包围体类
	/**
	* 每个场景对象，节点都拥有一个，自身包围体和世界包围体，世界包围体是通过自
	* 身包围体变换过来或者直接设置。包围体只要用来做碰撞检测，相机裁剪等。该类
	* 实现的包围体是一个包围球，因为球通常来说是最快的。
	*/
	class PX2_ENGINE_ITEM Bound
	{
	public:
		Bound ();
		~Bound ();

		// 赋值
		Bound& operator= (const Bound& bound);

		// 任何必须定义中心和半径
		inline void SetCenter (const APoint& center);
		inline void SetRadius (float radius);
		inline const APoint& GetCenter () const;
		inline float GetRadius () const;

		// 包围体操作
		int WhichSide (const HPlane& plane) const;
		void GrowToContain (const Bound& bound);
		void TransformBy (const Transform& transform, Bound& bound);
		void ComputeFromData (int numElements, int stride, const char* data);

		/// 线和包围体碰撞检测
		/**
		* 先的表现形式为P + t*D，P是原点，D是单位长度的方向向量。区间[tmin,tmax]
		* 取值含义：
		*	line:     tmin = -Mathf::MAX_REAL, tmax = Mathf::MAX_REAL
		*	ray:      tmin = 0.0f, tmax = Mathf::MAX_REAL
		*	segment:  tmin >= 0.0f, tmax > tmin
		*/
		bool TestIntersection (const APoint& origin, const AVector& direction,
			float tmin, float tmax) const;

		/// 两个静态包围盒碰撞检测
		bool TestIntersection (const Bound& bound) const;

		/// 两个移动包围盒的相交检测
		/**
		* Velocity0是调用的包围盒的速度，velocity1是输入的包围盒的速度。
		*/
		bool TestIntersection (const Bound& bound, float tmax,
			const AVector& velocity0, const AVector& velocity1) const;

		// 持久化支持
		inline static int GetStreamingSize ();

	private:
		APoint mCenter;
		float mRadius;
	};

#include "PX2Bound.inl"

}

#endif
