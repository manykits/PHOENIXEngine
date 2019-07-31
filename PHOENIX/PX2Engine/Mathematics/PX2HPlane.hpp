// PX2HPlane.hpp

#ifndef PX2HPLANE_HPP
#define PX2HPLANE_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	/// 齐次平面类
	/**
	* homogeneous plane，平面的表现形式为Dot(N,X) - C = 0, N = (n0,n1,n2,0)是一
	* 个单位长度的法线向量，C是平面常数，X = (x0,x1,x2,1)是平面上的任意一点。你
	* 必须确定法线向量是单位长度。整个存储的四元组为(n0,n1,n2,-c)。
	*/
	class PX2_ENGINE_ITEM HPlane
	{
	public:
		HPlane ();  //< 未初始化
		HPlane (const HPlane& plane);
		~HPlane ();

		// 直接设这法线向量和常量C
		HPlane (float normal0, float normal1, float normal2, float constant);
		HPlane (const AVector& normal, float constant);

		/// 输入法线和平面上一点
		/**
		* 常量点为Dot(N,X)。
		*/
		HPlane (const AVector& normal, const APoint& p);

		/// 根据平面上三个点P0,P1,P2构造
		/** 
		* N = Cross(P1-P0,P2-P0)/Length(Cross(P1-P0,P2-P0)), C = Dot(N,P0)
		*/
		HPlane (const APoint& p0, const APoint& p1, const APoint& p2);

		/// 根据整个元组(n0,n1,n2,-c)构造平面。
		HPlane (const HPoint& tuple);

		// 隐式转换到HPoint
		inline operator HPoint ();
		inline operator HPoint () const;

		// 访问
		inline operator const float* () const;
		inline operator float* ();
		inline float operator[] (int i) const;
		inline float& operator[] (int i);

		// 赋值
		HPlane& operator= (const HPlane& plane);

		// 比较 (STL容器排序支持)
		bool operator== (const HPlane& plane) const;
		bool operator!= (const HPlane& plane) const;
		bool operator<  (const HPlane& plane) const;
		bool operator<= (const HPlane& plane) const;
		bool operator>  (const HPlane& plane) const;
		bool operator>= (const HPlane& plane) const;

		// 成员
		inline void SetNormal (const AVector& normal);
		inline void SetConstant (float constant);
		inline AVector GetNormal () const;
		inline float GetConstant () const;

		// 计算L = Length(n0,n1,n2)并且设置平面到(n0,n1,n2,-c)/L。
		float Normalize (const float epsilon = Mathf::ZERO_TOLERANCE);

		/// 计算点到平面的距离
		/**
		* D = Dot(N,Q)-C,N是平面的法线，C是平面常量。D是带符号的距离，D为正，表
		* 明点在平面的正面；为负，表明点在平面的负面；为0，表明点在平面上。
		*/
		float DistanceTo (const APoint& p) const;

		/// 判断点在平面的哪一面
		/** 
		* 法线指向的面为正面,另一面为负面。在正面函数返回+1，负面返回-1，0表示
		* 点在平面上。
		*/
		int WhichSide (const APoint& p) const;

	private:
		// 存储的四元组(n0,n1,n2,-c).
		HPoint mTuple;
	};

#include "PX2HPlane.inl"

}

#endif