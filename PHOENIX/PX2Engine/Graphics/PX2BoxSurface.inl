// PX2BoxSurface.inl

//----------------------------------------------------------------------------
inline const BSplineVolumef* BoxSurface::GetVolume () const
{
	return mVolume;
}
//----------------------------------------------------------------------------
inline int BoxSurface::GetNumUSamples () const
{
	return mNumUSamples;
}
//----------------------------------------------------------------------------
inline int BoxSurface::GetNumVSamples () const
{
	return mNumVSamples;
}
//----------------------------------------------------------------------------
inline int BoxSurface::GetNumWSamples () const
{
	return mNumWSamples;
}
//----------------------------------------------------------------------------
