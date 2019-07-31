// PX2MessageHeader.hpp

#ifndef PX2MESSAGEHEADER_HPP
#define PX2MESSAGEHEADER_HPP

#include "PX2NetPre.hpp"
#include "PX2NameValueCollection.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM MessageHeader : public NameValueCollection
	{
	public:
		MessageHeader();
		MessageHeader(const MessageHeader& messageHeader);
		virtual ~MessageHeader();

		MessageHeader& operator = (const MessageHeader& messageHeader);

		virtual void write(std::ostream& ostr) const;
		virtual void read(std::istream& istr);
		int getFieldLimit() const;
		void setFieldLimit(int limit);

		bool hasToken(const std::string& fieldName, const std::string& token) const;
		static void splitElements(const std::string& s, std::vector<std::string>& elements, bool ignoreEmpty = true);
		static void splitParameters(const std::string& s, std::string& value, NameValueCollection& parameters);
		static void splitParameters(const std::string::const_iterator& begin, const std::string::const_iterator& end, NameValueCollection& parameters);
		static void quote(const std::string& value, std::string& result, bool allowSpace = false);

	private:
		enum Limits
		{
			MAX_NAME_LENGTH = 256,
			MAX_VALUE_LENGTH = 8192,
			DFL_FIELD_LIMIT = 100
		};

		int _fieldLimit;
	};


}

#endif