// PX2Texture3D.hpp

#ifndef PX2TEXTURE3D_HPP
#define PX2TEXTURE3D_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Texture.hpp"

namespace PX2
{
	
	/// 3D纹理类
	class PX2_ENGINE_ITEM Texture3D : public Texture
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Texture3D);

	public:
		Texture3D (Format tformat, int dimension0, int dimension1, int dimension2,
			int numLevels, Buffer::Usage usage);

		virtual ~Texture3D ();

		// 获得第0级别mipmap的宽度和高度
		inline int GetWidth () const;
		inline int GetHeight () const;
		inline int GetThickness () const;

		// minmap生成支持
		void GenerateMipmaps ();
		bool HasMipmaps () const;

		// 访问纹理数据
		char* GetData (int level) const;

	protected:
		// 支持mipmap的生成
		void ComputeNumLevelBytes ();
		void GenerateNextMipmap (int width, int height, int thickness,
			const char* texels, int widthNext, int heightNext, int thicknessNext,
			char* texelsNext, Float4* rgba);
	};

	PX2_REGISTER_STREAM(Texture3D);
	typedef Pointer0<Texture3D> Texture3DPtr;
#include "PX2Texture3D.inl"

}

#endif