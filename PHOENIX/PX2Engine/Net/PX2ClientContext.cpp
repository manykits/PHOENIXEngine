// PX2ClientContext.cpp

#include "PX2ClientContext.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Log.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//-----------------------------------------------------------------------------
// ClientContext
//-----------------------------------------------------------------------------
ClientContext::ClientContext()
{
	ClientID = 0;
	NumPendingIO = 0;
	TheBufferEvent = 0;
	PackageTotalLength = 0;

	RecvBuf = 0;
	RecvLen = 0;

	SendQue = 0;
	SendBuf = 0;
	SendLen = 0;
}
//-----------------------------------------------------------------------------
ClientContext::~ClientContext()
{
	if (SendQue)
	{
		delete SendQue;
		SendQue = 0;
	}

	if (RecvBuf)
	{
		delete[] RecvBuf;
		RecvBuf = 0;
	}

	if (SendBuf)
	{
		delete[] SendBuf;
		SendBuf = 0;
	}
}
//-----------------------------------------------------------------------------
void ClientContext::Init(Socket &s, unsigned int clientID)
{
	TheSocket = s;
	ClientID = clientID;
	NumPendingIO = 0;
	TheBufferEvent = 0;
	PackageTotalLength = 0;

	RecvBuf = 0;
	RecvLen = 0;

	SendQue = 0;
	SendBuf = 0;
	SendLen = 0;
}
//-----------------------------------------------------------------------------
const int maxRecvbuf = 64 * 1024;
const int maxSendbuf = 64 * 1024;
//-----------------------------------------------------------------------------
void ClientContext::InitOnPool()
{
	RecvBuf = new char[maxSendbuf];
	RecvLen = 0;

	int max_events[] = {
		1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
		1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
		1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024 };

	SendQue = new BufferEventQueue(16, 64 * 1024, max_events);
	SendBuf = new char[maxSendbuf];
	SendLen = 0;
}
//-----------------------------------------------------------------------------