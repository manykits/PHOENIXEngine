// PX2CullProperty.cpp

#include "PX2CullProperty.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, CullProperty);
PX2_IMPLEMENT_STREAM(CullProperty);
PX2_IMPLEMENT_FACTORY(CullProperty);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, CullProperty);

//----------------------------------------------------------------------------
CullProperty::CullProperty ()
:
Enabled(true),
CCWOrder(true)
{
	SetName("CProp");
}
//----------------------------------------------------------------------------
CullProperty::~CullProperty ()
{
}
//----------------------------------------------------------------------------
bool CullProperty::IsEuqal(CullProperty *propA, CullProperty *propB)
{
	if (propA->Enabled != propB->Enabled)
		return false;

	if (propA->CCWOrder != propB->CCWOrder)
		return false;

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void CullProperty::RegistProperties ()
{
	Object::RegistProperties();
	AddPropertyClass("CullProperty");

	AddProperty("Enabled", Object::PT_BOOL, Enabled);
	AddProperty("CCWOrder", Object::PT_BOOL, CCWOrder);
}
//----------------------------------------------------------------------------
void CullProperty::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("Enabled" == obj.Name)
	{
		Enabled = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("CCWOrder" == obj.Name)
	{
		CCWOrder = PX2_ANY_AS(obj.Data, bool);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
CullProperty::CullProperty (LoadConstructor value)
:
Object(value),
Enabled(false),
CCWOrder(false)
{
}
//----------------------------------------------------------------------------
void CullProperty::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(Enabled);
	source.ReadBool(CCWOrder);

	PX2_END_DEBUG_STREAM_LOAD(CullProperty, source);
}
//----------------------------------------------------------------------------
void CullProperty::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void CullProperty::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool CullProperty::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void CullProperty::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(Enabled);
	target.WriteBool(CCWOrder);

	PX2_END_DEBUG_STREAM_SAVE(CullProperty, target);
}
//----------------------------------------------------------------------------
int CullProperty::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_BOOLSIZE(Enabled);
	size += PX2_BOOLSIZE(CCWOrder);
	return size;
}
//----------------------------------------------------------------------------