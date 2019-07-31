// PX2HPlane.inl

//----------------------------------------------------------------------------
inline HPlane::operator HPoint ()
{
	return mTuple;
}
//----------------------------------------------------------------------------
inline HPlane::operator HPoint () const
{
	return mTuple;
}
//----------------------------------------------------------------------------
inline HPlane::operator const float* () const
{
	return (const float*)mTuple;
}
//----------------------------------------------------------------------------
inline HPlane::operator float* ()
{
	return (float*)mTuple;
}
//----------------------------------------------------------------------------
inline float HPlane::operator[] (int i) const
{
	return mTuple[i];
}
//----------------------------------------------------------------------------
inline float& HPlane::operator[] (int i)
{
	return mTuple[i];
}
//----------------------------------------------------------------------------
inline void HPlane::SetNormal (const AVector& normal)
{
	mTuple[0] = normal[0];
	mTuple[1] = normal[1];
	mTuple[2] = normal[2];
}
//----------------------------------------------------------------------------
inline void HPlane::SetConstant (float constant)
{
	mTuple[3] = -constant;
}
//----------------------------------------------------------------------------
inline AVector HPlane::GetNormal () const
{
	return AVector(mTuple[0], mTuple[1], mTuple[2]);
}
//----------------------------------------------------------------------------
inline float HPlane::GetConstant () const
{
	return -mTuple[3];
}
//----------------------------------------------------------------------------