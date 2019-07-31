// PX2Stream.inl

//----------------------------------------------------------------------------
inline Stream::StreamType Stream::GetStreamType () const
{
	return mStreamType;
}
//----------------------------------------------------------------------------
inline bool Stream::IsIn () const
{
	return ST_IN == mStreamType;
}
//----------------------------------------------------------------------------