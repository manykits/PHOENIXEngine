// PX2Light.hpp

#ifndef PX2LIGHT_HPP
#define PX2LIGHT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Float4.hpp"
#include "PX2APoint.hpp"

namespace PX2
{
	
	/// 灯光类
	class PX2_ENGINE_ITEM Light : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Light);

	public:
		enum Type
		{
			LT_AMBIENT,
			LT_DIRECTIONAL,
			LT_POINT,
			LT_SPOT,
			LT_QUANTITY
		};

		Light (Type eType = LT_AMBIENT);
		virtual ~Light ();

		// 设置灯光类型，缺省类型是LT_AMBIENT
		inline void SetType (Type type);
		inline Type GetType () const;

		// 灯光颜色
		Float4 Ambient;   //< default: (0,0,0,1)
		Float4 Diffuse;   //< default: (0,0,0,1)
		Float4 Specular;  //< default: (0,0,0,1)'

		float Range;

		// 衰减通常的调制模式为
		// m = 1/(C + L*d + Q*d*d)
		// C是常量系数，L是线性一次系数，Q是二次系数，d是从此顶点到光源的距离。
		// 为了对灯光的强度可调，用下面的模式替代
		// m = I/(C + L*d + Q*d*d)
		// I代表的是强度系数。
		float Constant;   //< default: 1
		float Linear;     //< default: 0
		float Quadratic;  //< default: 0
		float Intensity;  //< default: 1

		// 聚光灯参数。锥形角必须是以弧度制计算的。角度必须满足0 < Angle <= pi。
		float Angle;     //< default: pi
		float CosAngle;  //< default: -1
		float SinAngle;  //< default:  0
		float Exponent;  //< default:  1

		/**
		* 一个帮助函数，你可以设置Angle，内部会为你计算好CosAngle和SinAngle。
		* 锥形角必须是以弧度制计算的。
		*/
		void SetAngle (float angle);

		// 尽管标准的方向光和点光之需要direction向量，但我们容许使用全坐标系
		// 灯光的坐标系总是在世界坐标系中的。
		// {D,U,R}必须基于右手坐标系。每个向量是单位长度。向量必须互相彼此垂直，
		// 并且 R = Cross(D,U)。
		APoint Position; //< default: (0,0,0)
		AVector DVector; //< default: (0,0,-1)
		AVector UVector; //< default: (0,1,0)
		AVector RVector; //< default: (1,0,0)

		/**
		* 一个帮助函数，你可以设置direction向量，内部会为你计算好up和right向量。
		*/
		void SetDirection (const AVector& direction);

	protected:
		Type mType;
	};

	PX2_REGISTER_STREAM(Light);
	typedef Pointer0<Light> LightPtr;
#include "PX2Light.inl"

}

#endif