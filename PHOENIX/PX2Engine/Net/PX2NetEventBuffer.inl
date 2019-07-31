// PX2EventBuffer.inl

//----------------------------------------------------------------------------
inline void BufferEvent::Init(int bufsize)
{
	mBufferSize = bufsize;
	mDataLength = 0;
}
//----------------------------------------------------------------------------
inline void BufferEvent::Reset()
{
	mDataLength = 0;
}
//----------------------------------------------------------------------------
inline int BufferEvent::GetMessageID()
{
	return *(unsigned char *)mBuffer;
}
//----------------------------------------------------------------------------