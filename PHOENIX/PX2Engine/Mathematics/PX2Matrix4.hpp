// PX2Matrix4.hpp

#ifndef PX2MATRIX4_HPP
#define PX2MATRIX4_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Table.hpp"
#include "PX2Vector3.hpp"
#include "PX2Vector4.hpp"

namespace PX2
{

	template <typename Real>
	class Matrix4 : public Table<4,4,Real>
	{
	public:
		// 如果makeZero是'true'，创建一个0矩阵；否则创建一个单位矩阵。
		Matrix4 (bool makeZero = true);

		// 拷贝构造
		Matrix4 (const Matrix4& mat);

		// Input mrc is in row r, column c.
		Matrix4 (
			Real m00, Real m01, Real m02, Real m03,
			Real m10, Real m11, Real m12, Real m13,
			Real m20, Real m21, Real m22, Real m23,
			Real m30, Real m31, Real m32, Real m33);

		/// 从数组创建矩阵。
		/**
		* @param rowMajor
		*	true:  entry[0..8]={m00,m01,m02,m10,m11,m12,m20,m21,m22} [row major]
		*	false: entry[0..8]={m00,m10,m20,m01,m11,m21,m02,m12,m22} [col major]
		*/
		Matrix4 (const Real entry[16], bool rowMajor);

		// 赋值
		Matrix4& operator= (const Matrix4& mat);

		// 创建特定矩阵
		void MakeZero ();
		void MakeIdentity ();

		// 计算
		Matrix4 operator+ (const Matrix4& mat) const;
		Matrix4 operator- (const Matrix4& mat) const;
		Matrix4 operator* (Real scalar) const;
		Matrix4 operator/ (Real scalar) const;
		Matrix4 operator- () const;

		// 计算更新
		Matrix4& operator+= (const Matrix4& mat);
		Matrix4& operator-= (const Matrix4& mat);
		Matrix4& operator*= (Real scalar);
		Matrix4& operator/= (Real scalar);

		// M*vec
		Vector4<Real> operator* (const Vector4<Real>& vec) const;

		// u^T*M*v
		Real QForm (const Vector4<Real>& u, const Vector4<Real>& v) const;

		// M^T
		Matrix4 Transpose () const;

		// M*mat
		Matrix4 operator* (const Matrix4& mat) const;

		// M^T*mat
		Matrix4 TransposeTimes (const Matrix4& mat) const;

		// M*mat^T
		Matrix4 TimesTranspose (const Matrix4& mat) const;

		// M^T*mat^T
		Matrix4 TransposeTimesTranspose (const Matrix4& mat) const;

		// 其他操作
		Matrix4 Inverse (const Real epsilon = (Real)0) const;
		Matrix4 Adjoint () const;
		Real Determinant () const;

		// Projection matrices onto a specified plane (containing an 'origin'
		// point and a unit-length 'normal').
		void MakeObliqueProjection (const Vector3<Real>& normal,
			const Vector3<Real>& origin, const Vector3<Real>& direction);

		void MakePerspectiveProjection (const Vector3<Real>& normal,
			const Vector3<Real>& origin, const Vector3<Real>& eye);

		/// 创建平面反射矩阵
		void MakeReflection (const Vector3<Real>& normal,
			const Vector3<Real>& origin);

		// 特性值
		static const Matrix4 ZERO;
		static const Matrix4 IDENTITY;

	protected:
		using Table<4,4,Real>::mEntry;
	};

	// c * M
	template <typename Real>
	inline Matrix4<Real> operator* (Real scalar, const Matrix4<Real>& mat);

	// v^T * M
	template <typename Real>
	inline Vector4<Real> operator* (const Vector4<Real>& vec,
		const Matrix4<Real>& mat);

#include "PX2Matrix4.inl"

	typedef Matrix4<float> Matrix4f;
	typedef Matrix4<double> Matrix4d;

}

#endif