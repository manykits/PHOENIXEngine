// PX2OpenGLESTexture2D.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2TEXTURE2D_HPP
#define PX2OPENGLES2TEXTURE2D_HPP

#include "PX2OpenGLESRendererPre.hpp"
#include "PX2Texture2D.hpp"

namespace PX2
{

	class Renderer;

	class PdrTexture2D
	{
	public:
		PdrTexture2D (Renderer* renderer, const Texture2D* texture);
		~PdrTexture2D ();

		inline GLuint GetTexture () { return mTexture; }

		void Enable (Renderer* renderer, int textureUnit);
		void Enable (Renderer* renderer, int textureUnit, int loc);
		void Disable (Renderer* renderer, int textureUnit);
		void* Lock (int level, Buffer::Locking mode);
		void Unlock (int level);

	private:
		friend class PdrRenderTarget;
		PdrTexture2D(Renderer* renderer, bool isColortexture,
			const Texture2D* texture, bool autoGenMipMap);

		const Texture2D *mTex;
		GLuint mTexture, mInternalFormat, mFormat, mType;
		GLint mNumLevels;
		GLint mNumLevelBytes[Texture::MM_MAX_MIPMAP_LEVELS];
		GLint mDimension[2][Texture::MM_MAX_MIPMAP_LEVELS];
		bool mIsCompressed;
	};

}

#endif

#endif