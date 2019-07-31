// PX2UTF8Encoding.hpp

#ifndef PX2UTF8ENCODING_HPP
#define PX2UTF8ENCODING_HPP

#include "PX2CorePre.hpp"
#include "PX2TextEncoding.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UTF8Encoding : public TextEncoding
	{
	public:
		UTF8Encoding();
		~UTF8Encoding();

		const char* canonicalName() const;
		bool isA(const std::string& encodingName) const;
		const CharacterMap& characterMap() const;
		int convert(const unsigned char* bytes) const;
		int convert(int ch, unsigned char* bytes, int length) const;
		int queryConvert(const unsigned char* bytes, int length) const;
		int sequenceLength(const unsigned char* bytes, int length) const;

		static bool isLegal(const unsigned char *bytes, int length);

	private:
		static const char* _names[];
		static const CharacterMap _charMap;
	};


}

#endif