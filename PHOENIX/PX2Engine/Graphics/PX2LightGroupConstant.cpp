// PX2LightGroupConstant.cpp

#include "PX2LightGroupConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, LightGroupConstant);
PX2_IMPLEMENT_STREAM(LightGroupConstant);
PX2_IMPLEMENT_FACTORY(LightGroupConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, LightGroupConstant);

//----------------------------------------------------------------------------
LightGroupConstant::LightGroupConstant() :
ShaderFloat(NumMaxLights*2)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
LightGroupConstant::~LightGroupConstant()
{
}
//----------------------------------------------------------------------------
void LightGroupConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	mNumLights = renderable->GetNumLights();
	int useNumLights = Math<int>::Min(mNumLights, NumMaxLights);

	int i = 0;
	for (i = 0; i < useNumLights; i++)
	{
		const Light *light = renderable->GetLight(i);
		const APoint &pos = light->Position;
		float range = light->Range;
		mLightPosRange[i] = Float4(pos[0], pos[1], pos[2], range);
		mLightColor[i] = light->Diffuse;
	}

	//for (; i < NumMaxLights; i++)
	//{
	//	mLightPosRange[i] = Float4::ZERO;
	//	mLightColor[i] = Float4::ZERO;
	//}

	int j = 0;
	for (; j < useNumLights; j++)
	{
		float* target = mData + 8 * j;

		for (int k = 0; k < 4; k++)
		{
			*target++ = mLightPosRange[j][k];
		}

		for (int k = 0; k < 4; k++)
		{
			*target++ = mLightColor[j][k];
		}
	}

	for (; j < NumMaxLights; j++)
	{
		float* target = mData + 8 * j;

		for (int k = 0; k < 4; k++)
		{
			*target++ = 0.0f;
		}

		for (int k = 0; k < 4; k++)
		{
			*target++ = 0.0f;
		}
	}

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
LightGroupConstant::LightGroupConstant(LoadConstructor value) :
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void LightGroupConstant::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LightGroupConstant, source);
}
//----------------------------------------------------------------------------
void LightGroupConstant::Link(InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void LightGroupConstant::PostLink()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool LightGroupConstant::Register(OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LightGroupConstant::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LightGroupConstant, target);
}
//----------------------------------------------------------------------------
int LightGroupConstant::GetStreamingSize(Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
