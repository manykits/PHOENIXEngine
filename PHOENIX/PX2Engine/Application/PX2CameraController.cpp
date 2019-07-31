// PX2CameraController.cpp

#include "PX2CameraController.hpp"
#include "PX2CameraNode.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, CameraController);
PX2_IMPLEMENT_STREAM(CameraController);
PX2_IMPLEMENT_FACTORY(CameraController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, CameraController);

//----------------------------------------------------------------------------
CameraController::CameraController() :
mFocusMovable(0),
mDistance(15.0f),
mHorAngle(45.0f),
mVerAngle(15.0f),
mIsPressed(false)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
CameraController::~CameraController()
{
}
//----------------------------------------------------------------------------
void CameraController::SetFocusMovable(Movable *mov)
{
	mFocusMovable = mov;
}
//----------------------------------------------------------------------------
Movable *CameraController::GetFocursMovable() const
{
	return mFocusMovable;
}
//----------------------------------------------------------------------------
void CameraController::SetDistance(float distance)
{
	mDistance = distance;
}
//----------------------------------------------------------------------------
float CameraController::GetDistance() const
{
	return mDistance;
}
//----------------------------------------------------------------------------
void CameraController::SetHorAngle(float degree)
{
	mHorAngle = degree;
}
//----------------------------------------------------------------------------
float CameraController::GetHorAngle() const
{
	return mHorAngle;
}
//----------------------------------------------------------------------------
void CameraController::OnEvent(Event *ent)
{
	if (InputEventSpace::IsEqual(ent, InputEventSpace::MousePressed) ||
		InputEventSpace::IsEqual(ent, InputEventSpace::TouchPressed))
	{
		mIsPressed = true;

		InputEventData ied = ent->GetData<InputEventData>();
		mLastMouseButtonPos = ied.MTPos;
	}
	else if (InputEventSpace::IsEqual(ent, InputEventSpace::MouseReleased) ||
		InputEventSpace::IsEqual(ent, InputEventSpace::TouchReleased))
	{
		mIsPressed = false;

		InputEventData ied = ent->GetData<InputEventData>();
	}
	else if (InputEventSpace::IsEqual(ent, InputEventSpace::MouseMoved) ||
		InputEventSpace::IsEqual(ent, InputEventSpace::TouchMoved))
	{
		if (mIsPressed)
		{
			InputEventData ied = ent->GetData<InputEventData>();
			APoint mtPos = ied.MTPos;
			AVector moveDir = mtPos - mLastMouseButtonPos;
			
			float angleX = moveDir.X() * Mathf::DEG_TO_RAD * 4.0f;
			float angleZ = moveDir.Z() * Mathf::DEG_TO_RAD * 4.0f;
			mHorAngle += angleX;
			mVerAngle -= angleZ;

			mLastMouseButtonPos = mtPos;
		}
	}
}
//----------------------------------------------------------------------------
void CameraController::_Update(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	CameraNode *camNode = DynamicCast<CameraNode>(GetControlledable());
	if (!camNode)
		return;

	AVector dir = AVector::AnglesToDirection(Mathf::DEG_TO_RAD*mHorAngle,
		Mathf::DEG_TO_RAD*mVerAngle);
	dir.Normalize();

	APoint camPos = APoint::ORIGIN - dir * mDistance;
	camNode->LocalTransform.SetTranslate(camPos);
	
	if (mFocusMovable)
		camNode->LookAt(mFocusMovable);
	else
		camNode->LookAt(APoint(0.0f, 0.0f, 0.0f));
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
CameraController::CameraController(LoadConstructor value) :
Controller(value),
mFocusMovable(0),
mDistance(15.0f),
mHorAngle(45.0f),
mVerAngle(15.0f),
mIsPressed(false)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void CameraController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(CameraController, source);
}
//----------------------------------------------------------------------------
void CameraController::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void CameraController::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool CameraController::Register(OutStream& target) const
{
	return Controller::Register(target);
}
//----------------------------------------------------------------------------
void CameraController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(CameraController, target);
}
//----------------------------------------------------------------------------
int CameraController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
