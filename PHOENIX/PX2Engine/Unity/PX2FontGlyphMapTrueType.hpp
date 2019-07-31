// PX2FontGlyphMapTrueType.hpp

#ifndef PX2FONTGLYPHMAPTRUETYPE_HPP
#define PX2FONTGLYPHMAPTRUETYPE_HPP

#include "PX2UnityPre.hpp"
#include "PX2FontDefine.hpp"
#include "PX2Font.hpp"
#include "PX2FontGlyphMap.hpp"

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

namespace PX2
{

	class FontGlyphMapTrueType : public FontGlyphMap
	{
	public:
		FontGlyphMapTrueType ();
		virtual ~FontGlyphMapTrueType ();

		bool Initlize (int fontWidth, int fontHeight,
			const char *fontname, unsigned int fontExtStyle=FES_NORMAL);

		virtual void GetUnicodeCharSize (unsigned short wChar, float &width,
			float &height);
		virtual unsigned char *GetUnicodeCharBitmap (unsigned short wChar);

	protected:

#if defined(_WIN32) || defined(WIN32)
		HDC mhDC;
		HFONT mhFont;
		HBITMAP mhBitmap;
		HGDIOBJ mhOldBitmap;
		HGDIOBJ mhOldFont;
#endif
		unsigned int *mBitmapBits;
		unsigned char *mBitmapAlpha;
		float mEnCharWidth[256]; // 保存英文字符的宽度
		int mFontWidth;
		int mFontHeight;
		int mBitmapHeight;
	};

}

#endif