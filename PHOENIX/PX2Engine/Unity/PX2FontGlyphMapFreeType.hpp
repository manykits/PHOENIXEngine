// PX2FontGlyphMapFreeType.hpp

#ifndef PX2FONTGLYPHMAPFREETYPE_HPP
#define PX2FONTGLYPHMAPFREETYPE_HPP

#include "PX2FontDefine.hpp"
#include "PX2FontGlyphMap.hpp"
#include "PX2SmartPointer.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H

namespace PX2
{

	class FontGlyphMapFreeType : public FontGlyphMap
	{
	public:
		FontGlyphMapFreeType ();
		virtual ~FontGlyphMapFreeType ();

		bool Initlize (int fontWidth, int fontHeight,
			const char *fontname, unsigned int fontExtStyle=FES_NORMAL);

		virtual void GetUnicodeCharSize (unsigned short wChar, float &width,
			float &height);
		virtual unsigned char *GetUnicodeCharBitmap (unsigned short wChar);

		struct FontFaceInfo
		{
			FontFaceInfo ()
			{
				Face = 0;
				FaceBuffer = 0;
				FaceBufferSize = 0;
			}
			~FontFaceInfo ()
			{
				FT_Done_Face(Face);
			}

			std::string Fontname;
			Pointer1<char> FaceBuffer;
			int FaceBufferSize;
			FT_Face Face;
		};
		static bool InitlizeFreeType();
		static void TerminateFreeType();
		static FT_Face CreateFontFace(const char *fontName);
		static std::vector<FontFaceInfo*> mFontFaceInfos;

	protected:
		void LoadCharGlyph(unsigned short wChar);

		int mBaseLinePos; // 基线到文字上边界的距离(像素)
		bool mIsDrawUnderLine;
		int mUnderLinePos; // 下划线到基线的偏移
		int mUnderLineThick;
		int mFontWidth;
		int mFontHeight;
		int mBitmapHeight;
		FT_Face mFace;
		unsigned char *mBitmapAlpha;
		float mEnCharWidth[256]; // 保存英文字符的宽度
		
		static FT_Library msFTLibrary;
	};

}

#endif