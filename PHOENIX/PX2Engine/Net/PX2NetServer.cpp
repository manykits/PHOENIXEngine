// PX2NetServer.cpp

#include "PX2NetServer.hpp"
#include "PX2Log.hpp"
#include "PX2NetServerIocp.hpp"
#include "PX2NetServerPoll.hpp"
#include "PX2Time.hpp"
#include "PX2System.hpp"
#include "PX2DNS.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Server::Server()
{
}
//----------------------------------------------------------------------------
Server::Server(ServerType serverType, int port, int numMaxConnects, 
	int numMaxMsgHandlers):
	mServerType(serverType),
	mPort(port),
	mNumMaxConnects(numMaxConnects),
	mNumMaxMsgHandlers(numMaxMsgHandlers)
{
	int max_events[] = {
		100000, 100000, 100000, 100000, 100000, 100000,
		100000, 100000, 100000, 100000, 100000, 100000,
		100000, 100000, 100000, 100000, 100000, 100000,
		100000, 100000 };

	mBufferEventQue = new BufferEventQueue(64, 1024 * 64, max_events);

	mMsgHandlers.resize(numMaxMsgHandlers, 0);

	RegisterHandler(BufferEvent::MSGID_RESERVED, &Server::OnReservedMsg);

	if (ST_IOCP == serverType)
	{
#if defined (WIN32) || defined(_WIN32)
		mServerImp = new0 ServerIocp(mPort, mNumMaxConnects, mNumMaxMsgHandlers,
			mBufferEventQue);
#endif
	}
	else if (ST_POLL == serverType)
	{
		mServerImp = new0 ServerPoll(mPort, mNumMaxConnects, mNumMaxMsgHandlers,
			mBufferEventQue);
	}
	else
	{
		assertion(false, "not support now");
	}
}
//-----------------------------------------------------------------------------
Server::~Server()
{
	if (mBufferEventQue)
	{
		delete(mBufferEventQue);
		mBufferEventQue = 0;
	}
}
//-----------------------------------------------------------------------------
Server *Server::New(ServerType serverType, int port, int numMaxConnects,
	int numMaxMsgHandlers)
{
	return new0 Server(serverType, port, numMaxConnects, numMaxMsgHandlers);
}
//-----------------------------------------------------------------------------
int Server::GetPort() const
{
	return mPort;
}
//-----------------------------------------------------------------------------
Server::ServerType Server::GetServerType() const
{
	return mServerType;
}
//-----------------------------------------------------------------------------
ServerSocket Server::GetListenSocket()
{
	return mServerImp->GetListenSocket();
}
//-----------------------------------------------------------------------------
std::string Server::GetAddress()
{
	return mServerImp->GetListenSocket().GetAddress().ToString();
}
//-----------------------------------------------------------------------------
const std::vector<int> &Server::GetThreadIDs() const
{
	return mServerImp->GetThreadIDs();
}
//-----------------------------------------------------------------------------
void Server::LogStatus()
{
	int events_alloc[20];

	//int n1 = mServerImp->GetNumUsedBuffer();
	int n1 = 0;
	int n2 = mServerImp->GetClientMapSize();
	int n3 = mBufferEventQue->GetNumAllocEvent();
	int n4 = mBufferEventQue->GetEventQueLen();
	int n5 = mBufferEventQue->GetNumEventInPool(events_alloc);

	printf("sys info: %d, %d, %d, %d, %d, %d, %d, %d\n", n1, n2, n3, n4, n5,
		events_alloc[0], events_alloc[1], events_alloc[2]);
}
//-----------------------------------------------------------------------------
bool Server::Start()
{
	if (!mServerImp->Start())
	{
		PX2_LOG_ERROR("server start failed");
		return false;
	}

	PX2_LOG_INFO("server start succeeded");
	
	return true;
}
//-----------------------------------------------------------------------------
void Server::Shutdown()
{
	mServerImp->Shutdown();
}
//-----------------------------------------------------------------------------
void Server::Run(float elapsedTime)
{
	PX2_UNUSED(elapsedTime);

	if (mServerImp)
		mServerImp->OnRun();

	HandleClientEvents();
}
//-----------------------------------------------------------------------------
bool Server::IsStarted() const
{
	return mServerImp->IsStarted();
}
//-----------------------------------------------------------------------------
ClientContext *Server::GetClientContext(int clientID)
{
	return mServerImp->_GetClientContext(clientID);
}
//-----------------------------------------------------------------------------
int Server::GetNumConnects()
{
	return mServerImp->GetNumUsedContext();
}
//-----------------------------------------------------------------------------
void Server::DisconnectClient(unsigned int clientid)
{
	mServerImp->DisconnectClient(clientid);
}
//----------------------------------------------------------------------------
int Server::SendMsgToClientBuffer(int clientID, int msgid,
	const char *buf, int size)
{
	char buffer[MAX_OVERLAPBUF_ALLSZIE];
	WriteMessageLen(buffer, (MSGID_BYTES + size));
	WriteMessageID(buffer + MSGLEN_BYTES, msgid);
	memcpy(buffer + MSGLEN_BYTES + MSGID_BYTES, buf, size);
	int allbytes = size + MSGLEN_BYTES + MSGID_BYTES;

	if (!mServerImp->PostWrite(clientID, buffer, allbytes))
	{
		printf("SendMsgToClient, PostWrite false\n");
	}
	return 0;
}
//----------------------------------------------------------------------------
int Server::SendMsgToClientRawBuffer(int clientID, const char *buf, int size)
{
	char buffer[MAX_OVERLAPBUF_ALLSZIE];
	memcpy(buffer, buf, size);
	int allbytes = size;

	if (!mServerImp->PostWrite(clientID, buffer, allbytes))
	{
		printf("SendMsgToClient, PostWrite false\n");
	}
	return 0;
}
//----------------------------------------------------------------------------
int Server::HandleClientEvent(BufferEvent *ent)
{
	int msgid = ReadMessageID(ent->mBuffer);

	if (msgid < 0 || msgid >= int(mMsgHandlers.size()))
		return CER_MSGID_OUTRANGE;

	MsgHandleFunc pfunc = mMsgHandlers[msgid];
	if (!pfunc)
		return CER_HANDLER_NULL;

	return (this->*pfunc)(ent->ClientID,
		((const char *)ent->mBuffer) + MSGID_BYTES,
		ent->mDataLength - MSGID_BYTES);
}
//----------------------------------------------------------------------------
void Server::HandleClientEvents()
{
	BufferEvent *ent = 0;

	while (0 != (ent = mBufferEventQue->PopBufferEvent()))
	{
		int result = HandleClientEvent(ent);

		if (CER_SUCCEED != result)
		{
			PX2_LOG_ERROR("HandleClientEvent error, clientid=%d, result=%d£¬msgid=%d",
				ent->ClientID, result, ReadMessageID(ent->mBuffer));
			
			mServerImp->DisconnectClient(ent->ClientID); //!!!ÊÇ·ñÓ¦¸Ã¶Ïµô
		}

		mBufferEventQue->FreeBufferEvent(ent);
	}
}
//----------------------------------------------------------------------------
void Server::RegisterHandler(int msgid, MsgHandleFunc msgfunc)
{
	assertion(msgid >= 0 && msgid<(int)mMsgHandlers.size(), 
		"msgid must be in range");

	mMsgHandlers[msgid] = msgfunc;
}
//----------------------------------------------------------------------------
std::string Server::GetIP(const std::string &address)
{
	HostEntry host = DNS::GetHostByName(address);
	std::vector<IPAddress> addresses = host.GetAddresses();

	IPAddress adr;
	if (addresses.size() > 0)
	{
		adr = addresses[0];
	}
	std::string ipStr = adr.ToString();
	return ipStr;
}
//----------------------------------------------------------------------------
int Server::OnConnect(unsigned int clientid) 
{ 
	PX2_UNUSED(clientid);
	return 0;
}
//----------------------------------------------------------------------------
int Server::OnDisconnect(unsigned int clientid) 
{ 
	PX2_UNUSED(clientid);
	return 0;
}
//----------------------------------------------------------------------------
int Server::OnReservedMsg(unsigned int clientid, const void *pbuffer, int buflen)
{
	PX2_UNUSED(buflen);

	char flags = *(const char *)pbuffer;

	if (flags == 0) //connect
	{
		PX2_LOG_INFO("clientid=%d connect", clientid);

		OnConnect(clientid);
	}
	else if (flags == 1) //disconnect
	{
		PX2_LOG_INFO("clientid=%d disconnect", clientid);

		OnDisconnect(clientid);
	}

	return 0;
}
//----------------------------------------------------------------------------