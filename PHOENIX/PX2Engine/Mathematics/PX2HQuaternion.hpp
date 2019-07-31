// PX2HQuaternion.hpp

#ifndef PX2HQUATERNION_HPP
#define PX2HQUATERNION_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Float4.hpp"
#include "PX2HMatrix.hpp"

namespace PX2
{

	/// 仿真四元数类
	/**
	* 四元数的表现形式为q = w + x*i + y*j + z*k （w,x,y,z）没有必要是单位长度向
	* 量。
	*/
	class PX2_ENGINE_ITEM HQuaternion
	{
	public:
		HQuaternion ();  //< 未初始化
		HQuaternion (float w, float x, float y, float z);
		HQuaternion (const HQuaternion& q);

		/// 根据输入的旋转矩阵构造
		HQuaternion (const HMatrix& rot);

		/// 根据旋转轴和旋转角构造
		HQuaternion (const AVector& axis, float angle);

		// 参数访问：0 = w, 1 = x, 2 = y, 3 = z.
		inline operator const float* () const;
		inline operator float* ();
		inline float operator[] (int i) const;
		inline float& operator[] (int i);
		inline float W () const;
		inline float& W ();
		inline float X () const;
		inline float& X ();
		inline float Y () const;
		inline float& Y ();
		inline float Z () const;
		inline float& Z ();

		// 赋值
		HQuaternion& operator= (const HQuaternion& q);

		// 比较（用来支持STL容器排序等操作）
		bool operator== (const HQuaternion& q) const;
		bool operator!= (const HQuaternion& q) const;
		bool operator<  (const HQuaternion& q) const;
		bool operator<= (const HQuaternion& q) const;
		bool operator>  (const HQuaternion& q) const;
		bool operator>= (const HQuaternion& q) const;

		// 计算
		HQuaternion operator+ (const HQuaternion& q) const;
		HQuaternion operator- (const HQuaternion& q) const;
		HQuaternion operator* (const HQuaternion& q) const;
		HQuaternion operator* (float scalar) const;
		HQuaternion operator/ (float scalar) const;
		HQuaternion operator- () const;

		PX2_ENGINE_ITEM
		friend HQuaternion operator* (float scalar, const HQuaternion& q)
		{
			return q*scalar;
		}

		// 计算更新
		HQuaternion& operator+= (const HQuaternion& q);
		HQuaternion& operator-= (const HQuaternion& q);
		HQuaternion& operator*= (float scalar);
		HQuaternion& operator/= (float scalar);

		//  四元数，矩阵，axis-angle之间的转换
		void FromRotationMatrix (const HMatrix& rot);
		void ToRotationMatrix (HMatrix& rot) const;
		void FromAxisAngle (const AVector& axis, float angle);
		void ToAxisAngle (AVector& axis, float& angle) const;

		// 其他操作
		float Length () const;  //< 4个元素的长度
		float SquaredLength () const;  //< 4个元素的长度的平方
		float Dot (const HQuaternion& q) const;  //< 4个元素点乘之和
		float Normalize (const float epsilon = Mathf::ZERO_TOLERANCE);
		HQuaternion Inverse () const;  // apply to non-zero quaternion
		HQuaternion Conjugate () const;  // negate x, y, and z terms
		HQuaternion Exp () const;  // apply to quaternion with w = 0
		HQuaternion Log () const;  // apply to unit-length quaternion

		/// 使用四元数旋转向量
		AVector Rotate (const AVector& vec) const;

		/// 四元数旋转插值
		HQuaternion& Slerp (float t, const HQuaternion& p, const HQuaternion& q);

		// Intermediate terms for spherical quadratic interpolation.
		HQuaternion& Intermediate (const HQuaternion& q0, const HQuaternion& q1,
			const HQuaternion& q2);

		// Spherical quadratic interpolation.
		HQuaternion& Squad (float t, const HQuaternion& q0, const HQuaternion& a0,
			const HQuaternion& a1, const HQuaternion& q1);

		// 特定值
		static const HQuaternion ZERO;
		static const HQuaternion IDENTITY;

	private:
		// 存储循序为(w,x,y,z).
		float mTuple[4];
	};

#include "PX2HQuaternion.inl"

}

#endif