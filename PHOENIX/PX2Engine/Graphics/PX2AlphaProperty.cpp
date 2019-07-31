// PX2AlphaProperty.cpp

#include "PX2AlphaProperty.hpp"
#include "PX2Float3.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, AlphaProperty);
PX2_IMPLEMENT_STREAM(AlphaProperty);
PX2_IMPLEMENT_FACTORY(AlphaProperty);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, AlphaProperty);

//----------------------------------------------------------------------------
AlphaProperty::AlphaProperty() :
BlendEnabled(false),
SrcBlend(SBM_SRC_ALPHA),
DstBlend(DBM_ONE_MINUS_SRC_ALPHA),
CompareEnabled(false),
Compare(CM_ALWAYS),
Reference(0.0f),
ConstantColor(0.0f, 0.0f, 0.0f, 0.0f)
{
	SetName("AProp");
}
//----------------------------------------------------------------------------
AlphaProperty::~AlphaProperty ()
{
}
//----------------------------------------------------------------------------
bool AlphaProperty::IsEuqal(AlphaProperty *propA, AlphaProperty *propB)
{
	if (propA->BlendEnabled != propB->BlendEnabled)
		return false;

	if (propA->SrcBlend != propB->SrcBlend)
		return false;

	if (propA->DstBlend != propB->DstBlend)
		return false;

	if (propA->CompareEnabled != propB->CompareEnabled)
		return false;

	if (propA->Compare != propB->Compare)
		return false;

	if (propA->Reference != propB->Reference)
		return false;

	if (propA->ConstantColor != propB->ConstantColor)
		return false;

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void AlphaProperty::RegistProperties ()
{
	Object::RegistProperties();
	AddPropertyClass("AlphaProperty");

	AddProperty("BlendEnabled", Object::PT_BOOL, BlendEnabled);

	std::vector<std::string> srcBlends;
	srcBlends.push_back("SBM_ZERO");
	srcBlends.push_back("SBM_ONE");
	srcBlends.push_back("SBM_DST_COLOR");
	srcBlends.push_back("SBM_ONE_MINUS_DST_COLOR");
	srcBlends.push_back("SBM_SRC_ALPHA");
	srcBlends.push_back("SBM_ONE_MINUS_SRC_ALPHA");
	srcBlends.push_back("SBM_SRC_COLOR");
	srcBlends.push_back("SBM_ONE_MINUS_SRC_COLOR");
	srcBlends.push_back("SBM_DST_ALPHA");
	srcBlends.push_back("SBM_ONE_MINUS_DST_ALPHA");
	srcBlends.push_back("SBM_SRC_ALPHA_SATURATE");
	srcBlends.push_back("SBM_CONSTANT_COLOR");
	srcBlends.push_back("SBM_ONE_MINUS_CONSTANT_COLOR");
	srcBlends.push_back("SBM_CONSTANT_ALPHA");
	srcBlends.push_back("SBM_ONE_MINUS_CONSTANT_ALPHA");
	AddPropertyEnum("SrcBlendMode", (int)SrcBlend, srcBlends);

	std::vector<std::string> dstBlends;
	dstBlends.push_back("DBM_ZERO");
	dstBlends.push_back("DBM_ONE");
	dstBlends.push_back("DBM_SRC_COLOR");
	dstBlends.push_back("DBM_ONE_MINUS_SRC_COLOR");
	dstBlends.push_back("DBM_SRC_ALPHA");
	dstBlends.push_back("DBM_ONE_MINUS_SRC_ALPHA");
	dstBlends.push_back("DBM_DST_ALPHA");
	dstBlends.push_back("DBM_ONE_MINUS_DST_ALPHA");
	dstBlends.push_back("DBM_DST_COLOR");
	dstBlends.push_back("DBM_ONE_MINUS_DST_COLOR");
	dstBlends.push_back("DBM_CONSTANT_COLOR");
	dstBlends.push_back("DBM_ONE_MINUS_CONSTANT_COLOR");
	dstBlends.push_back("DBM_CONSTANT_ALPHA");
	dstBlends.push_back("DBM_ONE_MINUS_CONSTANT_ALPHA");
	AddPropertyEnum("DstBlend", (int)DstBlend, dstBlends);

	AddProperty("CompareEnabled", Object::PT_BOOL, CompareEnabled);

	std::vector<std::string> compareModes;
	compareModes.push_back("CM_NEVER");
	compareModes.push_back("CM_LESS");
	compareModes.push_back("CM_EQUAL");
	compareModes.push_back("CM_LEQUAL");
	compareModes.push_back("CM_GREATER");
	compareModes.push_back("CM_NOTEQUAL");
	compareModes.push_back("CM_GEQUAL");
	compareModes.push_back("CM_ALWAYS");
	AddPropertyEnum("Compare", (int)Compare, compareModes);

	AddProperty("Reference", Object::PT_FLOAT, Reference);

	Float3 constColor = Float3(ConstantColor[0], ConstantColor[1], ConstantColor[2]);
	float constAlpha = ConstantColor[3];
	AddProperty("ConstantColor", Object::PT_COLOR3FLOAT3, constColor);
	AddProperty("ConstantColorAlpha", Object::PT_FLOAT, constAlpha);
}
//----------------------------------------------------------------------------
void AlphaProperty::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("BlendEnabled" == obj.Name)
	{
		BlendEnabled = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("SrcBlendMode" == obj.Name)
	{
		SrcBlend = (SrcBlendMode)PX2_ANY_AS(obj.Data, int);
	}
	else if ("DstBlend" == obj.Name)
	{
		DstBlend = (DstBlendMode)PX2_ANY_AS(obj.Data, int);
	}
	else if ("CompareEnabled" == obj.Name)
	{
		CompareEnabled = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("Compare" == obj.Name)
	{
		Compare = (CompareMode)PX2_ANY_AS(obj.Data, int);
	}
	else if ("Reference" == obj.Name)
	{
		Reference = PX2_ANY_AS(obj.Data, float);
	}
	else if ("ConstantColor" == obj.Name)
	{
		Float3 constColor = PX2_ANY_AS(obj.Data, Float3);
		ConstantColor = Float4(constColor[0], constColor[1], constColor[2], ConstantColor[3]);
	}
	else if ("ConstantColorAlpha" == obj.Name)
	{
		float constColorAlpha = PX2_ANY_AS(obj.Data, float);
		ConstantColor = Float4(ConstantColor[0], ConstantColor[1], ConstantColor[2], constColorAlpha);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
AlphaProperty::AlphaProperty (LoadConstructor value)
    :
    Object(value),
    BlendEnabled(false),
    SrcBlend(SBM_ZERO),
    DstBlend(DBM_ZERO),
    CompareEnabled(false),
    Compare(CM_NEVER),
    Reference(0.0f),
    ConstantColor(0.0f, 0.0f, 0.0f, 0.0f)
{
}
//----------------------------------------------------------------------------
void AlphaProperty::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    Object::Load(source);
	PX2_VERSION_LOAD(source);

    source.ReadBool(BlendEnabled);
    source.ReadEnum(SrcBlend);
    source.ReadEnum(DstBlend);
    source.ReadBool(CompareEnabled);
    source.ReadEnum(Compare);
    source.Read(Reference);
    source.ReadAggregate(ConstantColor);

    PX2_END_DEBUG_STREAM_LOAD(AlphaProperty, source);
}
//----------------------------------------------------------------------------
void AlphaProperty::Link (InStream& source)
{
    Object::Link(source);
}
//----------------------------------------------------------------------------
void AlphaProperty::PostLink ()
{
    Object::PostLink();
}
//----------------------------------------------------------------------------
bool AlphaProperty::Register (OutStream& target) const
{
    return Object::Register(target);
}
//----------------------------------------------------------------------------
void AlphaProperty::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    Object::Save(target);
	PX2_VERSION_SAVE(target);

    target.WriteBool(BlendEnabled);
    target.WriteEnum(SrcBlend);
    target.WriteEnum(DstBlend);
    target.WriteBool(CompareEnabled);
    target.WriteEnum(Compare);
    target.Write(Reference);
    target.WriteAggregate(ConstantColor);

    PX2_END_DEBUG_STREAM_SAVE(AlphaProperty, target);
}
//----------------------------------------------------------------------------
int AlphaProperty::GetStreamingSize (Stream &stream) const
{
    int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
    size += PX2_BOOLSIZE(BlendEnabled);
    size += PX2_ENUMSIZE(SrcBlend);
    size += PX2_ENUMSIZE(DstBlend);
    size += PX2_BOOLSIZE(CompareEnabled);
    size += PX2_ENUMSIZE(Compare);
    size += sizeof(Reference);
    size += sizeof(ConstantColor);
    return size;
}
//----------------------------------------------------------------------------