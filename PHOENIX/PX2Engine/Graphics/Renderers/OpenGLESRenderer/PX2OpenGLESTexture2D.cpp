// PX2OpenGLESTexture2D.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESTexture2D.hpp"
#include "PX2OpenGLESMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrTexture2D::PdrTexture2D (Renderer*, const Texture2D* texture) :
mTex(texture),
mTexture(0)
{
	mInternalFormat = gOGLTextureInternalFormat[texture->GetFormat()];
	Texture::Format tdFormat = texture->GetFormat();
	mFormat = gOGLTextureFormat[tdFormat];
	mType = gOGLTextureType[tdFormat];
	int numBytes = texture->GetNumTotalBytes();
	int numLevels = texture->GetNumLevels();
	char *srcData = texture->GetData(0);

	mNumLevels = texture->GetNumLevels();
	int level;
	for (level = 0; level < mNumLevels; ++level)
	{
		mNumLevelBytes[level] = texture->GetNumLevelBytes(level);
		mDimension[0][level] = texture->GetDimension(0, level);
		mDimension[1][level] = texture->GetDimension(1, level);
	}
	for (/**/; level < Texture::MM_MAX_MIPMAP_LEVELS; ++level)
	{
		mNumLevelBytes[level] = 0;
		mDimension[0][level] = 0;
		mDimension[1][level] = 0;
	}

	char* newSrc = 0;

//#if defined(__ANDROID__)
//	int numElement = 0;
//	int srcBase = 0;
//	newSrc = srcData;
//#else
	int numElement = 0;
	int srcBase = 0;
	if (tdFormat == Texture::TF_A8R8G8B8)
	{
		numElement = numBytes/4;
		newSrc = new1<char>(numBytes);
		for (int i = 0; i < numElement; i++)
		{
			newSrc[srcBase + 2] = srcData[srcBase    ]; // B
			newSrc[srcBase + 1] = srcData[srcBase + 1]; // G
			newSrc[srcBase + 0] = srcData[srcBase + 2]; // R
			newSrc[srcBase + 3] = srcData[srcBase + 3]; // A
			srcBase += 4;
		}
	}
	else if (tdFormat == Texture::TF_R8G8B8)
	{
		numElement = numBytes/3;
		newSrc = new1<char>(numBytes);
		for (int i = 0; i < numElement; i++)
		{
			newSrc[srcBase + 0] = srcData[srcBase + 0]; // B
			newSrc[srcBase + 1] = srcData[srcBase + 1]; // G
			newSrc[srcBase + 2] = srcData[srcBase + 2]; // R
			srcBase += 3;
		}
	}
	else if (tdFormat == Texture::TF_A4R4G4B4)
	{
		numElement = numBytes/2;
		newSrc = new1<char>(numBytes);
		for(int i=0; i<numElement; i++)
		{
			unsigned short c = *(unsigned short *)&srcData[srcBase];
			*(unsigned short *)&newSrc[srcBase] = ((c&0xf)<<12) | ((c&0xf0)<<4) | ((c&0xf00)>>4) | ((c&0xf000)>>12);
			srcBase += 2;
		}
	}
	else
	{
		newSrc = srcData;
	}
//#endif

	// 创建和绑定
	PX2_GL_CHECK(glGenTextures(1, &mTexture));
	PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, mTexture));

	mIsCompressed = texture->IsCompressed();
	if (mIsCompressed)
	{
		for (level = 0; level < mNumLevels; ++level)
		{
			PX2_GL_CHECK(glCompressedTexImage2D(GL_TEXTURE_2D, level, mInternalFormat,
				mDimension[0][level], mDimension[1][level], 0,
				mNumLevelBytes[level], texture->GetData(level)));
		}
	}
	else
	{
		int onePixelBytes = Texture::msPixelSize[tdFormat];

		char *src1 = newSrc;
		int levelByte = 0;

		for (int level = 0; level < numLevels; ++level)
		{
			int curWidth = texture->GetDimension(0, level);
			int curHegiht = texture->GetDimension(1, level);

			levelByte = onePixelBytes*curWidth*curHegiht;

			PX2_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, mInternalFormat,
				mDimension[0][level], mDimension[1][level], 0, mFormat,
				mType, src1));

			src1 += levelByte;
		}

		if(newSrc != srcData)
		{
			delete1(newSrc);
		}
	}

	PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}
//----------------------------------------------------------------------------
PdrTexture2D::PdrTexture2D(Renderer* renderer, bool isColortexture,
	const Texture2D* texture, bool autoGenMipMap) :
mTex(texture),
mTexture(0)
{
	Texture::Format tdFormat = texture->GetFormat();
	mInternalFormat = gOGLTextureInternalFormat[tdFormat];
	mFormat = gOGLTextureFormat[tdFormat];
	mType = gOGLTextureType[tdFormat];
	mNumLevels = texture->GetNumLevels();

	if (isColortexture)
	{
		PX2_GL_CHECK(glGenTextures(1, &mTexture));
		PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, mTexture));

		PX2_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat,
			mTex->GetWidth(), mTex->GetHeight(), 0, mFormat, mType, NULL));
	}
	else
	{
		PX2_GL_CHECK(glGenTextures(1, &mTexture));
		PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, mTexture));

		PX2_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat,
			mTex->GetWidth(), mTex->GetHeight(), 0, mFormat,
			mType, NULL));
	}

	PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}
