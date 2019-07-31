// PX2Dx9Texture1D.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9TEXTURE1D_HPP
#define PX2DX9TEXTURE1D_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2Texture1D.hpp"

namespace PX2
{

	class Renderer;

	class PdrTexture1D
	{
	public:
		PdrTexture1D (Renderer* renderer, const Texture1D* texture);
		~PdrTexture1D ();

		void Enable (Renderer* renderer, int textureUnit);
		void Disable (Renderer* renderer, int textureUnit);
		void* Lock (int level, Buffer::Locking mode);
		void Unlock (int level);

	private:
		IDirect3DTexture9* mTexture;
	};

}

#endif

#endif