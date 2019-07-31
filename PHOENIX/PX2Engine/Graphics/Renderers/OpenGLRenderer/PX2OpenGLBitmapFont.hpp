// PX2OpenGLBitmapFont.hpp

#ifndef PX2OPENGLBITMAPFONT_HPP
#define PX2OPENGLBITMAPFONT_HPP

#ifdef PX2_USE_OPENGL

#include "PX2OpenGLRendererPre.hpp"

namespace PX2
{

	class BitmapFontChar
	{
	public:
		BitmapFontChar(int xOrigin, int yOrigin, int xSize, int ySize,
			const unsigned char* bitmap)
		{
			mXOrigin = xOrigin;
			mYOrigin = yOrigin;
			mXSize = xSize;
			mYSize = ySize;
			mBitmap = bitmap;
		}

		int mXOrigin, mYOrigin, mXSize, mYSize;
		const unsigned char* mBitmap;
	};

	class BitmapFont
	{
	public:
		BitmapFont(const char* name, int numCharacters,
			const BitmapFontChar* const* characters)
		{
			mName = name;
			mNumCharacters = numCharacters;
			mCharacters = characters;
		}

		const char* mName;
		int mNumCharacters;
		const BitmapFontChar* const* mCharacters;
	};

	extern const BitmapFont gVerdanaS16B0I0;

}

#endif

#endif
