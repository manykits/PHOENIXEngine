// PX2LightSpotConstant.cpp

#include "PX2LightSpotConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightSpotConstant);
PX2_IMPLEMENT_STREAM(LightSpotConstant);
PX2_IMPLEMENT_FACTORY(LightSpotConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightSpotConstant);

//----------------------------------------------------------------------------
LightSpotConstant::LightSpotConstant (Light* light) :
LightConstant(light)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
LightSpotConstant::~LightSpotConstant ()
{
}
//----------------------------------------------------------------------------
void LightSpotConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

	mData[0] = mLight->Angle;
	mData[1] = mLight->CosAngle;
	mData[2] = mLight->SinAngle;
	mData[3] = mLight->Exponent;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
LightSpotConstant::LightSpotConstant (LoadConstructor value) :
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightSpotConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LightSpotConstant, source);
}
//----------------------------------------------------------------------------
void LightSpotConstant::Link (InStream& source)
{
	LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightSpotConstant::PostLink ()
{
	LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightSpotConstant::Register (OutStream& target) const
{
	if (LightConstant::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LightSpotConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LightSpotConstant, target);
}
//----------------------------------------------------------------------------
int LightSpotConstant::GetStreamingSize (Stream &stream) const
{
	int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
