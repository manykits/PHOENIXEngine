// PX2OpenGLESTexture3D.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2TEXTURE3D_HPP
#define PX2OPENGLES2TEXTURE3D_HPP

#include "PX2OpenGLESRendererPre.hpp"
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
	};

}

#endif

#endif