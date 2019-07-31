// PX2LightSpecularConstant.cpp

#include "PX2LightSpecularConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightSpecularConstant);
PX2_IMPLEMENT_STREAM(LightSpecularConstant);
PX2_IMPLEMENT_FACTORY(LightSpecularConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightSpecularConstant);

//----------------------------------------------------------------------------
LightSpecularConstant::LightSpecularConstant (Light* light) :
LightConstant(light)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
LightSpecularConstant::~LightSpecularConstant ()
{
}
//----------------------------------------------------------------------------
void LightSpecularConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

	const float* source = (const float*)mLight->Specular;
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
LightSpecularConstant::LightSpecularConstant (LoadConstructor value)
:
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightSpecularConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LightSpecularConstant, source);
}
//----------------------------------------------------------------------------
void LightSpecularConstant::Link (InStream& source)
{
	LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightSpecularConstant::PostLink ()
{
	LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightSpecularConstant::Register (OutStream& target) const
{
	if (LightConstant::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LightSpecularConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LightSpecularConstant, target);
}
//----------------------------------------------------------------------------
int LightSpecularConstant::GetStreamingSize (Stream &stream) const
{
	int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
