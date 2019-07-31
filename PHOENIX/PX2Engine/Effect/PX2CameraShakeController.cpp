// PX2CameraShakeController.cpp

#include "PX2CameraShakeController.hpp"
#include "PX2CameraShake.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EffectableController, CameraShakeController);
PX2_IMPLEMENT_STREAM(CameraShakeController);
PX2_IMPLEMENT_FACTORY(CameraShakeController);
PX2_IMPLEMENT_DEFAULT_NAMES(EffectableController, CameraShakeController);

//----------------------------------------------------------------------------
CameraShakeController::CameraShakeController ()
{
}
//----------------------------------------------------------------------------
CameraShakeController::~CameraShakeController ()
{
}
//----------------------------------------------------------------------------
void CameraShakeController::Reset ()
{
	EffectableController::Reset();

	mCameraShakeObject = 0;
}
//----------------------------------------------------------------------------
bool CameraShakeController::Update(double applicationTime, double elapsedTime1)
{
	if (!EffectableController::Update(applicationTime, elapsedTime1))
	{
		return false;
	}

	CameraShake *cs = (CameraShake*)GetControlledable();
	float elapsedTime = (float)elapsedTime1;

	if (IsPlaying())
	{
		if (!mCameraShakeObject)
		{
			mCameraShakeObject = new0 CameraShakeObject();
			EffectableController::OnNewAEffectObject(mCameraShakeObject);
			mCameraShakeObject->Strength = cs->GetEmitStrength();
			mCameraShakeObject->StrengthInit = mCameraShakeObject->Strength;
		}

		ModulesUpdateEffectObject(mCameraShakeObject);

		mCameraShakeObject->Update(cs, elapsedTime);		
	}

	cs->GenBuffers();

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
CameraShakeController::CameraShakeController (LoadConstructor value)
	:
EffectableController(value)
{
}
//----------------------------------------------------------------------------
void CameraShakeController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	EffectableController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(CameraShakeController, source);
}
//----------------------------------------------------------------------------
void CameraShakeController::Link (InStream& source)
{
	EffectableController::Link(source);
}
//----------------------------------------------------------------------------
void CameraShakeController::PostLink ()
{
	EffectableController::PostLink();
}
//----------------------------------------------------------------------------
bool CameraShakeController::Register (OutStream& target) const
{
	if (EffectableController::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void CameraShakeController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	EffectableController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(CameraShakeController, target);
}
//----------------------------------------------------------------------------
int CameraShakeController::GetStreamingSize (Stream &stream) const
{
	int size = EffectableController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