//----------------------------------------------------------------------------
PdrTexture2D::~PdrTexture2D ()
{
	PX2_GL_CHECK(glDeleteTextures(1, &mTexture));
}
//----------------------------------------------------------------------------
void PdrTexture2D::Enable (Renderer* renderer, int textureUnit)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(textureUnit);

	assertion(false, "OpenglES should not call this function.\n");
}
//----------------------------------------------------------------------------
void PdrTexture2D::Enable (Renderer* renderer, int textureUnit, int loc)
{
	PX2_UNUSED(renderer);

	PX2_GL_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));
	PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, mTexture));
	PX2_GL_CHECK(glUniform1i(loc, textureUnit));
}
//----------------------------------------------------------------------------
void PdrTexture2D::Disable (Renderer*,int textureUnit)
{
	PX2_GL_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));
	PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}
//----------------------------------------------------------------------------
void* PdrTexture2D::Lock (int level, Buffer::Locking mode)
{
	PX2_UNUSED(mode);

	mInternalFormat = gOGLTextureInternalFormat[mTex->GetFormat()];
	Texture::Format tdFormat = mTex->GetFormat();
	mFormat = gOGLTextureFormat[tdFormat];
	mType = gOGLTextureType[tdFormat];
	int numBytes = mTex->GetNumTotalBytes();
	int numLevels = mTex->GetNumLevels();
	char *srcData = mTex->GetData(0);

	mNumLevels = mTex->GetNumLevels();
	for (level = 0; level < mNumLevels; ++level)
	{
		mNumLevelBytes[level] = mTex->GetNumLevelBytes(level);
		mDimension[0][level] = mTex->GetDimension(0, level);
		mDimension[1][level] = mTex->GetDimension(1, level);
	}
	for (/**/; level < Texture::MM_MAX_MIPMAP_LEVELS; ++level)
	{
		mNumLevelBytes[level] = 0;
		mDimension[0][level] = 0;
		mDimension[1][level] = 0;
	}

	char* newSrc = 0;
#if defined(__ANDROID__)
	int numElement = 0;
	int srcBase = 0;
	newSrc = srcData;
#else
	int numElement = 0;
	int srcBase = 0;
	if (tdFormat == Texture::TF_A8R8G8B8)
	{
		numElement = numBytes/4;
		newSrc = new1<char>(numBytes);
		for (int i = 0; i < numElement; i++)
		{
			newSrc[srcBase + 2] = srcData[srcBase    ]; // B
			newSrc[srcBase + 1] = srcData[srcBase + 1]; // G
			newSrc[srcBase + 0] = srcData[srcBase + 2]; // R
			newSrc[srcBase + 3] = srcData[srcBase + 3]; // A
			srcBase += 4;
		}
	}
	else if (tdFormat == Texture::TF_R8G8B8)
	{
		numElement = numBytes/3;
		newSrc = new1<char>(numBytes);
		for (int i = 0; i < numElement; i++)
		{
			newSrc[srcBase + 0] = srcData[srcBase + 0]; // B
			newSrc[srcBase + 1] = srcData[srcBase + 1]; // G
			newSrc[srcBase + 2] = srcData[srcBase + 2]; // R
			srcBase += 3;
		}
	}
	else if (tdFormat == Texture::TF_A4R4G4B4)
	{
		numElement = numBytes/2;
		newSrc = new1<char>(numBytes);
		for(int i=0; i<numElement; i++)
		{
			unsigned short c = *(unsigned short *)&srcData[srcBase];
			*(unsigned short *)&newSrc[srcBase] = ((c&0xf)<<12) | ((c&0xf0)<<4) | ((c&0xf00)>>4) | ((c&0xf000)>>12);
			srcBase += 2;
		}
	}
	else
	{
		newSrc = srcData;
	}
#endif

	// 创建和绑定
	PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, mTexture));

	mIsCompressed = mTex->IsCompressed();
	if (mIsCompressed)
	{
		for (level = 0; level < mNumLevels; ++level)
		{
			PX2_GL_CHECK(glCompressedTexImage2D(GL_TEXTURE_2D, level, mInternalFormat,
				mDimension[0][level], mDimension[1][level], 0,
				mNumLevelBytes[level], mTex->GetData(level)));
		}
	}
	else
	{
		int onePixelBytes = Texture::msPixelSize[tdFormat];

		char *src1 = newSrc;
		int levelByte = 0;

		for (int level = 0; level < numLevels; ++level)
		{
			int curWidth = mTex->GetDimension(0, level);
			int curHegiht = mTex->GetDimension(1, level);

			levelByte = onePixelBytes*curWidth*curHegiht;

			PX2_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, mInternalFormat,
				mDimension[0][level], mDimension[1][level], 0, mFormat,
				mType, src1));

			src1 += levelByte;
		}

		if(newSrc!=srcData)
		{
			delete1(newSrc);
		}
	}

	PX2_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

	return 0;
}
//----------------------------------------------------------------------------
void PdrTexture2D::Unlock (int level)
{
	PX2_UNUSED(level);
	assertion (false, "OpenglES does't support Unlock.");
}
//----------------------------------------------------------------------------

#endif
