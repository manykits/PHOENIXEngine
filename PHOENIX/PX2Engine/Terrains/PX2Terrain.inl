// PX2Terrain.inl

//----------------------------------------------------------------------------
inline Shine *Terrain::GetTerrainShine()
{
	return mTerrainShine;
}
//----------------------------------------------------------------------------
inline int Terrain::GetRowQuantity () const
{
	return mRowTo - mRowFrom;
}
//----------------------------------------------------------------------------
inline int Terrain::GetColQuantity () const
{
	return mColTo - mColFrom;
}
//----------------------------------------------------------------------------
inline int Terrain::GetNumVertexPage() const
{
	return mNumVertexPage;
}
//----------------------------------------------------------------------------
inline float Terrain::GetSpacing () const
{
	return mSpacing;
}
//----------------------------------------------------------------------------
inline float Terrain::GetJunglerFrequency ()
{
	return mJunglerFrequency;
}
//----------------------------------------------------------------------------
inline float Terrain::GetJunglerStrength ()
{
	return mJunglerStrength;
}
//----------------------------------------------------------------------------