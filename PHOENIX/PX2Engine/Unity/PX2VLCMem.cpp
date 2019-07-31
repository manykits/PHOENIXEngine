// PX2VLCMem.cpp

#include "PX2VLCMem.hpp"
#include "PX2Log.hpp"
using namespace PX2;


//----------------------------------------------------------------------------
VLCMem::VLCMem()
{
	mDesireWidth = 0;
	mDesireHeight = 0;
	mMediaWidth = 0;
	mMediaHeight = 0;
}
//----------------------------------------------------------------------------
VLCMem::~VLCMem()
{
}
//----------------------------------------------------------------------------
void VLCMem::OnFormatSetup()
{
}
//----------------------------------------------------------------------------
void VLCMem::SetDesireWidth(int dWidth)
{
	mDesireWidth = dWidth;
}
//----------------------------------------------------------------------------
void VLCMem::SetDesireHeight(int dHeight)
{
	mDesireHeight = dHeight;
}
//----------------------------------------------------------------------------
unsigned VLCMem::FormatCallback(char *chroma, unsigned *width, 
	unsigned *height, unsigned *pitches, unsigned *lines)
{
	if (original_media_width != mDesireWidth && original_media_height != mDesireHeight) 
	{
		float src_aspect = (float)*width / *height;
		float dst_aspect = (float)mDesireWidth / mDesireHeight;

		if (src_aspect > dst_aspect) 
		{
			if (mDesireWidth != (int)*width)
			{ //don't scale if size equal
				*width = mDesireWidth;
				*height = static_cast<unsigned>(*width / src_aspect + 0.5);
			}
		}
		else {
			if (mDesireHeight != (int)*height)
			{ //don't scale if size equal
				*height = mDesireHeight;
				*width = static_cast<unsigned>(*height * src_aspect + 0.5);
			}
		}
	}


	mMediaWidth = *width;
	mMediaHeight = *height;

	PX2_LOG_INFO("media width:%d, height:%d", mMediaWidth, mMediaHeight);

	const char DEF_CHROMA[] = "RV32";
	memcpy(chroma, DEF_CHROMA, sizeof(DEF_CHROMA) - 1);

	PX2_LOG_INFO("DEF_CHROMA");

	*pitches = mMediaWidth * DEF_PIXEL_BYTES;
	*lines = mMediaHeight;

	//+1 for vlc 2.0.3/2.1 bug workaround.
	//They writes after buffer ed boundary by some reason unknown to me...
	mFrameBuf.resize(*pitches * (*lines + 1));

	OnFormatSetup();

	PX2_LOG_INFO("OnFormatSetup");

	return 1;
}
//----------------------------------------------------------------------------
void VLCMem::CleanUpCallback()
{
	OnFrameCleanUp();

	mFrameBuf.resize(0);
	mMediaWidth = 0;
	mMediaHeight = 0;
}
//----------------------------------------------------------------------------
void* VLCMem::LockCallback(void **planes)
{
	*planes = mFrameBuf.empty() ? 0 : &mFrameBuf[0];
	return 0;
}
//----------------------------------------------------------------------------
void VLCMem::UnLockCallback(void *picture, void *const *planes)
{
}
//----------------------------------------------------------------------------
void VLCMem::DisplayCallback(void *picture)
{
	OnFrameReady(&mFrameBuf);
}
//----------------------------------------------------------------------------
unsigned VLCMem::_FormatCallback(void **opaque, char *chroma,
	unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines)
{
	return reinterpret_cast<VLCMem*>(*opaque)->FormatCallback(chroma,
		width, height,
		pitches, lines);
}
//----------------------------------------------------------------------------
void VLCMem::_CleanUpCallback(void *opaque)
{
	reinterpret_cast<VLCMem*>(opaque)->CleanUpCallback();
}
//----------------------------------------------------------------------------
void* VLCMem::_LockCallback(void *opaque, void **planes)
{
	return reinterpret_cast<VLCMem*>(opaque)->LockCallback(planes);
}
//----------------------------------------------------------------------------
void VLCMem::_UnLockCallback(void *opaque, void *picture, void *const *planes)
{
	reinterpret_cast<VLCMem*>(opaque)->UnLockCallback(picture, planes);
}
//----------------------------------------------------------------------------
void VLCMem::_DisplayCallback(void *opaque, void *picture)
{
	reinterpret_cast<VLCMem*>(opaque)->DisplayCallback(picture);
}
//----------------------------------------------------------------------------