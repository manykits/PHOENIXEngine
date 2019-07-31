// PX2FontGlyphMapTrueType.cpp

#include "PX2FontGlyphMapTrueType.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
FontGlyphMapTrueType::FontGlyphMapTrueType ()
{
#if defined(_WIN32) || defined(WIN32)
	mhDC = 0;
	mhFont = 0;
	mhBitmap = 0;
	mhOldBitmap = 0;
	mhOldFont = 0;
#endif
}
//----------------------------------------------------------------------------
FontGlyphMapTrueType::~FontGlyphMapTrueType ()
{
#if defined(_WIN32) || defined(WIN32)
	if (mhDC)
	{
		DeleteDC(mhDC);
		mhDC = 0;
	}

	if (mhBitmap)
	{
		DeleteObject(mhBitmap);
		mhBitmap = 0;
	}

	if (mhFont)
	{
		DeleteObject(mhFont);
		mhFont = 0;
	}
#endif

	if (mBitmapAlpha)
	{
		delete1(mBitmapAlpha);
		mBitmapAlpha = 0;
	}
}
//----------------------------------------------------------------------------
bool FontGlyphMapTrueType::Initlize (int fontWidth, int fontHeight,
	const char *fontname, unsigned int fontExtStyle)
{
	mFontWidth = fontWidth;
	mFontHeight = fontHeight;

#if defined(_WIN32) || defined(WIN32)
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = mFontWidth;
	bmi.bmiHeader.biHeight = -mFontHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount = 32;

	mhDC = CreateCompatibleDC(0);
	if (0 == mhDC)
		return false;

	SetMapMode(mhDC, MM_TEXT);

	mhBitmap = CreateDIBSection(mhDC, &bmi, DIB_RGB_COLORS, 
		(VOID**)&mBitmapBits, 0, 0);
	if (0 == mhBitmap)
		return false;
	memset(mBitmapBits, 0, mFontWidth*mFontHeight*4*sizeof(char));
	
	mBitmapAlpha = new1<unsigned char>(mFontWidth*mFontHeight);

	unsigned int nItalic = FALSE;
	unsigned int nUnderline = FALSE;
	if (fontExtStyle & FES_ITALIC )
		nItalic = TRUE;

	if (fontExtStyle & FES_UNDERLINE )
		nUnderline = TRUE;

	mhFont = CreateFont(-mFontHeight, 0, 0, 0, FW_NORMAL, nItalic, nUnderline,
		FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, VARIABLE_PITCH, fontname);
	if (0 == mhFont)
		return false;

	mhOldBitmap = SelectObject(mhDC, mhBitmap);
	mhOldFont = SelectObject(mhDC, mhFont);

	SetTextColor(mhDC, RGB(255, 255, 255));
	SetBkColor(mhDC, 0x00000000);
	SetTextAlign(mhDC, TA_TOP);

	for (int i=0; i<256; i++)
	{
		SIZE size;
		GetTextExtentPoint32(mhDC, (LPCTSTR)&i, 1, &size);
		mEnCharWidth[i] = (float)size.cx;
	}
#else
	PX2_UNUSED(fontname);
	PX2_UNUSED(fontExtStyle);
#endif

	return true;
}
//----------------------------------------------------------------------------
void FontGlyphMapTrueType::GetUnicodeCharSize (unsigned short wChar,
	float &width, float &height)
{
	if (wChar < (unsigned short)256)
	{
		width = mEnCharWidth[wChar];
		height = (float)mFontHeight;
	}
	else
	{
		width = (float)mFontWidth;
		height = (float)mFontHeight;
	}
}
//----------------------------------------------------------------------------
unsigned char *FontGlyphMapTrueType::GetUnicodeCharBitmap (
	unsigned short wChar)
{
	memset(mBitmapBits, 0, mFontWidth*mFontHeight*4*sizeof(char));

#if defined(_WIN32) || defined(WIN32)
	TextOutW(mhDC, 0, 0, (LPCWSTR)&wChar, 1);
#endif

	for (int y=0; y<mFontHeight; y++)
	{   
		unsigned int *src = mBitmapBits + mFontWidth * y;
		unsigned char *dst = mBitmapAlpha + mFontWidth * y;

		for (int x=0; x <mFontWidth; x++)
		{
			*dst = (unsigned char)(*src & 0xff);
			src++;
			dst++;
		}
	}

	return mBitmapAlpha;
}
//----------------------------------------------------------------------------