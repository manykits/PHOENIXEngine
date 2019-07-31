// PX2Matrix3.hpp

#ifndef PX2MATRIX3_HPP
#define PX2MATRIX3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Table.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	/// 三维矩阵类
	/**
	* 矩阵的坐标系为右手坐标系。通常这个矩阵被用作旋转矩阵。绕各个坐标轴的旋转矩
	* 阵为：
	*   RX =    1       0       0
	*           0     cos(t) -sin(t)
	*           0     sin(t)  cos(t)
	*
	*   RY =  cos(t)    0     sin(t)
	*           0       1       0
	*        -sin(t)    0     cos(t)
	*
	*   RZ =  cos(t) -sin(t)    0
	*         sin(t)  cos(t)    0
	*           0       0       1
	*
	* t>0的话，从坐标轴向原点看过去，为逆时针。
	*/
	template <typename Real>
	class Matrix3 : public Table<3,3,Real>
	{
	public:
		// 如果makeZero是'true'，创建一个0矩阵；否则创建一个单位矩阵。
		Matrix3 (bool makeZero = true);

		// 拷贝构造
		Matrix3 (const Matrix3& mat);

		// Input mrc is in row r, column c.
		Matrix3 (Real m00, Real m01, Real m02, Real m10, Real m11, Real m12,
			Real m20, Real m21, Real m22);

		/// 从数组创建矩阵。
		/**
		* @param rowMajor
		*	true:  entry[0..8]={m00,m01,m02,m10,m11,m12,m20,m21,m22} [row major]
		*	false: entry[0..8]={m00,m10,m20,m01,m11,m21,m02,m12,m22} [col major]
		*/
		Matrix3 (const Real entry[9], bool rowMajor);

		/// 根据向量的输入创建矩阵。
		/** 
		* @param columns
		*	true:  向量是矩阵中的列
		*	false: 向量是矩阵中的行
		*/
		Matrix3 (const Vector3<Real>& u, const Vector3<Real>& v,
			const Vector3<Real>& w, bool columns);
		Matrix3 (const Vector3<Real>* v, bool columns);

		/// 创建对角矩阵，m01 = m10 = m02 = m20 = m12 = m21 = 0。
		Matrix3 (Real m00, Real m11, Real m22);

		/// 创建旋转矩阵（角度为正->逆时针，角度为弧度制）。
		Matrix3 (const Vector3<Real>& axis, Real angle);

		// Create a tensor product U*V^T.
		Matrix3 (const Vector3<Real>& u, const Vector3<Real>& v);

		// 赋值
		Matrix3& operator= (const Matrix3& mat);

		// 创建特定矩阵
		Matrix3& MakeZero ();
		Matrix3& MakeIdentity ();
		Matrix3& MakeDiagonal (Real m00, Real m11, Real m22);
		Matrix3& MakeRotation (const Vector3<Real>& axis, Real angle);
		Matrix3& MakeTensorProduct (const Vector3<Real>& u,
			const Vector3<Real>& v);

		// 计算
		Matrix3 operator+ (const Matrix3& mat) const;
		Matrix3 operator- (const Matrix3& mat) const;
		Matrix3 operator* (Real scalar) const;
		Matrix3 operator/ (Real scalar) const;
		Matrix3 operator- () const;

		// 计算更新
		Matrix3& operator+= (const Matrix3& mat);
		Matrix3& operator-= (const Matrix3& mat);
		Matrix3& operator*= (Real scalar);
		Matrix3& operator/= (Real scalar);

		// M*vec
		Vector3<Real> operator* (const Vector3<Real>& vec) const;

		// u^T*M*v
		Real QForm (const Vector3<Real>& u, const Vector3<Real>& v) const;

		// M^T
		Matrix3 Transpose () const;

		// M*mat
		Matrix3 operator* (const Matrix3& mat) const;

		// M^T*mat
		Matrix3 TransposeTimes (const Matrix3& mat) const;

		// M*mat^T
		Matrix3 TimesTranspose (const Matrix3& mat) const;

		// M^T*mat^T
		Matrix3 TransposeTimesTranspose (const Matrix3& mat) const;

		// 其他操作
		Matrix3 TimesDiagonal (const Vector3<Real>& diag) const;  // M*D
		Matrix3 DiagonalTimes (const Vector3<Real>& diag) const;  // D*M
		Matrix3 Inverse (const Real epsilon = (Real)0) const;
		Matrix3 Adjoint () const;
		Real Determinant () const;

		/// 根据矩阵，球的旋转轴和旋转角。获得的角度是弧度制的。
		void ExtractAxisAngle (Vector3<Real>& axis, Real& angle) const;
		void Orthonormalize ();

		// The matrix must be symmetric.  Factor M = R * D * R^T where
		// R = [u0|u1|u2] is a rotation matrix with columns u0, u1, and u2 and
		// D = diag(d0,d1,d2) is a diagonal matrix whose diagonal entries are d0,
		// d1, and d2.  The eigenvector u[i] corresponds to eigenvector d[i].
		// The eigenvalues are ordered as d0 <= d1 <= d2.
		void EigenDecomposition (Matrix3& rot, Matrix3& diag) const;

		// 根据旋转欧拉角创建矩阵
		Matrix3 &MakeEulerXYZ (Real xAngle, Real yAngle, Real zAngle);
		Matrix3 &MakeEulerXZY (Real xAngle, Real zAngle, Real yAngle);
		Matrix3 &MakeEulerYXZ (Real yAngle, Real xAngle, Real zAngle);
		Matrix3 &MakeEulerYZX (Real yAngle, Real zAngle, Real xAngle);
		Matrix3 &MakeEulerZXY (Real zAngle, Real xAngle, Real yAngle);
		Matrix3 &MakeEulerZYX (Real zAngle, Real yAngle, Real xAngle);
		Matrix3 &MakeEulerXYX (Real x0Angle, Real yAngle, Real x1Angle);
		Matrix3 &MakeEulerXZX (Real x0Angle, Real zAngle, Real x1Angle);
		Matrix3 &MakeEulerYXY (Real y0Angle, Real xAngle, Real y1Angle);
		Matrix3 &MakeEulerYZY (Real y0Angle, Real zAngle, Real y1Angle);
		Matrix3 &MakeEulerZXZ (Real z0Angle, Real xAngle, Real z1Angle);
		Matrix3 &MakeEulerZYZ (Real z0Angle, Real yAngle, Real z1Angle);

		/// 从矩阵获得旋转欧拉角的结果
		enum EulerResult
		{
			// 结果是独一无二的
			EA_UNIQUE,

			// 结果不是独一无二的，所有角的和是一个常量。
			EA_NOT_UNIQUE_SUM,

			// 结果不是独一无二的，所有角的差事一个常量。
			EA_NOT_UNIQUE_DIF
		};

		// The return values are in the specified ranges:
		//   xAngle in [-pi,pi], yAngle in [-pi/2,pi/2], zAngle in [-pi,pi]
		// When the solution is not unique, zAngle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  zAngle + xAngle = c
		//   EA_NOT_UNIQUE_DIF:  zAngle - xAngle = c
		// for some angle c.
		EulerResult ExtractEulerXYZ (Real& xAngle, Real& yAngle, Real& zAngle)
			const;

		// The return values are in the specified ranges:
		//   xAngle in [-pi,pi], zAngle in [-pi/2,pi/2], yAngle in [-pi,pi]
		// When the solution is not unique, yAngle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  yAngle + xAngle = c
		//   EA_NOT_UNIQUE_DIF:  yAngle - xAngle = c
		// for some angle c.
		EulerResult ExtractEulerXZY (Real& xAngle, Real& zAngle, Real& yAngle)
			const;

		// The return values are in the specified ranges:
		//   yAngle in [-pi,pi], xAngle in [-pi/2,pi/2], zAngle in [-pi,pi]
		// When the solution is not unique, zAngle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  zAngle + yAngle = c
		//   EA_NOT_UNIQUE_DIF:  zAngle - yAngle = c
		// for some angle c.
		EulerResult ExtractEulerYXZ (Real& yAngle, Real& xAngle, Real& zAngle)
			const;

		// The return values are in the specified ranges:
		//   yAngle in [-pi,pi], zAngle in [-pi/2,pi/2], xAngle in [-pi,pi]
		// When the solution is not unique, xAngle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  xAngle + yAngle = c
		//   EA_NOT_UNIQUE_DIF:  xAngle - yAngle = c
		// for some angle c.
		EulerResult ExtractEulerYZX (Real& yAngle, Real& zAngle, Real& xAngle)
			const;

		// The return values are in the specified ranges:
		//   zAngle in [-pi,pi], xAngle in [-pi/2,pi/2], yAngle in [-pi,pi]
		// When the solution is not unique, yAngle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  yAngle + zAngle = c
		//   EA_NOT_UNIQUE_DIF:  yAngle - zAngle = c
		// for some angle c.
		EulerResult ExtractEulerZXY (Real& zAngle, Real& xAngle, Real& yAngle)
			const;

		// The return values are in the specified ranges:
		//   zAngle in [-pi,pi], yAngle in [-pi/2,pi/2], xAngle in [-pi,pi]
		// When the solution is not unique, xAngle = 0 is/ returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  xAngle + zAngle = c
		//   EA_NOT_UNIQUE_DIF:  xAngle - zAngle = c
		// for some angle c.
		EulerResult ExtractEulerZYX (Real& zAngle, Real& yAngle, Real& xAngle)
			const;

		// The return values are in the specified ranges:
		//   x0Angle in [-pi,pi], yAngle in [0,pi], x1Angle in [-pi,pi]
		// When the solution is not unique, x1Angle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  x1Angle + x0Angle = c
		//   EA_NOT_UNIQUE_DIF:  x1Angle - x0Angle = c
		// for some angle c.
		EulerResult ExtractEulerXYX (Real& x0Angle, Real& yAngle, Real& x1Angle)
			const;

		// The return values are in the specified ranges:
		//   x0Angle in [-pi,pi], zAngle in [0,pi], x1Angle in [-pi,pi]
		// When the solution is not unique, x1Angle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  x1Angle + x0Angle = c
		//   EA_NOT_UNIQUE_DIF:  x1Angle - x0Angle = c
		// for some angle c.
		EulerResult ExtractEulerXZX (Real& x0Angle, Real& zAngle, Real& x1Angle)
			const;

		// The return values are in the specified ranges:
		//   y0Angle in [-pi,pi], xAngle in [0,pi], y1Angle in [-pi,pi]
		// When the solution is not unique, y1Angle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  y1Angle + y0Angle = c
		//   EA_NOT_UNIQUE_DIF:  y1Angle - y0Angle = c
		// for some angle c.
		EulerResult ExtractEulerYXY (Real& y0Angle, Real& xAngle, Real& y1Angle)
			const;

		// The return values are in the specified ranges:
		//   y0Angle in [-pi,pi], zAngle in [0,pi], y1Angle in [-pi,pi]
		// When the solution is not unique, y1Angle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  y1Angle + y0Angle = c
		//   EA_NOT_UNIQUE_DIF:  y1Angle - y0Angle = c
		// for some angle c.
		EulerResult ExtractEulerYZY (Real& y0Angle, Real& zAngle, Real& y1Angle)
			const;

		// The return values are in the specified ranges:
		//   z0Angle in [-pi,pi], xAngle in [0,pi], z1Angle in [-pi,pi]
		// When the solution is not unique, z1Angle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  z1Angle + z0Angle = c
		//   EA_NOT_UNIQUE_DIF:  z1Angle - z0Angle = c
		// for some angle c.
		EulerResult ExtractEulerZXZ (Real& z0Angle, Real& xAngle, Real& z1Angle)
			const;

		// The return values are in the specified ranges:
		//   z0Angle in [-pi,pi], yAngle in [0,pi], z1Angle in [-pi,pi]
		// When the solution is not unique, z1Angle = 0 is returned.  Generally,
		// the set of solutions is
		//   EA_NOT_UNIQUE_SUM:  z1Angle + z0Angle = c
		//   EA_NOT_UNIQUE_DIF:  z1Angle - z0Angle = c
		// for some angle c.
		EulerResult ExtractEulerZYZ (Real& z0Angle, Real& yAngle, Real& z1Angle)
			const;

		// SLERP (spherical linear interpolation) without quaternions.  Computes
		// R(t) = R0*(Transpose(R0)*R1)^t.  If Q is a rotation matrix with
		// unit-length axis U and angle A, then Q^t is a rotation matrix with
		// unit-length axis U and rotation angle t*A.
		Matrix3& Slerp (Real t, const Matrix3& rot0, const Matrix3& rot1);

		// Polar decomposition, M = Q*S, where Q is orthogonal and S is symmetric.
		// This uses the singular value decomposition:
		//   M = L*D*Transpose(R) = (L*Transpose(R))*(R*D*Transpose(R)) = Q*S
		// where Q = L*Transpose(R) and S = R*D*Transpose(R).
		void PolarDecomposition (Matrix3& qMat, Matrix3& sMat);

		// Factor M = Q*D*U with orthogonal Q, diagonal D, upper triangular U.
		void QDUDecomposition (Matrix3& qMat, Matrix3& diag, Matrix3& uMat)
			const;

		// 特定值
		static const Matrix3 ZERO;
		static const Matrix3 IDENTITY;

	private:
		// Support for eigendecomposition.  The Tridiagonalize function applies
		// a Householder transformation to the matrix.  If that transformation
		// is the identity (the matrix is already tridiagonal), then the return
		// value is 'false'.  Otherwise, the transformation is a reflection and
		// the return value is 'true'.  The QLAlgorithm returns 'true' iff the
		// QL iteration scheme converged.
		bool Tridiagonalize (Real diagonal[3], Real subdiagonal[2]);
		bool QLAlgorithm (Real diagonal[3], Real subdiagonal[2]);

	protected:
		using Table<3,3,Real>::mEntry;
	};

	// c * M
	template <typename Real>
	inline Matrix3<Real> operator* (Real scalar, const Matrix3<Real>& mat);

	// v^T * M
	template <typename Real>
	inline Vector3<Real> operator* (const Vector3<Real>& vec,
		const Matrix3<Real>& mat);

#include "PX2Matrix3.inl"

	typedef Matrix3<float> Matrix3f;
	typedef Matrix3<double> Matrix3d;

}

#endif