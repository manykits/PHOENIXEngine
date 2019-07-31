// PX2SoundSystem.cpp

#include "PX2SoundSystem.hpp"
#include "PX2Time.hpp"
#include "PX2Log.hpp"

#ifdef PX2_USE_FMOD
#include "PX2FMODSoundSystem.hpp"
#endif

using namespace PX2;

//----------------------------------------------------------------------------
SoundSystemInitInfo::SoundSystemInitInfo() :
MaxChannels(32),
DopplerScale(1.0f),
DistanceFactor(1.0f),
RolloffScale(1.0f)
{
}
//----------------------------------------------------------------ss------------

//----------------------------------------------------------------------------
// SoundCreateInfo3D
//----------------------------------------------------------------------------
SoundCreateInfo3D::SoundCreateInfo3D() :
MinDistance(0.0f),
MaxDistance(0.0f),
Volume(1.0f),
IsLoop(true)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SoundSystem
//----------------------------------------------------------------------------
SoundSystem::SoundSystem() :
mSystemType(ST_NULL),
mDefaultMaxNumPlaySameTime(5),
mDefaultSameTimeRange(1.0f),
mIsMusicEnable(true),
mIsSoundEnable(true)
{
}
//----------------------------------------------------------------------------
SoundSystem::SoundSystem(SystemType type) :
mSystemType(type),
mDefaultMaxNumPlaySameTime(5),
mDefaultSameTimeRange(1.0f),
mIsMusicEnable(true),
mIsSoundEnable(true)
{
}
//----------------------------------------------------------------------------
SoundSystem::~SoundSystem()
{
}
//----------------------------------------------------------------------------
void SoundSystem::Clear()
{
}
//----------------------------------------------------------------------------
void SoundSystem::Update(double appSeconds, double elapsedSeconds)
{
	PX2_UNUSED(appSeconds);
	PX2_UNUSED(elapsedSeconds);
}
//----------------------------------------------------------------------------
SoundSystem *SoundSystem::Create(SystemType type,
	const SoundSystemInitInfo &initInfo)
{
	PX2_UNUSED(type);

#ifdef PX2_USE_FMOD
	FMODSoundSystem *soundSystem = new0 FMODSoundSystem();
	if (!soundSystem->Initialize(initInfo))
	{
		delete0(soundSystem);
		return 0;
	}

	return soundSystem;
#else
	PX2_UNUSED(initInfo);
	PX2_LOG_INFO("No sound system config.");
    
    SoundSystem *ss = new0 SoundSystem();
	return ss;
#endif
}
//----------------------------------------------------------------------------
void SoundSystem::SetListener(const APoint *position,
	const AVector *velocity, const AVector *forward,
	const AVector *up)
{
	if (position)
	{
		mListenPos = *position;
	}

	PX2_UNUSED(velocity);
	PX2_UNUSED(forward);
	PX2_UNUSED(up);
}
//----------------------------------------------------------------------------
void SoundSystem::PlayMusic(int channel, const char *filename,
                       bool isLoop, float fadeSeconds, float volume)
{
    PX2_UNUSED(channel);
    PX2_UNUSED(filename);
    PX2_UNUSED(isLoop);
    PX2_UNUSED(fadeSeconds);
    PX2_UNUSED(volume);
}
//----------------------------------------------------------------------------
void SoundSystem::SetMusicVolume(int channel, float volume)
{
    PX2_UNUSED(channel);
    PX2_UNUSED(volume);
}
//----------------------------------------------------------------------------
void SoundSystem::EnableMusic(bool enable)
{
	mIsMusicEnable = enable;

	for (int i = 0; i < 4; i++)
	{
		SetMusicVolume(i, enable ? 1.0f : 0.0f);
	}
}
//----------------------------------------------------------------------------
bool SoundSystem::PlaySound2DControl(const char *filename,
	float volume, bool isLoop, Sound *&sound)
{
	PX2_UNUSED(volume);
	PX2_UNUSED(isLoop);
	PX2_UNUSED(sound);

	if (!IsSoundEnable())
		return false;

	float curTime = (float)Time::GetTimeInSeconds();
	if (_CanPlaySameTime(filename, curTime))
	{
		mPlaySameTimeObjMap[filename].CurTiming = curTime;
		mPlaySameTimeObjMap[filename].CurNum++;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool SoundSystem::PlayASound(const char *filenameOrUrl, float volume, float life)
{
	if (!IsSoundEnable())
		return false;

	float curTime = (float)Time::GetTimeInSeconds();
	if (_CanPlaySameTime(filenameOrUrl, curTime))
	{
		mPlaySameTimeObjMap[filenameOrUrl].CurTiming = curTime;
		mPlaySameTimeObjMap[filenameOrUrl].CurNum++;

		return true;
	}

	PX2_UNUSED(life);

	return false;
}
//----------------------------------------------------------------------------
void SoundSystem::LoadSound(const char *filename)
{
	PX2_UNUSED(filename);
}
//----------------------------------------------------------------------------
void SoundSystem::UnloadSound(const char *filename)
{
	PX2_UNUSED(filename);
}
//----------------------------------------------------------------------------
bool SoundSystem::PlaySound3DControl(const char *filename,
	const SoundCreateInfo3D &createInfo, Sound *&sound)
{
	PX2_UNUSED(createInfo);
	PX2_UNUSED(sound);

	if (!IsSoundEnable())
		return false;

	float curTime = (float)Time::GetTimeInSeconds();
	if (_CanPlaySameTime(filename, curTime))
	{
		mPlaySameTimeObjMap[filename].CurTiming = curTime;
		mPlaySameTimeObjMap[filename].CurNum++;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void SoundSystem::EnableSounds(bool enable)
{
	mIsSoundEnable = enable;
}
//----------------------------------------------------------------------------
void SoundSystem::ClearAllSounds()
{
	mPlaySameTimeObjMap.clear();
}
//----------------------------------------------------------------------------
void SoundSystem::StartRecording(int seconds)
{
	PX2_UNUSED(seconds);
}
//----------------------------------------------------------------------------
Sound *SoundSystem::GetRecordingSound()
{
	return 0;
}
//----------------------------------------------------------------------------
void SoundSystem::StopRecording()
{
}
//----------------------------------------------------------------------------
void SoundSystem::GetRecordingBuf(unsigned char *&buf, unsigned int &size)
{
	PX2_UNUSED(buf);
	PX2_UNUSED(size);
}
//----------------------------------------------------------------------------
void SoundSystem::SetMaxNumPlaySameTime(const char *filename, int num)
{
	mPlaySameTimeObjMap[filename].MaxNum = num;
}
//----------------------------------------------------------------------------
int SoundSystem::GetMaxNumPlaySameTime(const char *filename) const
{
	std::map<FString, PlaySameTimeObj>::const_iterator it =
		mPlaySameTimeObjMap.find(filename);

	if (it != mPlaySameTimeObjMap.end())
	{
		return it->second.MaxNum;
	}

	return mDefaultMaxNumPlaySameTime;
}
//----------------------------------------------------------------------------
void SoundSystem::SetPlaySameTimeRange(const char *filename, float time)
{
	mPlaySameTimeObjMap[filename].SameTimeRange = time;
}
//----------------------------------------------------------------------------
float SoundSystem::GetPlaySameTimeRange(const char *filename) const
{
	std::map<FString, PlaySameTimeObj>::const_iterator it =
		mPlaySameTimeObjMap.find(filename);

	if (it != mPlaySameTimeObjMap.end())
	{
		return it->second.CurTiming;
	}

	return mDefaultSameTimeRange;
}
//----------------------------------------------------------------------------
bool SoundSystem::_CanPlaySameTime(const char *filename, float playTime)
{
	if (_IsFromWeb(filename))
		return true;

	// 时间判断
	std::map<FString, PlaySameTimeObj>::iterator itLastPlayTime = 
		mPlaySameTimeObjMap.find(filename);
	if (itLastPlayTime != mPlaySameTimeObjMap.end())
	{
		float timeDist = playTime - itLastPlayTime->second.CurTiming;

		if (timeDist < itLastPlayTime->second.SameTimeRange)
		{
			return false;
		}
	}

	// 数量判断
	std::map<FString, PlaySameTimeObj>::iterator it = 
		mPlaySameTimeObjMap.find(filename);
	if (it != mPlaySameTimeObjMap.end())
	{
		int curNum = it->second.CurNum;

		if (curNum >= it->second.MaxNum)
		{
			return false;
		}
	}

	if (mDefaultMaxNumPlaySameTime > 0)
		return true;

	return false;
}
//----------------------------------------------------------------------------
int SoundSystem::_GetNumPlaySameTime(const char *filename)
{
	std::map<FString, PlaySameTimeObj>::iterator it = 
		mPlaySameTimeObjMap.find(filename);

	if (it != mPlaySameTimeObjMap.end())
	{
		return it->second.MaxNum;
	}

	return 0;
}
//----------------------------------------------------------------------------
void SoundSystem::_MinusNumPlaySameTime(const char *filename)
{
	std::map<FString, PlaySameTimeObj>::iterator it = 
		mPlaySameTimeObjMap.find(filename);

	if (it != mPlaySameTimeObjMap.end())
	{
		it->second.CurNum--;

		if (it->second.CurNum < 0)
		{
			it->second.CurNum = 0;
		}
	}
}
//----------------------------------------------------------------------------
void SoundSystem::_ResetNumPlaySameTime(const char *filename)
{
	mPlaySameTimeObjMap.erase(filename);
}
//----------------------------------------------------------------------------
bool SoundSystem::_IsFromWeb(const std::string &filenameoOrUrl)
{
	std::string subHttp = std::string(filenameoOrUrl).substr(0, 4);
	if ("http" == subHttp)
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
SoundSystem::PlaySameTimeObj::PlaySameTimeObj() :
MaxNum(5),
CurNum(0),
SameTimeRange(1),
CurTiming(0.0f)
{
}
//----------------------------------------------------------------------------
