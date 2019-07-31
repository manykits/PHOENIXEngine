// PX2SocketImpl.inl

//----------------------------------------------------------------------------
inline px2_socket_t SocketImpl::GetSocket() const
{
	return mSocket;
}
//----------------------------------------------------------------------------
inline bool SocketImpl::IsInitialized() const
{
	return mSocket != PX2_INVALID_SOCKET;
}
//----------------------------------------------------------------------------
inline bool SocketImpl::IsBlocking() const
{
	return mIsBlocking;
}
//----------------------------------------------------------------------------