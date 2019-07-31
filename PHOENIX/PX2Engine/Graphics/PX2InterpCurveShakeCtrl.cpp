// PX2InterpCurveShakeCtrl.cpp

#include "PX2InterpCurveShakeCtrl.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloatController, InterpCurveShakeController);
PX2_IMPLEMENT_STREAM(InterpCurveShakeController);
PX2_IMPLEMENT_FACTORY(InterpCurveShakeController);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloatController, InterpCurveShakeController);

//----------------------------------------------------------------------------
InterpCurveShakeController::InterpCurveShakeController () :
mShakeRangeType(SRT_XYZ)
{
	SetName("ICCtrl_Shake");

	SetAttachUpdateInit(true);
	SetDetachResetInit(true);
}
//----------------------------------------------------------------------------
InterpCurveShakeController::~InterpCurveShakeController ()
{
}
//----------------------------------------------------------------------------
bool InterpCurveShakeController::Update (double applicationTime, 
	double elapsedTime1)
{
	if (Active)
	{
		float elapsedTime = (float)elapsedTime1;
		float maxPlayTime = GetMaxPlayTime();

		if (IsPlaying())
		{
			mPlayedTime += elapsedTime;

			float playedTimeMinusDelay = GetPlayedTimeMinusDelay();

			if (maxPlayTime>0.0f && playedTimeMinusDelay>=maxPlayTime)
			{
				Pause();
			}

			_Update(applicationTime, elapsedTime);

			if (!IsPlaying())
			{
				if (mPlayedCallback)
					mPlayedCallback(this);

				Controlledable *ctrlAble = GetControlledable();
				if (ctrlAble && IsPlayedDoDetach())
				{
					ctrlAble->DetachController(this);
					return true;
				}
			}
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void InterpCurveShakeController::_Update(double applicationTime, double 
	elapsedTime)
{	
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	if (mPoses.GetNumPoints() != mValues.GetNumPoints())
	{
		ReCalcCurve();
	}

	float playedTimeMinusDelay = GetPlayedTimeMinusDelay();
	float ctrlTime = (float)GetControlTimeByRangeTime(playedTimeMinusDelay);

	AVector posDir = mPoses.Eval(ctrlTime, Float3::ZERO);

	Movable* movable = StaticCast<Movable>(mObject);
	if (movable)
	{	
		movable->LocalTransform.SetTranslate(mInitPos + posDir);
	}
}
//----------------------------------------------------------------------------
void InterpCurveShakeController::ReCalcCurve ()
{
	mPoses.Reset();

	for (int i=0; i<mValues.GetNumPoints(); i++)
	{
		float inVal = mValues.Points[i].InVal;
		float outVal = mValues.Points[i].OutVal;

		float rangeX = 0.0f;
		float rangeY = 0.0f;
		float rangeZ = 0.0f;

		switch (mShakeRangeType)
		{
		case SRT_X:
			rangeX = Mathf::SymmetricRandom();
			break;
		case SRT_Y:
			rangeY = Mathf::SymmetricRandom();
			break;
		case SRT_Z:
			rangeZ = Mathf::SymmetricRandom();
			break;
		case SRT_XY:
			rangeX = Mathf::SymmetricRandom();
			rangeY = Mathf::SymmetricRandom();
			break;
		case SRT_YZ:
			rangeY = Mathf::SymmetricRandom();
			rangeZ = Mathf::SymmetricRandom();
			break;
		case SRT_XZ:
			rangeX = Mathf::SymmetricRandom();
			rangeZ = Mathf::SymmetricRandom();
			break;
		case SRT_XYZ:
			rangeX = Mathf::SymmetricRandom();
			rangeY = Mathf::SymmetricRandom();
			rangeZ = Mathf::SymmetricRandom();
			break;
		default:
			break;
		}

		AVector dir = AVector(rangeX, rangeY, rangeZ);
		dir.Normalize();

		APoint pos = APoint::ORIGIN + dir*outVal;

		mPoses.AddPoint(inVal, pos, Float3::ZERO, Float3::ZERO, ICM_LINEAR);
	}
}
//----------------------------------------------------------------------------
void InterpCurveShakeController::SetControlledable(Controlledable* object)
{
	if (object)
	{
		Movable* movable = StaticCast<Movable>(mObject);
		if (movable) mInitPos = movable->LocalTransform.GetTranslate();
	}
	else
	{
		Movable* movable = StaticCast<Movable>(mObject);
		if (movable) movable->LocalTransform.SetTranslate(mInitPos);

		mPoses.Reset();
	}

	InterpCurveFloatController::SetControlledable(object);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveShakeController::InterpCurveShakeController (LoadConstructor value)
	:
InterpCurveFloatController(value),
mShakeRangeType(SRT_XYZ)
{
}
//----------------------------------------------------------------------------
void InterpCurveShakeController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloatController::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mShakeRangeType);
	source.ReadAggregate(mInitPos);
	source.ReadAggregate(mPoses);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveShakeController, source);
}
//----------------------------------------------------------------------------
void InterpCurveShakeController::Link (InStream& source)
{
	InterpCurveFloatController::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveShakeController::PostLink ()
{
	InterpCurveFloatController::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveShakeController::Register (OutStream& target) const
{
	return InterpCurveFloatController::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveShakeController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloatController::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mShakeRangeType);
	target.WriteAggregate(mInitPos);
	target.WriteAggregate(mPoses);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveShakeController, target);
}
//----------------------------------------------------------------------------
int InterpCurveShakeController::GetStreamingSize (Stream &stream) const
{
	int size = InterpCurveFloatController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_ENUMSIZE(mShakeRangeType);
	size += sizeof(mInitPos);
	size += mPoses.GetStreamSize();
	return size;
}
//----------------------------------------------------------------------------