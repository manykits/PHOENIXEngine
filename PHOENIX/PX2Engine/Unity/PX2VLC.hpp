// PX2VLC.hpp

#ifndef PX2VLC_HPP
#define PX2VLC_HPP

#include "PX2VLCPre.hpp"
#include "PX2SmartPointer.hpp"

#if defined PX2_USE_VLC

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_player_t;

#endif

namespace PX2
{

	class VLCMem;

	class PX2_ENGINE_ITEM VLC
	{
	public:
		VLC();
		~VLC();

		static void Initlize();
		static void Ternimate();

		void Start(const std::string &filename);
		void Pause(bool pause);
		void Stop();

		enum State
		{
			S_NONE,
			S_OPENING,
			S_BUFFERING,
			S_PLAYING,
			S_PAUSED,
			S_STOPPED,
			S_ENDED,
			S_MAX_STATE
		};
		State GetState() const;

		int GetLengthMicroSeconds() const;
		int GetPlayingMicroSeconds() const;

		void SetPercent(float percent);

		void SetMem(VLCMem *mem);

	private:

#if defined PX2_USE_VLC
		static libvlc_instance_t *sInst;
		libvlc_media_t *mMedia;
		libvlc_media_player_t *mMediaPlayer;
#endif

		State mState;
		VLCMem *mVLCMem;
	};
	typedef Pointer0<VLC> VLCPtr;

}

#endif