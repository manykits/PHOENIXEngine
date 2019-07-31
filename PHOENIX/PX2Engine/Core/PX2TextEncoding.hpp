// PX2TextEncoding.hpp

#ifndef PX2TEXTENCODING_HPP
#define PX2TEXTENCODING_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class TextEncodingManager;

	class PX2_ENGINE_ITEM TextEncoding
	{
	public:
		enum
		{
			MAX_SEQUENCE_LENGTH = 4
		};

		typedef int CharacterMap[256];
		virtual ~TextEncoding();

		virtual const char* canonicalName() const = 0;
		virtual bool isA(const std::string& encodingName) const = 0;
		virtual const CharacterMap& characterMap() const = 0;
		virtual int convert(const unsigned char* bytes) const;
		virtual	int queryConvert(const unsigned char* bytes, int length) const;
		virtual int sequenceLength(const unsigned char* bytes, int length) const;
		virtual int convert(int ch, unsigned char* bytes, int length) const;
		static TextEncoding& byName(const std::string& encodingName);
		static TextEncoding *find(const std::string& encodingName);
		static void add(TextEncoding *encoding);
		static void add(TextEncoding *encoding, const std::string& name);
		static void remove(const std::string& encodingName);
		static TextEncoding *global(TextEncoding *encoding);
		static TextEncoding& global();

		static const std::string GLOBAL;

	protected:
		static TextEncodingManager& GetManager();
	};

}

#endif