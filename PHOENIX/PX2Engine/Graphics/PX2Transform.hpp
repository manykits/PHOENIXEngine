// PX2Transform.hpp

#ifndef PX2TRANSFORM_HPP
#define PX2TRANSFORM_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2HMatrix.hpp"
#include "PX2Math.hpp"
#include "PX2Function.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class FunObject;

	/// 变换类
	/**
	* 变换类的形式是Y = M*X+T，M是3x3的矩阵，T是一个变换向量。在绝大多数情况下，
	* M = R是一个旋转向量，或者M=R*S，R是一个旋转向量并且S是一个正的对角缩放矩
	* 阵。M也可以是任何3x3矩阵。向量X被向前变换到Y；逆向变换时从Y变换到X，X =
	* M^{-1}*(Y-T)。在特定的情况下M = R*S，逆向变换是X = S^{-1}*R^t*(Y-T)，
	* S^{-1}矩阵的对角和S的对角互为倒数；R^t是R的转置矩阵。整个变换最终被打包到
	* 矩阵mHMatrix，正向变换为{Y,1} = H*{X,1}，逆向变换为{X,1} = H^{-1}*{Y,1}。
	*/
	class PX2_ENGINE_ITEM Transform
	{
	public:
		/**
		* 初始为单位变换。
		*/
		Transform ();
		~Transform ();

		/// 将变换设置为单位变换。
		void MakeIdentity ();

		/// 将缩放处理为单位缩放
		void MakeUnitScale ();

		// 变换信息
		inline bool IsIdentity () const;      // I
		inline bool IsRSMatrix () const;      // R*S
		inline bool IsUniformScale () const;  // R*S, S = c*I

		// 成员访问
		/*
		*	(1) Set*函数会将mIsIdentity标志置为false。
		*	(2) SetRotate函数会将mIsRSMatrix标志置为true。如果mIsRSMatrix为false，
		*		在Debug模式下，调用GetRotate函数会触发一个断言失败。
		*	(3) SetMatrix函数会将mIsRSMatrix和mIsUniformScale置为false。
		*	(4) SetScale函数会将mIsUniformScale标志置为false。
		*		SetUniformScale函数将mIsUniformScale标志置为true。如果mIsUniformScale
		*		为false，在Debug模式下，调用GetUniformScale函数的调用会触发一个断言失败。
		*	(5) 所有Set*函数设置mInverseNeedsUpdate为true。当GetInverse函数被调用时，
		*		逆矩阵被重新计算，然后mInverseNeedsUpdate被设置为false。
		*/
		void SetRotate (const HMatrix& rotate);
		void SetRotate(float x, float y, float z);
		void SetRotate(const APoint &rot);
		void SetRotateDegree(float xDegree, float yDegree, float zDegree);
		void SetRotateDegreeX(float degree);
		void SetRotateDegreeY(float degree);
		void SetRotateDegreeZ(float degree);
		void SetRotate(const AVector &r, const AVector &d, const AVector &u);
		void SetMatrix (const HMatrix& matrix);
		void SetTranslate (const APoint& translate);
		void SetTranslate (float x, float y, float z);
		void SetTranslateX (float x);
		void SetTranslateY (float y);
		void SetTranslateZ (float z);
		void SetTranslateXY(float x, float z);
		void SetTranslateXZ(float x, float z);
		void SetScale (const APoint& scale);
		void SetUniformScale (float scale);
		inline const HMatrix& GetRotate () const;
		void GetRotate(float &x, float &y, float &z) const;
		APoint GetRotateXYZ() const;
		float GetRotateX() const;
		float GetRotateY() const;
		float GetRotateZ() const;
		APoint GetRotateDegreeXYZ() const;
		float GetRotateDegreeX() const;
		float GetRotateDegreeY() const;
		float GetRotateDegreeZ() const;
		inline const HMatrix& GetMatrix () const;
		inline const APoint& GetTranslate () const;
		inline const APoint& GetScale () const;
		inline float GetUniformScale () const;

		void SetDU(const AVector &dir, const AVector &uping = AVector::UNIT_Z);
		AVector GetDirection() const;
		void GetRDUVector(AVector &r, AVector &d, AVector &u) const;

		// For M = R*S, the largest value of S in absolute value is returned.
		// For general M, the max-row-sum norm is returned, which is a reasonable
		// measure of maximum scale of the transformation.
		float GetNorm () const;

		// 对点进行变换mHMatrix*pnt
		inline APoint operator* (const APoint& pnt) const;

		// 对向量进行变换mHMatrix*vec
		inline AVector operator* (const AVector& vec) const;

		// Matrix-matrix multiplication.
		Transform operator* (const Transform& transform) const;

		// 获得最终打包到的其次矩阵
		inline const HMatrix& Matrix () const;

		/// 获得最终打包到的其次矩阵的逆矩阵。
		/**
		* 如果需要重新计算的话，会重新计算。如果H = {{M,T},{0,1}}，那么H^{-1} =
		* {{M^{-1},-M^{-1}*T},{0,1}}。
		*/
		const HMatrix& Inverse () const;

		/// 获得反向变换
		Transform InverseTransform () const;

		// 特点值
		static const Transform IDENTITY;

		// 持久化支持
		inline static int GetStreamingSize ();

	private:
		// 当mMatrix，mTranslate或者mScale有改变时，更新最终矩阵mHMatrix。
		void UpdateHMatrix ();

		// 对输入矩阵左上角的3x3的部分进行求逆
		static void Invert3x3 (const HMatrix& mat, HMatrix& invMat);

		// 最终打包到的4x4其次矩阵H。H = {{M,T},{0,1}}，他的逆矩阵H^{-1} = 
		// {M^{-1},-M^{-1}*T,{0,1}}。逆矩阵mInvHMatrix在Inverse被调用时才会被
		// 调用。
		HMatrix mHMatrix;
		mutable HMatrix mInvHMatrix;

		HMatrix mMatrix;     // M (general) or R (rotation)
		APoint mTranslate;   // T
		APoint mScale;       // S
		bool mIsIdentity, mIsRSMatrix, mIsUniformScale;
		mutable bool mInverseNeedsUpdate;

		// 持久化支持
		friend class InStream;
		friend class OutStream;
	};

#include "PX2Transform.inl"

}

#endif