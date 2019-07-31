// PX2FMODSoundSystem.cpp

#include "PX2FMODSoundSystem.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Time.hpp"
#include "PX2Log.hpp"
#include "PX2SoundWav.hpp"

#ifdef PX2_USE_FMOD
#include "fmod.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// MusicChannelInfo
//----------------------------------------------------------------------------
FMODSoundSystem::MusicChannelInfo::MusicChannelInfo() :
ChannelMain(0),
ChannelFadein(0),
SoundMain(0),
SoundFadein(0),
FadeinTime(0.0f),
FadeinTimeLeft(0.0f),
MainVolume(0.0f),
FadeinVolume(0.0f),
Volume(0.0f)
{
}
//----------------------------------------------------------------------------
FMODSoundSystem::MusicChannelInfo::~MusicChannelInfo()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FMODSoundSystem
//----------------------------------------------------------------------------
FMODSoundSystem::FMODSoundSystem() :
SoundSystem(ST_FMOD),
mFMODSystem(0),
mChannelGroupMusic(0),
mChannelGroupSound(0)
{
}
//----------------------------------------------------------------------------
FMODSoundSystem::~FMODSoundSystem()
{
	Terminate();
}
//----------------------------------------------------------------------------
bool FMODSoundSystem::Initialize(const SoundSystemInitInfo &initInfo)
{
	if (FMOD::System_Create(&mFMODSystem) != FMOD_OK)
	{
		assertion(false, "FMOD::System_Create failed.");
		PX2_LOG_ERROR("FMOD::System_Create failed.");

		return false;
	}

	unsigned int version;
	if (mFMODSystem->getVersion(&version) != FMOD_OK)
	{
		assertion(false, "FMOD::getVersion failed.");
		PX2_LOG_ERROR("FMOD::getVersion failed.");

		return false;
	}

	if (version < FMOD_VERSION)
		return false;

#if defined __ANDROID__
	mFMODSystem->setOutput(FMOD_OUTPUTTYPE_OPENSL);
#endif

	if (mFMODSystem->init(initInfo.MaxChannels, FMOD_INIT_NORMAL, 0) != FMOD_OK)
	{
		assertion(false, "FMOD::init failed.");

		return false;
	}

	mFMODSystem->setStreamBufferSize(6400 * 1024, FMOD_TIMEUNIT_RAWBYTES);

	mFMODSystem->set3DSettings(initInfo.DopplerScale, initInfo.DistanceFactor,
		initInfo.RolloffScale);
	mFMODSystem->createChannelGroup(0, &mChannelGroupSound);

	PX2_LOG_INFO("FMODSoundSystem::Initialize suc.");

	return true;
}
//----------------------------------------------------------------------------
bool FMODSoundSystem::Terminate()
{
	mSounds.clear();
	mSoundResMap.clear();

	if (mFMODSystem)
		mFMODSystem->release();
	mFMODSystem = 0;

	return true;
}
//----------------------------------------------------------------------------
void FMODSoundSystem::Clear()
{
	mSoundResMap.clear();

	_ClearMusicChannel(0, true, true);
	_ClearMusicChannel(1, true, true);
	_ClearMusicChannel(2, true, true);
	_ClearMusicChannel(3, true, true);
}
//----------------------------------------------------------------------------
void FMODSoundSystem::Update(double appSeconds, double elapsedSeconds)
{
	PX2_UNUSED(appSeconds);

	float detalTime = (float)elapsedSeconds;

	for (int i = 0; i<4; i++)
	{
		if (mMusicChannel[i].FadeinTimeLeft > 0.0f)
		{
			if (mMusicChannel[i].FadeinTimeLeft > detalTime)
				mMusicChannel[i].FadeinTimeLeft -= detalTime;
			else
				mMusicChannel[i].FadeinTimeLeft = 0.0f;

			float blend = (float)mMusicChannel[i].FadeinTimeLeft /
				(float)mMusicChannel[i].FadeinTime;

			if (mMusicChannel[i].ChannelMain)
				mMusicChannel[i].ChannelMain->setVolume(
				mMusicChannel[i].MainVolume*blend);
			if (mMusicChannel[i].ChannelFadein)
				mMusicChannel[i].ChannelFadein->setVolume(
				mMusicChannel[i].FadeinVolume*(1.0f - blend));
		}

		if (mMusicChannel[i].FadeinTime != 0.0f &&
			mMusicChannel[i].FadeinTimeLeft == 0.0f)
		{
			if (mMusicChannel[i].ChannelMain)
			{
				mMusicChannel[i].ChannelMain->stop();
				mMusicChannel[i].SoundMain->release();
			}

			mMusicChannel[i].ChannelMain = mMusicChannel[i].ChannelFadein;
			mMusicChannel[i].SoundMain = mMusicChannel[i].SoundFadein;
			mMusicChannel[i].MainVolume = mMusicChannel[i].FadeinVolume;
			mMusicChannel[i].ChannelFadein = 0;
			mMusicChannel[i].SoundFadein = 0;
			mMusicChannel[i].FadeinTimeLeft = 0.0f;
		}
	}

	auto it = mSounds.begin();
	for (; it != mSounds.end();)
	{
		FMODSound *fmodSound = (FMODSound*)((Sound*)(*it));
		if (fmodSound->IsWebStream)
		{
			FMOD::Channel *channel = fmodSound->GetChannel();
			if (!channel)
			{
				FMOD::Channel *channel = 0;
				mFMODSystem->playSound(fmodSound->TheSoundRes->TheFMODSound, 
					mChannelGroupSound, true, &channel);

				if (channel)
				{
					fmodSound->SetChannel(channel);

					channel->setChannelGroup(mChannelGroupSound);
					channel->setVolume(fmodSound->Volume);
					channel->setLoopCount(fmodSound->IsLoop ? -1 : 0);
					channel->setPaused(false);
				}
				it++;
			}
			else
			{
				if (!(*it)->Update((float)elapsedSeconds))
				{
					Sound *sound = (*it);
					_ResetNumPlaySameTime(sound->Filename.c_str());
					it = mSounds.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
		else
		{
			if (!(*it)->Update((float)elapsedSeconds))
			{
				Sound *sound = (*it);
				_ResetNumPlaySameTime(sound->Filename.c_str());
				it = mSounds.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	if (mFMODSystem)
		mFMODSystem->update();
}
//----------------------------------------------------------------------------
void FMODSoundSystem::SetListener(const APoint *position,
	const AVector *velocity, const AVector *forward,
	const AVector *up)
{
	SoundSystem::SetListener(position, velocity, forward, up);

	bool hasPos = false;
	FMOD_VECTOR pos;
	if (position)
	{
		hasPos = true;
		pos.x = position->X();
		pos.y = position->Y();
		pos.z = position->Z();
	}

	bool hasVec = false;
	FMOD_VECTOR vec;
	if (velocity)
	{
		hasVec = true;
		vec.x = velocity->X();
		vec.y = velocity->Y();
		vec.z = velocity->Z();
	}

	bool hasForward = false;
	FMOD_VECTOR forw;
	if (forward)
	{
		hasForward = true;
		forw.x = forward->X();
		forw.y = forward->Y();
		forw.z = forward->Z();
	}

	bool hasUp = false;
	FMOD_VECTOR upTemp;
	if (up)
	{
		hasUp = true;
		upTemp.x = up->X();
		upTemp.y = up->Y();
		upTemp.z = up->Z();
	}

	mFMODSystem->set3DListenerAttributes(0, hasPos ? &pos : 0,
		hasVec ? &vec : 0, hasForward ? &forw : 0, hasUp ? &upTemp : 0);
}
//----------------------------------------------------------------------------
void FMODSoundSystem::PlayMusic(int channel, const char *filename,
	bool isLoop, float fadeSeconds, float volume)
{
	FMOD::Sound *sound = 0;
	MusicChannelInfo &info = mMusicChannel[channel];

	if (filename)
	{
		if (info.SoundMain && std::string(filename) == info.MainPath)
		{
			_ClearMusicChannel(channel, false, true);
			info.FadeinTime = 0.0f;
			info.FadeinTimeLeft = 0.0f;
			info.MainVolume = volume;
			info.ChannelMain->setVolume(volume);
			return;
		}
		else if (info.SoundFadein && std::string(filename) == info.FadeinPath)
		{
			info.FadeinVolume = volume;
			return;
		}
		else if (mFMODSystem->createStream(filename, FMOD_2D | FMOD_LOOP_NORMAL,
			0, &sound) != FMOD_OK)
		{
			PX2_LOG_INFO("create music stream %s failed", filename);
			sound = 0;
		}
	}

	_PlayMusicInternal(channel, sound, isLoop, fadeSeconds, volume);

	EnableMusic(IsMusicEnable());
}
//----------------------------------------------------------------------------
void FMODSoundSystem::SetMusicVolume(int channel, float volume)
{
	if (channel<0 || channel>4)
	{
		assertion(false, "channel should in right range.");
		return;
	}

	mMusicChannel[channel].Volume = volume;

	if (mMusicChannel[channel].ChannelMain)
		mMusicChannel[channel].ChannelMain->setVolume(volume);

	if (mMusicChannel[channel].ChannelFadein)
		mMusicChannel[channel].ChannelFadein->setVolume(volume);
}
//----------------------------------------------------------------------------
bool FMODSoundSystem::PlaySound2DControl(const char *filename,
	float volume, bool isLoop, Sound *&rSound)
{
	if (!SoundSystem::PlaySound2DControl(filename, volume, isLoop, rSound))
		return false;

	FMODSoundResPtr pres = GetSoundResource(filename, ST_2D);
	if (!pres) return false;

	FMOD::Sound* fSound = pres->TheFMODSound;
	FMODSound *soundObj = 0;

	if (fSound)
	{
		if (pres->IsWebStream)
		{
			FMOD::Channel *channel = 0;
			mFMODSystem->playSound(fSound, mChannelGroupSound, true, &channel);

			soundObj = new0 FMODSound(channel, true);
			soundObj->Filename = filename;
			soundObj->TheSoundRes = pres;
			soundObj->IsLoop = isLoop;
			soundObj->Volume = volume;

			if (channel)
			{
				soundObj->SetChannel(channel);

				channel->setChannelGroup(mChannelGroupSound);
				channel->setVolume(volume);
				channel->setLoopCount(isLoop ? -1 : 0);
				channel->setPaused(false);
			}
		}
		else
		{
			FMOD::Channel *channel = 0;
			mFMODSystem->playSound(fSound, mChannelGroupSound, true, &channel);
			if (channel)
			{
				channel->setChannelGroup(mChannelGroupSound);
				channel->setVolume(volume);
				channel->setLoopCount(isLoop ? -1 : 0);
				channel->setPaused(false);

				soundObj = new0 FMODSound(channel);
				soundObj->Filename = filename;
				soundObj->TheSoundRes = pres;
				soundObj->IsLoop = isLoop;
				soundObj->Volume = volume;
			}
		}
	}

	rSound = soundObj;

	return true;
}
//----------------------------------------------------------------------------
bool FMODSoundSystem::PlaySound3DControl(const char *filename,
	const SoundCreateInfo3D &createInfo, Sound *&rSound)
{
	if (!SoundSystem::PlaySound3DControl(filename, createInfo, rSound))
		return false;

	FMOD::Sound *fSound = GetSoundResource(filename, ST_3D)->TheFMODSound;
	FMODSound *soundObj = 0;

	if (fSound)
	{
		FMOD::Channel *channel = 0;
		mFMODSystem->playSound(fSound, mChannelGroupSound, true, &channel);
		if (channel)
		{
			channel->setChannelGroup(mChannelGroupSound);
			channel->setVolume(createInfo.Volume);
			channel->setLoopCount(createInfo.IsLoop ? -1 : 0);
			channel->set3DMinMaxDistance(createInfo.MinDistance,
				createInfo.MaxDistance);

			FMOD_VECTOR pos;
			pos.x = createInfo.Position.X();
			pos.y = createInfo.Position.Y();
			pos.z = createInfo.Position.Z();
			FMOD_VECTOR vec;
			vec.x = createInfo.Velocity.X();
			vec.y = createInfo.Velocity.Y();
			vec.z = createInfo.Velocity.Z();
			channel->set3DAttributes(&pos, &vec);
			channel->setPaused(false);

			soundObj = new0 FMODSound(channel);
			soundObj->Filename = filename;
		}
	}

	rSound = soundObj;

	return true;
}
//----------------------------------------------------------------------------
bool FMODSoundSystem::PlayASound(const char *filenameOrUrl, float volume,
	float life)
{
	Sound *sound;

	if (!PlaySound2DControl(filenameOrUrl, volume, false, sound))
		return false;

	if (sound)
	{
		sound = sound;
		sound->Life = life;
		mSounds.push_back(sound);
	}

	return true;
}
//----------------------------------------------------------------------------
void FMODSoundSystem::_PlayMusicInternal(int channel, FMOD::Sound *sound,
	bool isLoop, float fadeTime, float volume,
	const char *path)
{
	assertion(channel < 4, "channel should be less than 4.");

	if (channel < 0 || channel >= 4)
		return;

	mMusicChannel[channel].FadeinTime = fadeTime;
	mMusicChannel[channel].FadeinTime = fadeTime;

	if (fadeTime == 0.0f)
	{
		_ClearMusicChannel(channel, true, true);

		if (sound)
		{
			mFMODSystem->playSound(sound, mChannelGroupMusic, true,
				&mMusicChannel[channel].ChannelMain);
			mMusicChannel[channel].SoundMain = sound;
			mMusicChannel[channel].MainVolume = volume;
			mMusicChannel[channel].ChannelMain->setChannelGroup(mChannelGroupMusic);
			mMusicChannel[channel].ChannelMain->setVolume(volume);
			mMusicChannel[channel].ChannelMain->setLoopCount(isLoop ? -1 : 0);
			mMusicChannel[channel].ChannelMain->setPaused(false);
		}

		mMusicChannel[channel].MainPath = std::string(path);
	}
	else
	{
		_ClearMusicChannel(channel, false, true);

		if (sound)
		{
			mFMODSystem->playSound(sound, mChannelGroupMusic, true,
				&mMusicChannel[channel].ChannelFadein);
			mMusicChannel[channel].SoundFadein = sound;
			mMusicChannel[channel].FadeinVolume = volume;
			mMusicChannel[channel].ChannelFadein->setChannelGroup(mChannelGroupMusic);
			mMusicChannel[channel].ChannelFadein->setVolume(0.0f);
			mMusicChannel[channel].ChannelFadein->setLoopCount(isLoop ? -1 : 0);
			mMusicChannel[channel].ChannelFadein->setPaused(false);
		}

		mMusicChannel[channel].FadeinPath = std::string(path);
	}
}
//----------------------------------------------------------------------------
void FMODSoundSystem::_ClearMusicChannel(int channel, bool main, bool fadein)
{
	MusicChannelInfo &info = mMusicChannel[channel];
	if (main && info.ChannelMain)
	{
		info.ChannelMain->stop();
		info.ChannelMain = 0;
		if (info.SoundMain)
		{
			info.SoundMain->release();
			info.SoundMain = 0;
		}
	}

	if (fadein && info.ChannelFadein)
	{
		info.ChannelFadein->stop();
		info.ChannelFadein = 0;
		if (info.SoundFadein)
		{
			info.SoundFadein->release();
			info.SoundFadein = 0;
		}
	}
}
//----------------------------------------------------------------------------
FMODSoundResPtr FMODSoundSystem::GetSoundResource(
	const char *fileName, SoundType type)
{
	if (!fileName)
		return 0;

	FMODSoundResMap::iterator it = mSoundResMap.find(fileName);
	if (it != mSoundResMap.end())
	{
		return (*it).second;
	}

	if (_IsFromWeb(fileName))
	{
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.filebuffersize = 1024 * 16;

		FMOD::Sound *sound = 0;
		if (mFMODSystem->createSound(fileName, FMOD_CREATESTREAM | FMOD_NONBLOCKING, &exinfo, &sound)
			!= FMOD_OK)
		{
			assertion(false, "createSound %s failed.", fileName);
			PX2_LOG_ERROR("createSound %s failed.", fileName);
		}

		FMODSoundResPtr res = new0 FMODSoundRes;
		res->SoundFilename = std::string(fileName);
		res->TheFMODSound = sound;
		res->IsWebStream = true;
		
		// do not put in the map
		//mSoundResMap[res->SoundFilename] = res;

		return res;
	}
	else
	{
		int bufferSize = 0;
		char *buffer = 0;
		if (PX2_RM.LoadBuffer(fileName, bufferSize, buffer))
		{
			FMOD::Sound *sound = 0;
			FMOD_MODE mode = FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF | FMOD_OPENMEMORY;

			if (type == ST_2D)
				mode |= FMOD_2D;
			else
				mode |= FMOD_3D;

			FMOD_CREATESOUNDEXINFO exinfo;
			memset(&exinfo, 0, sizeof(exinfo));
			exinfo.cbsize = sizeof(exinfo);
			exinfo.length = bufferSize;

			if (mFMODSystem->createSound((const char*)buffer, mode, &exinfo,
				&sound) != FMOD_OK)
			{
				assertion(false, "createSound %s failed.", fileName);
				PX2_LOG_ERROR("createSound %s failed.", fileName);
			}

			FMODSoundResPtr res = new0 FMODSoundRes;
			res->SoundFilename = std::string(fileName);
			res->TheFMODSound = sound;
			mSoundResMap[res->SoundFilename] = res;

			return res;
		}
		else
		{
			assertion(false, "Open sound file failed: %s", fileName);
			PX2_LOG_ERROR("Open sound file failed: %s", fileName);

			return 0;
		}
	}
}
//----------------------------------------------------------------------------
void FMODSoundSystem::EnableSounds(bool enable)
{
	SoundSystem::EnableSounds(enable);

	mChannelGroupSound->setPaused(enable);
}
//----------------------------------------------------------------------------
#define RECORD_DEVICE_INDEX (0)
#define LATENCY_MS (50) /* Some devices will require higher latency to avoid glitches */
#define DRIFT_MS (1)
//----------------------------------------------------------------------------
void FMODSoundSystem::StartRecording(int seconds)
{
	int numDrivers = 0;
	FMOD_RESULT result = mFMODSystem->getRecordNumDrivers(NULL, &numDrivers);
	if (result != FMOD_OK)
	{
		PX2_LOG_ERROR("get record drivers failed!");
		assertion(false, "get record drivers failed!");
	}

	PX2_LOG_INFO("numDrivers is %d", numDrivers);

	int nativeRate = 0;
	int nativeChannels = 0;
	result = mFMODSystem->getRecordDriverInfo(RECORD_DEVICE_INDEX, NULL, 0, 
		NULL, &nativeRate, NULL, &nativeChannels, NULL);
	if (result != FMOD_OK)
	{
		PX2_LOG_ERROR("getRecordDriverInfo failed!");
		assertion(false, "getRecordDriverInfo failed!");
	}

	nativeRate = 16000;
	nativeChannels = 1;

	FMOD_CREATESOUNDEXINFO exinfo = { 0 };
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.numchannels = nativeChannels;
	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
	exinfo.defaultfrequency = nativeRate;
	exinfo.length = seconds * nativeRate * sizeof(short)* nativeChannels; /* 1 second buffer, size here doesn't change latency */

	FMOD::Sound *sound = NULL;
	result = mFMODSystem->createSound(0, FMOD_LOOP_OFF | FMOD_OPENUSER, &exinfo, &sound);
	
	mRecordingSound = new0 FMODSound(0, false);
	mRecordingSound->IsRecording = true;
	mRecordingSound->TheSoundRes = new0 FMODSoundRes();
	mRecordingSound->TheSoundRes->TheFMODSound = sound;

	result = mFMODSystem->recordStart(RECORD_DEVICE_INDEX, sound, false);
	if (result != FMOD_OK)
	{
		PX2_LOG_ERROR("recordStart failed!");
		assertion(false, "recordStart failed!");
	}

	unsigned int soundLength = 0;
	result = sound->getLength(&soundLength, FMOD_TIMEUNIT_PCM);
	if (result != FMOD_OK)
	{
		PX2_LOG_ERROR("sound getLength failed!");
		assertion(false, "sound getLength failed!");
	}
}
//----------------------------------------------------------------------------
Sound *FMODSoundSystem::GetRecordingSound()
{
	return mRecordingSound;
}
//----------------------------------------------------------------------------
void FMODSoundSystem::StopRecording()
{
	mFMODSystem->recordStop(RECORD_DEVICE_INDEX);

	//FMOD::Channel *channel = 0;
	//mFMODSystem->playSound(mRecordingSound->TheSoundRes->TheFMODSound,
	//	mChannelGroupSound, false, &channel);
	//if (channel)
	//{
	//	mRecordingSound->SetChannel(channel);

	//	channel->setChannelGroup(mChannelGroupSound);
	//	channel->setVolume(mRecordingSound->Volume);
	//	channel->setLoopCount(mRecordingSound->IsLoop ? -1 : 0);
	//	channel->setPaused(false);
	//}
}
//----------------------------------------------------------------------------
void FMODSoundSystem::GetRecordingBuf(unsigned char *&buf, unsigned int &size)
{
	//CWaveFile wave;
	//wave.read("2.wav");
	//size = wave.header->data->cb_size;
	//buf = new char[size];
	//memcpy(buf, (void*)&(wave.data[0]), size);

	unsigned char **dstBuf = &(buf);
	if (mRecordingSound)
	{
		int needSize = 7 * 16000 * sizeof(short)* 1;
		void *buf1 = 0;
		unsigned int length1 = 0;
		void *buf2 = 0;
		unsigned int length2 = 0;
		FMOD_RESULT result = mRecordingSound->TheSoundRes->TheFMODSound->lock(0,
			needSize, &buf1, &buf2, &length1, &length2);
		if (result == FMOD_OK)
		{
			if (length1 > 0)
			{
				buf = new unsigned char[length1];
				memset(buf, 0, length1);
				memcpy(buf, buf1, length1);
				size = length1;
			}
			mRecordingSound->TheSoundRes->TheFMODSound->unlock(buf1, buf2,
				length1, length2);

			Wave_header header(1, 16000, 16);
			std::string rwPath = PX2_RM.GetWriteablePath();
			CWaveFile::write(rwPath+"fmodsound.wav", header, buf, length1);
		}
		else
		{
			assertion(false, "voice locked failed");
		}
	}
}
//----------------------------------------------------------------------------
void FMODSoundSystem::ClearAllSounds()
{
	SoundSystem::ClearAllSounds();

	for (int i = 0; i < (int)mSounds.size(); i++)
	{
		Sound *sound = mSounds[i];
		if (sound)
		{
			sound->Stop();
		}
	}

	mSounds.clear();
}
//----------------------------------------------------------------------------

#endif