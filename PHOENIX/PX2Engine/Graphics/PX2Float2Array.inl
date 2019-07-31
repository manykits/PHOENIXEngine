// PX2FloatArray.inl

//----------------------------------------------------------------------------
inline int Float2Array::GetNumElements () const
{
	return mNumElements;
}
//----------------------------------------------------------------------------
inline Float2* Float2Array::GetData () const
{
	return mElements;
}
//----------------------------------------------------------------------------
inline Float2Array::operator const Float2* () const
{
	return mElements;
}
//----------------------------------------------------------------------------
inline Float2Array::operator Float2* ()
{
	return mElements;
}
//----------------------------------------------------------------------------
inline const Float2& Float2Array::operator[] (int i) const
{
	return mElements[i];
}
//----------------------------------------------------------------------------
inline Float2& Float2Array::operator[] (int i)
{
	return mElements[i];
}
//----------------------------------------------------------------------------
