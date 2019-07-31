// PX2OpenGLTexture2D.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLTEXTURE2D_H
#define PX2OPENGLTEXTURE2D_H

#include "PX2OpenGLRendererPre.hpp"
#include "PX2Texture2D.hpp"

namespace PX2
{

	class Renderer;

	class PdrTexture2D
	{
	public:
		// Construction and destruction.
		PdrTexture2D(Renderer* renderer, const Texture2D* texture);
		~PdrTexture2D();

		// Member access.
		inline GLuint GetTexture() { return mTexture; }

		// Texture operations.
		void Enable(Renderer* renderer, int textureUnit);
		void Enable(Renderer* renderer, int textureUnit, int loc);
		void Disable(Renderer* renderer, int textureUnit);
		void* Lock(int level, Buffer::Locking mode);
		void Unlock(int level);

	private:
		bool mIsDepthSencil;
		GLuint mTexture, mInternalFormat, mFormat, mType, mPreviousTexture;
		GLint mNumLevels;
		GLint mNumLevelBytes[Texture::MM_MAX_MIPMAP_LEVELS];
		GLint mDimension[2][Texture::MM_MAX_MIPMAP_LEVELS];
		GLuint mBuffer[Texture::MM_MAX_MIPMAP_LEVELS];
		void* mLockedMemory[Texture::MM_MAX_MIPMAP_LEVELS];
		bool mWriteLock[Texture::MM_MAX_MIPMAP_LEVELS];
		bool mIsCompressed;
	};

}

#endif

#endif