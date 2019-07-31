// PX2NetClientConnect.inl

//----------------------------------------------------------------------------
inline StreamSocket &ClientConnector::GetSocket ()
{
	return mSocket;
}
//----------------------------------------------------------------------------
inline int ClientConnector::GetConnectState()
{
	return mConnectState; 
}
//----------------------------------------------------------------------------
inline void ClientConnector::SetConnectState(int state)
{
	mConnectState = state;
	mConnStateTime = 0.0f;
}
//----------------------------------------------------------------------------
template<class T>
int ClientConnector::SendMsgToServer(int msgid, const T &msg)
{
	BufferEvent *pevent = MsgToBufferEvent(mSendQue, msgid, msg);
	if (!pevent) return -1;

	mSendQue->PostBufferEvent(pevent);

	return 0;
}
//----------------------------------------------------------------------------