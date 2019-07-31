// PX2OpenGLTexture1D.cpp

#ifdef PX2_USE_OPENGL

#include "PX2OpenGLTexture1D.hpp"
#include "PX2OpenGLMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrTexture1D::PdrTexture1D(Renderer*, const Texture1D* texture)
	:
	mPreviousTexture(0)
{
	mInternalFormat = gOGLTextureInternalFormat[texture->GetFormat()];
	mFormat = gOGLTextureFormat[texture->GetFormat()];
	mType = gOGLTextureType[texture->GetFormat()];
	GLuint usage = gOGLBufferUsage[texture->GetUsage()];

	// Create pixel buffer objects to store the texture data.
	mNumLevels = texture->GetNumLevels();
	int level;
	for (level = 0; level < mNumLevels; ++level)
	{
		mNumLevelBytes[level] = texture->GetNumLevelBytes(level);

		glGenBuffers(1, &mBuffer[level]);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, mBuffer[level]);
		glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, mNumLevelBytes[level], 0,
			usage);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

		mDimension[level] = texture->GetDimension(0, level);
		mLockedMemory[level] = 0;
		mWriteLock[level] = false;
	}
	for (/**/; level < Texture::MM_MAX_MIPMAP_LEVELS; ++level)
	{
		mBuffer[level] = 0;
		mNumLevelBytes[level] = 0;
		mDimension[level] = 0;
		mLockedMemory[level] = 0;
		mWriteLock[level] = false;
	}

	// Create a texture structure.
	glGenTextures(1, &mTexture);
	GLuint previousBind = BindTexture(Shader::ST_1D, mTexture);

	// Create the mipmap level structures.  No image initialization occurs.
	for (level = 0; level < mNumLevels; ++level)
	{
		glTexImage1D(GL_TEXTURE_1D, level, mInternalFormat, mDimension[level],
			0, mFormat, mType, 0);
	}

	glBindTexture(GL_TEXTURE_1D, previousBind);

	for (level = 0; level < mNumLevels; ++level)
	{
		void* data = Lock(level, Buffer::BL_WRITE_ONLY);
		memcpy(data, texture->GetData(level),
			texture->GetNumLevelBytes(level));
		Unlock(level);
	}
}
//----------------------------------------------------------------------------
PdrTexture1D::~PdrTexture1D()
{
	for (int level = 0; level < mNumLevels; ++level)
	{
		glDeleteBuffers(1, &mBuffer[level]);
	}
	glDeleteTextures(1, &mTexture);
}
//----------------------------------------------------------------------------
void PdrTexture1D::Enable(Renderer*, int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	mPreviousTexture = BindTexture(Shader::ST_1D, mTexture);
}
//----------------------------------------------------------------------------
void PdrTexture1D::Disable(Renderer*, int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_1D, mPreviousTexture);
}
//----------------------------------------------------------------------------
void* PdrTexture1D::Lock(int level, Buffer::Locking mode)
{
	if (!mLockedMemory[level])
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, mBuffer[level]);
		mLockedMemory[level] = glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB,
			gOGLBufferLocking[mode]);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

		mWriteLock[level] = (mode != Buffer::BL_READ_ONLY);
	}
	return mLockedMemory[level];
}
//----------------------------------------------------------------------------
void PdrTexture1D::Unlock(int level)
{
	if (mLockedMemory[level])
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, mBuffer[level]);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);

		if (mWriteLock[level])
		{
			GLuint previousBind = BindTexture(Shader::ST_1D, mTexture);

			// TODO:  When creating a texture with only level 0 (not a
			// complete set of mipmaps), the following code generates a
			// GL_INVALID_OPERATION on an AMD Radeon HD 7970 (Catalyst
			// 13.4 and Catalyst 13.6 beta).  My analysis shows that it
			// should not.  So for now, just use glTexImage1D.  This
			// problem does not occur on my NVIDIA graphics cards.
			//
			//glTexSubImage1D(GL_TEXTURE_1D, level, 0, mDimension[level],
			//    mFormat, mType, 0);

			glTexImage1D(GL_TEXTURE_1D, level, mInternalFormat,
				mDimension[level], 0, mFormat, mType, 0);

			glBindTexture(GL_TEXTURE_1D, previousBind);
			mWriteLock[level] = false;
		}

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		mLockedMemory[level] = 0;
	}
}
//----------------------------------------------------------------------------

#endif
