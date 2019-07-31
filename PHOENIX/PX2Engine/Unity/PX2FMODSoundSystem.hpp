// PX2FMODSoundSystem.hpp

#ifndef FMODSOUNDSYSTEM_HPP
#define FMODSOUNDSYSTEM_HPP

#ifdef PX2_USE_FMOD

#include "PX2UnityPre.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2FMODSound.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class FMODSoundSystem : public SoundSystem
	{
	public:
		FMODSoundSystem();
		virtual ~FMODSoundSystem();

		virtual void Clear();

		virtual void Update(double appSeconds, double elapsedSeconds);

		virtual void SetListener(const APoint *position,
			const AVector *velocity, const AVector *forward,
			const AVector *up);

		virtual void PlayMusic(int channel, const char *filename,
			bool isLoop, float fadeSeconds, float volume = 1.0f);
		virtual void SetMusicVolume(int channel, float volume);

		virtual bool PlaySound2DControl(const char *filename,
			float volume, bool isLoop, Sound *&sound);
		virtual bool PlaySound3DControl(const char *filename,
			const SoundCreateInfo3D &createInfo, Sound *&sound);
		virtual bool PlayASound(const char *filenameOrUrl, float volume = 1.0f,
			float life = 10.0f);
		virtual void ClearAllSounds();

		virtual void EnableSounds(bool enable);

		virtual void StartRecording(int seconds);
		virtual Sound *GetRecordingSound();
		virtual void StopRecording();
		virtual void GetRecordingBuf(unsigned char *&buf, unsigned int &size);

	public_internal:
		bool Initialize(const SoundSystemInitInfo &initInfo);
		bool Terminate();

	protected:
		FMOD::System *mFMODSystem;

		// Musics
		void _PlayMusicInternal(int channel, FMOD::Sound *sound,
			bool isLoop, float fadeTime, float volume = 1.0f,
			const char *path = "");
		void _ClearMusicChannel(int channel, bool main, bool fadein);

		FMOD::ChannelGroup *mChannelGroupMusic;

		struct MusicChannelInfo
		{
			MusicChannelInfo();
			~MusicChannelInfo();

			FMOD::Channel *ChannelMain;
			FMOD::Channel *ChannelFadein;
			FMOD::Sound *SoundMain;
			FMOD::Sound *SoundFadein;
			float FadeinTime; // seconds
			float FadeinTimeLeft;
			float MainVolume;
			float FadeinVolume;
			float Volume;
			std::string MainPath;
			std::string FadeinPath;
		};
		MusicChannelInfo mMusicChannel[4];

		// Sounds
		FMOD::ChannelGroup *mChannelGroupSound;

		enum SoundType
		{
			ST_2D,
			ST_3D
		};

		FMODSoundResPtr GetSoundResource(const char *fileName, SoundType type);
		FMODSoundResMap mSoundResMap;

		std::vector<SoundPtr> mSounds;
		FMODSoundPtr mRecordingSound;
	};

}

#endif

#endif