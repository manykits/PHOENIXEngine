// PX2FontGlyphMapFreeType.cpp

#include "PX2FontGlyphMapFreeType.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
FT_Library FontGlyphMapFreeType::msFTLibrary = 0;
std::vector<FontGlyphMapFreeType::FontFaceInfo*> FontGlyphMapFreeType
	::mFontFaceInfos;
//----------------------------------------------------------------------------
FontGlyphMapFreeType::FontGlyphMapFreeType ()
	:
mBaseLinePos(0),
mUnderLinePos(0),
mUnderLineThick(0),
mFontWidth(0),
mFontHeight(0),
mBitmapHeight(0),
mFace(0),
mBitmapAlpha(0)
{
	memset(mEnCharWidth, 0, 256*sizeof(float));
}
//----------------------------------------------------------------------------
FontGlyphMapFreeType::~FontGlyphMapFreeType ()
{	
	if (mBitmapAlpha)
	{
		delete1(mBitmapAlpha);
		mBitmapAlpha = 0;
	}
}
//----------------------------------------------------------------------------
bool FontGlyphMapFreeType::Initlize (int fontWidth, int fontHeight,
	const char *fontname, unsigned int fontExtStyle)
{
	mFace = CreateFontFace(fontname);
	if (!mFace)
	{
		assertion(false, "Initlize failed.\n");
		return false;
	}

	float scale = (float)fontHeight/(float)(mFace->ascender - mFace->descender);

	mBaseLinePos = (int)(mFace->ascender * scale);
	mUnderLinePos = (int)(mFace->underline_position * scale);
	mUnderLineThick = (int)(mFace->underline_thickness * scale);

	if (mUnderLineThick < 1)
		mUnderLineThick = 1;

	mFontWidth = fontWidth;
	mFontHeight = fontHeight;
	mBitmapHeight = fontHeight;

	mIsDrawUnderLine = (fontExtStyle&FES_UNDERLINE) > 0;

	mBitmapAlpha = new1<unsigned char>(mFontWidth*mBitmapHeight);

	for (unsigned short i=0; i<256; i++)
	{
		LoadCharGlyph(i);

		mEnCharWidth[i] = (float)(mFace->glyph->metrics.horiAdvance/64.0f);
	}

	return true;
}
//----------------------------------------------------------------------------
void FontGlyphMapFreeType::GetUnicodeCharSize (unsigned short wChar,
	float &width, float &height)
{
	if (wChar < 256)
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
unsigned char *FontGlyphMapFreeType::GetUnicodeCharBitmap(unsigned short wChar)
{
	LoadCharGlyph(wChar);

	int offsetY = mBaseLinePos-(int)(
		(float)mFace->glyph->metrics.horiBearingY/64.0f);
	int offsetX = (int)((float)mFace->glyph->metrics.horiBearingX/64.0f);

	if (offsetX < 0)
		offsetX = 0;

	if (mBitmapAlpha)
	{
		memset(mBitmapAlpha, 0, mFontWidth*mBitmapHeight*sizeof(unsigned char));
	}

	FT_Bitmap *bitmap = &(mFace->glyph->bitmap);

	if (bitmap->width>=mFontWidth && offsetX>0)
		offsetX = 0;

	if (bitmap->rows>=mBitmapHeight && offsetY>0)
		offsetY = 0;

	if (bitmap->pixel_mode == FT_PIXEL_MODE_MONO)
	{
		for (int y = 0; y < bitmap->rows; ++y)
		{
			if (y + offsetY < 0 || y + offsetY >= mBitmapHeight)
				continue;

			unsigned char *src = bitmap->buffer + (y * bitmap->pitch);
			unsigned char *dst = mBitmapAlpha + mFontWidth * (y + offsetY) + offsetX;
			for (int x = 0; x < bitmap->width; ++x)
			{
				if( x + offsetX < 0 || x + offsetX >= mFontWidth )
					continue;

				if( src[x / 8] & (0x80 >> (x & 7)) )
					*dst = 0xff;
				else
					*dst = 0x00;
				dst++;
			}
		}
	}
	else if (bitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
	{
		for (int y = 0; y < bitmap->rows; y++)
		{
			if(y+offsetY<0 || y+offsetY>=mBitmapHeight)
				continue;

			unsigned char *src = bitmap->buffer + (y * bitmap->pitch);
			unsigned char *dst = mBitmapAlpha + mFontWidth * (y + offsetY) + offsetX;

			for (int x = 0; x < bitmap->width; x++)
			{
				if(x+offsetX<0 || x+offsetX>=mFontWidth)
					continue;

				*dst = *src;

				src++;
				dst++;
			}
		}
	}

	if (mIsDrawUnderLine)
	{
		offsetY = mBaseLinePos + mUnderLinePos;
		assertion(mUnderLineThick + offsetY <= mBitmapHeight, "");

		// 绘制下划线
		for (int i=0; i<mUnderLineThick; i++)
		{
			unsigned char *dst = mBitmapAlpha + mFontWidth * (i + offsetY);
			for( int x = 0; x < mFontWidth; x++ )
			{
				*dst = 0xff;
				dst++;
			}
		}
	}

	return mBitmapAlpha;
}
//----------------------------------------------------------------------------
void FontGlyphMapFreeType::LoadCharGlyph(unsigned short wChar)
{
	FT_Set_Pixel_Sizes(mFace, mFontWidth, mFontHeight);

	unsigned int loadFlag = FT_LOAD_RENDER
		|FT_LOAD_FORCE_AUTOHINT|FT_LOAD_TARGET_NORMAL;

	if (0!=FT_Load_Char(mFace, wChar, loadFlag))
	{
		if (0!=FT_Load_Char(mFace, wChar, FT_LOAD_DEFAULT))
		{
			assertion(false, "FT_Load_Char error.\n");
		}
	}
}
//----------------------------------------------------------------------------
bool FontGlyphMapFreeType::InitlizeFreeType()
{
	int error = FT_Init_FreeType(&msFTLibrary);
	assertion(0==error, "FT_Init_FreeType error.\n");

	return 0==error;
}
//----------------------------------------------------------------------------
void FontGlyphMapFreeType::TerminateFreeType()
{
	for(size_t i=0; i<mFontFaceInfos.size(); i++)
	{
		FontFaceInfo *info = mFontFaceInfos[i];
		delete0(info);
	}
	mFontFaceInfos.clear();

	FT_Done_FreeType(msFTLibrary);
}
//----------------------------------------------------------------------------
FT_Face FontGlyphMapFreeType::CreateFontFace(const char *fontName)
{
	if (!msFTLibrary)
	{
		assertion(false, "msFTLibrary is not initlized yet.");

		return 0;
	}

	for (int i=0; i<(int)mFontFaceInfos.size(); i++)
	{
		if (fontName == mFontFaceInfos[i]->Fontname)
			return mFontFaceInfos[i]->Face;
	}

	FontFaceInfo *info = new0 FontFaceInfo();
	info->Fontname = fontName;

	char *buffer = 0;
	int bufferSize = 0;
	if (ResourceManager::GetSingleton().LoadBuffer(info->Fontname,
		bufferSize, buffer))
	{
		info->FaceBufferSize = bufferSize;
		info->FaceBuffer = buffer;

		int error = FT_New_Memory_Face(msFTLibrary, 
			(const unsigned char*)buffer, 
			(FT_Long)bufferSize, 0, &(info->Face));

		if (0 != error)
		{
			assertion(false, "FT_New_Memory_Face error.\n");
			delete0(info);
			return 0;
		}
		else
		{
			// do not delete info.FaceBuffer before destructor
			//delete1(info.FaceBuffer);
			
			mFontFaceInfos.push_back(info);

			return info->Face;
		}
	}
	else
	{
		delete0(info);
		return 0;
	}
}
//----------------------------------------------------------------------------