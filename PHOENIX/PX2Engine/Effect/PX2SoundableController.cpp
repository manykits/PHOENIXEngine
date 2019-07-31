// PX2SoundableController.cpp

#include "PX2SoundableController.hpp"
#include "PX2Soundable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EffectableController, SoundableController);
PX2_IMPLEMENT_STREAM(SoundableController);
PX2_IMPLEMENT_FACTORY(SoundableController);
PX2_IMPLEMENT_DEFAULT_NAMES(EffectableController, SoundableController);

//----------------------------------------------------------------------------
SoundableController::SoundableController ()
{
}
//----------------------------------------------------------------------------
SoundableController::~SoundableController ()
{
}
//----------------------------------------------------------------------------
void SoundableController::Reset ()
{
	EffectableController::Reset();
	mSoundableObject = 0;
}
//----------------------------------------------------------------------------
bool SoundableController::Update(double applicationTime, double elapsedTime1)
{
	// module update
	if (!EffectableController::Update(applicationTime, elapsedTime1))
		return false;

	Soundable *soundable = (Soundable*)GetControlledable();
	float elapsedTime = (float)elapsedTime1;

	if (IsPlaying())
	{
		if (!mSoundableObject)
		{
			soundable->PlaySound();

			mSoundableObject = new0 SoundableObject();			
			EffectableController::OnNewAEffectObject(mSoundableObject);
			mSoundableObject->Volume = soundable->GetVolume(); 
			mSoundableObject->VolumeInit = mSoundableObject->Volume;
		}

		ModulesUpdateEffectObject(mSoundableObject);

		mSoundableObject->Update(soundable, elapsedTime);
	}

	soundable->GenBuffers();

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
SoundableController::SoundableController (LoadConstructor value)
	:
EffectableController(value)
{
}
//----------------------------------------------------------------------------
void SoundableController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	EffectableController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(SoundableController, source);
}
//----------------------------------------------------------------------------
void SoundableController::Link (InStream& source)
{
	EffectableController::Link(source);
}
//----------------------------------------------------------------------------
void SoundableController::PostLink ()
{
	EffectableController::PostLink();
}
//----------------------------------------------------------------------------
bool SoundableController::Register (OutStream& target) const
{
	if (EffectableController::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void SoundableController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	EffectableController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(SoundableController, target);
}
//----------------------------------------------------------------------------
int SoundableController::GetStreamingSize (Stream &stream) const
{
	int size = EffectableController::GetStreamingSize(stream)
		+ PX2_VERSION_SIZE(mVersion); 

	return size;
}
//----------------------------------------------------------------------------