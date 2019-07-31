// PX2APoint.hpp

#ifndef PX2APOINT_HPP
#define PX2APOINT_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2AVector.hpp"
#include "PX2Vector2.hpp"

namespace PX2
{

	/// 仿真点类（affine point）
	class PX2_ENGINE_ITEM APoint : public HPoint
	{
	public:
		APoint ();  //< default (0,0,0,1)
		APoint (const APoint& pnt);
		APoint (float x, float y, float z);
		APoint (const Float3& tuple);
		APoint (const Vector3f& pnt);
		~APoint ();

		void SetX(float x);
		void SetY(float y);
		void SetZ(float z);
		void SetW(float w);
		void Set(float x, float y, float z, float w=1.0f);

		// 隐式转换
		inline operator const Float3& () const;
		inline operator Float3& ();
		inline operator const Vector3f& () const;
		inline operator Vector3f& ();

		// 赋值
		APoint& operator= (const APoint& pnt);

		// Arithmetic operations supported by affine algebra.

		// 点减去一个点，得到一个向量。
		AVector operator- (const APoint& pnt) const;

		// 点加上一个向量得到的是一个点
		APoint operator+ (const AVector& vec) const;
		APoint operator- (const AVector& vec) const;
		APoint& operator+= (const AVector& vec);
		APoint& operator-= (const AVector& vec);

		// 在仿真数学中，“点”是不可以被进行随便加减计算的。然而为了计算需要，
		// 添加了支持。

		APoint operator+ (const APoint& pnt) const;
		APoint operator* (float scalar) const;
		APoint operator/ (float scalar) const;

		PX2_ENGINE_ITEM
		friend APoint operator* (float scalar, const APoint& pnt);

		APoint& operator+= (const APoint& pnt);
		APoint& operator-= (const APoint& pnt);
		APoint& operator*= (float scalar);
		APoint& operator/= (float scalar);
		APoint operator- () const;

		/// 点和向量相乘
		/**
		* 在仿真数学中，点和向量的相乘是不允许的。然而在处理与平面相关问题时是
		* 非常方便的。平面的形式为Dot(N,X-P) = 0，N是法线向量，P是平面上一点，
		* X是平面上的任意点，X-P是一个向量，平面的形式可以转换为
		* Dot(N,X) = Dot(N,P)。这个不符合仿真数学的形式，但是我们需要计算
		* Dot(N,P)。在函数中，w被忽略，APoint被当做一个向量处理。
		*/
		float Dot (const AVector& vec) const;

		Vector2f To2() const;
		Vector3f To3() const;

		// 特定值
		static const APoint ORIGIN;  // (0,0,0,1)
		static const APoint UNIT;
	};

#include "PX2APoint.inl"

}

#endif
