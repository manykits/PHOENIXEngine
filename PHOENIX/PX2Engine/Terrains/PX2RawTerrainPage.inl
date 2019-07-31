// PX2RawTerrainPage.inl

//----------------------------------------------------------------------------
inline Texture2D *RawTerrainPage::GetTextureAlpha ()
{
	return mTextureAlpha;
}
//----------------------------------------------------------------------------
inline Float2 RawTerrainPage::GetUV0 ()
{
	return Float2(mUV01[0], mUV01[1]);
}
//----------------------------------------------------------------------------
inline Float2 RawTerrainPage::GetUV1 ()
{
	return Float2(mUV01[2], mUV01[3]);
}
//----------------------------------------------------------------------------
inline Float2 RawTerrainPage::GetUV2 ()
{
	return Float2(mUV23[0], mUV23[1]);
}
//----------------------------------------------------------------------------
inline Float2 RawTerrainPage::GetUV3 ()
{
	return Float2(mUV23[2], mUV23[3]);
}
//----------------------------------------------------------------------------
inline Float2 RawTerrainPage::GetUV4 ()
{
	return Float2(mUV4[0], mUV4[1]);
}
//----------------------------------------------------------------------------