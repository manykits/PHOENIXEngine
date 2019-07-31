// PX2CameraPlayController.cpp

#include "PX2CameraPlayController.hpp"
#include "PX2Time.hpp"
#include "PX2SizeNode.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, CameraPlayController);
PX2_IMPLEMENT_STREAM(CameraPlayController);
PX2_IMPLEMENT_FACTORY(CameraPlayController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, CameraPlayController);

//----------------------------------------------------------------------------
CameraPlayController::CameraPlayController() :
mIsTouchDown(false),
mIsTouchMoved(false),
mIsMiddleDown(false),
mCameraNode(0),
mTouchUpMoveLength(0.0f),
mTouchSizeNode(0),
mIsWidgetPressed(false),
mIsSizeNodePressed(false)
{
	SetName("CameraPlayController");

	mDistance = 8.0f;
	mDegreeHor = 0.0f;
	mDegreeVer = 30.0f;

	mCameraMin = 1.0f;
	mCameraMax = 20.0f;

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
CameraPlayController::~CameraPlayController()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void CameraPlayController::SetCameraNode(CameraNode *camNode)
{
	mCameraNode = camNode;
}
//----------------------------------------------------------------------------
CameraNode *CameraPlayController::GetCameraNode()
{
	return mCameraNode;
}
//----------------------------------------------------------------------------
void CameraPlayController::SetCameraDistance(float dist)
{
	mDistance = dist;
	mDistance = Mathf::Clamp(mDistance, mCameraMin, mCameraMax);
}
//----------------------------------------------------------------------------
void CameraPlayController::SetCameraDist(float min, float max)
{
	mCameraMin = min;
	mCameraMax = max;

	mDistance = Mathf::Clamp(mDistance, mCameraMin, mCameraMax);
}
//----------------------------------------------------------------------------
void CameraPlayController::GetCameraDist(float &min, float &max)
{
	min = mCameraMin;
	max = mCameraMax;
}
//----------------------------------------------------------------------------
AVector CameraPlayController::GetCurDir()
{
	return mCurDir;
}
//----------------------------------------------------------------------------
AVector _AnglesToDirection(float angle0, float angle1)
{
	return AVector(Mathf::Cos(angle1)*Mathf::Sin(angle0),
		Mathf::Cos(angle1)*Mathf::Cos(angle0), -Mathf::Sin(angle1));
}
//----------------------------------------------------------------------------
void CameraPlayController::_Update(double applicationTime, double elapsedTime)
{
	Controller::_Update(applicationTime, elapsedTime);

	UpdateCameraRot();
	UpdateCameraPos();
}
//----------------------------------------------------------------------------
void CameraPlayController::UpdateCameraRot()
{
	// params
	AVector toDir = _AnglesToDirection(Mathf::DEG_TO_RAD*mDegreeHor,
		Mathf::DEG_TO_RAD*mDegreeVer);
	toDir.Normalize();
	mCurDir = toDir;

	Node *target = DynamicCast<Node>(GetControlledable());
	if (!target) return;
	if (!mCameraNode) return;

	AVector camDir = mCurDir;
	AVector up = AVector::UNIT_Z;
	AVector camRight = camDir.Cross(up);
	camRight.Normalize();
	AVector camUp = camRight.Cross(camDir);
	camUp.Normalize();

	AVector::Orthonormalize(camRight, camDir, camUp);
	HMatrix matRot;
	matRot.SetColumn(0, camRight);
	matRot.SetColumn(1, camDir);
	matRot.SetColumn(2, camUp);
	AVector vec;
	vec.Set(0.0f, 0.0f, 0.0f, 1.0f);
	matRot.SetColumn(3, vec);

	mCameraNode->WorldTransformIsCurrent = true;
	mCameraNode->WorldTransform.SetRotate(matRot);
	mCameraNode->Update(Time::GetTimeInSeconds(), 0.0f);

	AVector ctrlDir = mCurDir;
	ctrlDir.Z() = 0.0f;
	ctrlDir.Normalize();
	AVector ctrlUp = AVector::UNIT_Z;
	AVector ctrlRight = ctrlDir.Cross(ctrlUp);
	ctrlRight.Normalize();
}
//----------------------------------------------------------------------------
void CameraPlayController::UpdateCameraPos()
{
	Node *target = DynamicCast<Node>(GetControlledable());
	if (!target) return;
	if (!mCameraNode) return;

	// target pos
	APoint targetPos = target->WorldTransform.GetTranslate();

	// adjust camera
	APoint camToPos = targetPos - mCurDir * mDistance;

	mCameraNode->WorldTransformIsCurrent = true;
	mCameraNode->WorldTransform.SetTranslate(camToPos);

	mCameraNode->Update(Time::GetTimeInSeconds(), 0.0f);
}
//----------------------------------------------------------------------------
void CameraPlayController::SetTouchSizeNode(SizeNode *sn)
{
	mTouchSizeNode = sn;

	if (mTouchSizeNode)
	{
		mTouchSizeNode->SetWidget(true);

		mTouchSizeNode->SetMemPickCallback(this,
			(SizeNode::MemPickCallback)(&CameraPlayController::_TouchCallback));
	}
}
//----------------------------------------------------------------------------
void CameraPlayController::_TouchCallback(SizeNode *object,
	SizeNodePickType ct, const CanvasInputData &inputData)
{
	if (mTouchSizeNode == object)
	{
		if (SizeNodePickType::SNPT_WIDGET_PICKED == ct)
		{
			if (inputData.PickType == UIPT_PRESSED)
			{
				mIsWidgetPressed = true;
				mIsSizeNodePressed = true;
			}
			else if (inputData.PickType == UIPT_RELEASED)
			{
				mIsWidgetPressed = false;
				mIsSizeNodePressed = false;
			}
			else if (inputData.PickType == UIPT_MOVED)
			{
				if (mIsWidgetPressed)
				{
					mDegreeHor += inputData.MoveDelta.X() * Mathf::DEG_TO_RAD * 8.0f;
					mDegreeVer -= inputData.MoveDelta.Z() * Mathf::DEG_TO_RAD * 8.0f;
				}
			}
		}
		else if (SizeNodePickType::SNPT_SIZENODE_PICKED == ct)
		{
			mIsSizeNodePressed = true;
		}
		else if (SizeNodePickType::SNPT_SIZENODE_NOTPICKED == ct)
		{
			mIsWidgetPressed = false;
			mIsSizeNodePressed = false;
		}
	}
}
//----------------------------------------------------------------------------
SizeNode *CameraPlayController::GetTouchSizeNode()
{
	return mTouchSizeNode;
}
//----------------------------------------------------------------------------
void CameraPlayController::OnEvent(Event *ent)
{
	if (InputEventSpace::IsIn(ent))
	{
		InputEventData data = ent->GetData<InputEventData>();

		if (!mTouchSizeNode)
		{
			// mouse
			APoint worldPos = data.MTPos;

			if (InputEventSpace::MousePressed == data.TheEventType)
			{
				if (MBID_LEFT == data.MButtonID ||
					MBID_RIGHT == data.MButtonID ||
					MBID_MIDDLE == data.MButtonID)
				{
					mIsTouchDown = true;
					mIsTouchMoved = false;

					if (MBID_MIDDLE == data.MButtonID)
						mIsMiddleDown = true;

					mLastPickPos_Touch = mPickPos_Touch;
					mPickPos_Touch = data.MTPos;
				}
			}
			else if (InputEventSpace::MouseReleased == data.TheEventType)
			{
				mIsTouchDown = false;
				mIsTouchMoved = false;

				if (MBID_MIDDLE == data.MButtonID)
				{
					mIsMiddleDown = false;
				}
			}
			else if (InputEventSpace::MouseMoved == data.TheEventType)
			{
				mLastPickPos_Touch = mPickPos_Touch;
				mPickPos_Touch = data.MTPos;
				AVector moveDelta = mPickPos_Touch - mLastPickPos_Touch;

				if (mIsTouchDown)
				{
					mIsTouchMoved = true;

					mDegreeHor += moveDelta.X() * 0.5f;
					mDegreeVer -= moveDelta.Z() * 0.1f;
					mDegreeVer = Mathf::Clamp(mDegreeVer, 0.0f, 80.0f);
				}
			}
			else if (InputEventSpace::TouchPressed == data.TheEventType)
			{
				mIsTouchDown = true;
				mIsTouchMoved = false;
				mTouchUpMoveLength = 0.0f;

				mStartPickPos_Touch = data.MTPos;
				mLastPickPos_Touch = data.MTPos;
				mPickPos_Touch = data.MTPos;
			}
			else if (InputEventSpace::TouchReleased == data.TheEventType)
			{
				mIsTouchDown = false;
				mPickPos_Touch = data.MTPos;

				AVector moveDiff = mPickPos_Touch - mStartPickPos_Touch;
				mTouchUpMoveLength = Mathf::Sqrt(moveDiff.X()*moveDiff.X()
					+ moveDiff.Z()*moveDiff.Z());
			}
			else if (InputEventSpace::TouchMoved == data.TheEventType)
			{
				mIsTouchMoved = true;

				mLastPickPos_Touch = mPickPos_Touch;
				mPickPos_Touch = data.MTPos;

				if (mIsTouchDown)
				{
					AVector moveDir = mPickPos_Touch - mLastPickPos_Touch;

					mDegreeHor += moveDir.X() * 0.5f;
					mDegreeVer -= moveDir.Z() * 0.1f;
					mDegreeVer = Mathf::Clamp(mDegreeVer, 0.0f, 80.0f);
				}
			}

			if (InputEventSpace::MouseWheeled == data.TheEventType && 
				mIsWidgetPressed)
			{
				mDistance += (data.MWheel * 0.01f);
				mDistance = Mathf::Clamp(mDistance, mCameraMin, mCameraMax);
			}
		}

		if (InputEventSpace::MouseWheeled == data.TheEventType)
		{
			mDistance += (data.MWheel * 0.01f);
			mDistance = Mathf::Clamp(mDistance, mCameraMin, mCameraMax);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
CameraPlayController::CameraPlayController(LoadConstructor value) :
Controller(value),
mCameraNode(0),
mTouchSizeNode(0),
mIsWidgetPressed(false),
mIsSizeNodePressed(false)
{
}
//----------------------------------------------------------------------------
void CameraPlayController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mCameraNode);

	PX2_END_DEBUG_STREAM_LOAD(CameraPlayController, source);
}
//----------------------------------------------------------------------------
void CameraPlayController::Link(InStream& source)
{
	Controller::Link(source);

	source.ResolveLink(mCameraNode);
}
//----------------------------------------------------------------------------
void CameraPlayController::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool CameraPlayController::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		target.Register(mCameraNode);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void CameraPlayController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mCameraNode);

	PX2_END_DEBUG_STREAM_SAVE(CameraPlayController, target);
}
//----------------------------------------------------------------------------
int CameraPlayController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mCameraNode);

	return size;
}
//----------------------------------------------------------------------------