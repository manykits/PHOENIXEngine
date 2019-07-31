// PX2Texture2D.hpp

#ifndef PX2TEXTURE2D_HPP
#define PX2TEXTURE2D_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Texture.hpp"

namespace PX2
{
	
	/// 2D纹理类
	class PX2_ENGINE_ITEM Texture2D : public Texture
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Texture2D);

	public:
		/**
		* 如果numLevels为0，会开辟所有级别minmap空间。
		*/
		Texture2D (Format tformat, int dimension0, int dimension1,
			int numLevels, Buffer::Usage usage = Buffer::BU_TEXTURE);

		virtual ~Texture2D ();

		// 获得第0级别mipmap的宽度和高度
		inline int GetWidth () const;
		inline int GetHeight () const;

		// minmap生成支持
		bool CanGenMinmaps () const;
		void GenerateMipmaps ();
		bool HasMipmaps () const;

		// 访问纹理数据
		char* GetData (int level) const;

	protected:
		// mipmap的生成支持
		void ComputeNumLevelBytes ();
		void GenerateNextMipmap (int width, int height, const char* texels,
			int widthNext, int heightNext, char* texelsNext, Float4* rgba);
	};

	PX2_REGISTER_STREAM(Texture2D);
	typedef Pointer0<Texture2D> Texture2DPtr;
#include "PX2Texture2D.inl"

}

#endif