// PX2OpenGLESTextureCube.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2TEXTURECUBE_HPP
#define PX2OPENGLES2TEXTURECUBE_HPP

#include "PX2OpenGLESRendererPre.hpp"
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
	};

}

#endif

#endif