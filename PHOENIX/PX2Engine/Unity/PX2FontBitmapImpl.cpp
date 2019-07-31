// PX2FontBitmapImpl.cpp

#include "PX2FontBitmapImpl.hpp"
#include "PX2CharCodingUTF8.hpp"
#include "PX2CharCodingGBK.hpp"
#include "PX2XMLData.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
FontBitmapImpl::FontBitmapImpl ()
	:
Font(Font::FT_BITMAP)
{
	mTexWidth = mFontPicSize;
	mTexHeight = mFontPicSize;
	mCharNumMaxCache = 5000;
}
//----------------------------------------------------------------------------
FontBitmapImpl::~FontBitmapImpl ()
{
	if (mCharCodingObj)
	{
		delete0(mCharCodingObj);
		mCharCodingObj = 0;
	}
}
//----------------------------------------------------------------------------
bool FontBitmapImpl::Initlize (int fontWidth, int fontHeight, 
	const char *fontFilename, CharCodingType codingType, unsigned int fontExtStyle)
{
	Font::Initlize(fontWidth, fontHeight, fontFilename, codingType, fontExtStyle);

	mFontFilename = fontFilename;

	if (CCT_UTF8 == codingType)
	{
		mCharCodingObj = new0 CharCodingUTF8();
	}
	else if (CCT_GBK == codingType)
	{
		mCharCodingObj = new0 CharCodingGBK();
	}

	mGlyphMap = this;

	std::string outPath;
	std::string outBaseFilename;
	StringHelp::SplitFilename(fontFilename, outPath, outBaseFilename);

	int bufferSize = 0;
	char *buffer = 0;
	if (PX2_RM.LoadBuffer(fontFilename, bufferSize, buffer))
	{
		XMLData data;
		if (data.LoadBuffer(buffer, bufferSize))
		{
			float imageWidth = 0.0f;
			float imageHeight = 0.0f;

			XMLNode rootNode = data.GetRootNode();
			XMLNode child = rootNode.IterateChild();
			while (!child.IsNull())
			{
				const std::string &nodeName = child.GetName();

				if ("image" == nodeName)
				{
					std::string text = child.GetText();
					std::string imagePath = outPath + text;

					mFontTex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(imagePath));
					if (mFontTex)
					{
						imageWidth = (float)mFontTex->GetWidth();
						imageHeight = (float)mFontTex->GetHeight();
					}
				}
				else if ("symbol" == nodeName)
				{
					std::string ch = child.GetChild("char").GetText();
					int x = StringHelp::StringToInt(child.GetChild("x").GetText());
					int y = StringHelp::StringToInt(child.GetChild("y").GetText());
					int width = StringHelp::StringToInt(child.GetChild("width").GetText());
					int height = StringHelp::StringToInt(child.GetChild("height").GetText());

					unsigned int unicode = mCharCodingObj->ToUnicode((const unsigned char*)ch.c_str());

					BitmapFontGlyph glyph;
					glyph.X = x;
					glyph.Y = y;
					glyph.numWidth = width;
					glyph.numHeight = height;

					float u0 = (float)x/imageWidth;
					float v0 = 1.0f-(float)(y+height)/imageHeight;
					float u1 = (x+width)/imageWidth;
					float v1 = v0 + (float)height/imageHeight;
					glyph.RectUV = Rectf(u0, v0, u1, v1);

					mMapGlyph[unicode] = glyph;
				}

				child = rootNode.IterateChild(child);
			}
		}
	}

	return true;
}
//----------------------------------------------------------------------------
bool FontBitmapImpl::IsSame (int fontWidth, int fontHeight,	
	const char *fontFilename, CharCodingType codingType,
	unsigned int fontExtStyle)
{
	PX2_UNUSED(fontExtStyle);

	if (mFontWidth != fontWidth)
		return false;

	if( mFontHeight != fontHeight)
		return false;

	if (mCodingType != codingType)
		return false;

	return mFontFilename == std::string(fontFilename);
}
//----------------------------------------------------------------------------
void FontBitmapImpl::GetUnicodeCharSize (unsigned short wChar, float &width,
	float &height)
{
	std::map<unsigned int, BitmapFontGlyph>::iterator it
		= mMapGlyph.find(wChar);

	if (it == mMapGlyph.end())
	{
		width = (float)mCharWidth;
		height = (float)mCharHeight;
	}
	else
	{
		width = (float)(*it).second.numWidth;
		height = (float)(*it).second.numHeight;
	}
}
//----------------------------------------------------------------------------
unsigned char *FontBitmapImpl::GetUnicodeCharBitmap (unsigned short wChar)
{
	PX2_UNUSED(wChar);
	return 0;
}
//----------------------------------------------------------------------------
Texture2D *FontBitmapImpl::TexUVMaping (const unsigned char *character,
	Rectf &rectUV)
{
	unsigned int unicode = mCharCodingObj->ToUnicode(character);
	std::map<unsigned int, BitmapFontGlyph>::iterator it 
		= mMapGlyph.find(unicode);

	if (it == mMapGlyph.end())
	{
		rectUV.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		rectUV = it->second.RectUV;
	}

	return mFontTex;
}
//----------------------------------------------------------------------------