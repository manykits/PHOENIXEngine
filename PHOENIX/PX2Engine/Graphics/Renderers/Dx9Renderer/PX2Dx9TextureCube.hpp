// PX2Dx9TextureCube.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9TEXTURECUBE_HPP
#define PX2DX9TEXTURECUBE_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2TextureCube.hpp"

namespace PX2
{

	class Renderer;

	class PdrTextureCube
	{
	public:
		PdrTextureCube (Renderer* renderer, const TextureCube* texture);
		~PdrTextureCube ();

		void Enable (Renderer* renderer, int textureUnit);
		void Disable (Renderer* renderer, int textureUnit);
		void* Lock (int face, int level, Buffer::Locking mode);
		void Unlock (int face, int level);

	private:
		IDirect3DCubeTexture9* mTexture;
	};

}

#endif

#endif