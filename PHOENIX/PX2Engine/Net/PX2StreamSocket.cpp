// PX2StreamSocket.cpp

#include "PX2StreamSocket.hpp"
#include "PX2StreamSocketImpl.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
StreamSocket::StreamSocket() :
Socket(new0 StreamSocketImpl)
{
}
//----------------------------------------------------------------------------
StreamSocket::StreamSocket(const SocketAddress& address) :
Socket(new0 StreamSocketImpl(address.GetFamily()))
{
	ConnectB(address);
}
//----------------------------------------------------------------------------
StreamSocket::StreamSocket(IPAddress::Family family) : 
Socket(new0 StreamSocketImpl(family))
{
}
//----------------------------------------------------------------------------
StreamSocket::StreamSocket(const Socket& socket) : 
Socket(socket)
{
	if (!dynamic_cast<StreamSocketImpl*>(GetImpl()))
	{
		assertion(false, "Cannot assign incompatible socket.\n");
	}
}
//----------------------------------------------------------------------------
StreamSocket::StreamSocket(px2_socket_t socket) :
Socket(new0 StreamSocketImpl(socket))
{
}
//----------------------------------------------------------------------------
StreamSocket::StreamSocket(SocketImpl* impl) :
Socket(impl)
{
	if (!dynamic_cast<StreamSocketImpl*>(GetImpl()))
	{
		assertion(false, "Cannot assign incompatible socket.\n");
	}
}
//----------------------------------------------------------------------------
StreamSocket::~StreamSocket()
{
}
//----------------------------------------------------------------------------
StreamSocket& StreamSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<StreamSocketImpl*>(socket.GetImpl()))
		Socket::operator = (socket);
	else
	{
		assertion(false, "Cannot assign incompatible socket");
	}

	return *this;
}
//----------------------------------------------------------------------------
int StreamSocket::ConnectB(const SocketAddress& address)
{
	return GetImpl()->ConnectB(address);
}
//----------------------------------------------------------------------------
int StreamSocket::ConnectB(const SocketAddress& address,
	const Timespan& timeout)
{
	return GetImpl()->ConnectB(address, timeout);
}
//----------------------------------------------------------------------------
int StreamSocket::ConnectNB(const SocketAddress& address)
{
	return GetImpl()->ConnectNB(address);
}
//----------------------------------------------------------------------------
void StreamSocket::ShutdownReceive()
{
	GetImpl()->ShutdownReceive();
}
//----------------------------------------------------------------------------
void StreamSocket::ShutdownSend()
{
	GetImpl()->ShutdownSend();
}
//----------------------------------------------------------------------------
void StreamSocket::Shutdown()
{
	GetImpl()->Shutdown();
}
//----------------------------------------------------------------------------
int StreamSocket::SendBytes(const void* buffer, int length, int flags)
{
	return GetImpl()->SendBytes(buffer, length, flags);
}
//----------------------------------------------------------------------------
int StreamSocket::ReceiveBytes(void* buffer, int length, int flags)
{
	return GetImpl()->ReceiveBytes(buffer, length, flags);
}
//----------------------------------------------------------------------------
void StreamSocket::SendUrgent(unsigned char data)
{
	GetImpl()->SendUrgent(data);
}
//----------------------------------------------------------------------------