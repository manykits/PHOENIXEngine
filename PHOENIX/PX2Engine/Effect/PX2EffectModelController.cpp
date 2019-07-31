// PX2CameraShakeController.cpp

#include "PX2EffectModelController.hpp"
#include "PX2EffectModel.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EffectableController, EffectModelController);
PX2_IMPLEMENT_STREAM(EffectModelController);
PX2_IMPLEMENT_FACTORY(EffectModelController);
PX2_IMPLEMENT_DEFAULT_NAMES(EffectableController, EffectModelController);

//----------------------------------------------------------------------------
EffectModelController::EffectModelController ()
{
}
//----------------------------------------------------------------------------
EffectModelController::~EffectModelController ()
{
}
//----------------------------------------------------------------------------
void EffectModelController::Reset ()
{
	EffectableController::Reset();

	mEffectModelObject = 0;
}
//----------------------------------------------------------------------------
bool EffectModelController::Update(double applicationTime, 
	double elapsedTime1)
{
	if (!EffectableController::Update(applicationTime, elapsedTime1))
	{
		return false;
	}

	EffectModel *cs = (EffectModel*)GetControlledable();
	float elapsedTime = (float)elapsedTime1;

	if (IsPlaying())
	{
		if (!mEffectModelObject)
		{
			mEffectModelObject = new0 EffectObject();
			EffectableController::OnNewAEffectObject(mEffectModelObject);
		}

		ModulesUpdateEffectObject(mEffectModelObject);

		mEffectModelObject->Update(cs, elapsedTime);		
	}

	cs->GenBuffers();

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
EffectModelController::EffectModelController (LoadConstructor value)
	:
EffectableController(value)
{
}
//----------------------------------------------------------------------------
void EffectModelController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	EffectableController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EffectModelController, source);
}
//----------------------------------------------------------------------------
void EffectModelController::Link (InStream& source)
{
	EffectableController::Link(source);
}
//----------------------------------------------------------------------------
void EffectModelController::PostLink ()
{
	EffectableController::PostLink();
}
//----------------------------------------------------------------------------
bool EffectModelController::Register (OutStream& target) const
{
	if (EffectableController::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EffectModelController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	EffectableController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EffectModelController, target);
}
//----------------------------------------------------------------------------
int EffectModelController::GetStreamingSize (Stream &stream) const
{
	int size = EffectableController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
