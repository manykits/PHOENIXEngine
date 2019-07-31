// PX2LightConstant.cpp

#include "PX2LightConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
#include "PX2EnvirParamController.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, LightConstant);
PX2_IMPLEMENT_STREAM(LightConstant);
PX2_IMPLEMENT_FACTORY(LightConstant);

//----------------------------------------------------------------------------
LightConstant::LightConstant(Light* light) :
ShaderFloat(1),
mLight(light),
mIsDefualtDir(false)
{
	EnableUpdater();

	if (!mLight) return;

	const float* source = (const float*)mLight->Diffuse;
	float* target = mData;
	for (int i = 0; i < 4; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------
LightConstant::~LightConstant()
{
}
//----------------------------------------------------------------------------
void LightConstant::SetDefaultDir(bool isDefaultDir)
{
	mIsDefualtDir = isDefaultDir;
}
//----------------------------------------------------------------------------
void LightConstant::SetLight(Light *light)
{
	mLight = light;
}
//----------------------------------------------------------------------------
Light* LightConstant::GetLight()
{
	return mLight;
}
//----------------------------------------------------------------------------
void LightConstant::Update(const ShaderStruct *struc)
{
	if (mIsDefualtDir) SetLight(struc->TheEnvirParam->GetLight_Dir());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* LightConstant::GetObjectByName(const std::string& name)
{
	Object* found = ShaderFloat::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mLight, name, found);
	return 0;
}
//----------------------------------------------------------------------------
void LightConstant::GetAllObjectsByName(const std::string& name,
	std::vector<Object*>& objects)
{
	ShaderFloat::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mLight, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
LightConstant::LightConstant(LoadConstructor value) :
ShaderFloat(value),
mIsDefualtDir(false)
{
}
//----------------------------------------------------------------------------
void LightConstant::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ShaderFloat::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mLight);
	source.ReadBool(mIsDefualtDir);

	PX2_END_DEBUG_STREAM_LOAD(LightConstant, source);
}
//----------------------------------------------------------------------------
void LightConstant::Link(InStream& source)
{
	ShaderFloat::Link(source);

	source.ResolveLink(mLight);
}
//----------------------------------------------------------------------------
void LightConstant::PostLink()
{
	ShaderFloat::PostLink();
}
//----------------------------------------------------------------------------
bool LightConstant::Register(OutStream& target) const
{
	if (ShaderFloat::Register(target))
	{
		target.Register(mLight);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LightConstant::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ShaderFloat::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mLight);
	target.WriteBool(mIsDefualtDir);

	PX2_END_DEBUG_STREAM_SAVE(LightConstant, target);
}
//----------------------------------------------------------------------------
int LightConstant::GetStreamingSize(Stream &stream) const
{
	int size = ShaderFloat::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mLight);
	size += PX2_BOOLSIZE(mIsDefualtDir);

	return size;
}
//----------------------------------------------------------------------------
