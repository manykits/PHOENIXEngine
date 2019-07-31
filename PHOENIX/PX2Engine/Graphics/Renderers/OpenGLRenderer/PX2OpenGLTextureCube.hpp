// PX2OpenGLTextureCube.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLTEXTURECUBE_H
#define PX2OPENGLTEXTURECUBE_H

#include "PX2OpenGLRendererPre.hpp"
#include "PX2TextureCube.hpp"

namespace PX2
{

	class Renderer;

	class PdrTextureCube
	{
	public:
		// Construction and destruction.
		PdrTextureCube(Renderer* renderer, const TextureCube* texture);
		~PdrTextureCube();

		// Texture operations.
		void Enable(Renderer* renderer, int textureUnit);
		void Disable(Renderer* renderer, int textureUnit);
		void* Lock(int face, int level, Buffer::Locking mode);
		void Unlock(int face, int level);

	private:
		GLuint mTexture, mInternalFormat, mFormat, mType, mPreviousTexture;
		GLint mNumLevels;
		GLint mNumLevelBytes[Texture::MM_MAX_MIPMAP_LEVELS];
		GLint mDimension[2][Texture::MM_MAX_MIPMAP_LEVELS];
		GLuint mBuffer[6][Texture::MM_MAX_MIPMAP_LEVELS];
		void* mLockedMemory[6][Texture::MM_MAX_MIPMAP_LEVELS];
		bool mWriteLock[6][Texture::MM_MAX_MIPMAP_LEVELS];
		bool mIsCompressed;
	};

}

#endif

#endif