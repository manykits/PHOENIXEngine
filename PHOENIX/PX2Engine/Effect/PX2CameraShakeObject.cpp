// PX2CameraShakeObject.cpp

#include "PX2CameraShakeObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
CameraShakeObject::CameraShakeObject () :
StrengthInit(1.0f),
Strength(1.0f)
{
}
//----------------------------------------------------------------------------
CameraShakeObject::~CameraShakeObject ()
{
}
//----------------------------------------------------------------------------
bool CameraShakeObject::Update (Effectable *able, float elapsedTime)
{
	if (!EffectObject::Update(able, elapsedTime))
		return false;

	return true;
}
//----------------------------------------------------------------------------