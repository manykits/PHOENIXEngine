// PX2ClientConnect.cpp

#include "PX2NetClientConnector.hpp"
#include "PX2Log.hpp"
#include "PX2SocketImpl.hpp"
#include "PX2NetError.hpp"
#include "PX2Time.hpp"
#include "PX2System.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
const int max_recvbuf = 64*1024;
const int max_sendbuf = 64*1024;
//----------------------------------------------------------------------------
int ClientConnector::_ClientOnRead()
{
	int nbytes = mSocket.ReceiveBytes(mRecvBuf+mRecvLen, max_recvbuf-mRecvLen, 0);

	if(nbytes < 0)
	{
		return -1;
	}

	std::string recvStr = std::string(mRecvBuf + mRecvLen, nbytes);
	_OnRecvCallbacks(recvStr);

	mRecvLen += nbytes;
	while(mRecvLen >= MSGLEN_BYTES)
	{
		int msglen = *(unsigned short *)mRecvBuf;
		int pkglen = msglen + MSGLEN_BYTES;
		if(pkglen > max_recvbuf)
		{
			return -2;
		}

		if(mRecvLen >= pkglen)
		{
			BufferEvent *pevent = mRecvQue->AllocBufferEvent(msglen);
			if (pevent)
			{
				pevent->PushData(mRecvBuf + MSGLEN_BYTES, msglen);
				mRecvQue->PostBufferEvent(pevent);

				memmove(mRecvBuf, mRecvBuf + pkglen, mRecvLen - pkglen);
				mRecvLen -= pkglen;
			}
		}
		else
		{
			break;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int ClientConnector::_ClientOnWrite()
{
	if(mSendLen == 0)
	{
		BufferEvent *pevent = mSendQue->PopBufferEvent();

		if(!pevent)
			return 0;

		int msgid = pevent->GetMessageID();
		if(msgid == BufferEvent::MSGID_RESERVED)
		{
			mSendQue->FreeBufferEvent(pevent);
			return -1;
		}

		assert(pevent->mDataLength + MSGLEN_BYTES <= max_sendbuf);

		// length
		*(unsigned short *)mSendBuf = (unsigned short)pevent->mDataLength;

		// data
		memcpy(mSendBuf + MSGLEN_BYTES, pevent->mBuffer, pevent->mDataLength);
		
		// all send length
		mSendLen = MSGLEN_BYTES + pevent->mDataLength;

		mSendQue->FreeBufferEvent(pevent);
	}

	int nbytes = mSocket.SendBytes(mSendBuf, mSendLen, 0);
	if(nbytes < 0)
	{
		return -1;
	}

	if(nbytes > 0)
	{
		memmove(mSendBuf, mSendBuf+nbytes, mSendLen-nbytes);
		mSendLen -= nbytes;
		assert(mSendLen >= 0);
	}

	return 0;
}
//----------------------------------------------------------------------------
ClientConnector::ClientConnector(int num_msghandlers) : 
mConnectState(CONNSTATE_INIT),
mConnStateTime(0)
{
	int max_events[] = {
		1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
		1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 
		1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024 };

	mSendQue = new BufferEventQueue(16, 64*1024, max_events);
	mRecvQue = new BufferEventQueue(16, 64*1024, max_events);

	mRecvBuf = new char[max_recvbuf];
	mSendBuf = new char[max_sendbuf];
	mRecvLen = mSendLen = 0;

	ServerMsgDesc tmpdesc = {NULL, false, -1};
	mMsgHandlers.resize(num_msghandlers, tmpdesc);
	RegisterHandler(0, &ClientConnector::_OnReservedMsg);
}
//----------------------------------------------------------------------------
ClientConnector::~ClientConnector()
{
	delete mSendQue;
	delete mRecvQue;
	delete[] mRecvBuf;
	delete[] mSendBuf;
}
//----------------------------------------------------------------------------
int ClientConnector::_InternalConnectB()
{
	mRecvLen = 0;
	mSendLen = 0;

	int result = mSocket.ConnectB(mConnectToAddr);

	if (0 != result)
	{
		return -1;
	}
	else
	{
		SetConnectState(CONNSTATE_CONNECTED);

		return 0;
	}
}
//----------------------------------------------------------------------------
int ClientConnector::_InternalConnectNB()
{
	mRecvLen = 0;
	mSendLen = 0;

	int result = mSocket.ConnectNB(mConnectToAddr);

	if (0 != result)
	{
		int err = NetError::LastError();
		if (err == PX2_EISCONN)
		{
			SetConnectState(CONNSTATE_CONNECTED);

			return 0;
		}
		else if (err == PX2_EWOULDBLOCK)
		{
			SetConnectState(CONNSTATE_TRYCONNECT);

			return 0;
		}

		return -1;
	}
	else
	{
		SetConnectState(CONNSTATE_CONNECTED);

		return 0;
	}
}
//----------------------------------------------------------------------------
int ClientConnector::ConnectB(const std::string &ip, int port)
{
	if (!IsEnable())
		return -1;

	SocketAddress sa(ip, (int16_t)port);
	return ConnectB(sa);
}
//----------------------------------------------------------------------------
int ClientConnector::ConnectB(const SocketAddress &addr)
{
	if (!IsEnable())
		return -1;

	mConnectToAddr = addr;

	return _InternalConnectB();
}
//----------------------------------------------------------------------------
int ClientConnector::ConnectNB(const std::string &ip, int port)
{
	if (!IsEnable())
		return -1;

	int16_t port16 = (int16_t)port;
	SocketAddress sa(ip, port16);
	return ConnectNB(sa);
}
//----------------------------------------------------------------------------
int ClientConnector::ConnectNB(const SocketAddress &addr)
{
	if (!IsEnable())
		return -1;

	mConnectToAddr = addr;
	
	return _InternalConnectNB();
}
//----------------------------------------------------------------------------
void ClientConnector::_InternalDisconnect()
{
	mRecvLen = 0;
	mSendLen = 0;

	mSocket.Shutdown();
	mSocket.Close();
}
//----------------------------------------------------------------------------
void ClientConnector::Disconnect()
{
	if (!IsEnable())
		return;

	//if (CONNSTATE_CONNECTED == GetConnectState())
	{
		_InternalDisconnect();
		SetConnectState(CONNSTATE_INIT);
	}
}
//----------------------------------------------------------------------------
int ClientConnector::Reconnect(BufferEvent *ent)
{
	if (!IsEnable())
		return -1;
	
	assert(mConnectState == CONNSTATE_CONN_ERROR);

	_InternalDisconnect();
	_InternalConnectNB();

	if(mConnectState == CONNSTATE_CONN_ERROR)
	{
		return -1;
	}

	if (ent)
	{
		mSendQue->InsertEventFront(ent);
	}

	return 0;
}
//----------------------------------------------------------------------------
bool ClientConnector::IsHasRecvCallback(ClientConnectorRecvCallback cb)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		ClientConnectorRecvCallback callback = mCallbacks[i];
		if (callback == cb)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool ClientConnector::AddRecvCallback(ClientConnectorRecvCallback callback)
{
	if (IsHasRecvCallback(callback))
		return false;

	mCallbacks.push_back(callback);

	return true;
}
//----------------------------------------------------------------------------
void ClientConnector::_OnRecvCallbacks(const std::string &recvStr)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		ClientConnectorRecvCallback callback = mCallbacks[i];
		if (callback)
		{
			callback(this, recvStr);
		}
	}
}
//----------------------------------------------------------------------------
void ClientConnector::RegisterHandler(int msgid, ServerMsgHandleFunc msgfunc,
	bool need_answer, int other_answer_msg)
{
	assert(msgid>=0 && msgid<int(mMsgHandlers.size()));

	mMsgHandlers[msgid].handler = msgfunc;
	mMsgHandlers[msgid].need_answer = need_answer;
	mMsgHandlers[msgid].other_answermsg = other_answer_msg;
}
//----------------------------------------------------------------------------
int ClientConnector::_OnReservedMsg(const void *pbuffer, int buflen)
{
	PX2_UNUSED(buflen);

	char flags = *(const char *)pbuffer;
	if(flags == 0) SetConnectState(CONNSTATE_CONNECTED);
	else if(flags == 1) SetConnectState(CONNSTATE_INIT);
	else
	{
		assertion(false, "");
	}

	return 0;
}
//----------------------------------------------------------------------------
int ClientConnector::SendMsgToServerBuffer(int msgid, const char *buf,
	int size)
{
	BufferEvent *pevent = MsgToBufferEventBuffer(mSendQue, msgid, buf, size);
	if (!pevent) return -1;

	mSendQue->PostBufferEvent(pevent);

	return 0;
}
//----------------------------------------------------------------------------
int ClientConnector::SendRawBuffer(const char *buf, int size)
{
	return GetSocket().SendBytes(buf, size);
}
//----------------------------------------------------------------------------
int ClientConnector::Update(float elapsedSeconds)
{
	mConnStateTime += elapsedSeconds;
	if (CONNSTATE_INIT == mConnectState || CONNSTATE_CONN_ERROR == mConnectState)
		return 0;

	Socket::SocketList listRead;
	Socket::SocketList listWrite;
	Socket::SocketList listExcept;
	listRead.push_back(mSocket);
	listWrite.push_back(mSocket);
	listExcept.push_back(mSocket);
	Timespan span(5000);

	int rc = mSocket.Select(listRead, listWrite, listExcept, span);
	if (rc == -1)
	{
		PX2_LOG_ERROR("mSocket.Select error.");
		return -1;
	}

	// 超时进入下一轮循环
	if (rc == 0)
		return 0;

	if(CONNSTATE_TRYCONNECT == mConnectState)
	{
		if(listWrite.size() > 0)
		{
			SetConnectState(CONNSTATE_CONNECTED);
		}
		if(listExcept.size() > 0)
		{
			SetConnectState(CONNSTATE_INIT);
			return 0;
		}
	}
	else
	{
		if(listRead.size() > 0)
		{
			if (_ClientOnRead() < 0)
			{
				SetConnectState(CONNSTATE_CONN_ERROR);
				return -1;
			}
		}

		if(listWrite.size() > 0)
		{
			if(_ClientOnWrite() < 0)
			{
				SetConnectState(CONNSTATE_CONN_ERROR);
				return -1;
			}
		}

		if(listExcept.size() > 0)
		{
			SetConnectState(CONNSTATE_CONN_ERROR);
			return -1;
		}
	}
	return _HandleServerMsg();
}
//----------------------------------------------------------------------------
int ClientConnector::_HandleServerBufferEvent(BufferEvent *pevent)
{
	int msgid = ReadMessageID(pevent->mBuffer);

	if(msgid<0 || msgid>=int(mMsgHandlers.size()))
	{
		return -2;
	}

	ServerMsgHandleFunc func = mMsgHandlers[msgid].handler;
	if(func == NULL)
	{
		return -3;
	}

	if((this->*func)(pevent->mBuffer+MSGID_BYTES, pevent->mDataLength-MSGID_BYTES) < 0)
	{
		return -4;
	}
	return 0;
}
//----------------------------------------------------------------------------
int ClientConnector::_HandleServerMsg()
{
	BufferEvent *pevent;
	while((pevent=mRecvQue->PopBufferEvent()) != NULL)
	{
		int ret = _HandleServerBufferEvent(pevent);
		mRecvQue->FreeBufferEvent(pevent);

		if(ret < 0) return ret;
	}
	return 0;
}
//----------------------------------------------------------------------------