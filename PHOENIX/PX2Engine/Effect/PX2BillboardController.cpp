// PX2BillboardController.cpp

#include "PX2BillboardController.hpp"
#include "PX2Billboard.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EffectableController, BillboardController);
PX2_IMPLEMENT_STREAM(BillboardController);
PX2_IMPLEMENT_FACTORY(BillboardController);
PX2_IMPLEMENT_DEFAULT_NAMES(EffectableController, BillboardController);

//----------------------------------------------------------------------------
BillboardController::BillboardController () :
mBillboardObject(0)
{
}
//----------------------------------------------------------------------------
BillboardController::~BillboardController ()
{
}
//----------------------------------------------------------------------------
void BillboardController::Reset ()
{
	EffectableController::Reset();
	mBillboardObject = 0;
}
//----------------------------------------------------------------------------
bool BillboardController::Update(double applicationTime, double elapsedTime1)
{
	// module update
	if (!EffectableController::Update(applicationTime, elapsedTime1))
		return false;

	Billboard *billboard = DynamicCast<Billboard>(mObject);
	float elapsedTime = (float)elapsedTime1;

	if (IsPlaying())
	{
		if (!mBillboardObject)
		{
			mBillboardObject = new0 EffectObject();

			EffectableController::OnNewAEffectObject(mBillboardObject);
		}

		ModulesUpdateEffectObject(mBillboardObject);
		mBillboardObject->Update(billboard, elapsedTime);
	}
	else if (billboard->IsDoAlphaDisAfterStop())
	{
		if (mBillboardObject)
		{
			float curAlpha = mBillboardObject->Alpha;
			float speed = billboard->GetDoAlphaDisAfterStopSpeed();
			if (curAlpha > 0.0f)
			{
				curAlpha -= speed*elapsedTime;
				if (curAlpha < 0.0f)
					curAlpha = 0.0f;
				mBillboardObject->Alpha = curAlpha;
			}
		}
	}
	
	if (billboard->IsDynamic())
	{
		billboard->GenBuffers();
	}
	else 
	{
		if (!billboard->IsBufferEverGenerated())
		{
			billboard->GenBuffers();
		}
	}

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
BillboardController::BillboardController (LoadConstructor value)
	:
EffectableController(value),
mBillboardObject(0)
{
}
//----------------------------------------------------------------------------
void BillboardController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	EffectableController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(BillboardController, source);
}
//----------------------------------------------------------------------------
void BillboardController::Link (InStream& source)
{
	EffectableController::Link(source);
}
//----------------------------------------------------------------------------
void BillboardController::PostLink ()
{
	EffectableController::PostLink();
}
//----------------------------------------------------------------------------
bool BillboardController::Register (OutStream& target) const
{
	if (EffectableController::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void BillboardController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	EffectableController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(BillboardController, target);
}
//----------------------------------------------------------------------------
int BillboardController::GetStreamingSize (Stream &stream) const
{
	int size = EffectableController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
