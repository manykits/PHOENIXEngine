// PX2FloatArray.inl

//----------------------------------------------------------------------------
inline int FloatArray::GetNumElements () const
{
	return mNumElements;
}
//----------------------------------------------------------------------------
inline float* FloatArray::GetData () const
{
	return mElements;
}
//----------------------------------------------------------------------------
inline FloatArray::operator const float* () const
{
	return mElements;
}
//----------------------------------------------------------------------------
inline FloatArray::operator float* ()
{
	return mElements;
}
//----------------------------------------------------------------------------
inline const float& FloatArray::operator[] (int i) const
{
	return mElements[i];
}
//----------------------------------------------------------------------------
inline float& FloatArray::operator[] (int i)
{
	return mElements[i];
}
//----------------------------------------------------------------------------