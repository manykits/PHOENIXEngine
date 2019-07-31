// PX2CountingStream.inl

//----------------------------------------------------------------------------
inline int CountingStreamBuf::chars() const
{
	return _chars;
}
//----------------------------------------------------------------------------
inline int CountingStreamBuf::lines() const
{
	return _lines;
}
//----------------------------------------------------------------------------
inline int CountingStreamBuf::pos() const
{
	return _pos;
}
//----------------------------------------------------------------------------
inline int CountingStreamBuf::GetCurrentLineNumber() const
{
	return _lines;
}
//----------------------------------------------------------------------------
inline int CountingIOS::chars() const
{
	return _buf.chars();
}
//----------------------------------------------------------------------------
inline int CountingIOS::lines() const
{
	return _buf.lines();
}
//----------------------------------------------------------------------------
inline int CountingIOS::pos() const
{
	return _buf.pos();
}
//----------------------------------------------------------------------------
inline int CountingIOS::GetCurrentLineNumber() const
{
	return _buf.GetCurrentLineNumber();
}
//----------------------------------------------------------------------------