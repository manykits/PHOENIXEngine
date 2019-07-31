// PX2Shine.cpp

#include "PX2Shine.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Shine);
PX2_IMPLEMENT_STREAM(Shine);
PX2_IMPLEMENT_FACTORY(Shine);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, Shine);

//----------------------------------------------------------------------------
Shine::Shine ()
:
Emissive(0.0f, 0.0f, 0.0f, 0.0f),
Ambient(1.0f, 1.0f, 1.0f, 1.0f),
Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
Specular(0.3f, 0.3f, 0.3f, 0.0f)
{
	ReCalTemp();
}
//----------------------------------------------------------------------------
void Shine::ReCalTemp ()
{
	mEmissiveTemp = Float3(Emissive[0], Emissive[1], Emissive[2]);
	mEmissiveAlphaTemp = Emissive[3];
	mAmbientTemp = Float3(Ambient[0], Ambient[1], Ambient[2]);
	mAmbientAlphaTemp = Ambient[3];
	mDiffuseTemp = Float3(Diffuse[0], Diffuse[1], Diffuse[2]);
	mDiffuseAlphaTemp = Diffuse[3];
	mSpecularTemp = Float3(Specular[0], Specular[1], Specular[2]);
	mSpecularExponentTemp = Specular[3];
}
//----------------------------------------------------------------------------
Shine::~Shine ()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Shine::RegistProperties ()
{
	Object::RegistProperties();

	AddPropertyClass("Shine");

	AddProperty("Emissive", Object::PT_COLOR3FLOAT3, mEmissiveTemp);
	AddProperty("Ambient", Object::PT_COLOR3FLOAT3, mAmbientTemp);
	AddProperty("AmbientAlpha", Object::PT_FLOAT, mAmbientAlphaTemp);
	AddProperty("Diffuse", Object::PT_COLOR3FLOAT3, mDiffuseTemp);
	AddProperty("DiffuseAlpha", Object::PT_FLOAT, mDiffuseAlphaTemp);
	AddProperty("Specular", Object::PT_COLOR3FLOAT3, mSpecularTemp);
	AddProperty("SpecularExponent", Object::PT_FLOAT, mSpecularExponentTemp);
}
//----------------------------------------------------------------------------
void Shine::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("Emissive" == obj.Name)
	{
		mEmissiveTemp = PX2_ANY_AS(obj.Data, Float3);
		Emissive = Float4(mEmissiveTemp[0], mEmissiveTemp[1], mEmissiveTemp[2],
			mEmissiveAlphaTemp);
	}
	else if ("Ambient" == obj.Name)
	{
		mAmbientTemp = PX2_ANY_AS(obj.Data, Float3);
		Ambient = Float4(mAmbientTemp[0], mAmbientTemp[1], mAmbientTemp[2], 
			mAmbientAlphaTemp);
	}
	else if ("AmbientAlpha" == obj.Name)
	{
		mAmbientAlphaTemp = PX2_ANY_AS(obj.Data, float);
		Ambient = Float4(mAmbientTemp[0], mAmbientTemp[1], mAmbientTemp[2], 
			mAmbientAlphaTemp);
	}
	else if ("Diffuse" == obj.Name)
	{
		mDiffuseTemp = PX2_ANY_AS(obj.Data, Float3);
		Diffuse = Float4(mDiffuseTemp[0], mDiffuseTemp[1], mDiffuseTemp[2], 
			mDiffuseAlphaTemp);
	}
	else if ("DiffuseAlpha" == obj.Name)
	{
		mDiffuseAlphaTemp = PX2_ANY_AS(obj.Data, float);
		Diffuse = Float4(mDiffuseTemp[0], mDiffuseTemp[1], mDiffuseTemp[2], 
			mDiffuseAlphaTemp);
	}
	else if ("Specular" == obj.Name)
	{
		mSpecularTemp = PX2_ANY_AS(obj.Data, Float3);
		Specular = Float4(mSpecularTemp[0], mSpecularTemp[1], mSpecularTemp[2], 
			mSpecularExponentTemp);
	}
	else if ("SpecularExponent" == obj.Name)
	{
		mSpecularExponentTemp = PX2_ANY_AS(obj.Data, float);
		Specular = Float4(mSpecularTemp[0], mSpecularTemp[1], mSpecularTemp[2], 
			mSpecularExponentTemp);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Shine::Shine (LoadConstructor value)
:
Object(value),
Emissive(0.0f, 0.0f, 0.0f, 0.0f),
Ambient(0.0f, 0.0f, 0.0f, 0.0f),
Diffuse(0.5f, 0.5f, 0.5f, 1.0f),
Specular(0.0f, 0.0f, 0.0f, 0.0f)
{
}
//----------------------------------------------------------------------------
void Shine::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregate(Emissive);
	source.ReadAggregate(Ambient);
	source.ReadAggregate(Diffuse);
	source.ReadAggregate(Specular);

	mEmissiveTemp = Float3(Emissive[0], Emissive[1], Emissive[2]);
	mEmissiveAlphaTemp = Emissive[3];
	mAmbientTemp = Float3(Ambient[0], Ambient[1], Ambient[2]);
	mAmbientAlphaTemp = Ambient[3];
	mDiffuseTemp = Float3(Diffuse[0], Diffuse[1], Diffuse[2]);
	mDiffuseAlphaTemp = Diffuse[3];
	mSpecularTemp = Float3(Specular[0], Specular[1], Specular[2]);
	mSpecularExponentTemp = Specular[3];

	PX2_END_DEBUG_STREAM_LOAD(Shine, source);
}
//----------------------------------------------------------------------------
void Shine::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void Shine::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool Shine::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void Shine::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregate(Emissive);
	target.WriteAggregate(Ambient);
	target.WriteAggregate(Diffuse);
	target.WriteAggregate(Specular);

	PX2_END_DEBUG_STREAM_SAVE(Shine, target);
}
//----------------------------------------------------------------------------
int Shine::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(Emissive);
	size += sizeof(Ambient);
	size += sizeof(Diffuse);
	size += sizeof(Specular);
	return size;
}
//----------------------------------------------------------------------------