// PX2Matrix2.hpp

#ifndef PX2MATRIX2_HPP
#define PX2MATRIX2_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Table.hpp"
#include "PX2Vector2.hpp"

namespace PX2
{

	/// 二维矩阵类
	/**
	* 旋转矩阵的形式为
	*	R = cos(t) -sin(t)
	*		sin(t)  cos(t)
	*
	* t>0的话，在xy平面上，为逆时针。
	*/
	template <typename Real>
	class Matrix2 : public Table<2,2,Real>
	{
	public:
		// 如果makeZero是'true'，创建一个0矩阵；否则创建一个单位矩阵。
		Matrix2 (bool makeZero = true);

		// 拷贝构造
		Matrix2 (const Matrix2& mat);

		// Input mrc is in row r, column c.
		Matrix2 (Real m00, Real m01, Real m10, Real m11);

		/// 从数组创建矩阵。
		/**
		* @param rowMajor
		*	true:  entry[0..3] = {m00,m01,m10,m11}  [row major]
		*	false: entry[0..3] = {m00,m10,m01,m11}  [column major]
		*/
		Matrix2 (const Real entry[4], bool rowMajor);

		/// 根据向量的输入创建矩阵。
		/** 
		* @param columns
		*	true:  向量是矩阵中的列
		*	false: 向量是矩阵中的行
		*/
		Matrix2 (const Vector2<Real>& u, const Vector2<Real>& v,
			bool columns);
		Matrix2 (const Vector2<Real>* vectors, bool columns);

		/// 创建对角矩阵，m01 = m10 = 0。
		Matrix2 (Real m00, Real m11);

		/// 创建旋转矩阵（角度为正->逆时针，角度为弧度制）。
		Matrix2 (Real angle);

		// Create a tensor product U*V^T.
		Matrix2 (const Vector2<Real>& u, const Vector2<Real>& v);

		// 赋值
		Matrix2& operator= (const Matrix2& mat);

		// 创建特定矩阵
		void MakeZero ();
		void MakeIdentity ();
		void MakeDiagonal (Real m00, Real m11);
		void MakeRotation (Real angle);
		void MakeTensorProduct (const Vector2<Real>& u, const Vector2<Real>& v);

		// 数值计算
		Matrix2 operator+ (const Matrix2& mat) const;
		Matrix2 operator- (const Matrix2& mat) const;
		Matrix2 operator* (Real scalar) const;
		Matrix2 operator/ (Real scalar) const;
		Matrix2 operator- () const;

		// 计算更新
		Matrix2& operator+= (const Matrix2& mat);
		Matrix2& operator-= (const Matrix2& mat);
		Matrix2& operator*= (Real scalar);
		Matrix2& operator/= (Real scalar);

		// M*vec
		Vector2<Real> operator* (const Vector2<Real>& vec) const;

		// u^T*M*v
		Real QForm (const Vector2<Real>& u, const Vector2<Real>& v) const;

		// M^T
		Matrix2 Transpose () const;

		// M*mat
		Matrix2 operator* (const Matrix2& mat) const;

		// M^T*mat
		Matrix2 TransposeTimes (const Matrix2& mat) const;

		// M*mat^T
		Matrix2 TimesTranspose (const Matrix2& mat) const;

		// M^T*mat^T
		Matrix2 TransposeTimesTranspose (const Matrix2& mat) const;

		// 其他操作
		Matrix2 Inverse (const Real epsilon = (Real)0) const;
		Matrix2 Adjoint () const;
		Real Determinant () const;

		// The matrix must be a rotation for these functions to be valid.  The
		// last function uses Gram-Schmidt orthonormalization applied to the
		// columns of the rotation matrix.  The angle must be in radians, not
		// degrees.
		void ExtractAngle (Real& angle) const;
		void Orthonormalize ();

		// The matrix must be symmetric.  Factor M = R * D * R^T where
		// R = [u0|u1] is a rotation matrix with columns u0 and u1 and
		// D = diag(d0,d1) is a diagonal matrix whose diagonal entries are d0
		// and d1.  The eigenvector u[i] corresponds to eigenvector d[i].  The
		// eigenvalues are ordered as d0 <= d1.
		void EigenDecomposition (Matrix2& rot, Matrix2& diag) const;

		// 特定值
		static const Matrix2 ZERO;
		static const Matrix2 IDENTITY;

	protected:
		using Table<2,2,Real>::mEntry;
	};

	// c * M
	template <typename Real>
	inline Matrix2<Real> operator* (Real scalar, const Matrix2<Real>& mat);

	// v^T * M
	template <typename Real>
	inline Vector2<Real> operator* (const Vector2<Real>& vec,
		const Matrix2<Real>& mat);

#include "PX2Matrix2.inl"

	typedef Matrix2<float> Matrix2f;
	typedef Matrix2<double> Matrix2d;

}

#endif
