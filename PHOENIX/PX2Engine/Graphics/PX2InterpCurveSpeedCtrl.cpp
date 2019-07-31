// PX2InterpCurveSpeedCtrl.cpp

#include "PX2InterpCurveSpeedCtrl.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI_V(PX2, InterpCurveFloat3Controller, InterpCurveSpeedController, 1);
PX2_IMPLEMENT_STREAM(InterpCurveSpeedController);
PX2_IMPLEMENT_FACTORY(InterpCurveSpeedController);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloat3Controller, InterpCurveSpeedController);

//----------------------------------------------------------------------------
InterpCurveSpeedController::InterpCurveSpeedController () :
InterpCurveFloat3Controller(Float3::ZERO),
mMaxLength(-1.0f),
mAllLength(0.0f)
{
	SetName("ICCtrl_Speed");

	SetRelatively(false);
}
//----------------------------------------------------------------------------
InterpCurveSpeedController::~InterpCurveSpeedController ()
{

}
//----------------------------------------------------------------------------
void InterpCurveSpeedController::Reset ()
{
	InterpCurveFloat3Controller::Reset();

	Movable* movable = StaticCast<Movable>(mObject);
	if (movable)
	{
		movable->LocalTransform.SetTranslate(mInitPos);
	}
}
//----------------------------------------------------------------------------
void InterpCurveSpeedController::SetControlledable(Controlledable* object)
{
	if (object)
	{
		if (IsAttachUpdateInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable)
			{
				mInitPos = movable->LocalTransform.GetTranslate();
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
				movable->LocalTransform.SetTranslate(mInitPos);
			}
		}
	}

	InterpCurveFloat3Controller::SetControlledable(object);
}
//----------------------------------------------------------------------------
void InterpCurveSpeedController::_Update(double applicationTime, 
	double elapsedTime1)
{
	InterpCurveFloat3Controller::_Update(applicationTime, elapsedTime1);

	if (0.0f >= GetPlayedTimeMinusDelay())
		return;

	const AVector &curValue = GetCurValueRelatived();
	float elapsedTime = (float)elapsedTime1;

	AVector dis = curValue * elapsedTime;
	float disLength = dis.Normalize();

	float allLengthTemp = mAllLength;
	allLengthTemp += disLength;

	if (mMaxLength >= 0.0f)
	{
		if (allLengthTemp >= mMaxLength)
		{
			disLength = mMaxLength - mAllLength;
			mAllLength = mMaxLength;
			Pause();
		}
		else
		{
			mAllLength = allLengthTemp;
		}
	
		Movable* movable = StaticCast<Movable>(mObject);
		if (movable)
		{
			APoint curPos = movable->LocalTransform.GetTranslate() + dis * disLength;
			movable->LocalTransform.SetTranslate(curPos);
		}
	}
	else
	{
		Movable* movable = StaticCast<Movable>(mObject);
		if (movable)
		{
			APoint curPos = movable->LocalTransform.GetTranslate() + dis * disLength;
			movable->LocalTransform.SetTranslate(curPos);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveSpeedController::InterpCurveSpeedController (LoadConstructor value)
	:
InterpCurveFloat3Controller(value),
mMaxLength(-1.0f),
mAllLength(0.0f)
{
}
//----------------------------------------------------------------------------
void InterpCurveSpeedController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloat3Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mMaxLength);

	if (1 == GetReadedVersion())
	{
		source.ReadAggregate(mInitPos);
	}

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveSpeedController, source);
}
//----------------------------------------------------------------------------
void InterpCurveSpeedController::Link (InStream& source)
{
	InterpCurveFloat3Controller::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveSpeedController::PostLink ()
{
	InterpCurveFloat3Controller::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveSpeedController::Register (OutStream& target) const
{
	return InterpCurveFloat3Controller::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveSpeedController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloat3Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mMaxLength);
	target.WriteAggregate(mInitPos);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveSpeedController, target);
}
//----------------------------------------------------------------------------
int InterpCurveSpeedController::GetStreamingSize (Stream &stream) const
{
	int size = InterpCurveFloat3Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mMaxLength);

	if (Stream::ST_IN == stream.GetStreamType())
	{
		if (0 == GetReadedVersion())
		{
		}
		else
		{
			size += sizeof(mInitPos);
		}
	}
	else
	{
		size += sizeof(mInitPos);
	}

	return size;
}
//----------------------------------------------------------------------------