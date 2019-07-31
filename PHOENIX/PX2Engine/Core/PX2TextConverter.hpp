// PX2TextConverter.hpp

#ifndef PX2TEXTCONVERTER_HPP
#define PX2TEXTCONVERTER_HPP

#include "PX2CorePre.hpp"
#include "PX2TextEncoding.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TextConverter
	{
	public:
		typedef int(*Transform)(int);

		TextConverter(const TextEncoding& inEncoding, const TextEncoding& outEncoding, int defaultChar = '?');
		~TextConverter();
		/// Destroys the TextConverter.

		int convert(const std::string& source, std::string& destination, Transform trans);
		int convert(const void* source, int length, std::string& destination, Transform trans);
		int convert(const std::string& source, std::string& destination);
		int convert(const void* source, int length, std::string& destination);

	private:
		TextConverter();
		TextConverter(const TextConverter&);
		TextConverter& operator = (const TextConverter&);

		const TextEncoding& _inEncoding;
		const TextEncoding& _outEncoding;
		int _defaultChar;
	};

}

#endif