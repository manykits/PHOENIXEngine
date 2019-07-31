// PX2ShaderFloat.hpp

#ifndef PX2SHADERFLOAT_HPP
#define PX2SHADERFLOAT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2ShaderStruct.hpp"

namespace PX2
{
	
	/// 着色器常量寄存器类
	/**
	* 至少有1个寄存器，每个常量都是4个浮点值。
	*/
	class PX2_ENGINE_ITEM ShaderFloat : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ShaderFloat);

	public:
		ShaderFloat ();
		/// 非缺省构造函数
		/**
		* 输入的寄存器数量必须非负。每一个寄存器都是4个floating-point值。
		*/
		ShaderFloat (int numRegisters);
		virtual ~ShaderFloat ();

		// 设置数常量寄存器数量
		void SetNumRegisters (int numRegisters);

		// 成员访问
		inline int GetNumRegisters () const;
		inline const float* GetData () const;
		inline float* GetData ();

		//设置寄存器值
		/*
		* SetRegister从data拷贝4个float到register i。
		* SetRegisters从data拷贝mNumElements个float到register。
		* Get*正好相反。
		*/
		void SetRegister (int i, const float* data);
		void SetRegisters (const float* data);
		void GetRegister (int i, float* data);
		float *GetRegister(int i);
		void GetRegisters (float* data);

		/// 设置寄存器值
		/**
		* 和SetRegisters的意思是一样的。当一个寄存器结构定义了到'const float*'
		* 的隐式转换，这个赋值函数就非常有用了。
		*/
		ShaderFloat& operator= (const float* data);

		// 通过索引访问数据
		inline float operator[] (int i) const;
		inline float& operator[] (int i);

		// 这个函数容许着色器常量的动态更新。比如在跟新相机后，相机着色器常量也
		// 需要改变。
		inline void EnableUpdater ();
		inline void DisableUpdater ();
		inline bool AllowUpdater () const;
		virtual void Update (const ShaderStruct *struc);

	protected:
		int mNumElements;
		float* mData;
		bool mAllowUpdater;
	};

	PX2_REGISTER_STREAM(ShaderFloat);
	typedef Pointer0<ShaderFloat> ShaderFloatPtr;
#include "PX2ShaderFloat.inl"

}

#endif
