// PX2Buffer.inl

//----------------------------------------------------------------------------
inline int Buffer::GetNumElements () const
{
	return mNumElements;
}
//----------------------------------------------------------------------------
inline int Buffer::GetNumMaxElements() const
{
	return mNumMaxElements;
}
//----------------------------------------------------------------------------
inline int Buffer::GetElementSize () const
{
	return mElementSize;
}
//----------------------------------------------------------------------------
inline Buffer::Usage Buffer::GetUsage () const
{
	return mUsage;
}
//----------------------------------------------------------------------------
inline void Buffer::SetNumElements (int numElements)
{
	mNumElements = numElements;
}
//----------------------------------------------------------------------------
inline int Buffer::GetNumBytes () const
{
	return mNumBytes;
}
//----------------------------------------------------------------------------
inline const char* Buffer::GetData () const
{
	return mData;
}
//----------------------------------------------------------------------------
