// PX2LightWorldPositionConstant.cpp

#include "PX2LightWorldPositionConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightWorldPositionConstant);
PX2_IMPLEMENT_STREAM(LightWorldPositionConstant);
PX2_IMPLEMENT_FACTORY(LightWorldPositionConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightWorldPositionConstant)

//----------------------------------------------------------------------------
LightWorldPositionConstant::LightWorldPositionConstant (Light* light) :
LightConstant(light)
{
}
//----------------------------------------------------------------------------
LightWorldPositionConstant::~LightWorldPositionConstant ()
{
}
//----------------------------------------------------------------------------
void LightWorldPositionConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

	const APoint& worldPosition = mLight->Position;

	const float* source = (const float*)worldPosition;
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
LightWorldPositionConstant::LightWorldPositionConstant(LoadConstructor value) :
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightWorldPositionConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LightWorldPositionConstant, source);
}
//----------------------------------------------------------------------------
void LightWorldPositionConstant::Link (InStream& source)
{
	LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightWorldPositionConstant::PostLink ()
{
	LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightWorldPositionConstant::Register (OutStream& target) const
{
	if (LightConstant::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LightWorldPositionConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LightWorldPositionConstant, target);
}
//----------------------------------------------------------------------------
int LightWorldPositionConstant::GetStreamingSize (Stream &stream) const
{
	int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
