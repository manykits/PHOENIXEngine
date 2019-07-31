// PX2FMODSound.hpp

#ifndef PX2FMODSOUND_HPP
#define PX2FMODSOUND_HPP

#ifdef PX2_USE_FMOD

#include "PX2UnityPre.hpp"
#include "PX2Sound.hpp"
#include "fmod.hpp"

namespace PX2
{

	struct FMODSoundRes
	{
		FMODSoundRes();
		~FMODSoundRes();

		bool IsWebStream;

		std::string SoundFilename;
		FMOD::Sound *TheFMODSound;
	};
	typedef Pointer0 <FMODSoundRes> FMODSoundResPtr;
	typedef std::map<std::string, FMODSoundResPtr> FMODSoundResMap;

	class FMODSound : public Sound
	{
	public:
		FMODSound(FMOD::Channel *channel, bool isWebStream=false);
		virtual ~FMODSound();

		void SetChannel(FMOD::Channel *channel);
		FMOD::Channel *GetChannel();

		virtual bool IsPlaying();
		virtual void SetVolume(float volume);
		virtual void SetPaused(bool paused);
		virtual void Stop();

		virtual void SetPosition(const APoint &position);
		virtual void SetVelocity(const AVector &velocity);
		virtual void SetDistance(float minDistance, float maxDistance);
		virtual bool Update(float elapsedSeconds);

		bool IsWebStream;
		bool IsRecording;
		FMODSoundResPtr TheSoundRes;
		bool IsLoop;
		float Volume;

	private:
		FMODSound();
		void _CheckStraving();

		FMOD::Channel *mChannel;
		bool mIsStraving;
	};
	typedef Pointer0<FMODSound> FMODSoundPtr;

}

#endif

#endif
