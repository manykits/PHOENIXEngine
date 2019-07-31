// PX2TransformController.cpp

#include "PX2TransformController.hpp"
#include "PX2Movable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI_V(PX2, Controller, TransformController, 1);
PX2_IMPLEMENT_STREAM(TransformController);
PX2_IMPLEMENT_FACTORY(TransformController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, TransformController);

//----------------------------------------------------------------------------
TransformController::TransformController (const Transform& trans) :
mTransform(trans),
mIsWorld(false)
{
	SetName("TransformController");
}
//----------------------------------------------------------------------------
TransformController::~TransformController ()
{
}
//----------------------------------------------------------------------------
bool TransformController::Update(double applicationTime, double elapsedTime)
{
	if (!Controller::Update(applicationTime, elapsedTime))
	{
		return false;
	}

	Movable* movable = StaticCast<Movable>(mObject);

	if (mIsWorld)
	{
		movable->WorldTransform = mTransform;
	}
	else
	{
		movable->LocalTransform = mTransform;
	}

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
TransformController::TransformController (LoadConstructor value)
:
Controller(value),
mIsWorld(false)
{
}
//----------------------------------------------------------------------------
void TransformController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregate(mTransform);
	
	if (0 == GetReadedVersion())
	{
	}
	else
	{
		source.ReadBool(mIsWorld);
	}

	PX2_END_DEBUG_STREAM_LOAD(TransformController, source);
}
//----------------------------------------------------------------------------
void TransformController::Link (InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void TransformController::PostLink ()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool TransformController::Register (OutStream& target) const
{
	return Controller::Register(target);
}
//----------------------------------------------------------------------------
void TransformController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregate(mTransform);
	target.WriteBool(mIsWorld);

	PX2_END_DEBUG_STREAM_SAVE(TransformController, target);
}
//----------------------------------------------------------------------------
int TransformController::GetStreamingSize (Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += mTransform.GetStreamingSize();

	if (Stream::ST_IN == stream.GetStreamType())
	{
		if (0 == GetReadedVersion())
		{
		}
		else
		{
			size += PX2_BOOLSIZE(mIsWorld);
		}
	}
	else
	{
		size += PX2_BOOLSIZE(mIsWorld);
	}

	return size;
}
//----------------------------------------------------------------------------
