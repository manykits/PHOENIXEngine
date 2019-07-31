// PX2InterpCurveTransCtrl.cpp

#include "PX2InterpCurveTranslateCtrl.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloat3Controller, InterpCurveTranslateController);
PX2_IMPLEMENT_STREAM(InterpCurveTranslateController);
PX2_IMPLEMENT_FACTORY(InterpCurveTranslateController);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloat3Controller, InterpCurveTranslateController);

//----------------------------------------------------------------------------
InterpCurveTranslateController::InterpCurveTranslateController () :
InterpCurveFloat3Controller(Float3::ZERO),
mIsWorld(false)
{
	SetName("ICCtrl_Translate");

	SetPriority(11);
}
//----------------------------------------------------------------------------
InterpCurveTranslateController::~InterpCurveTranslateController ()
{
}
//----------------------------------------------------------------------------
void InterpCurveTranslateController::SetWorld(bool world)
{
	mIsWorld = world;
}
//----------------------------------------------------------------------------
bool InterpCurveTranslateController::IsWorld() const
{
	return mIsWorld;
}
//----------------------------------------------------------------------------
void InterpCurveTranslateController::Reset ()
{
	InterpCurveFloat3Controller::Reset();

	Movable* movable = StaticCast<Movable>(mObject);
	if (movable)
	{
		movable->LocalTransform.SetTranslate(mInitValue);
	}
}
//----------------------------------------------------------------------------
void InterpCurveTranslateController::_Update (double applicationTime,
	double elapsedTime)
{
	InterpCurveFloat3Controller::_Update(applicationTime, elapsedTime);

	const Float3 &curValue = GetCurValueRelatived();
	
	Movable* movable = StaticCast<Movable>(mObject);
	if (movable)
	{
		if (!IsWorld())
		{
			movable->LocalTransform.SetTranslate(curValue);
		}
		else
		{
			APoint curPos = curValue;
			Movable *parent = DynamicCast<Movable>(movable->GetParent());
			if (parent)
			{
				APoint toPos = parent->WorldTransform.InverseTransform() 
					* curPos;
				movable->LocalTransform.SetTranslate(toPos);
			}
		}
	}
}
//----------------------------------------------------------------------------
void InterpCurveTranslateController::SetControlledable(Controlledable* object)
{
	if (object)
	{
		if (IsAttachUpdateInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable)
			{
				mInitValue = movable->LocalTransform.GetTranslate();
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
				movable->LocalTransform.SetTranslate(mInitValue);
			}
		}
	}

	InterpCurveFloat3Controller::SetControlledable(object);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveTranslateController::InterpCurveTranslateController (
	LoadConstructor value) :
InterpCurveFloat3Controller(value),
mIsWorld(false)
{
}
//----------------------------------------------------------------------------
void InterpCurveTranslateController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloat3Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsWorld);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveTranslateController, source);
}
//----------------------------------------------------------------------------
void InterpCurveTranslateController::Link (InStream& source)
{
	InterpCurveFloat3Controller::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveTranslateController::PostLink ()
{
	InterpCurveFloat3Controller::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveTranslateController::Register (OutStream& target) const
{
	return InterpCurveFloat3Controller::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveTranslateController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloat3Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsWorld);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveTranslateController, target);
}
//----------------------------------------------------------------------------
int InterpCurveTranslateController::GetStreamingSize (Stream &stream) const
{
	int size = InterpCurveFloat3Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsWorld);

	return size;
}
//----------------------------------------------------------------------------