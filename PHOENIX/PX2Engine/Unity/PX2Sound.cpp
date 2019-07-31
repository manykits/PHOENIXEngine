// PX2Sound.cpp

#include "PX2Sound.hpp"
#include "PX2SoundSystem.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Sound::Sound() :
Life(10.0f),
Age(0.0f)
{
}
//----------------------------------------------------------------------------
Sound::~Sound()
{
}
//----------------------------------------------------------------------------
void Sound::MinusNumPlaySameTime()
{
	SoundSystem *sys = SoundSystem::GetSingletonPtr();
	if (sys)
	{
		sys->_MinusNumPlaySameTime(Filename.c_str());
	}
}
//----------------------------------------------------------------------------
bool Sound::Update(float elapsedSeconds)
{
	if (Life < 0.0f)
		return true;

	Age += elapsedSeconds;

	if (Age >= Life)
		return false;

	return true;
}
//----------------------------------------------------------------------------