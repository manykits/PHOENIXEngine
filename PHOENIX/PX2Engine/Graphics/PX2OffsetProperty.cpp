// PX2OffsetProperty.cpp

#include "PX2OffsetProperty.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, OffsetProperty);
PX2_IMPLEMENT_STREAM(OffsetProperty);
PX2_IMPLEMENT_FACTORY(OffsetProperty);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, OffsetProperty);

//----------------------------------------------------------------------------
OffsetProperty::OffsetProperty() :
FillEnabled(false),
LineEnabled(false),
PointEnabled(false),
AllowRed(true),
AllowGreen(true),
AllowBlue(true),
AllowAlpha(true),
Scale(0.0f),
Bias(0.0f)
{
	SetName("OProp");
}
//----------------------------------------------------------------------------
OffsetProperty::~OffsetProperty ()
{
}
//----------------------------------------------------------------------------
bool OffsetProperty::IsEuqal(OffsetProperty *propA, OffsetProperty *propB)
{
	if (propA->FillEnabled != propB->FillEnabled)
		return false;

	if (propA->LineEnabled != propB->LineEnabled)
		return false;

	if (propA->PointEnabled != propB->PointEnabled)
		return false;

	if (propA->AllowRed != propB->AllowRed)
		return false;

	if (propA->AllowGreen != propB->AllowGreen)
		return false;

	if (propA->AllowBlue != propB->AllowBlue)
		return false;

	if (propA->AllowAlpha != propB->AllowAlpha)
		return false;

	if (propA->Scale != propB->Scale)
		return false;

	if (propA->Bias != propB->Bias)
		return false;

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void OffsetProperty::RegistProperties ()
{
	Object::RegistProperties();
	AddPropertyClass("OffsetProperty");

	AddProperty("FillEnabled", Object::PT_BOOL, FillEnabled);
	AddProperty("LineEnabled", Object::PT_BOOL, LineEnabled);
	AddProperty("PointEnabled", Object::PT_BOOL, PointEnabled);
	AddProperty("Scale", Object::PT_FLOAT, Scale);
	AddProperty("Bias", Object::PT_FLOAT, Bias);
}
//----------------------------------------------------------------------------
void OffsetProperty::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("FillEnabled" == obj.Name)
	{
		FillEnabled = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("LineEnabled" == obj.Name)
	{
		LineEnabled = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("PointEnabled" == obj.Name)
	{
		PointEnabled = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("Scale" == obj.Name)
	{
		Scale = PX2_ANY_AS(obj.Data, float);
	}
	else if ("Bias" == obj.Name)
	{
		Bias = PX2_ANY_AS(obj.Data, float);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
OffsetProperty::OffsetProperty(LoadConstructor value) :
Object(value),
FillEnabled(false),
LineEnabled(false),
PointEnabled(false),
AllowRed(true),
AllowGreen(true),
AllowBlue(true),
AllowAlpha(true),
Scale(0.0f),
Bias(0.0f)
{
}
//----------------------------------------------------------------------------
void OffsetProperty::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    Object::Load(source);
	PX2_VERSION_LOAD(source);

    source.ReadBool(FillEnabled);
    source.ReadBool(LineEnabled);
    source.ReadBool(PointEnabled);
	//source.ReadBool(AllowRed);
	//source.ReadBool(AllowGreen);
	//source.ReadBool(AllowBlue);
	//source.ReadBool(AllowAlpha);
    source.Read(Scale);
    source.Read(Bias);

    PX2_END_DEBUG_STREAM_LOAD(OffsetProperty, source);
}
//----------------------------------------------------------------------------
void OffsetProperty::Link (InStream& source)
{
    Object::Link(source);
}
//----------------------------------------------------------------------------
void OffsetProperty::PostLink ()
{
    Object::PostLink();
}
//----------------------------------------------------------------------------
bool OffsetProperty::Register (OutStream& target) const
{
    return Object::Register(target);
}
//----------------------------------------------------------------------------
void OffsetProperty::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    Object::Save(target);
	PX2_VERSION_SAVE(target);

    target.WriteBool(FillEnabled);
    target.WriteBool(LineEnabled);
    target.WriteBool(PointEnabled);
	//target.WriteBool(AllowRed);
	//target.WriteBool(AllowGreen);
	//target.WriteBool(AllowBlue);
	//target.WriteBool(AllowAlpha);
    target.Write(Scale);
    target.Write(Bias);

    PX2_END_DEBUG_STREAM_SAVE(OffsetProperty, target);
}
//----------------------------------------------------------------------------
int OffsetProperty::GetStreamingSize (Stream &stream) const
{
    int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
    size += PX2_BOOLSIZE(FillEnabled);
    size += PX2_BOOLSIZE(LineEnabled);
    size += PX2_BOOLSIZE(PointEnabled);
	//size += PX2_BOOLSIZE(AllowRed);
	//size += PX2_BOOLSIZE(AllowGreen);
	//size += PX2_BOOLSIZE(AllowBlue);
	//size += PX2_BOOLSIZE(AllowAlpha);
    size += sizeof(Scale);
    size += sizeof(Bias);
    return size;
}
//----------------------------------------------------------------------------