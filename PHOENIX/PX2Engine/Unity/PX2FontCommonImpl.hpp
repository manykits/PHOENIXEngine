// PX2FontCommonImpl.hpp

#ifndef PX2FONTCOMMONIMPL_HPP
#define PX2FONTCOMMONIMPL_HPP

#include "PX2Font.hpp"

namespace PX2
{

	class CharLinkNode
	{
	public:
		CharLinkNode ()
		{
			IndexToCache = -1;
			CharUniqueID = 0;
			Prev = 0;
			Next = 0;
		}
		~CharLinkNode ()
		{
		}

		int IndexToCache;
		unsigned int CharUniqueID;
		CharLinkNode *Prev;
		CharLinkNode *Next;
	};

	class FontCommonImpl : public Font
	{
	public:
		FontCommonImpl (FontGlyphMap::GlyphType type
			=FontGlyphMap::GT_FREETYPE);
		virtual ~FontCommonImpl ();

		FontGlyphMap::GlyphType GetGlyphType ();

		virtual bool Initlize (int fontWidth, int fontHeight, 
			const char *fontFilename, CharCodingType codingType,
			unsigned int fontExtStyle);

		virtual bool IsSame (int fontWidth, int fontHeight,
			const char *fontFilename, CharCodingType codingType,
			unsigned int fontExtStyle);

	protected:
		bool CreateTexture ();
		virtual Texture2D *TexUVMaping (const unsigned char *character,
			Rectf &rectUV);
		void UpdateTexture (const unsigned char *character, int cell);
		void UseCacheCell (CharLinkNode *p);

		FontGlyphMap::GlyphType mGlyphType;
		Texture2DPtr mFontTex;
		unsigned char *mTexData;
		int mCharNumWidth; // 缓存字符一行个数
		int mCharNumHeight; // 缓存字符的行数
		typedef std::map<unsigned int, int> CacheMap;
		CacheMap mCacheMap;
		CharLinkNode *mCharList;
		CharLinkNode *mCharListHead;
		CharLinkNode *mCharListEnd;
	};

#include "PX2FontCommonImpl.inl"

}

#endif