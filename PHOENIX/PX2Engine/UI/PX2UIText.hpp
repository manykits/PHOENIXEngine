// PX2UIText.hpp

#ifndef PX2UITEXT_HPP
#define PX2UITEXT_HPP

#include "PX2UIPre.hpp"
#include "PX2Font.hpp"
#include "PX2MathHelp.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIText : public TriMesh
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UIText);
		PX2_DECLARE_STREAM(UIText);

	public:
		UIText();
		virtual ~UIText();

		enum FontType
		{
			FT_FREETYPE,
			FT_BITMAP,
			FT_MAXTYPE
		};
		FontType GetFontType() const;

		void SetFont(const std::string &fontFilename);
		void SetFont(const std::string &fontFilename, int fontWidth,
			int fontHeight, unsigned int fontStyle = FES_NORMAL);
		void SetFontBimMap(const std::string &fontFilename);
		Font *GetFont();

		virtual void SetText(const std::string &text);
		const std::string &GetText() const;
		void SetKey(const std::string &key);
		const std::string &GetKey() const;

		// FES_NORMAL
		// FES_ITALIC
		// FES_UNDERLINE
		void SetFontStyle(int style);
		int GetFontStyle() const;
		bool IsItalic() const;
		bool IsUnderLine() const;

		// FD_NORMAL 0
		// FD_SHADOW 1
		// FD_BORDER 2
		void SetDrawStyle(int style);
		int GetDrawStyle() const;

		void SetFontColor(const Float3 &color);
		Float3 GetFontColor() const;
		void SetFontAlpha(float alpha);
		float GetFontAlpha() const;

		void SetBorderShadowColor(const Float3 &color);
		Float3 GetBorderShadowColor() const;

		void SetBorderShadowAlpha(float alpha);
		float GetBorderShadowAlpha() const;

		void SetShadowBorderSize(float size);
		float GetShadowBorderSize() const;

		enum RectUseage
		{
			RU_NONE,
			RU_ALIGNS,
			RU_CLIPWARP
		};
		void SetRectUseage(RectUseage useage);
		RectUseage GetRectUseage() const;

		void SetRect(const Rectf &rect);
		const Rectf &GetRect() const;
		void SetOffset(const Float2 &offset);
		const Float2 &GetOffset() const;
		void SetSpace(const Float2 &space); // 列间距,和行间距
		const Float2 &GetSpace() const;

		// TEXTALIGN_LEFT
		// TEXTALIGN_HCENTER
		// TEXTALIGN_RIGHT
		// TEXTALIGN_TOP
		// TEXTALIGN_VCENTER
		// TEXTALIGN_BOTTOM
		void SetAligns(int aligns);
		int GetAligns() const;
		int GetHAlign() const;
		int GetVAlign() const;

		// 使用自动换行
		// RectUseage需要设置为RectUseage::RU_CLIPWARP
		// 保持单词完整, 如果单词超过Rect::Width,自动换行
		void SetAutoWarp(bool warp);
		bool IsAutoWarp() const;

		void SetDoCharTranslate(bool doTranslate);
		bool IsDoCharTranslate() const;

		void SetPointAsPunctuation(bool asPunctuation);
		bool IsPointAsPunctuation() const;

		void SetFontScale(float scale);
		float GetFontScale() const;

		void SetFontSize(int size);
		void SetFontSize(int width, int height);
		void SetFontWidthHeight(int width, int height);
		void SetFontWidth(int width);
		int GetFontWidth();
		void SetFontHeight(int height);
		int GetFontHeight();

		virtual float GetTextWidth() const;
		virtual float GetTextWidth(const std::string &text) const;

	protected:
		void _Init();
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		void ReCreateFont();
		void ReCreate();

		std::string mText;
		std::string mKey;
		RectUseage mRectUseage;
		int mTextAligns;
		int mFontStyle;
		int mDrawStyle;
		Float4 mFontColor;
		Float4 mBorderShadowColor;
		float mShadowBorderSize;
		std::string mFontFilename;
		int mFontWidth;
		int mFontHeight;
		Rectf mRect;
		Float2 mOffset;
		Float2 mSpace;
		bool mIsAutoWarp;
		bool mIsDoCharTranslate;
		bool mIsPointAsPunctuation;
		float mFontScale;

		Texture2DPtr mFontTex;

		FontType mFontType;

		FontPtr mFont;
		bool mIsNeedReCreate;
		bool mIsNeedReCreateFont;
	};

	PX2_REGISTER_STREAM(UIText);
	typedef Pointer0<UIText> UITextPtr;
#include "PX2UIText.inl"

}

#endif