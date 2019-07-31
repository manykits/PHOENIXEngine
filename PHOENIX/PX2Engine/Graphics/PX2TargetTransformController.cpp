// PX2TargetTransformController.cpp

#include "PX2TargetTransformController.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, TransformController, TargetTransformController);
PX2_IMPLEMENT_STREAM(TargetTransformController);
PX2_IMPLEMENT_FACTORY(TargetTransformController);
PX2_IMPLEMENT_DEFAULT_NAMES(TransformController, TargetTransformController);

//----------------------------------------------------------------------------
TargetTransformController::TargetTransformController (
	const Transform &worldTrans)
	:
TransformController(worldTrans),
mTarget(0),
mSpeed(60.0f),
mPlayedTime(0.0f),
mIsPlaying(false),
mTolerance(0.4f),
mArriveAtTargetCallback(0)
{
	SetWorld(true);
	SetStartPos(worldTrans.GetTranslate());

	SetName("TargetTransformController");
}
//----------------------------------------------------------------------------
TargetTransformController::~TargetTransformController ()
{
}
//----------------------------------------------------------------------------
void TargetTransformController::SetStartPos (const APoint &pos)
{
	mStartPos = pos;

	Movable *mov = DynamicCast<Movable>(GetControlledable());
	if (!mov)
		return;

	mov->WorldTransformIsCurrent = true;
	mov->WorldTransform.SetTranslate(mStartPos);
}
//----------------------------------------------------------------------------
void TargetTransformController::SetTarget (Movable *target)
{
	mTarget = target;
}
//----------------------------------------------------------------------------
void TargetTransformController::SetTargetPos (const APoint &pos)
{
	mTargetPos = pos;
}
//----------------------------------------------------------------------------
void TargetTransformController::SetArriveAtTargetCallback (
	TTCArriveAtTargetCallback callback)
{
	mArriveAtTargetCallback = callback;
}
//----------------------------------------------------------------------------
void TargetTransformController::OnArriveAtTarget ()
{
}
//----------------------------------------------------------------------------
void TargetTransformController::Reset ()
{
	mPlayedTime = 0.0f;
}
//----------------------------------------------------------------------------
void TargetTransformController::Play ()
{
	mIsPlaying = true;
}
//----------------------------------------------------------------------------
void TargetTransformController::Pause()
{
	mIsPlaying = false;
}
//----------------------------------------------------------------------------
void TargetTransformController::SetControlledable(Controlledable* object)
{
	if (object)
	{
		SetStartPos(mTransform.GetTranslate());
	}

	TransformController::SetControlledable(object);
}
//----------------------------------------------------------------------------
bool TargetTransformController::Update(double applicationTime, 
	double elapsedTime1)
{
	if (!Controller::Update(applicationTime, elapsedTime1))
		return false;

	Movable *mov = DynamicCast<Movable>(GetControlledable());
	if (!mov)
		return false;

	PX2_UNUSED(applicationTime);
	float elapsedTime = (float)elapsedTime1;

	if (IsPlaying())
	{
		mPlayedTime += elapsedTime;

		APoint curPos = mov->WorldTransform.GetTranslate();
		APoint targetPos;

		if (mTarget)
		{
			targetPos = mTarget->WorldTransform.GetTranslate();
		}
		else
		{
			targetPos = mTargetPos;
		}

		AVector dir = targetPos - curPos;
		float dirLength = dir.Normalize();
		PX2_UNUSED(dirLength);

		APoint tempPos = curPos + dir * mSpeed * elapsedTime;
		AVector tempDir = targetPos - tempPos;
		float tempDis = tempDir.Length();
		if (tempDis < mTolerance)
		{
			mov->WorldTransformIsCurrent = true;
			mov->WorldTransform.SetTranslate(targetPos);
			OnArriveAtTarget();
			if (mArriveAtTargetCallback)
				mArriveAtTargetCallback(this);
			Pause();
		}
		else
		{
			mov->WorldTransformIsCurrent = true;
			mov->WorldTransform.SetTranslate(tempPos);
		}
	}

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
TargetTransformController::TargetTransformController (LoadConstructor value)
	:
TransformController(value),
mTarget(0),
mSpeed(60.0f),
mPlayedTime(0.0f),
mIsPlaying(false),
mTolerance(10.0f),
mArriveAtTargetCallback(0)
{
}
//----------------------------------------------------------------------------
void TargetTransformController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TransformController::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregate(mStartPos);
	source.ReadPointer(mTarget);
	source.ReadAggregate(mTargetPos);
	source.Read(mSpeed);
	source.Read(mTolerance);

	PX2_END_DEBUG_STREAM_LOAD(TargetTransformController, source);
}
//----------------------------------------------------------------------------
void TargetTransformController::Link (InStream& source)
{
	TransformController::Link(source);

	source.ResolveLink(mTarget);
}
//----------------------------------------------------------------------------
void TargetTransformController::PostLink ()
{
	TransformController::PostLink();
}
//----------------------------------------------------------------------------
bool TargetTransformController::Register (OutStream& target) const
{
	if (TransformController::Register(target))
	{
		target.Register(mTarget);
	}

	return false;
}
//----------------------------------------------------------------------------
void TargetTransformController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TransformController::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregate(mStartPos);
	target.WritePointer(mTarget);
	target.WriteAggregate(mTargetPos);
	target.Write(mSpeed);
	target.Write(mTolerance);

	PX2_END_DEBUG_STREAM_SAVE(TargetTransformController, target);
}
//----------------------------------------------------------------------------
int TargetTransformController::GetStreamingSize (Stream &stream) const
{
	int size = TransformController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mStartPos);
	size += PX2_POINTERSIZE(mTarget);
	size += sizeof(mTargetPos);
	size += sizeof(mSpeed);
	size += sizeof(mTolerance);

	return size;
}
//----------------------------------------------------------------------------
