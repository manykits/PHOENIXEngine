// PX2StencilProperty.cpp

#include "PX2StencilProperty.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, StencilProperty);
PX2_IMPLEMENT_STREAM(StencilProperty);
PX2_IMPLEMENT_FACTORY(StencilProperty);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, StencilProperty);

//----------------------------------------------------------------------------
StencilProperty::StencilProperty ()
:
Enabled(false),
Compare(CM_NEVER),
Reference(0),
Mask(UINT_MAX),
WriteMask(UINT_MAX),
OnFail(OT_KEEP),
OnZFail(OT_KEEP),
OnZPass(OT_KEEP)
{
	SetName("SProp");
}
//----------------------------------------------------------------------------
StencilProperty::~StencilProperty ()
{
}
//----------------------------------------------------------------------------
bool StencilProperty::IsEuqal(StencilProperty *propA, StencilProperty *propB)
{
	if (propA->Enabled != propB->Enabled)
		return false;

	if (propA->Compare != propB->Compare)
		return false;

	if (propA->Reference != propB->Reference)
		return false;

	if (propA->Mask != propB->Mask)
		return false;

	if (propA->WriteMask != propB->WriteMask)
		return false;

	if (propA->OnFail != propB->OnFail)
		return false;

	if (propA->OnZFail != propB->OnZFail)
		return false;

	if (propA->OnZPass != propB->OnZPass)
		return false;

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void StencilProperty::RegistProperties ()
{
	Object::RegistProperties();
	AddPropertyClass("StencilProperty");

	AddProperty("Enabled", Object::PT_BOOL, Enabled);

	std::vector<std::string> compares;
	compares.push_back("CM_NEVER");
	compares.push_back("CM_LESS");
	compares.push_back("CM_EQUAL");
	compares.push_back("CM_LEQUAL");
	compares.push_back("CM_GREATER");
	compares.push_back("CM_NOTEQUAL");
	compares.push_back("CM_GEQUAL");
	compares.push_back("CM_ALWAYS");
	AddPropertyEnum("Compare", (int)Compare, compares);

	AddProperty("Reference", Object::PT_INT, Reference);
	AddProperty("Mask", Object::PT_INT, Mask);
	AddProperty("WriteMask", Object::PT_INT, WriteMask);

	std::vector<std::string> ops;
	ops.push_back("OT_KEEP");
	ops.push_back("OT_ZERO");
	ops.push_back("OT_REPLACE");
	ops.push_back("OT_INCREMENT");
	ops.push_back("OT_DECREMENT");
	ops.push_back("OT_INVERT");
	AddPropertyEnum("OnFail", (int)OnFail, ops);
	AddPropertyEnum("OnZFail", (int)OnZFail, ops);
	AddPropertyEnum("OnZPass", (int)OnZPass, ops);
}
//----------------------------------------------------------------------------
void StencilProperty::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("Enabled" == obj.Name)
	{
		Enabled = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("Compare" == obj.Name)
	{
		Compare = (CompareMode)PX2_ANY_AS(obj.Data, int);
	}
	else if ("Reference" == obj.Name)
	{
		Reference = (unsigned int)PX2_ANY_AS(obj.Data, int);
	}
	else if ("Mask" == obj.Name)
	{
		Mask = (unsigned int)PX2_ANY_AS(obj.Data, int);
	}
	else if ("WriteMask" == obj.Name)
	{
		WriteMask = (unsigned int)PX2_ANY_AS(obj.Data, int);
	}
	else if ("OnFail" == obj.Name)
	{
		OnFail = (OperationType)PX2_ANY_AS(obj.Data, int);
	}
	else if ("OnZFail" == obj.Name)
	{
		OnZFail = (OperationType)PX2_ANY_AS(obj.Data, int);
	}
	else if ("OnZPass" == obj.Name)
	{
		OnZPass = (OperationType)PX2_ANY_AS(obj.Data, int);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
StencilProperty::StencilProperty (LoadConstructor value)
:
Object(value),
Enabled(false),
Compare(CM_NEVER),
Reference(0),
Mask(0),
WriteMask(0),
OnFail(OT_KEEP),
OnZFail(OT_KEEP),
OnZPass(OT_KEEP)
{
}
//----------------------------------------------------------------------------
void StencilProperty::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(Enabled);
	source.ReadEnum(Compare);
	source.Read(Reference);
	source.Read(Mask);
	source.Read(WriteMask);
	source.ReadEnum(OnFail);
	source.ReadEnum(OnZFail);
	source.ReadEnum(OnZPass);

	PX2_END_DEBUG_STREAM_LOAD(StencilProperty, source);
}
//----------------------------------------------------------------------------
void StencilProperty::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void StencilProperty::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool StencilProperty::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void StencilProperty::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(Enabled);
	target.WriteEnum(Compare);
	target.Write(Reference);
	target.Write(Mask);
	target.Write(WriteMask);
	target.WriteEnum(OnFail);
	target.WriteEnum(OnZFail);
	target.WriteEnum(OnZPass);

	PX2_END_DEBUG_STREAM_SAVE(StencilProperty, target);
}
//----------------------------------------------------------------------------
int StencilProperty::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_BOOLSIZE(Enabled);
	size += PX2_ENUMSIZE(Compare);
	size += sizeof(Reference);
	size += sizeof(Mask);
	size += sizeof(WriteMask);
	size += PX2_ENUMSIZE(OnFail);
	size += PX2_ENUMSIZE(OnZFail);
	size += PX2_ENUMSIZE(OnZPass);
	return size;
}
//----------------------------------------------------------------------------