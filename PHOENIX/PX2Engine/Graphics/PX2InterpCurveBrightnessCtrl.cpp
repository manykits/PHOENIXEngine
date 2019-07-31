// PX2InterpCurveBrightnessCtrl.cpp

#include "PX2InterpCurveBrightnessCtrl.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloatController, InterpCurveBrightnessController);
PX2_IMPLEMENT_STREAM(InterpCurveBrightnessController);
PX2_IMPLEMENT_FACTORY(InterpCurveBrightnessController);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloatController, InterpCurveBrightnessController);

//----------------------------------------------------------------------------
InterpCurveBrightnessController::InterpCurveBrightnessController() :
InterpCurveFloatController(1.0f)
{
	SetName("ICCtrl_Brightness");

	SetRelatively(false);
}
//----------------------------------------------------------------------------
InterpCurveBrightnessController::~InterpCurveBrightnessController()
{
}
//----------------------------------------------------------------------------
void InterpCurveBrightnessController::_Update(double applicationTime,
	double elapsedTime)
{
	InterpCurveFloatController::_Update(applicationTime, elapsedTime);

	float curValue = GetCurValueRelatived();

	Movable* movable = StaticCast<Movable>(mObject);
	if (movable)
	{
		movable->SetBrightness(curValue);
	}
}
//----------------------------------------------------------------------------
void InterpCurveBrightnessController::SetControlledable(Controlledable* object)
{
	if (object)
	{
		if (IsAttachUpdateInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable) mInitValue = movable->GetBrightness();
		}
	}
	else
	{
		if (IsDetachResetInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable) movable->SetBrightness(mInitValue);
		}
	}

	InterpCurveFloatController::SetControlledable(object);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveBrightnessController::InterpCurveBrightnessController(LoadConstructor value)
:
InterpCurveFloatController(value)
{
}
//----------------------------------------------------------------------------
void InterpCurveBrightnessController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloatController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveBrightnessController, source);
}
//----------------------------------------------------------------------------
void InterpCurveBrightnessController::Link(InStream& source)
{
	InterpCurveFloatController::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveBrightnessController::PostLink()
{
	InterpCurveFloatController::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveBrightnessController::Register(OutStream& target) const
{
	return InterpCurveFloatController::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveBrightnessController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloatController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveBrightnessController, target);
}
//----------------------------------------------------------------------------
int InterpCurveBrightnessController::GetStreamingSize(Stream &stream) const
{
	int size = InterpCurveFloatController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------