// PX2ShineAmbientConstant.cpp

#include "PX2ShineAmbientConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, ShineAmbientConstant);
PX2_IMPLEMENT_STREAM(ShineAmbientConstant);
PX2_IMPLEMENT_FACTORY(ShineAmbientConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, ShineAmbientConstant);

//----------------------------------------------------------------------------
ShineAmbientConstant::ShineAmbientConstant()
	:
	ShaderFloat(1)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
ShineAmbientConstant::~ShineAmbientConstant ()
{
}
//----------------------------------------------------------------------------
void ShineAmbientConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	const float* source = (const float*)renderable->GetShine()->Ambient;
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
ShineAmbientConstant::ShineAmbientConstant (LoadConstructor value)
:
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void ShineAmbientConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(ShineAmbientConstant, source);
}
//----------------------------------------------------------------------------
void ShineAmbientConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void ShineAmbientConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool ShineAmbientConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ShineAmbientConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(ShineAmbientConstant, target);
}
//----------------------------------------------------------------------------
int ShineAmbientConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
