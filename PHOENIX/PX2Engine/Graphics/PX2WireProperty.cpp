// PX2WireProperty.cpp

#include "PX2WireProperty.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, WireProperty);
PX2_IMPLEMENT_STREAM(WireProperty);
PX2_IMPLEMENT_FACTORY(WireProperty);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, WireProperty);

//----------------------------------------------------------------------------
WireProperty::WireProperty ():
Enabled(false)
{
	SetName("WProp");
}
//----------------------------------------------------------------------------
WireProperty::~WireProperty ()
{
}
//----------------------------------------------------------------------------
bool WireProperty::IsEuqal(WireProperty *propA, WireProperty *propB)
{
	if (propA->Enabled != propB->Enabled)
		return false;

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void WireProperty::RegistProperties ()
{
	Object::RegistProperties();
	AddPropertyClass("WireProperty");

	AddProperty("Enabled", Object::PT_BOOL, Enabled);
}
//----------------------------------------------------------------------------
void WireProperty::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("Enabled" == obj.Name)
	{
		Enabled = PX2_ANY_AS(obj.Data, bool);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
WireProperty::WireProperty (LoadConstructor value)
    :
    Object(value),
    Enabled(false)
{
}
//----------------------------------------------------------------------------
void WireProperty::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    Object::Load(source);
	PX2_VERSION_LOAD(source);

    source.ReadBool(Enabled);

    PX2_END_DEBUG_STREAM_LOAD(WireProperty, source);
}
//----------------------------------------------------------------------------
void WireProperty::Link (InStream& source)
{
    Object::Link(source);
}
//----------------------------------------------------------------------------
void WireProperty::PostLink ()
{
    Object::PostLink();
}
//----------------------------------------------------------------------------
bool WireProperty::Register (OutStream& target) const
{
    return Object::Register(target);
}
//----------------------------------------------------------------------------
void WireProperty::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    Object::Save(target);
	PX2_VERSION_SAVE(target);

    target.WriteBool(Enabled);

    PX2_END_DEBUG_STREAM_SAVE(WireProperty, target);
}
//----------------------------------------------------------------------------
int WireProperty::GetStreamingSize (Stream &stream) const
{
    int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
    size += PX2_BOOLSIZE(Enabled);
    return size;
}
//----------------------------------------------------------------------------