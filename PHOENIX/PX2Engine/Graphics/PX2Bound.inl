// PX2Bound.inl

//----------------------------------------------------------------------------
inline void Bound::SetCenter (const APoint& center)
{
	mCenter = center;
}
//----------------------------------------------------------------------------
inline void Bound::SetRadius (float radius)
{
	mRadius = radius;
}
//----------------------------------------------------------------------------
inline const APoint& Bound::GetCenter () const
{
	return mCenter;
}
//----------------------------------------------------------------------------
inline float Bound::GetRadius () const
{
	return mRadius;
}
//----------------------------------------------------------------------------
inline int Bound::GetStreamingSize ()
{
	return (int)(sizeof(APoint) + sizeof(float));
}
//----------------------------------------------------------------------------