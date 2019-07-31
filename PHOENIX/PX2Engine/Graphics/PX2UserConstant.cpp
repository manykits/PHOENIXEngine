// PX2UserConstant.cpp

#include "PX2UserConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
#include "PX2Time.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, UserConstant);
PX2_IMPLEMENT_STREAM(UserConstant);
PX2_IMPLEMENT_FACTORY(UserConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, UserConstant);

//----------------------------------------------------------------------------
UserConstant::UserConstant ()
	:
ShaderFloat(1)
{
	EnableUpdater();
}
//----------------------------------------------------------------------------
UserConstant::~UserConstant ()
{
}
//----------------------------------------------------------------------------
void UserConstant::Update(const ShaderStruct*)
{
	float val = (float)Time::GetTimeInSeconds();
	(*this)[0] = val;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
UserConstant::UserConstant (LoadConstructor value)
	:
ShaderFloat(value)
{
}
//----------------------------------------------------------------------------
void UserConstant::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UserConstant, source);
}
//----------------------------------------------------------------------------
void UserConstant::Link (InStream& source)
{
	ShaderFloat::Link(source);
}
//----------------------------------------------------------------------------
void UserConstant::PostLink ()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool UserConstant::Register (OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void UserConstant::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UserConstant, target);
}
//----------------------------------------------------------------------------
int UserConstant::GetStreamingSize (Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	return size;
}
//----------------------------------------------------------------------------
