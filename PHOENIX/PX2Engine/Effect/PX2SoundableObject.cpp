// PX2SoundableObject.cpp

#include "PX2SoundableObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SoundableObject::SoundableObject ()
	:
VolumeInit(1.0f),
Volume(1.0f)
{
}
//----------------------------------------------------------------------------
SoundableObject::~SoundableObject ()
{
}
//----------------------------------------------------------------------------
bool SoundableObject::Update (Effectable *able, float elapsedTime)
{
	if (!EffectObject::Update(able, elapsedTime))
		return false;

	return true;
}
//----------------------------------------------------------------------------