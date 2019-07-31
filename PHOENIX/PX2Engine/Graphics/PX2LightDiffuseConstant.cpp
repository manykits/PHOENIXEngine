// PX2LightDiffuseConstant.cpp

#include "PX2LightDiffuseConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightDiffuseConstant);
PX2_IMPLEMENT_STREAM(LightDiffuseConstant);
PX2_IMPLEMENT_FACTORY(LightDiffuseConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightDiffuseConstant);

//----------------------------------------------------------------------------
LightDiffuseConstant::LightDiffuseConstant (Light* light) :
LightConstant(light)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
LightDiffuseConstant::~LightDiffuseConstant ()
{
}
//----------------------------------------------------------------------------
void LightDiffuseConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

	const float* source = (const float*)mLight->Diffuse;
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
LightDiffuseConstant::LightDiffuseConstant (LoadConstructor value) :
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightDiffuseConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LightDiffuseConstant, source);
}
//----------------------------------------------------------------------------
void LightDiffuseConstant::Link (InStream& source)
{
	LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightDiffuseConstant::PostLink ()
{
	LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightDiffuseConstant::Register (OutStream& target) const
{
	if (LightConstant::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LightDiffuseConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LightDiffuseConstant, target);
}
//----------------------------------------------------------------------------
int LightDiffuseConstant::GetStreamingSize (Stream &stream) const
{
	int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
