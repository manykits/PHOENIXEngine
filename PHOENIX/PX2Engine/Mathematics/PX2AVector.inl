// PX2AVector.inl

//----------------------------------------------------------------------------
inline void AVector::SetX(float x)
{
	mTuple[0] = x;
}
//----------------------------------------------------------------------------
inline void AVector::SetY(float y)
{
	mTuple[1] = y;
}
//----------------------------------------------------------------------------
inline void AVector::SetZ(float z)
{
	mTuple[2] = z;
}
//----------------------------------------------------------------------------
inline void AVector::SetW(float w)
{
	mTuple[3] = w;
}
//----------------------------------------------------------------------------
inline void AVector::Set(float x, float y, float z, float w)
{
	mTuple[0] = x;
	mTuple[1] = y;
	mTuple[2] = z;
	mTuple[3] = w;
}
//----------------------------------------------------------------------------
inline AVector::operator const Float3& () const
{
    return *(const Float3*)mTuple;
}
//----------------------------------------------------------------------------
inline AVector::operator Float3& ()
{
    return *(Float3*)mTuple;
}
//----------------------------------------------------------------------------
inline AVector::operator const Vector3f& () const
{
    return *(const Vector3f*)mTuple;
}
//----------------------------------------------------------------------------
inline AVector::operator Vector3f& ()
{
    return *(Vector3f*)mTuple;
}
//----------------------------------------------------------------------------