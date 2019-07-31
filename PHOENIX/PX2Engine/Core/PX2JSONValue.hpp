// PX2JSONValue.hpp

#ifndef PX2JSONVALUE_HPP
#define PX2JSONVALUE_HPP

#include "PX2CorePre.hpp"
#include "PX2Object.hpp"

#define PX2_USE_RAPID_JSON 1
class JsonObject;

namespace PX2
{

	class PX2_ENGINE_ITEM JSONValue
	{
	public:
		JSONValue();
#if defined PX2_USE_RAPID_JSON
		JSONValue(void* jsValue);
#endif
		JSONValue(JsonObject &obj);
		JSONValue(const JsonObject &obj);
		JSONValue(const JSONValue &rhs);
		~JSONValue();

		bool IsNull();
		int ToInt();
		float ToFloat();
		bool ToBool();
		std::string ToString();
		JSONValue ToArray();
		bool ToIntVal(int &value);
		bool ToFloatVal(float &value);
		bool ToBoolVal(bool &value);
		bool ToStringVal(std::string &value);
		int GetStringLength(std::string &value);

		bool IsArray();
		int GetArraySize();
		JSONValue GetArrayElement(int i);

		bool IsHasMember(const std::string &name);
		JSONValue GetMember(const std::string &name);

	private:
		void *mValue;
	};

}

#endif