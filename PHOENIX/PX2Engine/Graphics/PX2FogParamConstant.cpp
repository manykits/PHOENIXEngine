// PX2FogParamConstant.cpp

#include "PX2FogParamConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
#include "PX2EnvirParamController.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, FogParamConstant);
PX2_IMPLEMENT_STREAM(FogParamConstant);
PX2_IMPLEMENT_FACTORY(FogParamConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, FogParamConstant);

//----------------------------------------------------------------------------
FogParamConstant::FogParamConstant ()
	:
ShaderFloat(1)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
FogParamConstant::~FogParamConstant ()
{
}
//----------------------------------------------------------------------------
void FogParamConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	Float4 fogParam = renderable->UpdateFogParam(
		struc->TheEnvirParam->GetFogParam());
	const float* source = (const float*)fogParam;
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
FogParamConstant::FogParamConstant (LoadConstructor value)
	:
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void FogParamConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(FogParamConstant, source);
}
//----------------------------------------------------------------------------
void FogParamConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void FogParamConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool FogParamConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void FogParamConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(FogParamConstant, target);
}
//----------------------------------------------------------------------------
int FogParamConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	return size;
}
//----------------------------------------------------------------------------
