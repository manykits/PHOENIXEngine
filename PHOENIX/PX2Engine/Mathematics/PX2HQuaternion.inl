// PX2HQuaternion.inl

//----------------------------------------------------------------------------
inline HQuaternion::operator const float* () const
{
	return mTuple;
}
//----------------------------------------------------------------------------
inline HQuaternion::operator float* ()
{
	return mTuple;
}
//----------------------------------------------------------------------------
inline float HQuaternion::operator[] (int i) const
{
	return mTuple[i];
}
//----------------------------------------------------------------------------
inline float& HQuaternion::operator[] (int i)
{
	return mTuple[i];
}
//----------------------------------------------------------------------------
inline float HQuaternion::W () const
{
	return mTuple[0];
}
//----------------------------------------------------------------------------
inline float& HQuaternion::W ()
{
	return mTuple[0];
}
//----------------------------------------------------------------------------
inline float HQuaternion::X () const
{
	return mTuple[1];
}
//----------------------------------------------------------------------------
inline float& HQuaternion::X ()
{
	return mTuple[1];
}
//----------------------------------------------------------------------------
inline float HQuaternion::Y () const
{
	return mTuple[2];
}
//----------------------------------------------------------------------------
inline float& HQuaternion::Y ()
{
	return mTuple[2];
}
//----------------------------------------------------------------------------
inline float HQuaternion::Z () const
{
	return mTuple[3];
}
//----------------------------------------------------------------------------
inline float& HQuaternion::Z ()
{
	return mTuple[3];
}
//----------------------------------------------------------------------------
