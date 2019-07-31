// PX2FogColorHeightConstant.cpp

#include "PX2FogColorHeightConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
#include "PX2EnvirParamController.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, FogColorHeightConstant);
PX2_IMPLEMENT_STREAM(FogColorHeightConstant);
PX2_IMPLEMENT_FACTORY(FogColorHeightConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, FogColorHeightConstant);

//----------------------------------------------------------------------------
FogColorHeightConstant::FogColorHeightConstant ()
	:
ShaderFloat(1)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
FogColorHeightConstant::~FogColorHeightConstant ()
{
}
//----------------------------------------------------------------------------
void FogColorHeightConstant::Update(const ShaderStruct *struc)
{
	PX2_UNUSED(struc);

	const float* source = (const float*)struc->TheEnvirParam
		->GetFogColorHeight();
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
FogColorHeightConstant::FogColorHeightConstant (LoadConstructor value) :
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void FogColorHeightConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(FogColorHeightConstant, source);
}
//----------------------------------------------------------------------------
void FogColorHeightConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void FogColorHeightConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool FogColorHeightConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void FogColorHeightConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(FogColorHeightConstant, target);
}
//----------------------------------------------------------------------------
int FogColorHeightConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	return size;
}
//----------------------------------------------------------------------------
