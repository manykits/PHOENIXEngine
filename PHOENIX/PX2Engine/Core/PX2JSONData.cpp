// PX2JSONData.cpp

#include "PX2JSONData.hpp"
#include "PX2Memory.hpp"
#if defined PX2_USE_RAPID_JSON
#include "rapidjson.h"
#include "document.h"
#endif
using namespace PX2;

//----------------------------------------------------------------------------
JSONData::JSONData():
mDocument(0),
mJsonObject(0)
{
}
//----------------------------------------------------------------------------
JSONData::~JSONData()
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc)
	{
		delete0(doc);
	}
#endif
}
//----------------------------------------------------------------------------
bool JSONData::LoadBuffer(const std::string &strBuffer)
{
	return LoadBuffer(strBuffer.c_str(), (int)strBuffer.length());
}
//----------------------------------------------------------------------------
bool JSONData::LoadBuffer(const char *buffer, int size)
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = new0 rapidjson::Document();
	mDocument = doc;

	if (doc->Parse((char*)buffer, size).HasParseError())
	{
		rapidjson::ParseErrorCode code = doc->GetParseError();

		return false;
	}

	return true;
#endif

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::IsArray()
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->IsArray();
#endif

	return false;
}
//----------------------------------------------------------------------------
int JSONData::GetArraySize()
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetArray().Size();
#endif

	return 0;
}
//----------------------------------------------------------------------------
JSONValue JSONData::GetArrayElement(int i)
{
	if (!IsArray())
		return JSONValue();

#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	rapidjson::Value& val = (*doc)[i];
	return JSONValue(&val);
#endif

	return JSONValue();
}
//----------------------------------------------------------------------------
bool JSONData::IsNull()
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->IsNull();
#endif

	return false;
}
//----------------------------------------------------------------------------
int JSONData::ToInt()
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetInt();
#endif

	return true;
}
//----------------------------------------------------------------------------
float JSONData::ToFloat()
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetFloat();
#endif

	return 0.0f;
}
//----------------------------------------------------------------------------
bool JSONData::ToBool()
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetBool();
#endif

	return false;
}
//----------------------------------------------------------------------------
std::string JSONData::ToString()
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->GetString();
#endif

	return "";
}
//----------------------------------------------------------------------------
bool JSONData::ToIntVal(int &value)
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc->IsBool())
	{
		value = doc->GetInt();

		return true;
	}
#endif

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::ToFloatVal(float &value)
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc->IsBool())
	{
		value = doc->GetFloat();

		return true;
	}
#endif

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::ToBoolVal(bool &value)
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc->IsBool())
	{
		value = doc->GetBool();

		return true;
	}
#endif

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::ToStringVal(std::string &value)
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	if (doc->IsBool())
	{
		value = doc->GetString();

		return true;
	}
#endif

	return false;
}
//----------------------------------------------------------------------------
bool JSONData::IsHasMember(const std::string &name)
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	return doc->HasMember(name.c_str());
#endif

	return false;
}
//----------------------------------------------------------------------------
JSONValue JSONData::GetMember(const std::string &name)
{
#if defined PX2_USE_RAPID_JSON
	rapidjson::Document *doc = (rapidjson::Document *)mDocument;
	rapidjson::Value& a = (*doc)[name.c_str()];
	return JSONValue(&a);
#endif

	return JSONValue();
}
//----------------------------------------------------------------------------