// PX2InterpCurveColorCtrl.cpp

#include "PX2InterpCurveColorCtrl.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloat3Controller, InterpCurveColorController);
PX2_IMPLEMENT_STREAM(InterpCurveColorController);
PX2_IMPLEMENT_FACTORY(InterpCurveColorController);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloat3Controller, InterpCurveColorController);

//----------------------------------------------------------------------------
InterpCurveColorController::InterpCurveColorController() :
InterpCurveFloat3Controller(Float3::ZERO)
{
	SetName("ICCtrl_Color");

	SetRelatively(false);
}
//----------------------------------------------------------------------------
InterpCurveColorController::~InterpCurveColorController()
{
}
//----------------------------------------------------------------------------
void InterpCurveColorController::_Update(double applicationTime,
	double elapsedTime)
{
	InterpCurveFloat3Controller::_Update(applicationTime, elapsedTime);

	const Float3 &curValue = GetCurValueRelatived();

	Movable* movable = StaticCast<Movable>(mObject);
	if (movable)
	{
		movable->SetColor(curValue);
	}
}
//----------------------------------------------------------------------------
void InterpCurveColorController::SetControlledable(Controlledable* object)
{
	if (object)
	{
		if (IsAttachUpdateInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable)
			{
				mInitValue = movable->GetColor();
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
				movable->SetColor(mInitValue);
			}
		}
	}

	InterpCurveFloat3Controller::SetControlledable(object);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveColorController::InterpCurveColorController(LoadConstructor value) :
InterpCurveFloat3Controller(value)
{
}
//----------------------------------------------------------------------------
void InterpCurveColorController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloat3Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveColorController, source);
}
//----------------------------------------------------------------------------
void InterpCurveColorController::Link(InStream& source)
{
	InterpCurveFloat3Controller::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveColorController::PostLink()
{
	InterpCurveFloat3Controller::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveColorController::Register(OutStream& target) const
{
	return InterpCurveFloat3Controller::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveColorController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloat3Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveColorController, target);
}
//----------------------------------------------------------------------------
int InterpCurveColorController::GetStreamingSize(Stream &stream) const
{
	int size = InterpCurveFloat3Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	return size;
}
//----------------------------------------------------------------------------