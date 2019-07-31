// PX2VLC.cpp

#include "PX2VLC.hpp"
#include "PX2VLCMem.hpp"
#include "PX2Log.hpp"
#include "PX2Application.hpp"
using namespace PX2;

#if defined PX2_USE_VLC
#include "vlc/vlc.h"
#endif

#if defined PX2_USE_VLC
libvlc_instance_t *VLC::sInst = 0;
#endif
//----------------------------------------------------------------------------
VLC::VLC()
{
#if defined PX2_USE_VLC
	mMediaPlayer = 0;
	mMedia = 0;
#endif
}
//----------------------------------------------------------------------------
unsigned video_format_stub(void**, char*, unsigned*, unsigned*, unsigned*, unsigned*)
{
	return 0;
}
void* video_fb_lock_stub(void*, void** planes)
{
	planes[0] = planes[1] = planes[3] = 0;
	return 0;
}
//----------------------------------------------------------------------------
VLC::~VLC()
{
#if defined PX2_USE_VLC
	if (mMediaPlayer)
	{
		libvlc_video_set_callbacks(mMediaPlayer, video_fb_lock_stub, 0, 0, 0);
		libvlc_video_set_format_callbacks(mMediaPlayer, video_format_stub, 0);

		libvlc_media_player_stop(mMediaPlayer);
		libvlc_media_player_release(mMediaPlayer);
		mMediaPlayer = 0;
	}
#endif
}
//----------------------------------------------------------------------------
void VLC::Initlize()
{
#if defined PX2_USE_VLC
	if (0 == sInst)
	{
		sInst = libvlc_new(0, 0);

		if (!sInst)
		{
			PX2_LOG_ERROR("libvlc_new %s", libvlc_errmsg());
		}
	}
#endif
}
//----------------------------------------------------------------------------
void VLC::Ternimate()
{
#if defined PX2_USE_VLC
	if (sInst)
	{
		libvlc_release(sInst);
		sInst = 0;
	}
#endif
}
//----------------------------------------------------------------------------
void VLC::Start(const std::string &filename)
{
	PX2_LOG_INFO("vlc start filename %s", filename.c_str());

#if defined PX2_USE_VLC
	mMedia = libvlc_media_new_location(sInst, filename.c_str());
	//mMedia = libvlc_media_new_path(sInst, filename.c_str());

	mMediaPlayer = libvlc_media_player_new_from_media(mMedia);
	libvlc_media_release(mMedia);
	mMedia = 0;

	void *ptData = PX2_APP.GetPt_Data();
	void *ptData1 = PX2_APP.GetPt_Data1();

#if defined(_WIN32)
	PX2_UNUSED(ptData1);
	libvlc_media_player_set_hwnd(mMediaPlayer, ptData);
#elif defined(__LINUX__)
	libvlc_media_player_set_xwindow(mMediaPlayer, *((uint32_t*)ptData1));
#endif

	//on windows
	//libvlc_media_player_set_hwnd(mMPlayer, screen_hwnd);

	libvlc_media_player_play(mMediaPlayer);

#endif

	if (mVLCMem)
	{
		SetMem(mVLCMem);
	}
}
//----------------------------------------------------------------------------
void VLC::Pause(bool pause)
{
	int doPause = 0;
	if (pause)
		doPause = 1;
	
#if defined PX2_USE_VLC
	if (mMedia)
		libvlc_media_player_set_pause(mMediaPlayer, doPause);
#endif
}
//----------------------------------------------------------------------------
void VLC::Stop()
{
#if defined PX2_USE_VLC
	if (mMediaPlayer)
	{
		libvlc_media_player_stop(mMediaPlayer);
		libvlc_media_player_release(mMediaPlayer);
	}

	mMediaPlayer = 0;
#endif
}
//----------------------------------------------------------------------------
VLC::State VLC::GetState() const
{
	State state = S_NONE;

#if defined PX2_USE_VLC

	if (mMediaPlayer)
	{
		libvlc_state_t st = libvlc_media_player_get_state(mMediaPlayer);
		if (libvlc_Opening == st)
		{
			state = S_OPENING;
		}
		else if (libvlc_Buffering == st)
		{
			state = S_BUFFERING;
		}
		else if (libvlc_Playing == st)
		{
			state = S_PLAYING;
		}
		else if (libvlc_Paused == st)
		{
			state = S_PAUSED;
		}  
		else if (libvlc_Stopped == st)
		{
			state = S_STOPPED;
		}
		else if (libvlc_Ended == st)
		{
			state = S_ENDED;
		}
	}

#endif
	
	return state;
}
//----------------------------------------------------------------------------
int VLC::GetLengthMicroSeconds() const
{
	int length = 0;
#if defined PX2_USE_VLC
	if (mMediaPlayer)
		length = (int)libvlc_media_player_get_length(mMediaPlayer);
#endif

	return length;
}
//----------------------------------------------------------------------------
int VLC::GetPlayingMicroSeconds() const
{
	int curtime = 0;
#if defined PX2_USE_VLC
	if (mMediaPlayer)
		curtime = (int)libvlc_media_player_get_time(mMediaPlayer);
#endif

	return curtime;
}
//----------------------------------------------------------------------------
void VLC::SetPercent(float percent)
{
#if defined PX2_USE_VLC
	if (mMediaPlayer)
		libvlc_media_player_set_position(mMediaPlayer, percent);
#endif
}
//----------------------------------------------------------------------------
void VLC::SetMem(VLCMem *mem)
{
	mVLCMem = mem;

#if defined PX2_USE_VLC
	if (mMediaPlayer)
	{
		libvlc_video_set_callbacks(mMediaPlayer,
			VLCMem::_LockCallback, VLCMem::_UnLockCallback, VLCMem::_DisplayCallback, mem);

		libvlc_video_set_format_callbacks(mMediaPlayer,
			VLCMem::_FormatCallback, VLCMem::_CleanUpCallback);
	}
#endif
}
//----------------------------------------------------------------------------