// PX2InterpCurveTransCtrl.cpp

#include "PX2InterpCurveScaleCtrl.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloat3Controller, InterpCurveScaleController);
PX2_IMPLEMENT_STREAM(InterpCurveScaleController);
PX2_IMPLEMENT_FACTORY(InterpCurveScaleController);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloat3Controller, InterpCurveScaleController);

//----------------------------------------------------------------------------
InterpCurveScaleController::InterpCurveScaleController () :
InterpCurveFloat3Controller(Float3::UNIT)
{
	SetName("ICCtrl_Scale");

	SetPriority(13);
}
//----------------------------------------------------------------------------
InterpCurveScaleController::~InterpCurveScaleController ()
{
}
//----------------------------------------------------------------------------
void InterpCurveScaleController::_Update (double applicationTime, 
	double elapsedTime)
{	
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	float ctrlTime = (float)GetControlTimeByRangeTime(GetPlayedTimeMinusDelay());
	mCurValueRaw = mValues.Eval(ctrlTime, Float3::ZERO);
	mCurValueRelatived = mCurValueRaw;

	if (mIsRelatively)
	{
		mCurValueRelatived[0] *= mInitValue[0];
		mCurValueRelatived[1] *= mInitValue[1];
		mCurValueRelatived[2] *= mInitValue[2];
	}

	const Float3 &curValue = GetCurValueRelatived();

	Movable* movable = StaticCast<Movable>(mObject);
	if (movable)
	{	
		movable->LocalTransform.SetScale(curValue);
	}
}
//----------------------------------------------------------------------------
void InterpCurveScaleController::SetControlledable(Controlledable* object)
{
	if (object)
	{
		if (IsAttachUpdateInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable)
			{
				mInitValue = movable->LocalTransform.GetScale();
			}
		}
	}
	else
	{
		if (IsDetachResetInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable)
			{
				movable->LocalTransform.SetScale(mInitValue);
			}
		}
	}

	InterpCurveFloat3Controller::SetControlledable(object);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveScaleController::InterpCurveScaleController (LoadConstructor value)
	:
InterpCurveFloat3Controller(value)
{
}
//----------------------------------------------------------------------------
void InterpCurveScaleController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloat3Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveScaleController, source);
}
//----------------------------------------------------------------------------
void InterpCurveScaleController::Link (InStream& source)
{
	InterpCurveFloat3Controller::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveScaleController::PostLink ()
{
	InterpCurveFloat3Controller::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveScaleController::Register (OutStream& target) const
{
	return InterpCurveFloat3Controller::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveScaleController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloat3Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveScaleController, target);
}
//----------------------------------------------------------------------------
int InterpCurveScaleController::GetStreamingSize (Stream &stream) const
{
	int size = InterpCurveFloat3Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	return size;
}
//----------------------------------------------------------------------------