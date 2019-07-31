// PX2InterpCurveAlphaCtrl.cpp

#include "PX2InterpCurveAlphaCtrl.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloatController, InterpCurveAlphaController);
PX2_IMPLEMENT_STREAM(InterpCurveAlphaController);
PX2_IMPLEMENT_FACTORY(InterpCurveAlphaController);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloatController, InterpCurveAlphaController);

//----------------------------------------------------------------------------
InterpCurveAlphaController::InterpCurveAlphaController () :
InterpCurveFloatController(1.0f)
{
	SetName("ICCtrl_Alpha");

	SetRelatively(false);
}
//----------------------------------------------------------------------------
InterpCurveAlphaController::~InterpCurveAlphaController ()
{
}
//----------------------------------------------------------------------------
void InterpCurveAlphaController::_Update(double applicationTime,
	double elapsedTime)
{
	InterpCurveFloatController::_Update(applicationTime, elapsedTime);

	float curValue = GetCurValueRelatived();

	Movable* movable = StaticCast<Movable>(mObject);
	if (movable)
	{
		movable->SetAlpha(curValue);
	}
}
//----------------------------------------------------------------------------
void InterpCurveAlphaController::SetControlledable(Controlledable* object)
{
	InterpCurveFloatController::SetControlledable(object);

	if (object)
	{
		if (IsAttachUpdateInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable) mInitValue = movable->GetAlpha();
		}
	}
	else
	{
		if (IsDetachResetInit())
		{
			Movable* movable = StaticCast<Movable>(mObject);
			if (movable) movable->SetAlpha(mInitValue);
		}
	}

	InterpCurveFloatController::SetControlledable(object);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveAlphaController::InterpCurveAlphaController (LoadConstructor value)
	:
InterpCurveFloatController(value)
{
}
//----------------------------------------------------------------------------
void InterpCurveAlphaController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloatController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveAlphaController, source);
}
//----------------------------------------------------------------------------
void InterpCurveAlphaController::Link (InStream& source)
{
	InterpCurveFloatController::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveAlphaController::PostLink ()
{
	InterpCurveFloatController::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveAlphaController::Register (OutStream& target) const
{
	return InterpCurveFloatController::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveAlphaController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloatController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveAlphaController, target);
}
//----------------------------------------------------------------------------
int InterpCurveAlphaController::GetStreamingSize (Stream &stream) const
{
	int size = InterpCurveFloatController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------