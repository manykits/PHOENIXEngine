// PX2FontBitmapImpl.hpp

#ifndef PX2FONTBITMAPIMPL_HPP
#define PX2FONTBITMAPIMPL_HPP

#include "PX2FontGlyphMapFreeType.hpp"
#include "PX2Font.hpp"
#include "PX2Texture2D.hpp"

namespace PX2
{
	
	class FontBitmapImpl : public Font, public FontGlyphMap
	{
	public:
		FontBitmapImpl ();
		virtual ~FontBitmapImpl ();

		virtual bool Initlize (int fontWidth, int fontHeight, 
			const char *fontFilename, CharCodingType codingType,
			unsigned int fontExtStyle);

		virtual bool IsSame (int fontWidth, int fontHeight,	
			const char *fontFilename, CharCodingType codingType,
			unsigned int fontExtStyle);

	protected:
		virtual void GetUnicodeCharSize (unsigned short wChar, float &width,
			float &height);
		virtual unsigned char *GetUnicodeCharBitmap (unsigned short wChar);
		virtual Texture2D *TexUVMaping (const unsigned char *character,
			Rectf &rectUV);

		struct BitmapFontGlyph
		{
			BitmapFontGlyph ()
			{
				X = 0;
				Y = 0;
				numWidth = 0;
				numHeight = 0;
			}

			int X;
			int Y;
			int numWidth;
			int numHeight;
			Rectf RectUV;
		};

		Texture2DPtr mFontTex;
		int mCharNumWidth; // 缓存字符一行个数
		int mCharNumHeight; // 缓存字符的行数
		std::map<unsigned int, BitmapFontGlyph> mMapGlyph;
	};

}

#endif