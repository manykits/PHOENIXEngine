// PX2GeneralAlloctor.inl

//----------------------------------------------------------------------------
inline size_t GeneralAlloctor::FreeListIndex(size_t numBytes) 
{
	return (numBytes + (size_t)Align - 1)/(size_t)Align - 1;
}
//----------------------------------------------------------------------------
inline size_t GeneralAlloctor::RoundUp (size_t numBytes)
{
	return ((numBytes + (size_t)Align - 1) & ~((size_t)Align - 1));
}
//----------------------------------------------------------------------------