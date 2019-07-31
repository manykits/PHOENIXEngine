// PX2FontManager.hpp

#ifndef PX2FONTMANAGER_HPP
#define PX2FONTMANAGER_HPP

#include "PX2UnityPre.hpp"
#include "PX2FontDefine.hpp"
#include "PX2Font.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2TriMesh.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM FontManager : public Singleton<FontManager>
	{
	public:
		FontManager ();
		virtual ~FontManager();

		bool Initlize ();
		bool Terminate ();

		void DumpTex ();

		void Update ();
		void SetNeedUpdate();

		Font *GetDefaultFont ();

		Font *CreateTypeFont (int fontWidth, int fontHeight,
			const char *fontFilename, CharCodingType codingType,
			unsigned int fontExtStyle=FES_NORMAL);

		Font *CreateBitmapFont (int fontWidth, int fontHeight, const char *fontFilename,
			CharCodingType codingType);

		virtual void RenderText (TriMesh *mesh, Font *font, const char *text,
			unsigned int style, float x, float y, const Float2 &space, const Float4 &color,
			const Float4 &borderShadowColor, float shadowBorderSize, float scale=1.0f,
			bool doTransfer=false, float depth=0.0f);

		virtual void RenderTextRect(TriMesh *mesh, Font *font, const char *text,
			unsigned int style, unsigned int align, Rectf &rect, const Float2 &space,
			float offsetX, float offsetY,
			const Float4 &color, const Float4 &borderShadowColor,
			float shadowBorderSize, float scale=1.0f,  bool doTransfer=false);

		virtual void RenderTextRect (TriMesh *mesh, Font *font, const char *text, 
			unsigned int style,	Rectf &rect,  const Float2 &space, float offsetX, float offsetY, 
			bool autoWrap, const Float4 &color, const Float4 &borderShadowColor,
			float shadowBorderSize, float scale = 1.0f, bool doTransfer = false, 
			bool isPointAsPunctuation = true);

	protected:
		FontPtr mDefaultFont;
		std::vector<FontPtr> mFonts;
	};

#include "PX2FontManager.inl"

#define PX2_FM FontManager::GetSingleton()

}

#endif