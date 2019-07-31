// PX2Object_Property.cpp

#include "PX2Object.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
int Object::msNumProperties = 0;
Object::PropertyObject::PropertyObject()
	:
	Enable(true),
	Type(Object::PT_MAX_TYPE)
{
	SliderMin = 0.0f;
	SliderMax = 10.0f;
}
//----------------------------------------------------------------------------
Object::PropertyObject::~PropertyObject()
{
}
//----------------------------------------------------------------------------
Object::PropertyObject *Object::PropertyObject::GetPropertyByTag(
	const std::string &tag)
{
	for (int i = 0; i < (int)SubObjects.size(); i++)
	{
		if (tag == SubObjects[i].Tag)
			return &SubObjects[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
Object::PropertyObject *Object::PropertyObject::GetPropertyByName(
	const std::string &name)
{
	for (int i = 0; i < (int)SubObjects.size(); i++)
	{
		if (name == SubObjects[i].Tag)
			return &SubObjects[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
std::string Object::GenPropertyTag()
{
	msNumProperties++;
	return "ObjectPropertyTag" + StringHelp::IntToString(msNumProperties);
}
//----------------------------------------------------------------------------
void Object::RegistProperties()
{
	AddProperty("Object", PT_CLASS, Any());

	const std::string name = GetName();
	AddProperty("Name", PT_STRING, Any(name), mIsNameChangeable);

	const std::string &rttiName = GetRttiType().GetName();
	AddProperty("RttiName", PT_STRING, Any(rttiName), false);

	AddProperty("IsEnable", PT_BOOL, IsEnable());
}
//----------------------------------------------------------------------------
void Object::OnPropertyChanged(const PropertyObject &obj)
{
	if ("Name" == obj.Name)
	{
		std::string val = *(Any_Cast<std::string>(&obj.Data));
		SetName(val);
	}
	else if ("IsEnable" == obj.Name)
	{
		Enable(PX2_ANY_AS(obj.Data, bool));
	}
}
//----------------------------------------------------------------------------
void Object::SetNamePropChangeable(bool isNamePropChangeable)
{
	mIsNameChangeable = isNamePropChangeable;
}
//----------------------------------------------------------------------------
bool Object::IsNamePropChangeable() const
{
	return mIsNameChangeable;
}
//----------------------------------------------------------------------------
std::string Object::AddProperty(const std::string &name, PropertyType type,
	const Any &data, bool enable)
{
	PropertyObject obj;
	obj.Tag = GenPropertyTag();
	obj.Name = name;
	obj.Type = type;
	obj.Enable = enable;
	obj.Data = data;

	mProperties.push_back(obj);

	return obj.Tag;
}
//----------------------------------------------------------------------------
std::string Object::AddPropertyFloatSlider(const std::string &name, float val,
	float min, float max, bool enable)
{
	std::string tag = GenPropertyTag();

	PropertyObject obj;
	obj.Tag = tag;
	obj.Name = name;
	obj.Type = PT_FLOATSLIDER;
	obj.Enable = enable;
	obj.Data = val;
	obj.SliderMin = min;
	obj.SliderMax = max;

	mProperties.push_back(obj);

	return obj.Tag;
}
//----------------------------------------------------------------------------
std::string Object::AddPropertyEnum(const std::string &name, int val,
	const std::vector<std::string> &enums, bool enable)
{
	std::string tag = GenPropertyTag();

	PropertyObject obj;
	obj.Tag = tag;
	obj.Name = name;
	obj.Type = PT_ENUM;
	obj.Enable = enable;
	obj.Data = val;
	obj.Data1 = enums;

	mProperties.push_back(obj);

	return obj.Tag;
}
//----------------------------------------------------------------------------
std::string Object::AddPropertyClass(const std::string &name)
{
	return AddProperty(name, Object::PT_CLASS, Any());
}
//----------------------------------------------------------------------------
std::string Object::AddProperty(PropertyObject &obj)
{
	obj.Tag = GenPropertyTag();

	mProperties.push_back(obj);

	return obj.Tag;
}
//----------------------------------------------------------------------------
void Object::RemoveAllProperties()
{
	mProperties.clear();
}
//----------------------------------------------------------------------------
std::string Object::AddPropertySub(const std::string &parTag,
	const std::string &name, PropertyType type, const Any &data, bool enable)
{
	if (parTag.empty())
	{
		AddProperty(name, type, data, enable);
	}
	else
	{
		Object::PropertyObject *po = GetPropertyByTag(parTag);
		if (po)
		{
			PropertyObject obj;
			obj.Tag = GenPropertyTag();
			obj.Name = name;
			obj.Type = type;
			obj.Enable = enable;
			obj.Data = data;

			po->SubObjects.push_back(obj);

			return obj.Tag;
		}
	}

	return "";
}
//----------------------------------------------------------------------------
std::string Object::AddPropertyEnumSub(const std::string &parTag,
	const std::string &name, int val, const std::vector<std::string> &enums,
	bool enable)
{
	if (parTag.empty())
	{
		AddPropertyEnum(name, val, enums, enable);
	}
	else
	{
		Object::PropertyObject *po = GetPropertyByTag(parTag);
		if (po)
		{
			std::string tag = GenPropertyTag();

			PropertyObject obj;
			obj.Tag = tag;
			obj.Name = name;
			obj.Type = PT_ENUM;
			obj.Enable = enable;
			obj.Data = val;
			obj.Data1 = enums;

			po->SubObjects.push_back(obj);

			return obj.Tag;
		}
	}

	return "";
}
//----------------------------------------------------------------------------
const std::vector<Object::PropertyObject> &Object::GetProperties() const
{
	return mProperties;
}
//----------------------------------------------------------------------------
std::vector<Object::PropertyObject> &Object::GetProperties()
{
	return mProperties;
}
//----------------------------------------------------------------------------
Object::PropertyObject *Object::GetPropertyByName(const std::string &name)
{
	for (int i = 0; i < (int)mProperties.size(); i++)
	{
		Object::PropertyObject *propObj = mProperties[i].GetPropertyByName(name);
		if (propObj)
			return propObj;

		if (name == mProperties[i].Name)
			return &mProperties[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
Object::PropertyObject *Object::GetPropertyByTag(const std::string &tag)
{
	for (int i = 0; i < (int)mProperties.size(); i++)
	{
		Object::PropertyObject *propObj = mProperties[i].GetPropertyByTag(tag);
		if (propObj)
			return propObj;

		if (tag == mProperties[i].Tag)
			return &mProperties[i];
	}

	return 0;
}
//----------------------------------------------------------------------------