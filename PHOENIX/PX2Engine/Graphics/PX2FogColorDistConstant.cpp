// PX2FogColorDistConstant.cpp

#include "PX2FogColorDistConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
#include "PX2EnvirParamController.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, FogColorDistConstant);
PX2_IMPLEMENT_STREAM(FogColorDistConstant);
PX2_IMPLEMENT_FACTORY(FogColorDistConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, FogColorDistConstant);

//----------------------------------------------------------------------------
FogColorDistConstant::FogColorDistConstant ()
	:
ShaderFloat(1)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
FogColorDistConstant::~FogColorDistConstant ()
{
}
//----------------------------------------------------------------------------
void FogColorDistConstant::Update(const ShaderStruct *struc)
{
	PX2_UNUSED(struc);

	const float* source = (const float*)struc->TheEnvirParam
		->GetFogColorDist();
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
FogColorDistConstant::FogColorDistConstant (LoadConstructor value)
	:
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void FogColorDistConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(FogColorDistConstant, source);
}
//----------------------------------------------------------------------------
void FogColorDistConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void FogColorDistConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool FogColorDistConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void FogColorDistConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(FogColorDistConstant, target);
}
//----------------------------------------------------------------------------
int FogColorDistConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	return size;
}
//----------------------------------------------------------------------------
