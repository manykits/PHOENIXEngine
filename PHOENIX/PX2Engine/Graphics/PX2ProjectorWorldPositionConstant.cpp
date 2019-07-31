// PX2ProjectorWorldPositionConstant.cpp

#include "PX2ProjectorWorldPositionConstant.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, ProjectorWorldPositionConstant);
PX2_IMPLEMENT_STREAM(ProjectorWorldPositionConstant);
PX2_IMPLEMENT_FACTORY(ProjectorWorldPositionConstant);

//----------------------------------------------------------------------------
ProjectorWorldPositionConstant::ProjectorWorldPositionConstant (
	Projector* projector) :
ShaderFloat(1),
mProjector(projector)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
ProjectorWorldPositionConstant::~ProjectorWorldPositionConstant ()
{
}
//----------------------------------------------------------------------------
Projector* ProjectorWorldPositionConstant::GetProjector ()
{
	return mProjector;
}
//----------------------------------------------------------------------------
void ProjectorWorldPositionConstant::Update(const ShaderStruct *)
{
	const APoint& worldPosition = mProjector->GetPosition();

	const float* source = (const float*)worldPosition;
	float* target = mData;
	for (int i = 0; i < 4; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Ãû³Æ
//----------------------------------------------------------------------------
Object* ProjectorWorldPositionConstant::GetObjectByName (
	const std::string& name)
{
	Object* found = ShaderFloat::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mProjector, name, found);
	return 0;
}
//----------------------------------------------------------------------------
void ProjectorWorldPositionConstant::GetAllObjectsByName (
	const std::string& name, std::vector<Object*>& objects)
{
	ShaderFloat::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mProjector, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
ProjectorWorldPositionConstant::ProjectorWorldPositionConstant (
	LoadConstructor value)
	:
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void ProjectorWorldPositionConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mProjector);

	PX2_END_DEBUG_STREAM_LOAD(ProjectorWorldPositionConstant, source);
}
//----------------------------------------------------------------------------
void ProjectorWorldPositionConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);

	source.ResolveLink(mProjector);
}
//----------------------------------------------------------------------------
void ProjectorWorldPositionConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool ProjectorWorldPositionConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		target.Register(mProjector);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ProjectorWorldPositionConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mProjector);

	PX2_END_DEBUG_STREAM_SAVE(ProjectorWorldPositionConstant, target);
}
//----------------------------------------------------------------------------
int ProjectorWorldPositionConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_POINTERSIZE(mProjector);
	return size;
}
//----------------------------------------------------------------------------
