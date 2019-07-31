// PX2PushTransformController.cpp

#include "PX2PushTransformController.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, PushTransformController);
PX2_IMPLEMENT_STREAM(PushTransformController);
PX2_IMPLEMENT_FACTORY(PushTransformController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, PushTransformController);

//----------------------------------------------------------------------------
PushTransformController::PushTransformController ()
	:
mTransType(TT_TRANSLATE),
	mMaxVelocity(400.0f),
	mFriction(200.0f),
	mIsXY(false)
{
}
//----------------------------------------------------------------------------
PushTransformController::~PushTransformController ()
{
}
//----------------------------------------------------------------------------
void PushTransformController::AddVelocity (const AVector &addedVec)
{
	AVector vec = GetVelocity();
	vec += addedVec;
	SetVelocity(vec);
}
//----------------------------------------------------------------------------
void PushTransformController::SetVelocity (const AVector &vec)
{
	AVector vecTemp = vec;
	float vecLength = vecTemp.Normalize();

	if (vecLength > mMaxVelocity)
	{
		vecLength = mMaxVelocity;
	}

	mVelocity = vecTemp*vecLength;
}
//----------------------------------------------------------------------------
bool PushTransformController::Update(double applicationTime, 
	double elapsedTime1)
{
	if (!Controller::Update(applicationTime, elapsedTime1))
		return false;

	if (mVelocity == AVector::ZERO)
		return true;

	float elapsedTime = (float)elapsedTime1;

	AVector velocityDis = mVelocity*elapsedTime;
	AVector movedDis;

	if (IsXY())
	{
		movedDis.X() = velocityDis.X();
		movedDis.Y() = velocityDis.Y();
	}
	else
	{
		movedDis.X() = velocityDis.X();
		movedDis.Z() = velocityDis.Z();
	}

	OnMoving(movedDis);
	
	// 减小速度
	AVector vecTemp = mVelocity;
	float vecLength = vecTemp.Normalize();
	vecLength -= mFriction * elapsedTime;
	if (vecLength <= 0.0f)
	{
		SetVelocity(AVector::ZERO);
	}
	else
	{
		SetVelocity(vecTemp * vecLength);
	}

	return true;
}
//----------------------------------------------------------------------------
void PushTransformController::OnMoving (const AVector &dis)
{
	Movable *movable = DynamicCast<Movable>(mObject);
	if (!movable)
		return;
	
	APoint curPos = movable->LocalTransform.GetTranslate();
	
	// 移动
	if (TT_TRANSLATE == mTransType)
	{
		movable->LocalTransform.SetTranslate(curPos+dis);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
PushTransformController::PushTransformController (LoadConstructor value)
	:
Controller(value),
mTransType(TT_TRANSLATE),
mMaxVelocity(100.0f),
mFriction(10.0f),
mIsXY(false)
{
}
//----------------------------------------------------------------------------
void PushTransformController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mTransType);
	source.Read(mMaxVelocity);
	source.Read(mFriction);
	source.ReadAggregate(mVelocity);

	PX2_END_DEBUG_STREAM_LOAD(PushTransformController, source);
}
//----------------------------------------------------------------------------
void PushTransformController::Link (InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void PushTransformController::PostLink ()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool PushTransformController::Register (OutStream& target) const
{
	return Controller::Register(target);
}
//----------------------------------------------------------------------------
void PushTransformController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mTransType);
	target.Write(mMaxVelocity);
	target.Write(mFriction);
	target.WriteAggregate(mVelocity);

	PX2_END_DEBUG_STREAM_SAVE(PushTransformController, target);
}
//----------------------------------------------------------------------------
int PushTransformController::GetStreamingSize (Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mTransType);
	size += sizeof(mMaxVelocity);
	size += sizeof(mFriction);
	size += sizeof(mVelocity);

	return size;
}
//----------------------------------------------------------------------------
