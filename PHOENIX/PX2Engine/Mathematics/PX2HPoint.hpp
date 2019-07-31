// PX2HPoint.hpp

#ifndef PX2HPOINT_HPP
#define PX2HPOINT_HPP

#include "PX2MathematicsPre.hpp"

namespace PX2
{

	/// 点/向量类
	/**
	* 点/向量类是一个其次坐标下的点(x,y,z,w)。如果代表一个点，则w = 1，如APoint。
	* 如果w = 0代表一个向量，如AVector。
	*/
	class PX2_ENGINE_ITEM HPoint
	{
	public:
		HPoint ();  //< 未初始化
		HPoint (const HPoint& pnt);
		HPoint (float x, float y, float z, float w);
		~HPoint (); 

		// 成员访问
		inline operator const float* () const;
		inline operator float* ();
		inline float operator[] (int i) const;
		inline float& operator[] (int i);
		inline float X () const;
		inline float& X ();
		inline float Y () const;
		inline float& Y ();
		inline float Z () const;
		inline float& Z ();
		inline float W () const;
		inline float& W ();

		// 赋值
		HPoint& operator= (const HPoint& pnt);

		// 比较
		bool operator== (const HPoint& pnt) const;
		bool operator!= (const HPoint& pnt) const;
		bool operator<  (const HPoint& pnt) const;
		bool operator<= (const HPoint& pnt) const;
		bool operator>  (const HPoint& pnt) const;
		bool operator>= (const HPoint& pnt) const;

	protected:
		float mTuple[4];
	};

#include "PX2HPoint.inl"

}

#endif