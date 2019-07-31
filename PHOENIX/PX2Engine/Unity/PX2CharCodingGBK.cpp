// PX2CharCodingGBK.cpp

#include "PX2CharCodingGBK.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

CharCodingGBK::CharCodingGBK ()
{
}
//----------------------------------------------------------------------------
CharCodingGBK::~CharCodingGBK ()
{
}
//----------------------------------------------------------------------------
int CharCodingGBK::JumpOverSpaces (const char *text)
{
	int charNum = 0;
	const unsigned char *p = (const unsigned char*)text;

	bool cont = true;
	while (cont)
	{
		if (*p == ' ') // 半角空格
		{
			p++;
			charNum++;
		}
		else if (*p == 0xa1 && *(p+1) == 0xa1) // 全角空格
		{
			p += 2;
			charNum += 2;
		}
		else
		{
			break;
		}
	}

	return charNum;
}
//----------------------------------------------------------------------------
int CharCodingGBK::GetAChar (const char *text, unsigned char *character)
{
	if (0==text || (int)strlen(text)<=1)
		return 0;

	int startGroup = 0x81;

	const unsigned char *p = (unsigned char *)text;
	unsigned char c = *p;
	unsigned char c1 = *(p + 1);

	if (c >= startGroup) // 中文
	{
		character[0] = c;
		character[1] = c1;
		character[2] = 0;
		return 2;
	}
	else if ((c >= 32) && (c <= 127)) // 英文，包括空格
	{
		character[0] = 0;
		character[1] = c;
		character[2] = 0;
		return 1;
	}
	else
	{
		character[0] = 0;
		character[1] = 0;
		return 1;
	}
}
//----------------------------------------------------------------------------
int CharCodingGBK::GetCharBytes (const unsigned char *character)
{
	if (character[0] >= 0x81)
		return 2; // 中文

	return 1; // 英文
}
//----------------------------------------------------------------------------
int CharCodingGBK::GetControlCode (const char *text, CharCtrlCode &ctrlCode, 
	bool doTransfer)
{
	if (!text)
		return 0;

	const unsigned char *p = (unsigned char *)text;
	unsigned char c = *p;

	if (c == 0)
	{
		ctrlCode = CCC_ENDOFSTRING;
		return 0;
	}

	unsigned char c1 = *(p + 1);
	if (c == '\n')
	{
		ctrlCode = CCC_NEWLINE;
		return 1;
	}
	else if (c == '\\' && c1 == 'n')
	{
		ctrlCode = CCC_NEWLINE;
		return 2;
	}
	else if (c == '\r')
	{
		ctrlCode = CCC_RETURN;
		return 1;
	}
	else if (doTransfer && c == '#' && c1 != '#')
	{
		ctrlCode = CCC_TRANSFER;
		return 1;
	}
	else
	{
		ctrlCode = CCC_CHARACTER;
		return 0;
	}
}
//----------------------------------------------------------------------------
bool CharCodingGBK::IsEnglish (const unsigned char *character)
{
	if (character[1] >= 33 && character[1] <= 127)
		return true;

	return false;
}
//----------------------------------------------------------------------------
bool CharCodingGBK::IsPunctuation(const unsigned char *character)
{
	if (character[0] == 0 && 
		(character[1] == ',' || character[1] == '.' || character[1] == '!' || 
		 character[1] == '?' || character[1] == ':' || character[1] == ';'))
		return true;

	return false;
}
//----------------------------------------------------------------------------
bool CharCodingGBK::IsPoint(const unsigned char *character)
{
	if (character[0] == 0 && character[1] == '.')
		return true;

	return false;
}
//----------------------------------------------------------------------------
unsigned short CharCodingGBK::ToUnicode(const unsigned char *character)
{
	wchar_t wChar;

	if (character[0] == 0)
		StringHelp::AnsiToUnicode(&wChar, 1, (const char*)character+1, 1);

	StringHelp::AnsiToUnicode(&wChar, 1, (const char*)character, 2);

	return wChar;
}
//----------------------------------------------------------------------------
unsigned int CharCodingGBK::ToUniqueID(const unsigned char* character)
{
	if (character[0] == 0)
		return character[1];

	return ((unsigned int)character[0]) * 256 + character[1];
}
//----------------------------------------------------------------------------