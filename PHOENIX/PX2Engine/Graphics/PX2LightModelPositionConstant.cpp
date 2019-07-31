// PX2LightModelPositionConstant.cpp

#include "PX2LightModelPositionConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightModelPositionConstant);
PX2_IMPLEMENT_STREAM(LightModelPositionConstant);
PX2_IMPLEMENT_FACTORY(LightModelPositionConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightModelPositionConstant);

//----------------------------------------------------------------------------
LightModelPositionConstant::LightModelPositionConstant (Light* light) :
LightConstant(light)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
LightModelPositionConstant::~LightModelPositionConstant ()
{
}
//----------------------------------------------------------------------------
void LightModelPositionConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

	const Renderable *renderable = struc->TheRenderable;

	const APoint& worldPosition = mLight->Position;
	const HMatrix& worldInvMatrix = renderable->WorldTransform.Inverse();
	APoint modelPosition = worldInvMatrix*worldPosition;

	const float* source = (const float*)modelPosition;
	float* target = mData;
	for (int i = 0; i < 4; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
LightModelPositionConstant::LightModelPositionConstant(LoadConstructor value) :
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightModelPositionConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LightModelPositionConstant, source);
}
//----------------------------------------------------------------------------
void LightModelPositionConstant::Link (InStream& source)
{
	LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightModelPositionConstant::PostLink ()
{
	LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightModelPositionConstant::Register (OutStream& target) const
{
	if (LightConstant::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LightModelPositionConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LightModelPositionConstant, target);
}
//----------------------------------------------------------------------------
int LightModelPositionConstant::GetStreamingSize (Stream &stream) const
{
	int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
