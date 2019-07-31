// PX2LightAmbientConstant.cpp

#include "PX2LightAmbientConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightAmbientConstant);
PX2_IMPLEMENT_STREAM(LightAmbientConstant);
PX2_IMPLEMENT_FACTORY(LightAmbientConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightAmbientConstant);

//----------------------------------------------------------------------------
LightAmbientConstant::LightAmbientConstant(Light* light) :
LightConstant(light)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
LightAmbientConstant::~LightAmbientConstant ()
{
}
//----------------------------------------------------------------------------
void LightAmbientConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

    const float* source = (const float*)mLight->Ambient;
    float* target = mData;
    for (int i = 0; i < 4; ++i)
    {
        *target++ = *source++;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
LightAmbientConstant::LightAmbientConstant (LoadConstructor value) :
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightAmbientConstant::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

    PX2_END_DEBUG_STREAM_LOAD(LightAmbientConstant, source);
}
//----------------------------------------------------------------------------
void LightAmbientConstant::Link (InStream& source)
{
    LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightAmbientConstant::PostLink ()
{
    LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightAmbientConstant::Register (OutStream& target) const
{
    if (LightConstant::Register(target))
    {
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
void LightAmbientConstant::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

    PX2_END_DEBUG_STREAM_SAVE(LightAmbientConstant, target);
}
//----------------------------------------------------------------------------
int LightAmbientConstant::GetStreamingSize (Stream &stream) const
{
    int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

    return size;
}
//----------------------------------------------------------------------------
