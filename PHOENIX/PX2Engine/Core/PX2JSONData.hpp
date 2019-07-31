// PX2JSONData.hpp

#ifndef PX2JSONDATA_HPP
#define PX2JSONDATA_HPP

#include "PX2CorePre.hpp"
#include "PX2Object.hpp"
#include "PX2JSONValue.hpp"

class JsonObject;

namespace PX2
{

	class PX2_ENGINE_ITEM JSONData
	{
	public:
		JSONData();
		~JSONData();

		bool LoadBuffer(const char *buffer, int size);
		bool LoadBuffer(const std::string &strBuffer);
		
		bool IsNull();
		int ToInt();
		float ToFloat();
		bool ToBool();
		std::string ToString();
		bool ToIntVal(int &value);
		bool ToFloatVal(float &value);
		bool ToBoolVal(bool &value);
		bool ToStringVal(std::string &value);

		bool IsArray();
		int GetArraySize();
		JSONValue GetArrayElement(int i);

		bool IsHasMember(const std::string &name);
		JSONValue GetMember(const std::string &name);

	private:
		void *mDocument;
		JsonObject *mJsonObject;
	};

}

#endif