// PX2Dx9Texture3D.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9TEXTURE3D_HPP
#define PX2DX9TEXTURE3D_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2Texture3D.hpp"

namespace PX2
{

	class Renderer;

	class PdrTexture3D
	{
	public:
		PdrTexture3D (Renderer* renderer, const Texture3D* texture);
		~PdrTexture3D ();

		void Enable (Renderer* renderer, int textureUnit);
		void Disable (Renderer* renderer, int textureUnit);
		void* Lock (int level, Buffer::Locking mode);
		void Unlock (int level);

	private:
		IDirect3DVolumeTexture9* mTexture;
	};

}

#endif

#endif