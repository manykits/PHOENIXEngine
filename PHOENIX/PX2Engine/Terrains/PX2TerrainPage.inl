// PX2TerrainPage.inl

//----------------------------------------------------------------------------
inline int TerrainPage::GetJunglerNum ()
{
	return (int)mJunglers.size();
}
//----------------------------------------------------------------------------
inline Jungler *TerrainPage::GetJungler (int i)
{
	return mJunglers[i];
}
//----------------------------------------------------------------------------
inline int TerrainPage::GetSize () const
{
	return mNumVertexPage;
}
//----------------------------------------------------------------------------
inline const float* TerrainPage::GetHeights () const
{
	return mHeights;
}
//----------------------------------------------------------------------------
inline const Float2& TerrainPage::GetOrigin () const
{
	return mOrigin;
}
//----------------------------------------------------------------------------
inline float TerrainPage::GetSpacing () const
{
	return mSpacing;
}
//----------------------------------------------------------------------------
inline float TerrainPage::GetWidth () const
{
	return mSpacing * mNumVertexPageM1;
}
//----------------------------------------------------------------------------
inline float TerrainPage::GetX (int x) const
{
	return mOrigin[0] + mSpacing*(float)x;
}
//----------------------------------------------------------------------------
inline float TerrainPage::GetY (int y) const
{
	return mOrigin[1] + mSpacing*(float)y;
}
//----------------------------------------------------------------------------
inline float TerrainPage::GetHeight (int index) const
{
	return mHeights[index];
}
//----------------------------------------------------------------------------