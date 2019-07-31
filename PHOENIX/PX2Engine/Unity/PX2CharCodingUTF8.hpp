// PX2CharCodingUTF8.hpp

#ifndef PX2CHARCODINGUTF8_HPP
#define PX2CHARCODINGUTF8_HPP

#include "PX2CharCoding.hpp"

namespace PX2
{

	class CharCodingUTF8 : public CharCoding
	{
	public:
		CharCodingUTF8 ();
		virtual ~CharCodingUTF8 ();

		virtual int JumpOverSpaces (const char *text);
		virtual int GetAChar (const char *text, unsigned char *character);
		virtual int GetCharBytes (const unsigned char *character);
		virtual int GetControlCode (const char *text, CharCtrlCode &ctrlCode, 
			bool doTransfer);
		virtual bool IsEnglish (const unsigned char *character);
		virtual bool IsPunctuation(const unsigned char *character);
		virtual bool IsPoint(const unsigned char *character);
		virtual unsigned short ToUnicode(const unsigned char *character);
		virtual unsigned int ToUniqueID(const unsigned char* character);
	};

}

#endif