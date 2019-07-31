// PX2ShineSpecularConstant.cpp

#include "PX2ShineSpecularConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, ShineSpecularConstant);
PX2_IMPLEMENT_STREAM(ShineSpecularConstant);
PX2_IMPLEMENT_FACTORY(ShineSpecularConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, ShineSpecularConstant);

//----------------------------------------------------------------------------
ShineSpecularConstant::ShineSpecularConstant () :
ShaderFloat(1)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
ShineSpecularConstant::~ShineSpecularConstant ()
{
}
//----------------------------------------------------------------------------
void ShineSpecularConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	const float* source = (const float*)renderable->GetShine()->Specular;
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
ShineSpecularConstant::ShineSpecularConstant (LoadConstructor value) :
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void ShineSpecularConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(ShineSpecularConstant, source);
}
//----------------------------------------------------------------------------
void ShineSpecularConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void ShineSpecularConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool ShineSpecularConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ShineSpecularConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(ShineSpecularConstant, target);
}
//----------------------------------------------------------------------------
int ShineSpecularConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
