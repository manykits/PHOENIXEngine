// PX2LightModelDVectorConstant.cpp

#include "PX2LightModelDVectorConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightModelDVectorConstant);
PX2_IMPLEMENT_STREAM(LightModelDVectorConstant);
PX2_IMPLEMENT_FACTORY(LightModelDVectorConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightModelDVectorConstant)

//----------------------------------------------------------------------------
LightModelDVectorConstant::LightModelDVectorConstant (Light* light) :
LightConstant(light)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
LightModelDVectorConstant::~LightModelDVectorConstant ()
{
}
//----------------------------------------------------------------------------
void LightModelDVectorConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

	const Renderable *renderable = struc->TheRenderable;

	const HMatrix& worldInvMatrix = renderable->WorldTransform.Inverse();
	AVector modelDVector = worldInvMatrix*mLight->DVector;
	modelDVector.Normalize();

	const float* source = (const float*)modelDVector;
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
LightModelDVectorConstant::LightModelDVectorConstant (LoadConstructor value) :
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightModelDVectorConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LightModelDVectorConstant, source);
}
//----------------------------------------------------------------------------
void LightModelDVectorConstant::Link (InStream& source)
{
	LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightModelDVectorConstant::PostLink ()
{
	LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightModelDVectorConstant::Register (OutStream& target) const
{
	if (LightConstant::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LightModelDVectorConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LightModelDVectorConstant, target);
}
//----------------------------------------------------------------------------
int LightModelDVectorConstant::GetStreamingSize (Stream &stream) const
{
	int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
