// PX2Color.hpp

#ifndef PX2COLOR_HPP
#define PX2COLOR_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Color
	{
	public:
		/// 从8-bit的通道里，将颜色转换到32-bit的颜色，alpha通道被设置到255。
		inline static unsigned int MakeR8G8B8 (unsigned char red,
			unsigned char green, unsigned char blue);

		/// 从8-bit的通道里，将颜色转换到32-bit的颜色。
		inline static unsigned int MakeR8G8B8A8 (unsigned char red,
			unsigned char green, unsigned char blue, unsigned char alpha);

		/// 从32-bit颜色里获得8-bit的颜色。alpha通道被忽略。
		inline static void ExtractR8G8B8 (unsigned int color,
			unsigned char& red, unsigned char& green, unsigned char& blue);

		/// 从32-bit颜色里获得8-bit的颜色。
		inline static void ExtractR8G8B8A8 (unsigned int color,
			unsigned char& red, unsigned char& green, unsigned char& blue,
			unsigned char& alpha);

		enum
		{
			CF_QUANTITY = 23  // = Texture::TF_QUANTITY
		};

		// 将特定格式的转换到32-bit的RGBA
		static void ConvertFromR5G6B5 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA1R5G5B5 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA4R4G4B4 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromL8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA8L8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromR8G8B8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA8R8G8B8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA8B8G8R8 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromL16 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromG16R16 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA16B16G16R16 (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromR16F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromG16R16F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA16B16G16R16F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromR32F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromG32R32F (int numTexels, const char* inTexels,
			Float4* outTexels);

		static void ConvertFromA32B32G32R32F (int numTexels, const char* inTexels,
			Float4* outTexels);

		// 从32-bit的RGBA转换到特定格式
		static void ConvertToR5G6B5 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA1R5G5B5 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA4R4G4B4 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToL8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA8L8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToR8G8B8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA8R8G8B8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA8B8G8R8 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToL16 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToG16R16 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA16B16G16R16 (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToR16F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToG16R16F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA16B16G16R16F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToR32F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToG32R32F (int numTexels, const Float4* inTexels,
			char* outTexels);

		static void ConvertToA32B32G32R32F (int numTexels, const Float4* inTexels,
			char* outTexels);

		// 一个查找表，方便应用程序查找时使用。
		typedef void (*ConvertFromFunction)(int, const char*, Float4*);
		static ConvertFromFunction FromFunction[CF_QUANTITY];

		typedef void (*ConvertToFunction)(int, const Float4*, char*);
		static ConvertToFunction ToFunction[CF_QUANTITY];
	};

#include "PX2Color.inl"
}

#endif