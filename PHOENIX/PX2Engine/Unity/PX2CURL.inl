// PX2CURL.inl

//----------------------------------------------------------------------------
inline CurlObj::GetType CurlObj::GetGetType() const
{
	return mGetType;
}
//----------------------------------------------------------------------------
inline char *CurlObj::GetGettedMemory()
{
	return mGettedMemory;
}
//----------------------------------------------------------------------------
inline const char *CurlObj::GetGettedMemory() const
{
	return mGettedMemory;
}
//----------------------------------------------------------------------------
inline int CurlObj::GetGettedMemorySize() const
{
	return mCurGettedSize;
}
//----------------------------------------------------------------------------
inline bool CurlObj::IsGettedOK() const
{
	return mIsGettedOK;
}
//----------------------------------------------------------------------------
inline float CurlObj::GetGettedProgress() const
{
	return mGettedProgress;
}
//----------------------------------------------------------------------------