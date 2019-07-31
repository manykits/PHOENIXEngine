// PX2AVector.hpp

#ifndef PX2AVECTOR_HPP
#define PX2AVECTOR_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2HPoint.hpp"
#include "PX2Float3.hpp"
#include "PX2Vector3.hpp"
#include "PX2Vector2.hpp"

namespace PX2
{

	/// 仿真向量类（affine vector）
	class PX2_ENGINE_ITEM AVector : public HPoint
	{
	public:
		AVector ();  //< default (0,0,0,0)
		AVector (const AVector& vec);
		AVector (float x, float y, float z);
		AVector (const Float3& tuple);
		AVector (const Vector3f& vec);
		~AVector ();

		void SetX(float x);
		void SetY(float y);
		void SetZ(float z);
		void SetW(float w);
		void Set(float x, float y, float z, float w = 1.0f);

		// 隐式转换
		inline operator const Float3& () const;
		inline operator Float3& ();
		inline operator const Vector3f& () const;
		inline operator Vector3f& ();

		// 赋值
		AVector& operator= (const AVector& vec);

		// 计算操作
		AVector operator+ (const AVector& vec) const;
		AVector operator- (const AVector& vec) const;
		AVector operator* (float scalar) const;
		AVector operator/ (float scalar) const;
		AVector operator- () const;

		PX2_ENGINE_ITEM
		friend AVector operator* (float scalar, const AVector& vec);

		// 计算更新
		AVector& operator+= (const AVector& vec);
		AVector& operator-= (const AVector& vec);
		AVector& operator*= (float scalar);
		AVector& operator/= (float scalar);

		// 向量操作
		float Length () const;
		float SquaredLength () const;
		float Dot (const AVector& vec) const;
		AVector Dot(float length) const;
		AVector Over(float val) const;
		AVector Add(const AVector &vec) const;
		float Normalize (const float epsilon = Mathf::ZERO_TOLERANCE);
		AVector &Truncate (float max);
		AVector Cross (const AVector& vec) const;
		AVector UnitCross (const AVector& vec) const;

		Vector2f To2();

		// Gram-Schmidt正交化。
		/*
		* 输入的必须是未初始化的非0向量
		*/
		static void Orthonormalize (AVector& vec0, AVector& vec1, AVector& vec2);
		static void Orthonormalize (AVector* vec);

		/// 生成正交向量轴
		/**
		* 输入vec2必须是非0向量。输出时正交化的向量{vec0,vec1,vec2}。输入向量
		* vec2会被这个函数单位化。如果vec2已经是单位化的，使用
		* GenerateComplementBasis去计算vec1和vec2。
		*/
		static void GenerateOrthonormalBasis (AVector& vec0, AVector& vec1,
			AVector& vec2);

		/// 根据输入的vec2(vec2必须是单位向量)建立缺省的{vec0,vec1,vec2}
		/**
		* 输入vec2必须是单位向量。输出时正交化的向量{vec0,vec1,vec2}。输入向量W
		* 会被这个函数单位化。
		*/
		static void GenerateComplementBasis (AVector& vec0, AVector& vec1,
			const AVector& vec2);

		// 常用值
		static const AVector ZERO;    //< (0,0,0,0)
		static const AVector UNIT_X;  //< (1,0,0,0)
		static const AVector UNIT_Y;  //< (0,1,0,0)
		static const AVector UNIT_Z;  //< (0,0,1,0)
		static const AVector UNIT;  //< (1,1,1,0)

		static AVector AnglesToDirection(float angle0, float angle1);
	};

#include "PX2AVector.inl"

}

#endif