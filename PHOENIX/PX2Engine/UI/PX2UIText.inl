// PX2UIText.inl

//----------------------------------------------------------------------------
inline UIText::FontType UIText::GetFontType() const
{
	return mFontType;
}
//----------------------------------------------------------------------------
inline Font *UIText::GetFont()
{
	return mFont;
}
//----------------------------------------------------------------------------
inline const std::string &UIText::GetText() const
{
	return mText;
}
//----------------------------------------------------------------------------
inline const std::string &UIText::GetKey() const
{
	return mKey;
}
//----------------------------------------------------------------------------
inline int UIText::GetFontStyle() const
{
	return mFontStyle;
}
//----------------------------------------------------------------------------
inline bool UIText::IsItalic() const
{
	return 0 != (mFontStyle&FES_ITALIC);
}
//----------------------------------------------------------------------------
inline bool UIText::IsUnderLine() const
{
	return 0 != (mFontStyle&FES_UNDERLINE);
}
//----------------------------------------------------------------------------
inline int UIText::GetDrawStyle() const
{
	return mDrawStyle;
}
//----------------------------------------------------------------------------
inline int UIText::GetAligns() const
{
	return mTextAligns;
}
//----------------------------------------------------------------------------
inline Float3 UIText::GetFontColor() const
{
	return MathHelp::Float4ToFloat3(mFontColor);
}
//----------------------------------------------------------------------------
inline float UIText::GetFontAlpha() const
{
	return mFontColor[3];
}
//----------------------------------------------------------------------------
inline Float3 UIText::GetBorderShadowColor() const
{
	return MathHelp::Float4ToFloat3(mBorderShadowColor);
}
//----------------------------------------------------------------------------
inline float UIText::GetBorderShadowAlpha() const
{
	return mBorderShadowColor[3];
}
//----------------------------------------------------------------------------
inline float UIText::GetShadowBorderSize() const
{
	return mShadowBorderSize;
}
//----------------------------------------------------------------------------
inline UIText::RectUseage UIText::GetRectUseage() const
{
	return mRectUseage;
}
//----------------------------------------------------------------------------
inline const Rectf &UIText::GetRect() const
{
	return mRect;
}
//----------------------------------------------------------------------------
inline const Float2 &UIText::GetOffset() const
{
	return mOffset;
}
//----------------------------------------------------------------------------
inline const Float2 &UIText::GetSpace() const
{
	return mSpace;
}
//----------------------------------------------------------------------------
inline bool UIText::IsAutoWarp() const
{
	return mIsAutoWarp;
}
//----------------------------------------------------------------------------
inline bool UIText::IsDoCharTranslate() const
{
	return mIsDoCharTranslate;
}
//----------------------------------------------------------------------------
inline bool UIText::IsPointAsPunctuation() const
{
	return mIsPointAsPunctuation;
}
//----------------------------------------------------------------------------
inline float UIText::GetFontScale() const
{
	return mFontScale;
}
//----------------------------------------------------------------------------
inline int UIText::GetFontWidth()
{
	return mFontWidth;
}
//----------------------------------------------------------------------------
inline int UIText::GetFontHeight()
{
	return mFontHeight;
}
//----------------------------------------------------------------------------