// PX2ShineDiffuseConstant.cpp

#include "PX2ShineDiffuseConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, ShineDiffuseConstant);
PX2_IMPLEMENT_STREAM(ShineDiffuseConstant);
PX2_IMPLEMENT_FACTORY(ShineDiffuseConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, ShineDiffuseConstant);

//----------------------------------------------------------------------------
ShineDiffuseConstant::ShineDiffuseConstant () :
ShaderFloat(1)
{
    EnableUpdater();
}
//----------------------------------------------------------------------------
ShineDiffuseConstant::~ShineDiffuseConstant ()
{
}
//----------------------------------------------------------------------------
void ShineDiffuseConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	const float* source = (const float*)renderable->GetShine()->Diffuse;
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
ShineDiffuseConstant::ShineDiffuseConstant (LoadConstructor value) :
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void ShineDiffuseConstant::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

    PX2_END_DEBUG_STREAM_LOAD(ShineDiffuseConstant, source);
}
//----------------------------------------------------------------------------
void ShineDiffuseConstant::Link (InStream& source)
{
    ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void ShineDiffuseConstant::PostLink ()
{
    ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool ShineDiffuseConstant::Register (OutStream& target) const
{
    if (ShaderFloat::Register(target))
    {
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
void ShineDiffuseConstant::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

    PX2_END_DEBUG_STREAM_SAVE(ShineDiffuseConstant, target);
}
//----------------------------------------------------------------------------
int ShineDiffuseConstant::GetStreamingSize (Stream &stream) const
{
    int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

    return size;
}
//----------------------------------------------------------------------------
