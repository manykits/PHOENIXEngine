// PX2InputPushTransformController.cpp

#include "PX2InputPushTransformController.hpp"
#include "PX2Time.hpp"
#include "PX2Math.hpp"
#include "PX2EventWorld.hpp"
#include "PX2Movable.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, PushTransformController, InputPushTransformController);
PX2_IMPLEMENT_STREAM(InputPushTransformController);
PX2_IMPLEMENT_FACTORY(InputPushTransformController);
PX2_IMPLEMENT_DEFAULT_NAMES(PushTransformController, InputPushTransformController);

//----------------------------------------------------------------------------
InputPushTransformController::InputPushTransformController() :
mPushTriggerSpeed(100.0f),
mPressedTime(0.0f),
mReleasedTime(0.0f),
mSampingTiming(0.0f),
mMinPos(AVector::ZERO),
mMaxPos(AVector::ZERO),
mMinPosSmall(AVector::ZERO),
mMaxPosSmall(AVector::ZERO),
mSideMoveLength(0.0f),
mIsPressedValid(false),
mConvertCallback(0)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
InputPushTransformController::~InputPushTransformController()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void InputPushTransformController::SetLockDir(const AVector &dir)
{
	mLockDir = dir;
	mLockDir.Normalize();
}
//----------------------------------------------------------------------------
void InputPushTransformController::SetReleaseVelocity(const AVector &velocity)
{
	mReleasedVec = velocity;
	SetVelocity(velocity);
}
//----------------------------------------------------------------------------
void InputPushTransformController::SetVelocity(const AVector &velocity)
{
	PushTransformController::SetVelocity(velocity);
}
//----------------------------------------------------------------------------
void InputPushTransformController::SetTransScope(const AVector &min,
	const AVector &max)
{
	mMinPos = min;
	mMaxPos = max;
}
//----------------------------------------------------------------------------
void InputPushTransformController::SetTransScopeSmall(const AVector &min, 
	const AVector &max)
{
	mMinPosSmall = min;
	mMaxPosSmall = max;

	mSideMoveLength = (mMinPosSmall - mMinPos).Length();
}
//----------------------------------------------------------------------------
bool InputPushTransformController::Update(double applicationTime, 
	double elapsedTime1)
{
	if (!Controller::Update(applicationTime, elapsedTime1))
		return false;

	float elapsedTime = (float)elapsedTime1;

	if (mIsPressedValid)
	{
		mSampingTiming += elapsedTime;
		if (mSampingTiming >= 0.03f)
		{
			AVector deltaVec = mCurTouchPos - mLastSamplePos;
			mLastSamplePos = mCurTouchPos;

			mTouchMoveSpeed = deltaVec / 0.03f;

			mSampingTiming = 0.0f;
		}
	}

	if (!IsSmallTransScope())
	{
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
	}
	else
	{
		if (!mIsPressedValid) // 没有按下
		{
			if (0.0f == mSideMoveLength)
				mSideMoveLength = 1.0f;

			AVector smallDir = GetSmallTransDir();
			if (AVector::ZERO != smallDir)
			{
				float moveDirLength = smallDir.Normalize();
				float adjuge = moveDirLength / mSideMoveLength;

				mVelocity += smallDir * mFriction * adjuge * adjuge * elapsedTime;
			}
		}
		else
		{
			mVelocity = AVector::ZERO;
		}
	}

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

	AVector beforeSmallDir = GetSmallTransDir();

	OnMoving(movedDis);

	if (IsSmallTransScope())
	{
		AVector smallDir = GetSmallTransDir();
		if (smallDir == AVector::ZERO && beforeSmallDir != AVector::ZERO)
		{
			SetVelocity(AVector::ZERO);
		}
	}

	return true;
}
//----------------------------------------------------------------------------
bool InputPushTransformController::IsSmallTransScope()
{
	AVector smallScope = mMaxPosSmall - mMinPosSmall;
	float lengthSquare = smallScope.SquaredLength();

	return 0.0f != lengthSquare;
}
//----------------------------------------------------------------------------
AVector InputPushTransformController::GetSmallTransDir()
{
	Movable *movable = DynamicCast<Movable>(mObject);
	if (!movable)
		AVector::ZERO;

	APoint curPos = movable->LocalTransform.GetTranslate();

	AVector moveDir = AVector::ZERO;
	if (curPos.X() < mMinPosSmall.X())
	{
		moveDir.X() = mMinPosSmall.X() - curPos.X();
	}

	if (curPos.Y() < mMinPosSmall.Y())
	{
		moveDir.Y() = mMinPosSmall.Y() - curPos.Y();
	}

	if (curPos.Z() < mMinPosSmall.Z())
	{
		moveDir.Z() = mMinPosSmall.Z() - curPos.Z();
	}

	if (curPos.X()>mMaxPosSmall.X())
	{
		moveDir.X() = mMaxPosSmall.X() - curPos.X();
	}

	if (curPos.Y() > mMaxPosSmall.Y())
	{
		moveDir.Y() = mMaxPosSmall.Y() - curPos.Y();
	}

	if (curPos.Z() > mMaxPosSmall.Z())
	{
		moveDir.Z() = mMaxPosSmall.Z() - curPos.Z();
	}

	return moveDir;
}
//----------------------------------------------------------------------------
void InputPushTransformController::OnMoving(const AVector &dis)
{
	PushTransformController::OnMoving(dis);

	Movable *movable = DynamicCast<Movable>(mObject);
	if (!movable)
		return;

	APoint curPos = movable->LocalTransform.GetTranslate();

	TransScope(curPos);

	movable->LocalTransform.SetTranslate(curPos);
}
//----------------------------------------------------------------------------
void InputPushTransformController::OnEvent(Event *event)
{
	if (!Active) return;
	if (!IsPlaying()) return;

	Movable *mov = DynamicCast<Movable>(GetControlledable());
	if (!mov) return;

	if (InputEventSpace::IsEqual(event, InputEventSpace::MousePressed) ||
		InputEventSpace::IsEqual(event,InputEventSpace::TouchPressed))
	{
		InputEventData data = event->GetData<InputEventData>();

		mIsPressedValid = true;
		mSampingTiming = 0.0f;

		mPressedTime = (float)Time::GetTimeInSeconds();
		mPressedPos = data.MTPos;

		if (mConvertCallback)
		{
			mConvertCallback(mPressedPos, (int)data.MTPos.X(), (int)data.MTPos.Z());
		}

		mCurTouchPos = mPressedPos;
		mLastTouchPos = mCurTouchPos;
		mLastSamplePos = mCurTouchPos;

		SetVelocity(AVector::ZERO);
	}
	else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseReleased) ||
		InputEventSpace::IsEqual(event, InputEventSpace::TouchReleased))
	{
		InputEventData data = event->GetData<InputEventData>();

		mSampingTiming = 0.0f;

		if (!mIsPressedValid) return;

		mIsPressedValid = false;

		mReleasedTime = (float)Time::GetTimeInSeconds();
		mReleasedPos = data.MTPos;

		if (mConvertCallback)
		{
			mConvertCallback(mPressedPos, (int)data.MTPos.X(), (int)data.MTPos.Z());
		}

		float deltaTime = mReleasedTime - mPressedTime;
		if (deltaTime <= 0.0f) deltaTime = 1.0f;

		//AVector speed = mReleasedPos - mPressedPos;
		//speed /= deltaTime;
		AVector speed = mTouchMoveSpeed;
		float judge = 0.0f;
		if (mLockDir != AVector::ZERO)
		{
			judge = mLockDir.Dot(speed);
		}
		else
		{
			judge = speed.Length();
		}
		judge = Mathf::FAbs(judge);

		if (judge >= mPushTriggerSpeed)
		{
			if (mLockDir != AVector::ZERO)
			{
				speed.X() *= mLockDir.X();
				speed.Y() *= mLockDir.Y();
				speed.Z() *= mLockDir.Z();
			}

			SetReleaseVelocity(speed * judge);
		}

		if (IsSmallTransScope() && GetSmallTransDir() != AVector::ZERO)
		{
			SetReleaseVelocity(AVector::ZERO);
		}
	}
	else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseMoved) ||
		InputEventSpace::IsEqual(event, InputEventSpace::TouchMoved))
	{
		InputEventData data = event->GetData<InputEventData>();

		if (!mIsPressedValid) return;

		mCurTouchPos = data.MTPos;

		if (mConvertCallback)
		{
			mConvertCallback(mPressedPos, (int)data.MTPos.X(), (int)data.MTPos.Z());
		}

		AVector moveDis = mCurTouchPos - mLastTouchPos;

		APoint pos = mov->LocalTransform.GetTranslate();
		pos += moveDis;
		TransScope(pos);
		mov->LocalTransform.SetTranslate(pos);

		mLastTouchPos = mCurTouchPos;
	}
	else if (InputEventSpace::IsEqual(event, InputEventSpace::TouchCancelled))
	{
		mIsPressedValid = false;
	}
}
//----------------------------------------------------------------------------
bool InputPushTransformController::TransScope(APoint &pos)
{
	if (mMinPos.X() > mMaxPos.X() || mMinPos.Y() > mMaxPos.Y() ||
		mMinPos.Z() > mMaxPos.Z() || mMinPos == mMaxPos)
	{
		return false;
	}

	if (mMinPos.X() > pos.X())
	{
		pos.X() = mMinPos.X();
		mVelocity.X() = 0.0f;
	}

	if (mMinPos.Y() > pos.Y())
	{
		pos.Y() = mMinPos.Y();
		mVelocity.Y() = 0.0f;
	}

	if (mMinPos.Z() > pos.Z())
	{
		pos.Z() = mMinPos.Z();
		mVelocity.Z() = 0.0f;
	}

	if (pos.X() > mMaxPos.X())
	{
		pos.X() = mMaxPos.X();
		mVelocity.X() = 0.0f;
	}

	if (pos.Y() > mMaxPos.Y())
	{
		pos.Y() = mMaxPos.Y();
		mVelocity.Y() = 0.0f;
	}

	if (pos.Z() > mMaxPos.Z())
	{
		pos.Z() = mMaxPos.Z();
		mVelocity.Z() = 0.0f;
	}

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
InputPushTransformController::InputPushTransformController(
	LoadConstructor value) :
PushTransformController(value),
mPushTriggerSpeed(100.0f),
mPressedTime(0.0f),
mReleasedTime(0.0f),
mSampingTiming(0.0f),
mMinPos(AVector::ZERO),
mMaxPos(AVector::ZERO),
mMinPosSmall(AVector::ZERO),
mMaxPosSmall(AVector::ZERO),
mSideMoveLength(0.0f),
mIsPressedValid(false),
mConvertCallback(0)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void InputPushTransformController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	PushTransformController::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregate(mLockDir);
	source.Read(mPushTriggerSpeed);

	PX2_END_DEBUG_STREAM_LOAD(InputPushTransformController, source);
}
//----------------------------------------------------------------------------
void InputPushTransformController::Link(InStream& source)
{
	PushTransformController::Link(source);
}
//----------------------------------------------------------------------------
void InputPushTransformController::PostLink()
{
	PushTransformController::PostLink();
}
//----------------------------------------------------------------------------
bool InputPushTransformController::Register(OutStream& target) const
{
	return PushTransformController::Register(target);
}
//----------------------------------------------------------------------------
void InputPushTransformController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	PushTransformController::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregate(mLockDir);
	target.Write(mPushTriggerSpeed);

	PX2_END_DEBUG_STREAM_SAVE(InputPushTransformController, target);
}
//----------------------------------------------------------------------------
int InputPushTransformController::GetStreamingSize(Stream &stream) const
{
	int size = PushTransformController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mLockDir);
	size += sizeof(mPushTriggerSpeed);

	return size;
}
//----------------------------------------------------------------------------
