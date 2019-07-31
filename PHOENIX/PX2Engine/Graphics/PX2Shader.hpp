// PX2Shader.hpp

#ifndef PX2SHADER_HPP
#define PX2SHADER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Float4.hpp"

namespace PX2
{
	
	/// 着色器基类
	/**
	* 顶点，像素着色器等从此类派生。该类是一个抽象基类，无法实例化。
	*/
	class PX2_ENGINE_ITEM Shader : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Shader);

	public:
		// 最大版本数量。如果你增加了新的版本，需要改变这个数值。
		enum { MAX_PROFILES = 6 };

		enum Profile
		{
			P_NONE,
			P_1_1,
			P_2_0,
			P_3_0,
			P_ARBVP1,
			P_OPENGLES2,
		};

		// 着色器程序输入和输出变量类型
		enum VariableType
		{
			VT_NONE,

			VT_FLOAT,
			VT_FLOAT1,   VT_FLOAT2,   VT_FLOAT3,   VT_FLOAT4,
			VT_FLOAT1X1, VT_FLOAT1X2, VT_FLOAT1X3, VT_FLOAT1X4,
			VT_FLOAT2X1, VT_FLOAT2X2, VT_FLOAT2X3, VT_FLOAT2X4,
			VT_FLOAT3X1, VT_FLOAT3X2, VT_FLOAT3X3, VT_FLOAT3X4,
			VT_FLOAT4X1, VT_FLOAT4X2, VT_FLOAT4X3, VT_FLOAT4X4,

			VT_HALF,
			VT_HALF1,    VT_HALF2,    VT_HALF3,    VT_HALF4,
			VT_HALF1X1,  VT_HALF1X2,  VT_HALF1X3,  VT_HALF1X4,
			VT_HALF2X1,  VT_HALF2X2,  VT_HALF2X3,  VT_HALF2X4,
			VT_HALF3X1,  VT_HALF3X2,  VT_HALF3X3,  VT_HALF3X4,
			VT_HALF4X1,  VT_HALF4X2,  VT_HALF4X3,  VT_HALF4X4,

			VT_INT,
			VT_INT1,     VT_INT2,     VT_INT3,     VT_INT4,
			VT_INT1X1,   VT_INT1X2,   VT_INT1X3,   VT_INT1X4,
			VT_INT2X1,   VT_INT2X2,   VT_INT2X3,   VT_INT2X4,
			VT_INT3X1,   VT_INT3X2,   VT_INT3X3,   VT_INT3X4,
			VT_INT4X1,   VT_INT4X2,   VT_INT4X3,   VT_INT4X4,

			VT_FIXED,
			VT_FIXED1,   VT_FIXED2,   VT_FIXED3,   VT_FIXED4,
			VT_FIXED1X1, VT_FIXED1X2, VT_FIXED1X3, VT_FIXED1X4,
			VT_FIXED2X1, VT_FIXED2X2, VT_FIXED2X3, VT_FIXED2X4,
			VT_FIXED3X1, VT_FIXED3X2, VT_FIXED3X3, VT_FIXED3X4,
			VT_FIXED4X1, VT_FIXED4X2, VT_FIXED4X3, VT_FIXED4X4,

			VT_BOOL,
			VT_BOOL1,    VT_BOOL2,    VT_BOOL3,    VT_BOOL4,
			VT_BOOL1X1,  VT_BOOL1X2,  VT_BOOL1X3,  VT_BOOL1X4,
			VT_BOOL2X1,  VT_BOOL2X2,  VT_BOOL2X3,  VT_BOOL2X4,
			VT_BOOL3X1,  VT_BOOL3X2,  VT_BOOL3X3,  VT_BOOL3X4,
			VT_BOOL4X1,  VT_BOOL4X2,  VT_BOOL4X3,  VT_BOOL4X4,

			VT_QUANTITY
		};

		// 输入输出变量的语义
		enum VariableSemantic
		{
			VS_NONE,
			VS_POSITION,        // ATTR0
			VS_BLENDWEIGHT,     // ATTR1
			VS_NORMAL,          // ATTR2
			VS_COLOR0,          // ATTR3 (and for render targets)
			VS_COLOR1,          // ATTR4 (and for render targets)
			VS_FOGCOORD,        // ATTR5
			VS_PSIZE,           // ATTR6
			VS_BLENDINDICES,    // ATTR7
			VS_TEXCOORD0,       // ATTR8
			VS_TEXCOORD1,       // ATTR9
			VS_TEXCOORD2,       // ATTR10
			VS_TEXCOORD3,       // ATTR11
			VS_TEXCOORD4,       // ATTR12
			VS_TEXCOORD5,       // ATTR13
			VS_TEXCOORD6,       // ATTR14
			VS_TEXCOORD7,       // ATTR15
			VS_FOG,             // same as VS_FOGCOORD (ATTR5)
			VS_TANGENT,         // same as VS_TEXCOORD6 (ATTR14)
			VS_BINORMAL,        // same as VS_TEXCOORD7 (ATTR15)
			VS_COLOR2,          // support for multiple render targets
			VS_COLOR3,          // support for multiple render targets
			VS_DEPTH0,          // support for multiple render targets
			VS_QUANTITY
		};

		// 纹理采样器类型
		enum SamplerType
		{
			ST_NONE,
			ST_1D,
			ST_2D,
			ST_3D,
			ST_CUBE,
			ST_QUANTITY
		};

		// 纹理采样器纹理过滤模式
		enum SamplerFilter
		{
			SF_NONE,
			SF_NEAREST,          // nearest neighbor (default)
			SF_LINEAR,           // linear filtering
			SF_NEAREST_NEAREST,  // nearest within image, nearest across images
			SF_NEAREST_LINEAR,   // nearest within image, linear across images
			SF_LINEAR_NEAREST,   // linear within image, nearest across images
			SF_LINEAR_LINEAR,    // linear within image, linear across images
			SF_QUANTITY
		};

		// 最大各向异性值
		enum
		{
			MAX_ANISOTROPY = 16
		};

		// 纹理寻址模式
		enum SamplerCoordinate
		{
			SC_NONE,
			SC_CLAMP,
			SC_REPEAT,
			SC_MIRRORED_REPEAT,
			SC_CLAMP_BORDER,
			SC_CLAMP_EDGE,  //< default
			SC_QUANTITY
		};

		// Default lodBias		0
		// Default anisotropy	1
		// Default borderColor	(0,0,0,0)

	protected:
		/// 抽象基类，构造函数
		/**
		* 这个类定义了shader，但是不没有包含着色器的常量参数和纹理参数。每个
		* “shader实例”会定义具体常量参数和纹理参数，常量参数被放在
		* MaterialInstance中。profileOwner表示shader是不是预先分配好内存。
		*/
		Shader ();
		Shader (const std::string& programName, int numInputs, int numOutputs,
			int numConstants, int numSamplers, bool profileOwner);
	public:
		virtual ~Shader ();

		void SetInput (int i, const std::string& name, VariableType type,
			VariableSemantic semantic);

		void SetOutput (int i, const std::string& name, VariableType type,
			VariableSemantic semantic);

		void SetConstant (int i, const std::string& name, int numRegistersUsed);

		void SetSampler (int i, const std::string& name, SamplerType type);
		void SetFilter (int i, SamplerFilter filter);
		void SetCoordinate (int i, int dim, SamplerCoordinate coordinate);
		void SetLodBias (int i, float lodBias);
		void SetAnisotropy (int i, float anisotropy);
		void SetBorderColor (int i, const Float4& borderColor);

		// 以下3个函数中，如果构造函数中的“profileOwner”为true。与版本相关的
		// 数据是被动态分配的，析构函数会对其释放进行管理。
		void SetBaseRegister (int profile, int i, int baseRegister); //< 第i个常量从第baseRegister个寄存器开始
		void SetTextureUnit (int profile, int i, int textureUnit);
		void SetProgram (int profile, const std::string& program);
		void SetProgramFilename(int profile, const std::string& filename);

		// 以下3个函数，如果构造函数中的“profileOwner”为“false”，你可以将与
		// 版本相关的数据放在全局变量中，析构函数不会对数据的释放进行管理。
		void SetBaseRegisters (int* baseRegisters[MAX_PROFILES]);
		void SetTextureUnits (int* textureUnits[MAX_PROFILES]);
		void SetPrograms (std::string programs[MAX_PROFILES]);

		// 成员访问

		// 与版本无关的数据
		inline int GetNumInputs () const;
		const std::string& GetInputName (int i) const;
		VariableType GetInputType (int i) const;
		VariableSemantic GetInputSemantic (int i) const;

		inline int GetNumOutputs () const;
		const std::string& GetOutputName (int i) const;
		VariableType GetOutputType (int i) const;
		VariableSemantic GetOutputSemantic (int i) const;

		inline int GetNumConstants () const;
		const std::string& GetConstantName (int i) const;
		int GetNumRegistersUsed (int i) const;

		inline int GetNumSamplers () const;
		const std::string& GetSamplerName (int i) const;
		SamplerType GetSamplerType (int i) const;
		SamplerFilter GetFilter (int i) const;
		SamplerCoordinate GetCoordinate (int i, int dim) const;
		float GetLodBias (int i) const;
		float GetAnisotropy (int i) const;
		Float4 GetBorderColor (int i) const;

		// 与版本相关的数据
		int GetBaseRegister (int profile, int i) const;
		int GetTextureUnit (int profile, int i) const;
		const std::string* GetProgram (int profile) const;

		// shader标识
		void SetShaderKey (int key);
		int GetShaderKey () const;

		// 只有在OpengES中，渲染器需要调用此接口
		void SetTextureLoc (int i, int loc);
		int GetTextureLoc (int i) const;

		// pdr
	public:
		void *PdrPointer;

	protected:
		// 输入顶点 名称，类型，语义
		int mNumInputs;
		std::string* mInputName;
		VariableType* mInputType;
		VariableSemantic* mInputSemantic;

		// 输出顶点 名称，类型，语义
		int mNumOutputs;
		std::string* mOutputName;
		VariableType* mOutputType;
		VariableSemantic* mOutputSemantic;

		int mNumConstants;
		std::string* mConstantName;
		int* mNumRegistersUsed;

		int mNumSamplers;
		std::string* mSamplerName;
		SamplerType* mSamplerType;
		SamplerFilter* mFilter;
		SamplerCoordinate* mCoordinate[3];
		float* mLodBias;
		float* mAnisotropy;
		Float4* mBorderColor;

		bool mProfileOwner;
		int* mBaseRegister[MAX_PROFILES];
		int* mTextureUnit[MAX_PROFILES];
		std::string* mProgram[MAX_PROFILES];
		std::string* mProgramFilename[MAX_PROFILES];

		int mShaderKey;

		static std::string msNullString;

		// 不需要保存
		int mTexLoc[8];
	};

	PX2_REGISTER_STREAM(Shader);
	typedef Pointer0<Shader> ShaderPtr;
#include "PX2Shader.inl"

}

#endif
