// PX2NetServerPoll.cpp

#include "PX2NetServerPoll.hpp"
#include "PX2Log.hpp"
#include "PX2System.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
const int max_recvbuf = 64 * 1024;
const int max_sendbuf = 64 * 1024;
//----------------------------------------------------------------------------
ServerPoll::ServerPoll(int port, int numMaxConnects, int numMaxMsgHandlers,
	BufferEventQueue *peventque) :
	ServerImp(port, numMaxConnects, numMaxMsgHandlers, peventque),
	mThreadListen(0)
{
}
//----------------------------------------------------------------------------
ServerPoll::~ServerPoll()
{
}
//----------------------------------------------------------------------------
bool ServerPoll::Start()
{
	mIsShutdown = false;

	if (0 != mListenSocket.Bind(SocketAddress((uint16_t)mListenPort)))
		return false;

	mListenSocket.Listen();

	mThreadListen = new0 Thread("ServerPollThread");
	mThreadListen->Start(*this);

	mIsStarted = true;

	return true;
}
//----------------------------------------------------------------------------
void ServerPoll::Run()
{
	_AcceptRun();
}
//----------------------------------------------------------------------------
void ServerPoll::OnRun()
{
	_ReadWriteRun();
}
//----------------------------------------------------------------------------
void ServerPoll::_AcceptRun()
{
	while (!mIsShutdown)
	{
		Timespan timeout(250000);
		if (mListenSocket.Poll(timeout, Socket::SELECT_READ))
		{
			StreamSocket ss = mListenSocket.AcceptConnection();

			_AddClientSocket(ss);
		}
	}
}
//----------------------------------------------------------------------------
void ServerPoll::_ReadWriteRun()
{
	Socket::SocketList readable;
	Socket::SocketList writable;
	Socket::SocketList except;
	std::map<px2_socket_t, ClientContext*> contextMap;

	int nSockets = 0;
	{
		ScopedCS cs(&mContextMapMutex);

		std::map<unsigned int, ClientContext *>::iterator it = mClientMap.begin();
		for (; it != mClientMap.end(); it++)
		{
			contextMap[it->second->TheSocket.GetSocket()] = it->second;

			readable.push_back(it->second->TheSocket);
			writable.push_back(it->second->TheSocket);
			except.push_back(it->second->TheSocket);

			nSockets++;
		}
	}

	Timespan timeout(250000);
	if (nSockets == 0)
	{
		// odle
		/*_*/
	}
	else if (Socket::Select(readable, writable, except, timeout))
	{
		// busy

		for (Socket::SocketList::iterator it = readable.begin(); it != readable.end(); ++it)
		{
			ClientContext *context = contextMap[(*it).GetSocket()];
			if (context)
			{
				if (context->TheSocket.IsValid())
				{
					_OnRead(context);
				}
			}
		}

		for (Socket::SocketList::iterator it = writable.begin(); it != writable.end(); ++it)
		{
			ClientContext *context = contextMap[(*it).GetSocket()];
			if (context)
			{
				if (context->TheSocket.IsValid())
				{
					_OnWrite(context);
				}
			}
		}

		for (Socket::SocketList::iterator it = except.begin(); it != except.end(); ++it)
		{
		}
	}
	else
	{
		// time out
		/*_*/
	}
}
//----------------------------------------------------------------------------
int ServerPoll::_OnRead(ClientContext *pcontext)
{
	_EnterPendingIO(pcontext);

	StreamSocket streamSocket = pcontext->TheSocket;

	int nbytes = streamSocket.ReceiveBytes(pcontext->RecvBuf + pcontext->RecvLen, max_recvbuf - pcontext->RecvLen, 0);

	if (nbytes < 0)
	{
		_CloseClientSocket(pcontext);
		_LeavePendingIO(pcontext);

		return -1;
	}

	_LeavePendingIO(pcontext);

	pcontext->RecvLen += nbytes;
	while (pcontext->RecvLen >= MSGLEN_BYTES)
	{
		int msglen = *(unsigned short *)pcontext->RecvBuf;
		int pkglen = msglen + MSGLEN_BYTES;
		if (pkglen > max_recvbuf)
		{
			return -2;
		}

		if (pcontext->RecvLen >= pkglen)
		{
			BufferEvent *pevent = mBufferEventQue->AllocBufferEvent(msglen);
			pevent->ClientID = pcontext->ClientID;
			pevent->PushData(pcontext->RecvBuf + MSGLEN_BYTES, msglen);

			mBufferEventQue->PostBufferEvent(pevent);

			memmove(pcontext->RecvBuf, pcontext->RecvBuf + pkglen, pcontext->RecvLen - pkglen);
			pcontext->RecvLen -= pkglen;
		}
		else
		{
			break;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int ServerPoll::_OnWrite(ClientContext *pcontext)
{
	StreamSocket streamSocket = pcontext->TheSocket;

	if (0 == pcontext->SendLen)
	{
		BufferEvent *pevent = pcontext->SendQue->PopBufferEvent();

		if (!pevent)
			return 0;

		int msgid = pevent->GetMessageID();
		if (msgid == BufferEvent::MSGID_RESERVED)
		{
			pcontext->SendQue->FreeBufferEvent(pevent);
			return -1;
		}

		assert(pevent->mDataLength + MSGLEN_BYTES <= max_sendbuf);

		// length
		*(unsigned short *)pcontext->SendBuf = (unsigned short)pevent->mDataLength;
		memcpy(pcontext->SendBuf + MSGLEN_BYTES, pevent->mBuffer, pevent->mDataLength);

		// all send length
		pcontext->SendLen = MSGLEN_BYTES + pevent->mDataLength;

		pcontext->SendQue->FreeBufferEvent(pevent);
	}

	_EnterPendingIO(pcontext);

	int nbytes = streamSocket.SendBytes(pcontext->SendBuf, pcontext->SendLen, 0);
	if (nbytes < 0)
	{
		_CloseClientSocket(pcontext);
		_LeavePendingIO(pcontext);

		return -1;
	}

	if (nbytes > 0)
	{
		memmove(pcontext->SendBuf, pcontext->SendBuf + nbytes, pcontext->SendLen - nbytes);
		pcontext->SendLen -= nbytes;
		assert(pcontext->SendLen >= 0);
	}

	_LeavePendingIO(pcontext);

	return 0;
}
//----------------------------------------------------------------------------
void ServerPoll::Shutdown()
{
	mIsShutdown = true;
	mIsStarted = false;

	System::SleepSeconds(0.1f);

	if (mThreadListen)
	{
		mThreadListen->Join();
		delete0(mThreadListen);
	}

	mListenSocket.Close();
}
//----------------------------------------------------------------------------
void ServerPoll::DisconnectClient(unsigned int clientID)
{
	ScopedCS cs(&mContextMapMutex);

	auto iter = mClientMap.find(clientID);
	if (iter != mClientMap.end())
	{
		ClientContext *pcontext = iter->second;
		mClientMap.erase(iter);

		_EnterPendingIO(pcontext);
	}
}
//----------------------------------------------------------------------------
void ServerPoll::DisconnectAll()
{
	ScopedCS cs(&mContextMapMutex);

	auto iter = mClientMap.begin();
	while (iter != mClientMap.end())
	{
		ClientContext *pcontext = iter->second;
		iter = mClientMap.erase(iter);

		_EnterPendingIO(pcontext);
	}
}
//----------------------------------------------------------------------------
bool ServerPoll::_AddClientSocket(Socket &s)
{
	assertion(s.IsValid(), "socket must valid");

	ClientContext* pcontext = _AllocContext(s);
	if (pcontext == 0)
	{
		s.Close();
		return false;
	}

	pcontext->InitOnPool();

	if (!_PostRead(pcontext, 0))
	{
		PX2_LOG_ERROR("post read error");
		goto error_handle;
	}

	mBufferEventQue->PostConnectEvent(pcontext->ClientID);

	return true;

error_handle:

	DisconnectClient(pcontext->ClientID);

	return false;
}
//----------------------------------------------------------------------------
void ServerPoll::_CloseClientSocket(ClientContext *pcontext)
{
	ScopedCS cs(&mContextLock);

	if (pcontext->TheSocket.IsValid())
	{
		pcontext->TheSocket.Close();
	}
}
//----------------------------------------------------------------------------
bool ServerPoll::_PostRead(ClientContext *pcontext, ServerBuffer *pbuf)
{
	PX2_UNUSED(pcontext);
	PX2_UNUSED(pbuf);

	return true;
}
//----------------------------------------------------------------------------
bool ServerPoll::_PostWrite(ClientContext *pcontext, char *psrc, int srclen)
{
	// we do not put length int BufferEvent
	BufferEvent *pevent = pcontext->SendQue->AllocBufferEvent(srclen - MSGLEN_BYTES);
	if (pevent == 0) return 0;

	pevent->ClientID = pcontext->ClientID;
	pevent->PushData(psrc + MSGLEN_BYTES, srclen - MSGLEN_BYTES);
	
	pcontext->SendQue->PostBufferEvent(pevent);

	return true;
}
//----------------------------------------------------------------------------