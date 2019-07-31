// PX2APoint.inl

//----------------------------------------------------------------------------
inline void APoint::SetX(float x)
{
	mTuple[0] = x;
}
//----------------------------------------------------------------------------
inline void APoint::SetY(float y)
{
	mTuple[1] = y;
}
//----------------------------------------------------------------------------
inline void APoint::SetZ(float z)
{
	mTuple[2] = z;
}
//----------------------------------------------------------------------------
inline void APoint::SetW(float w)
{
	mTuple[3] = w;
}
//----------------------------------------------------------------------------
inline void APoint::Set(float x, float y, float z, float w)
{
	mTuple[0] = x;
	mTuple[1] = y;
	mTuple[2] = z;
	mTuple[3] = w;
}
//----------------------------------------------------------------------------
inline APoint::operator const Float3& () const
{
	return *(const Float3*)mTuple;
}
//----------------------------------------------------------------------------
inline APoint::operator Float3& ()
{
	return *(Float3*)mTuple;
}
//----------------------------------------------------------------------------
inline APoint::operator const Vector3f& () const
{
	return *(const Vector3f*)mTuple;
}
//----------------------------------------------------------------------------
inline APoint::operator Vector3f& ()
{
	return *(Vector3f*)mTuple;
}
//----------------------------------------------------------------------------