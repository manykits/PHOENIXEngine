// PX2OpenGLTexture3D.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLTEXTURE3D_H
#define PX2OPENGLTEXTURE3D_H

#include "PX2OpenGLRendererPre.hpp"
#include "PX2Texture3D.hpp"

namespace PX2
{

	class Renderer;

	class PdrTexture3D
	{
	public:
		PdrTexture3D(Renderer* renderer, const Texture3D* texture);
		~PdrTexture3D();

		void Enable(Renderer* renderer, int textureUnit);
		void Disable(Renderer* renderer, int textureUnit);
		void* Lock(int level, Buffer::Locking mode);
		void Unlock(int level);

	private:
		GLuint mTexture, mInternalFormat, mFormat, mType, mPreviousTexture;
		GLint mNumLevels;
		GLint mNumLevelBytes[Texture::MM_MAX_MIPMAP_LEVELS];
		GLint mDimension[3][Texture::MM_MAX_MIPMAP_LEVELS];
		GLuint mBuffer[Texture::MM_MAX_MIPMAP_LEVELS];
		void* mLockedMemory[Texture::MM_MAX_MIPMAP_LEVELS];
		bool mWriteLock[Texture::MM_MAX_MIPMAP_LEVELS];
	};

}

#endif

#endif