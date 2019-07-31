// PX2BufferIO.inl

//----------------------------------------------------------------------------
inline BufferIO::operator bool () const
{
	return mMode != BM_NONE;
}
//----------------------------------------------------------------------------
inline const char* BufferIO::GetBuffer () const
{
	return mBuffer;
}
//----------------------------------------------------------------------------
inline int BufferIO::GetNumBytesTotal () const
{
	return mNumBytesTotal;
}
//----------------------------------------------------------------------------
inline int BufferIO::GetNumBytesProcessed () const
{
	return mNumBytesProcessed;
}
//----------------------------------------------------------------------------
inline int BufferIO::GetMode () const
{
	return mMode;
}
//----------------------------------------------------------------------------