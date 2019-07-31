// PX2UIVLCMemObj.cpp

#include "PX2UIVLCMemObj.hpp"
#include "PX2Log.hpp"
#include "PX2Time.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
VLCMemObj::VLCMemObj() :
mTex2D(0),
mIsTextureUpdated(true),
mIsShowPic(true)
{
	mFPS = 30;
	mLastReadyTime = 0.0f;
}
//----------------------------------------------------------------------------
VLCMemObj::~VLCMemObj()
{
}
//----------------------------------------------------------------------------
bool VLCMemObj::_CheckUpdateTime()
{
	float time = Time::GetTimeInSeconds();
	float space = time - mLastReadyTime;
	if (space > 1.0f / (float)mFPS)
	{
		mLastReadyTime = time;
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void VLCMemObj::SetMediaWidthHeight(int width, int height)
{
	mMediaWidth = width;
	mMediaHeight = height;
}
//----------------------------------------------------------------------------
int VLCMemObj::GetMediaWidth() const
{
	return mMediaWidth;
}
//----------------------------------------------------------------------------
int VLCMemObj::GetMediaHeight() const
{
	return mMediaHeight;
}
//----------------------------------------------------------------------------
const std::vector<char> &VLCMemObj::GetLastBuffer()
{
	return mLastFrameBuf;
}
//----------------------------------------------------------------------------
void VLCMemObj::ShowPic(bool isShow)
{
	mIsShowPic = isShow;
}
//----------------------------------------------------------------------------
void VLCMemObj::OnFormatSetup()
{
	if (mMediaWidth > 0 && mMediaHeight > 0)
	{
		mTex2D = new0 Texture2D(Texture::TF_A8R8G8B8, mMediaWidth, mMediaHeight, 1);
	}
	else
	{
		mTex2D = 0;
	}
}
//----------------------------------------------------------------------------
void VLCMemObj::OnFrameReady(const std::vector<char>* frameBuf)
{
	if (!_CheckUpdateTime())
		return;

	if (mIsShowPic)
	{
		if (mMediaWidth > 0 && mMediaHeight > 0)
		{
			const std::vector<char> &fromBufs = *frameBuf;
			mLastFrameBuf = fromBufs;

			int width = mTex2D->GetWidth();
			int height = mTex2D->GetHeight();
			char* pDest = mTex2D->GetData(0);

			int offsetSrc = 0;
			int offsetDst = 0;

			for (int row = 0; row < width; ++row)
			{
				for (int col = 0; col < height; ++col)
				{
#if defined (__ANDROID__)
					pDest[offsetDst] = fromBufs[offsetSrc + 2];
					pDest[offsetDst + 1] = fromBufs[offsetSrc + 1];
					pDest[offsetDst + 2] = fromBufs[offsetSrc + 0];
					pDest[offsetDst + 3] = fromBufs[offsetSrc + 3];
#else
					pDest[offsetDst] = fromBufs[offsetSrc + 0];			// b
					pDest[offsetDst + 1] = fromBufs[offsetSrc + 1];		// g 
					pDest[offsetDst + 2] = fromBufs[offsetSrc + 2];		// r
					pDest[offsetDst + 3] = fromBufs[offsetSrc + 3];
#endif

					offsetSrc += 4;
					offsetDst += 4;
		}
	}
}

		ScopedCS cs(&mMutex);
		{
			mIsTextureUpdated = true;
		}
	}
}
//----------------------------------------------------------------------------
void VLCMemObj::OnFrameReady(int width, int height,
	const char* buf, int size)
{
	if (!_CheckUpdateTime())
		return;

	if (mMediaWidth > 0 && mMediaHeight > 0)
	{
		int width = mTex2D->GetWidth();
		int height = mTex2D->GetHeight();
		char* pDest = mTex2D->GetData(0);

		int offsetSrc = 0;
		int offsetDst = 0;

		for (int col = 0; col < width; ++col)
		{
			for (int row = 0; row < height; ++row)
			{
				if (4 == size)
				{
#if defined (__ANDROID__)
					pDest[offsetDst] = buf[offsetSrc + 0];			// b
					pDest[offsetDst + 1] = buf[offsetSrc + 1];		// g 
					pDest[offsetDst + 2] = buf[offsetSrc + 2];		// r
					pDest[offsetDst + 3] = buf[offsetSrc + 3];
#else
					pDest[offsetDst] = buf[offsetSrc + 0];			// b
					pDest[offsetDst + 1] = buf[offsetSrc + 1];		// g 
					pDest[offsetDst + 2] = buf[offsetSrc + 2];		// r
					pDest[offsetDst + 3] = buf[offsetSrc + 3];
#endif
					offsetSrc += 4;
					offsetDst += 4;
				}
				else if (3 == size)
				{
#if defined (__ANDROID__)
					pDest[offsetDst] = buf[offsetSrc + 0];			// b
					pDest[offsetDst + 1] = buf[offsetSrc + 1];		// g 
					pDest[offsetDst + 2] = buf[offsetSrc + 2];		// r
					pDest[offsetDst + 3] = 255;
#else
					pDest[offsetDst] = buf[offsetSrc + 2];			// b
					pDest[offsetDst + 1] = buf[offsetSrc + 1];		// g 
					pDest[offsetDst + 2] = buf[offsetSrc + 0];		// r
					pDest[offsetDst + 3] = 255;
#endif

					offsetSrc += 3;
					offsetDst += 4;
				}
			}
		}
	}

	ScopedCS cs(&mMutex);
	{
		mIsTextureUpdated = true;
	}
}
//----------------------------------------------------------------------------
void VLCMemObj::OnFrameCleanUp()
{
}
//----------------------------------------------------------------------------
void VLCMemObj::SetTextureUpdated(bool updated)
{
	ScopedCS cs(&mMutex);
	mIsTextureUpdated = updated;
}
//----------------------------------------------------------------------------
bool VLCMemObj::IsTextureUpdated() const
{
	return mIsTextureUpdated;
}
//----------------------------------------------------------------------------
Texture2D *VLCMemObj::GetTex2D()
{
	return mTex2D;
}
//----------------------------------------------------------------------------