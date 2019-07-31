// PX2HTTPServerSession.cpp

#include "PX2HTTPServerSession.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPServerSession::HTTPServerSession(const StreamSocket& socket,
	HTTPServerParams *params) :
HTTPSession(socket, params->GetKeepAlive()),
mFirstRequest(true),
mKeepAliveTimeout(params->GetKeepAliveTimeout()),
mMaxKeepAliveRequests(params->GetMaxKeepAliveRequests())
{
	SetTimeout(params->GetTimeout());

	this->GetSocket().SetReceiveTimeout(params->GetTimeout());
}
//----------------------------------------------------------------------------
HTTPServerSession::~HTTPServerSession()
{
}
//----------------------------------------------------------------------------
bool HTTPServerSession::HasMoreRequests()
{
	if (!GetSocket().GetImpl()->IsInitialized())
		return false;

	if (mFirstRequest)
	{
		mFirstRequest = false;
		--mMaxKeepAliveRequests;
		return GetSocket().Poll(GetTimeout(), Socket::SELECT_READ);
	}
	else if (mMaxKeepAliveRequests != 0 && GetKeepAlive())
	{
		if (mMaxKeepAliveRequests > 0)
			--mMaxKeepAliveRequests;
		return buffered() > 0 || GetSocket().Poll(
			mKeepAliveTimeout, Socket::SELECT_READ);
	}
	else return false;
}
//----------------------------------------------------------------------------
SocketAddress HTTPServerSession::GetClientAddress()
{
	return GetSocket().GetPeerAddress();
}
//----------------------------------------------------------------------------
SocketAddress HTTPServerSession::GerverAddress()
{
	return GetSocket().GetAddress();
}
//----------------------------------------------------------------------------
bool HTTPServerSession::CanKeepAlive() const
{
	return mMaxKeepAliveRequests != 0;
}
//----------------------------------------------------------------------------