// PX2Object_Rtti.cpp

#include "PX2Object.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
const Rtti Object::TYPE("PX2.Object", 0, 0);
//----------------------------------------------------------------------------
const Rtti& Object::GetRttiType() const
{
	return TYPE;
}
//----------------------------------------------------------------------------
unsigned short Object::GetRttiVersion() const
{
	return GetRttiType().GetVersion();
}
//----------------------------------------------------------------------------
bool Object::IsExactly(const Rtti& type) const
{
	return GetRttiType().IsExactly(type);
}
//----------------------------------------------------------------------------
bool Object::IsDerived(const Rtti& type) const
{
	return GetRttiType().IsDerived(type);
}
//----------------------------------------------------------------------------
bool Object::IsExactlyTypeOf(const Object* object) const
{
	return object && GetRttiType().IsExactly(object->GetRttiType());
}
//----------------------------------------------------------------------------
bool Object::IsDerivedTypeOf(const Object* object) const
{
	return object && GetRttiType().IsDerived(object->GetRttiType());
}
//----------------------------------------------------------------------------