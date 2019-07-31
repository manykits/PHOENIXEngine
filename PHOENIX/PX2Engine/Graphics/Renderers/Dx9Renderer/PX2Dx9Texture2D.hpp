// PX2Dx9Texture2D.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9TEXTURE2D_HPP
#define PX2DX9TEXTURE2D_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2Texture2D.hpp"
#include "PX2Memory.hpp"

namespace PX2
{

	class Renderer;

	class PdrTexture2D
	{
	public:
		PdrTexture2D (Renderer* renderer, const Texture2D* texture);
		~PdrTexture2D ();

		void Enable (Renderer* renderer, int textureUnit, int loc);
		void Enable (Renderer* renderer, int textureUnit);
		void Disable (Renderer* renderer, int textureUnit);
		void* Lock (int level, Buffer::Locking mode);
		void Unlock (int level);

	private:
		// Constructor for creating the color/depth textures for a render target.
		friend class Memory;
		friend class PdrRenderTarget;
		PdrTexture2D (Renderer* renderer, bool isColortexture,
			const Texture2D* texture, bool autoGenMipMap);

		IDirect3DTexture9* mTexture;
	};

}

#endif

#endif