// PX2ShineEmissiveConstant.cpp

#include "PX2ShineEmissiveConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, ShineEmissiveConstant);
PX2_IMPLEMENT_STREAM(ShineEmissiveConstant);
PX2_IMPLEMENT_FACTORY(ShineEmissiveConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, ShineEmissiveConstant);

//----------------------------------------------------------------------------
ShineEmissiveConstant::ShineEmissiveConstant ()
:
ShaderFloat(1)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
ShineEmissiveConstant::~ShineEmissiveConstant ()
{
}
//----------------------------------------------------------------------------
void ShineEmissiveConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	const float* source = (const float*)renderable->GetShine()->Emissive;
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
ShineEmissiveConstant::ShineEmissiveConstant (LoadConstructor value)
:
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void ShineEmissiveConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(ShineEmissiveConstant, source);
}
//----------------------------------------------------------------------------
void ShineEmissiveConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void ShineEmissiveConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool ShineEmissiveConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ShineEmissiveConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(ShineEmissiveConstant, target);
}
//----------------------------------------------------------------------------
int ShineEmissiveConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------