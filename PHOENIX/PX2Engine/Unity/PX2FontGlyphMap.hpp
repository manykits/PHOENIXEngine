// PX2FontGlyphMap.hpp

#ifndef PX2FONTGLYPHMAP_HPP
#define PX2FONTGLYPHMAP_HPP

#include "PX2UnityPre.hpp"

namespace PX2
{

	class FontGlyphMap
	{
	public:
		FontGlyphMap () {}
		virtual ~FontGlyphMap () {}

		enum GlyphType
		{
			GT_FREETYPE,
			GT_TRUETYPE,
			GT_MAX_TYPE
		};

		/// 取得所传UNICODE字符的长和宽
		virtual void GetUnicodeCharSize (unsigned short wChar, float &width,
			float &height) = 0;
		
		/// 根据传入的UNICODE字符反回对应的点阵位图
		virtual unsigned char *GetUnicodeCharBitmap (unsigned short wChar) = 0;		
	};

}

#endif