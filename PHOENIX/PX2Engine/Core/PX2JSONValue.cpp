// PX2JSONValue.cpp

#include "PX2JSONValue.hpp"
#if defined PX2_USE_RAPID_JSON
#include "rapidjson.h"
#include "document.h"
#endif
using namespace PX2;

//----------------------------------------------------------------------------
JSONValue::JSONValue():
mValue(0)
{
}
//----------------------------------------------------------------------------
#if defined PX2_USE_RAPID_JSON
JSONValue::JSONValue(void* jsValue)
{
	mValue = jsValue;
}
#endif
//----------------------------------------------------------------------------
JSONValue::JSONValue(JsonObject &obj)
{
}
//----------------------------------------------------------------------------
JSONValue::JSONValue(const JsonObject &obj)
{
}
//----------------------------------------------------------------------------
JSONValue::JSONValue(const JSONValue &rhs)
{
	mValue = rhs.mValue;
}
//----------------------------------------------------------------------------
JSONValue::~JSONValue()
{
}
//----------------------------------------------------------------------------
bool JSONValue::IsNull()
{
	if (!mValue) return true;
	
#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->IsNull();
#endif

	return false;
}
//----------------------------------------------------------------------------
int JSONValue::ToInt()
{
	if (!mValue) return 0;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetInt();
#endif

	return 0;
}
//----------------------------------------------------------------------------
float JSONValue::ToFloat()
{
	if (!mValue) return 0.0f;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetFloat();
#endif

	return 0.0f;
}
//----------------------------------------------------------------------------
bool JSONValue::ToBool()
{
	if (!mValue) return false;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetBool();
#endif

	return false;
}
//----------------------------------------------------------------------------
std::string JSONValue::ToString()
{
	if (!mValue) return "";

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	const char *str = jValue->GetString();
	int length = jValue->GetStringLength();

	return std::string(str, length);
#endif

	return "";
}
//----------------------------------------------------------------------------
JSONValue JSONValue::ToArray()
{
	if (!mValue) return JSONValue();

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return JSONValue(jValue);
#endif

	return JSONValue();
}
//----------------------------------------------------------------------------
bool JSONValue::ToIntVal(int &value)
{
	if (!mValue) return false;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	value = jValue->GetInt();
#endif

	return false;
}
//----------------------------------------------------------------------------
bool JSONValue::ToFloatVal(float &value)
{
	if (!mValue) return false;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	value = jValue->GetFloat();
#endif

	return false;
}
//----------------------------------------------------------------------------
bool JSONValue::ToBoolVal(bool &value)
{
	if (!mValue) return false;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	value = jValue->GetBool();
#endif

	return false;
}
//----------------------------------------------------------------------------
bool JSONValue::ToStringVal(std::string &value)
{
	if (!mValue) return false;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	value = jValue->GetString();
#endif

	return false;
}
//----------------------------------------------------------------------------
int JSONValue::GetStringLength(std::string &value)
{
	if (!mValue) return 0;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetStringLength();
#endif

	return 0;
}
//----------------------------------------------------------------------------
bool JSONValue::IsArray()
{
	if (!mValue) return false;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->IsArray();
#endif

	return false;
}
//----------------------------------------------------------------------------
int JSONValue::GetArraySize()
{
	if (!mValue) return 0;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->GetArray().Size();
#endif

	return 0;
}
//----------------------------------------------------------------------------
JSONValue JSONValue::GetArrayElement(int i)
{
	if (!mValue) return JSONValue();

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	int arraySize = jValue->GetArray().Size();

	if (0 <= i && i < arraySize)
	{
		rapidjson::Value &je = (*jValue).GetArray()[i];
		return JSONValue(&je);
	}
	else
	{
		return JSONValue();
	}
#endif

	return JSONValue();
}
//----------------------------------------------------------------------------
bool JSONValue::IsHasMember(const std::string &name)
{
	if (!mValue) return false;

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	return jValue->HasMember(name.c_str());
#endif

	return false;
}
//----------------------------------------------------------------------------
JSONValue JSONValue::GetMember(const std::string &name)
{
	if (!mValue) return JSONValue();
	if (!IsHasMember(name)) return JSONValue();

#if defined PX2_USE_RAPID_JSON
	rapidjson::Value *jValue = (rapidjson::Value*)mValue;
	rapidjson::Value &jm = (*jValue)[name.c_str()];
	return JSONValue(&jm);
#endif

	return JSONValue();
}
//----------------------------------------------------------------------------