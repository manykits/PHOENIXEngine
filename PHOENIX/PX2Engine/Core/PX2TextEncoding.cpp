// PX2TextEncoding.cpp

#include "PX2TextEncoding.hpp"
#include "PX2SingletonHolder.hpp"
#include "PX2Exception.hpp"
#include "PX2UTF8Encoding.hpp"
using namespace PX2;

namespace PX2
{

	class TextEncodingManager
	{
	public:
		TextEncodingManager()
		{
			TextEncoding* pUtf8Encoding(new UTF8Encoding);
			add(pUtf8Encoding, TextEncoding::GLOBAL);
		}

		~TextEncodingManager()
		{
		}

		void add(TextEncoding* pEncoding)
		{
			add(pEncoding, pEncoding->canonicalName());
		}

		void add(TextEncoding* pEncoding, const std::string& name)
		{
			_encodings[name] = pEncoding;
		}

		void remove(const std::string& name)
		{
			_encodings.erase(name);
		}

		TextEncoding* find(const std::string& name) const
		{
			EncodingMap::const_iterator it = _encodings.find(name);
			if (it != _encodings.end())
				return it->second;

			for (it = _encodings.begin(); it != _encodings.end(); ++it)
			{
				if (it->second->isA(name))
					return it->second;
			}
			return 0;
		}

	private:
		TextEncodingManager(const TextEncodingManager&);
		TextEncodingManager& operator = (const TextEncodingManager&);

		typedef std::map<std::string, TextEncoding*> EncodingMap;
		EncodingMap    _encodings;
	};
}
//----------------------------------------------------------------------------
const std::string TextEncoding::GLOBAL;
//----------------------------------------------------------------------------
TextEncoding::~TextEncoding()
{
}
//----------------------------------------------------------------------------
int TextEncoding::convert(const unsigned char* bytes) const
{
	return static_cast<int>(*bytes);
}
//----------------------------------------------------------------------------
int TextEncoding::convert(int ch, unsigned char* bytes, int length) const
{
	return 0;
}
//----------------------------------------------------------------------------
int TextEncoding::queryConvert(const unsigned char* bytes, int length) const
{
	return (int)*bytes;
}
//----------------------------------------------------------------------------
int TextEncoding::sequenceLength(const unsigned char* bytes, int length) const
{
	return 1;
}
//----------------------------------------------------------------------------
TextEncoding& TextEncoding::byName(const std::string& encodingName)
{
	TextEncoding* pEncoding = GetManager().find(encodingName);
	if (pEncoding)
		return *pEncoding;
	else
		throw NotFoundException(encodingName);
}
//----------------------------------------------------------------------------
TextEncoding* TextEncoding::find(const std::string& encodingName)
{
	return GetManager().find(encodingName);
}
//----------------------------------------------------------------------------
void TextEncoding::add(TextEncoding* pEncoding)
{
	GetManager().add(pEncoding, pEncoding->canonicalName());
}
//----------------------------------------------------------------------------
void TextEncoding::add(TextEncoding* pEncoding, const std::string& name)
{
	GetManager().add(pEncoding, name);
}
//----------------------------------------------------------------------------
void TextEncoding::remove(const std::string& encodingName)
{
	GetManager().remove(encodingName);
}
//----------------------------------------------------------------------------
TextEncoding* TextEncoding::global(TextEncoding* encoding)
{
	TextEncoding* prev = find(GLOBAL);
	add(encoding, GLOBAL);
	return prev;
}
//----------------------------------------------------------------------------
TextEncoding& TextEncoding::global()
{
	return byName(GLOBAL);
}
//----------------------------------------------------------------------------
static SingletonHolder<PX2::TextEncodingManager> sh;
//----------------------------------------------------------------------------
PX2::TextEncodingManager& TextEncoding::GetManager()
{
	return *sh.Get();
}
//----------------------------------------------------------------------------