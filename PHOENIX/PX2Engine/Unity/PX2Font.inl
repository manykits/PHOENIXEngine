// PX2Font.inl

//----------------------------------------------------------------------------
inline const std::string &Font::GetFontFilename () const
{
	return mFontFilename;
}
//----------------------------------------------------------------------------
inline unsigned int Font::GetFontExtStyle () const
{
	return mFontExtStyle;
}
//----------------------------------------------------------------------------
inline Font::FontType Font::GetFontType ()
{
	return mFontType;
}
//----------------------------------------------------------------------------
inline Texture2D *Font::GetTexture ()
{
	return mTexture;
}
//----------------------------------------------------------------------------
inline bool Font::IsNeedUpdate ()
{
	return mIsNeedUpdate;
}
//----------------------------------------------------------------------------