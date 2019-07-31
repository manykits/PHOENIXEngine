// PX2LightWorldDVectorConstant.cpp

#include "PX2LightWorldDVectorConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, LightConstant, LightWorldDVectorConstant);
PX2_IMPLEMENT_STREAM(LightWorldDVectorConstant);
PX2_IMPLEMENT_FACTORY(LightWorldDVectorConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(LightConstant, LightWorldDVectorConstant)

//----------------------------------------------------------------------------
LightWorldDVectorConstant::LightWorldDVectorConstant (Light* light) :
LightConstant(light)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
LightWorldDVectorConstant::~LightWorldDVectorConstant ()
{
}
//----------------------------------------------------------------------------
void LightWorldDVectorConstant::Update(const ShaderStruct *struc)
{
	LightConstant::Update(struc);

	AVector worldDVector = mLight->DVector;
	worldDVector.Normalize();

	const float* source = (const float*)worldDVector;
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
LightWorldDVectorConstant::LightWorldDVectorConstant (LoadConstructor value) :
LightConstant(value)
{
}
//----------------------------------------------------------------------------
void LightWorldDVectorConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	LightConstant::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LightWorldDVectorConstant, source);
}
//----------------------------------------------------------------------------
void LightWorldDVectorConstant::Link (InStream& source)
{
	LightConstant::Link(source);
}
//----------------------------------------------------------------------------
void LightWorldDVectorConstant::PostLink ()
{
	LightConstant::PostLink();
}
//----------------------------------------------------------------------------
bool LightWorldDVectorConstant::Register (OutStream& target) const
{
	if (LightConstant::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LightWorldDVectorConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	LightConstant::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LightWorldDVectorConstant, target);
}
//----------------------------------------------------------------------------
int LightWorldDVectorConstant::GetStreamingSize (Stream &stream) const
{
	int size = LightConstant::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
