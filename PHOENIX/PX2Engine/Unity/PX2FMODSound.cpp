// PX2FMODSound.cpp

#ifdef PX2_USE_FMOD

#include "PX2FMODSound.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// FMODSoundRes
//----------------------------------------------------------------------------
FMODSoundRes::FMODSoundRes() :
TheFMODSound(0),
IsWebStream(false)
{
}
//----------------------------------------------------------------------------
FMODSoundRes::~FMODSoundRes()
{
	if (TheFMODSound)
		TheFMODSound->release();

	TheFMODSound = 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
FMODSound::FMODSound(FMOD::Channel *channel, bool isWebStream)
{
	mChannel = channel;
	IsWebStream = isWebStream;
	IsRecording = false;
	IsLoop = false;
	Volume = 1.0f;
	mIsStraving = false;
}
//----------------------------------------------------------------------------
FMODSound::~FMODSound()
{
	if (mChannel)
		mChannel->stop();
}
//----------------------------------------------------------------------------
void FMODSound::SetChannel(FMOD::Channel *channel)
{
	mChannel = channel;
}
//----------------------------------------------------------------------------
FMOD::Channel *FMODSound::GetChannel()
{
	return mChannel;
}
//----------------------------------------------------------------------------
bool FMODSound::IsPlaying()
{
	bool isPlaying = false;
	mChannel->isPlaying(&isPlaying);

	return isPlaying;
}
//----------------------------------------------------------------------------
void FMODSound::SetVolume(float volume)
{
	mChannel->setVolume(volume);
}
//----------------------------------------------------------------------------
void FMODSound::SetPaused(bool paused)
{
	mChannel->setPaused(paused);
}
//----------------------------------------------------------------------------
void FMODSound::Stop()
{
	mChannel->stop();
}
//----------------------------------------------------------------------------
void FMODSound::SetPosition(const APoint &position)
{
	FMOD_VECTOR pos;
	pos.x = position.X();
	pos.y = position.Y();
	pos.z = position.Z();

	if (mChannel)
		mChannel->set3DAttributes(&pos, 0);
}
//----------------------------------------------------------------------------
void FMODSound::SetVelocity(const AVector &velocity)
{
	FMOD_VECTOR vec;
	vec.x = velocity.X();
	vec.y = velocity.Y();
	vec.z = velocity.Z();

	if (mChannel)
		mChannel->set3DAttributes(0, &vec);
}
//----------------------------------------------------------------------------
void FMODSound::SetDistance(float minDistance, float maxDistance)
{
	if (mChannel)
		mChannel->set3DMinMaxDistance(minDistance, maxDistance);
}
//----------------------------------------------------------------------------
void FMODSound::_CheckStraving()
{
	if (TheSoundRes)
	{
		unsigned int    percent = 0;
		bool            paused = false;
		FMOD_OPENSTATE   openstate = FMOD_OPENSTATE_READY;
		TheSoundRes->TheFMODSound->getOpenState(&openstate, &percent,
			&mIsStraving, 0);
	}
}
//----------------------------------------------------------------------------
bool FMODSound::Update(float elapsedSeconds)
{
	if (!Sound::Update(elapsedSeconds))
		return false;

	_CheckStraving();

	if (TheSoundRes)
	{
		bool starving = false;

		unsigned int pos = 0;
		if (mChannel)
		{
			mChannel->getPosition(&pos, FMOD_TIMEUNIT_MS);

			/* Silence the stream until we have sufficient data for smooth playback. */
			mChannel->setMute(starving);
		}
	}

	return true;
}
//----------------------------------------------------------------------------

#endif