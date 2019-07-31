// PX2InterpCurveController.cpp

#include "PX2InterpCurveController.hpp"
#include "PX2Controlledable.hpp"
#include "PX2Node.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, InterpCurveController);
PX2_IMPLEMENT_STREAM(InterpCurveController);
PX2_IMPLEMENT_FACTORY(InterpCurveController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, InterpCurveController);

//----------------------------------------------------------------------------
InterpCurveController::InterpCurveController ()
	:
mIsRelatively(true),
mIsDoAttachUpdate(true),
mIsDoDetachReset(true)
{
	SetName("InterpCurveController");
}
//----------------------------------------------------------------------------
InterpCurveController::~InterpCurveController ()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void InterpCurveController::RegistProperties ()
{
	Controller::RegistProperties();

	AddPropertyClass("InterpCurveController");
	AddProperty("IsAttachUpdateInit", PT_BOOL, IsAttachUpdateInit());
	AddProperty("IsDetachResetInit", PT_BOOL, IsDetachResetInit());
	AddProperty("IsRelatively", PT_BOOL, IsRelatively());
}
//----------------------------------------------------------------------------
void InterpCurveController::OnPropertyChanged (const PropertyObject &obj)
{
	Controller::OnPropertyChanged(obj);

	if ("IsAttachUpdateInit" == obj.Name)
	{
		SetAttachUpdateInit(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsDetachResetInit" == obj.Name)
	{
		SetDetachResetInit(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsRelatively" == obj.Name)
	{
		SetRelatively(PX2_ANY_AS(obj.Data, bool));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveController::InterpCurveController(LoadConstructor value) :
Controller(value),
mIsRelatively(true),
mIsDoAttachUpdate(true),
mIsDoDetachReset(true)
{
}
//----------------------------------------------------------------------------
void InterpCurveController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsRelatively);
	source.ReadBool(mIsDoAttachUpdate);
	source.ReadBool(mIsDoDetachReset);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveController, source);
}
//----------------------------------------------------------------------------
void InterpCurveController::Link (InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveController::PostLink ()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveController::Register (OutStream& target) const
{
	return Controller::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsRelatively);
	target.WriteBool(mIsDoAttachUpdate);
	target.WriteBool(mIsDoDetachReset);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveController, target);
}
//----------------------------------------------------------------------------
int InterpCurveController::GetStreamingSize (Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsRelatively);
	size += PX2_BOOLSIZE(mIsDoAttachUpdate);
	size += PX2_BOOLSIZE(mIsDoDetachReset);

	return size;
}
//----------------------------------------------------------------------------