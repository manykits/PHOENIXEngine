// PX2OpenGLTexture1D.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLTEXTURE1D_H
#define PX2OPENGLTEXTURE1D_H

#include "PX2OpenGLRendererPre.hpp"
#include "PX2Texture1D.hpp"

namespace PX2
{

	class Renderer;

	class PdrTexture1D
	{
	public:
		// Construction and destruction.
		PdrTexture1D(Renderer* renderer, const Texture1D* texture);
		~PdrTexture1D();

		// Texture operations.
		void Enable(Renderer* renderer, int textureUnit);
		void Disable(Renderer* renderer, int textureUnit);
		void* Lock(int level, Buffer::Locking mode);
		void Unlock(int level);

	private:
		GLuint mTexture, mInternalFormat, mFormat, mType, mPreviousTexture;
		GLint mNumLevels;
		GLint mNumLevelBytes[Texture::MM_MAX_MIPMAP_LEVELS];
		GLint mDimension[Texture::MM_MAX_MIPMAP_LEVELS];
		GLuint mBuffer[Texture::MM_MAX_MIPMAP_LEVELS];
		void* mLockedMemory[Texture::MM_MAX_MIPMAP_LEVELS];
		bool mWriteLock[Texture::MM_MAX_MIPMAP_LEVELS];
	};

}

#endif

#endif