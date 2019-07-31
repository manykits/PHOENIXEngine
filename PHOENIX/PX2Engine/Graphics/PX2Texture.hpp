// PX2Texture.hpp

#ifndef PX2TEXTURE_HPP
#define PX2TEXTURE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Buffer.hpp"
#include "PX2FileIO.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	/// 纹理基类
	class PX2_ENGINE_ITEM Texture : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Texture);

	public:
		/// 纹理像素格式
		enum Format
		{
			TF_NONE,

			// Small-bit color formats.
			TF_R5G6B5,
			TF_A1R5G5B5,
			TF_A4R4G4B4,

			// 8-bit integer formats.
			TF_A8,
			TF_L8,
			TF_A8L8,
			TF_R8G8B8,
			TF_A8R8G8B8,
			TF_A8B8G8R8,

			// 16-bit integer formats.
			TF_L16,
			TF_G16R16,
			TF_A16B16G16R16,

			// 16-bit floating-point formats ('half float' channels).
			TF_R16F,
			TF_G16R16F,
			TF_A16B16G16R16F,

			// 32-bit floating-point formats ('float' channels).
			TF_R32F,
			TF_G32R32F,
			TF_A32B32G32R32F,

			// DXT compressed formats.
			TF_DXT1,
			TF_DXT3,
			TF_DXT5,

			// Depth-stencil format.
			TF_D16,
			TF_D24S8,

			//ios compressed formats
			TF_RGB_PVRTC_4B,
			TF_RGB_PVRTC_2B,
			TF_RGBA_PVRTC_4B,
			TF_RGBA_PVRTC_2B,

			//android compressed format
			TF_RGB_ETC1,

			TF_QUANTITY
		};

		enum Type
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_CUBE,
			TT_QUANTITY
		};

		// 纹理的最大mipmap等级。这个比当代显卡支持的等级高。一个4096x4096的位图
		// 有13个等级。
		enum
		{
			MM_MAX_MIPMAP_LEVELS = 16
		};

	protected:
		// 抽象基类
		Texture (Format tformat, Type type, Buffer::Usage usage, int numLevels);
	public:
		virtual ~Texture ();

		// 成员访问
		inline Format GetFormat () const;
		inline Type GetTextureType () const;
		inline Buffer::Usage GetUsage () const;
		inline int GetNumLevels () const;
		inline int GetNumDimensions () const;
		inline int GetDimension (int i, int level) const;
		inline int GetNumLevelBytes (int level) const;
		inline int GetNumTotalBytes () const;
		inline int GetLevelOffset (int level) const;
		inline int GetPixelSize () const;
		inline static int GetPixelSize (Format tformat);
		inline bool IsCompressed () const;
		inline bool IsMipmapable () const;

		/// 获得纹理buffer
		/**
		* 通过此函数，可以获得buffer的内存，buffer的字节数通过GetNumTotalBytes()
		* 获得。函数获得buffer为只读的。不要将其转换为"char*"，否则你在改变buffer
		* 后，渲染器无法知道。
		*/
		inline const char* GetData () const;
		bool IsDataNull () const;

		void SetBindDoClearData (bool doClearData);
		bool IsBindDoClearData () const;
		void ClearData ();

		// 用户自定义数据
		/**
		* 这些由应用程序设置，Texture暂且不知道其作用。
		*/
		enum { MAX_USER_FIELDS = 8 };
		inline void SetUserField (int i, int userField);
		inline int GetUserField (int i) const;

		// pdr
	public:
		void *PdrPointer;

	public_internal:
		static int msNumDimensions[TT_QUANTITY];
		static int msPixelSize[TF_QUANTITY];
		static bool msMipmapable[TF_QUANTITY];

	protected:
		// 传给构造函数的数据
		Format mFormat;
		Type mType;
		Buffer::Usage mUsage;
		int mNumLevels;

		// 由构造函数和派生类构造函数计算出的数据
		int mNumDimensions; // 维度的数字表示1，2，3
		int mDimension[3][MM_MAX_MIPMAP_LEVELS]; // 保存每个维度每个级别像素宽度
		int mNumLevelBytes[MM_MAX_MIPMAP_LEVELS]; // 保存每个级别字节数
		int mNumTotalBytes; // 字节总数
		int mLevelOffsets[MM_MAX_MIPMAP_LEVELS];

		/// 用户自定义的，存储应用程序特别的数据
		int mUserField[MAX_USER_FIELDS];

		/// 在内存中存储的图片数据
		char* mData;
		bool mIsBindDoClearData;

		// mipmap生成支持
		/*
		* 每一种格式都先被转换为每个像素32bit，mipmap生成后，将像素转换到原始像
		* 素格式。
		*/

		typedef void (*ConvertFrom)(int, const char*, Float4*);
		static ConvertFrom msConvertFrom[TF_QUANTITY];

		typedef void (*ConvertTo)(int, const Float4*, char*);
		static ConvertTo msConvertTo[TF_QUANTITY];
	};

	PX2_REGISTER_STREAM(Texture);
	typedef Pointer0<Texture> TexturePtr;
#include "PX2Texture.inl"

}

#endif