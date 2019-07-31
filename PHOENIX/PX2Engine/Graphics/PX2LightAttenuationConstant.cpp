// PX2LightAttenuationConstant.cpp

#include "PX2LightAttenuationConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightAttenuationConstant);
PX2_IMPLEMENT_STREAM(LightAttenuationConstant);
PX2_IMPLEMENT_FACTORY(LightAttenuationConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightAttenuationConstant);

//----------------------------------------------------------------------------
LightAttenuationConstant::LightAttenuationConstant (Light* light) :
LightConstant(light)
{
}
//----------------------------------------------------------------------------
LightAttenuationConstant::~LightAttenuationConstant ()
{
}
//----------------------------------------------------------------------------
void LightAttenuationConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

    mData[0] = mLight->Constant;
    mData[1] = mLight->Linear;
    mData[2] = mLight->Quadratic;
    mData[3] = mLight->Intensity;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
LightAttenuationConstant::LightAttenuationConstant (LoadConstructor value) :
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightAttenuationConstant::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

    PX2_END_DEBUG_STREAM_LOAD(LightAttenuationConstant, source);
}
//----------------------------------------------------------------------------
void LightAttenuationConstant::Link (InStream& source)
{
    LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightAttenuationConstant::PostLink ()
{
    LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightAttenuationConstant::Register (OutStream& target) const
{
    if (LightConstant::Register(target))
    {
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
void LightAttenuationConstant::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

    PX2_END_DEBUG_STREAM_SAVE(LightAttenuationConstant, target);
}
//----------------------------------------------------------------------------
int LightAttenuationConstant::GetStreamingSize (Stream &stream) const
{
    int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

    return size;
}
//----------------------------------------------------------------------------