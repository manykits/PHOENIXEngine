// PX2HMatrix.hpp

#ifndef PX2HMATRIX_HPP
#define PX2HMATRIX_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2APoint.hpp"
#include "PX2Matrix3.hpp"

namespace PX2
{

	/// 齐次矩阵类
	/**
	* homogeneous matrix
	*/
	class PX2_ENGINE_ITEM HMatrix
	{
	public:
		HMatrix ();  //< 未初始化
		HMatrix (const HMatrix& mat);
		HMatrix (const Matrix3f& mat);

		/// 如果makeZero为'true'，创建0矩阵，否则创建单位矩阵。
		HMatrix (bool makeZero);

		/// 输入的mrc，r代表行，c代表列。
		HMatrix (
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		/// 根据输入的数组创建一个矩阵
		/**
		* @param rowMajor
		*	true:  entry[0..15]={m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,
		*                       m23,m30,m31,m32,m33} [row major]
		*	false: entry[0..15]={m00,m10,m20,m30,m01,m11,m21,m31,m02,m12,m22,
		*                       m32,m03,m13,m23,m33} [col major]
		*						m32,m03,m13,m23,m33} [col major]
		*/
		HMatrix (const float* entry, bool rowMajor);

		/// 根据的元组创建矩阵
		/**
		* @param columns
		*	true : 输入的元组是矩阵的列
		*	false ：输入的元组是矩阵的行 
		* r d u
		*/
		HMatrix (const float* tuple0, const float* tuple1, const float* tuple2,
			const float* tuple3, bool columns);

		/// 创建对角矩阵
		HMatrix (float m00, float m11, float m22);

		/// 创建旋转矩阵
		/**
		* 如果输入的角度为正，旋转方向为逆时针。角度必须是弧度制。
		*/
		HMatrix (const AVector& axis, float angle);

		~HMatrix ();

		/// 隐式转换，3x3的部分被拷贝到Matrix3f对象中。
		operator Matrix3f () const;

		// 访问
		inline operator const float* () const;
		inline operator float* ();
		inline const float* operator[] (int row) const;
		inline float* operator[] (int row);
		inline float operator() (int row, int column) const;
		inline float& operator() (int row, int column);
		void SetRow (int row, const HPoint& hpnt);
		void GetRow (int row, HPoint& hpnt) const;
		// right, dir, up, AVector(0,0,0,1)
		void SetColumn (int column, const HPoint& hpnt);
		void GetColumn (int column, HPoint& hpnt) const;

		// 矩阵式按照行主序列排列的，将其转换为列主序列。数组columnMajor至少拥
		// 有16个元素的空间。
		void GetColumnMajor (float* columnMajor) const;

		// 赋值
		HMatrix& operator= (const HMatrix& mat);
		HMatrix& operator= (const Matrix3f& mat);

		// 比较
		bool operator== (const HMatrix& mat) const;
		bool operator!= (const HMatrix& mat) const;
		bool operator<  (const HMatrix& mat) const;
		bool operator<= (const HMatrix& mat) const;
		bool operator>  (const HMatrix& mat) const;
		bool operator>= (const HMatrix& mat) const;

		// 计算
		HMatrix operator+ (const HMatrix& mat) const;
		HMatrix operator- (const HMatrix& mat) const;
		HMatrix operator* (float scalar) const;
		HMatrix operator/ (float scalar) const;
		HMatrix operator- () const;

		PX2_ENGINE_ITEM
		friend HMatrix operator* (float scalar, const HMatrix& mat);

		// 计算更新
		HMatrix& operator+= (const HMatrix& mat);
		HMatrix& operator-= (const HMatrix& mat);
		HMatrix& operator*= (float scalar);
		HMatrix& operator/= (float scalar);

		// 其他矩阵操作
		HMatrix &MakeZero ();  //< Z
		HMatrix &MakeIdentity ();  //< I
		HMatrix &MakeDiagonal (float m00, float m11, float m22);  //< D
		HMatrix &MakeRotation (const AVector& axis, float angle);  //< R
		HMatrix Transpose () const;  //< M^T
		HMatrix Inverse (const float epsilon = 0.0f) const;  //< M^{-1}
		HMatrix Adjoint () const;  //< M^{adj}
		float Determinant () const;  //< det(M)
		HMatrix operator* (const HMatrix& mat) const;  //< M*mat
		HMatrix TransposeTimes (const HMatrix& mat) const;  //< M^T*mat
		HMatrix TimesTranspose (const HMatrix& mat) const;  //< M*mat^T
		HMatrix TransposeTimesTranspose (const HMatrix& mat) const;  //< M^T*mat^T
		HMatrix TimesDiagonal (const APoint& diag) const;  //< M*D
		HMatrix DiagonalTimes (const APoint& diag) const;  //< D*M
		void Orthonormalize (); //< 矩阵的3x3，左上角部分应用此函数

		// 矩阵和齐次点之间的操作
		/*
		* M和P都是齐次的（M不需要时仿真矩阵，p也不需要w=1）。
		*/
		HPoint operator* (const HPoint& p) const;  // M*p

		PX2_ENGINE_ITEM
		friend HPoint operator* (const HPoint& p, const HMatrix& mat);  // p*M

		void BatchMultiply (int numPoints, const HPoint* input,
			HPoint* output) const;  // M*p[0], ..., M*p[n-1]

		// 仿真矩阵和仿真点之间的操作
		APoint operator* (const APoint& p) const;  // M*p
		void BatchMultiply (int numPoints, const APoint* input,
			APoint* output) const;  // M*p[0], ..., M*p[n-1]

		// 仿真矩阵和仿真向量之间的操作
		AVector operator* (const AVector& p) const;  // M*v
		void BatchMultiply (int numPoints, const AVector* input,
			AVector* output) const;  // M*v[0], ..., M*v[n-1]

		// Compute a quadratic forms.
		float QForm (const HPoint& p0, const HPoint& p1) const;  // p0^T*M*p1

		// Set the transformation to an oblique projection matrix onto a
		// specified plane.  The plane has an 'origin' point and a unit-length
		// 'normal'.
		void MakeObliqueProjection (const APoint& origin, const AVector& normal,
			const AVector& direction);

		/// 计算透视矩阵
		/**
		* 计算透视到特定平面的矩阵。平面有平面原点，单位长度法线，eye为透视原点。
		*/
		void MakePerspectiveProjection (const APoint& origin,
			const AVector& normal, const APoint& eye);

		/// 计算反射矩阵
		/**
		* origin是反射面上的一个点，normal是一个法向量。
		*/
		void MakeReflection (const APoint& origin, const AVector& normal);

		// 常用值
		static const HMatrix ZERO;
		static const HMatrix IDENTITY;

	private:
		// 矩阵被以行主序列顺序存储
		float mEntry[16];
	};

#include "PX2HMatrix.inl"

}

#endif