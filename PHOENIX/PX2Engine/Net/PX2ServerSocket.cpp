// PX2ServerSocket.cpp

#include "PX2ServerSocket.hpp"
#include "PX2ServerSocketImpl.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ServerSocket::ServerSocket() :
Socket(new0 ServerSocketImpl)
{
}
//----------------------------------------------------------------------------
ServerSocket::ServerSocket(const Socket& socket) :
Socket(socket)
{
	if (!dynamic_cast<ServerSocketImpl*>(GetImpl()))
	{
		assertion(false, "Cannot assign incompatible socket.\n");
	}
}
//----------------------------------------------------------------------------
ServerSocket::ServerSocket(const SocketAddress& address, int backlog) :
Socket(new0 ServerSocketImpl)
{
	GetImpl()->Bind(address, true);
	GetImpl()->Listen(backlog);
}
//----------------------------------------------------------------------------
ServerSocket::ServerSocket(int16_t port, int backlog) :
Socket(new0 ServerSocketImpl)
{
	IPAddress wildcardAddr;
	SocketAddress address(wildcardAddr, port);
	GetImpl()->Bind(address, true);
	GetImpl()->Listen(backlog);
}
//----------------------------------------------------------------------------
ServerSocket::ServerSocket(SocketImpl* pImpl, bool ignore) :
Socket(pImpl)
{
	PX2_UNUSED(ignore);
}
//----------------------------------------------------------------------------
ServerSocket::~ServerSocket()
{
}
//----------------------------------------------------------------------------
ServerSocket& ServerSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<ServerSocketImpl*>(socket.GetImpl()))
		Socket::operator = (socket);
	else
	{
		assertion(false, "Cannot assign incompatible socket.\n");
	}
	return *this;
}
//----------------------------------------------------------------------------
int ServerSocket::Bind(const SocketAddress& address, bool reuseAddress)
{
	return GetImpl()->Bind(address, reuseAddress);
}
//----------------------------------------------------------------------------
int ServerSocket::Bind(int16_t port, bool reuseAddress)
{
	IPAddress wildcardAddr;
	SocketAddress address(wildcardAddr, port);
	return GetImpl()->Bind(address, reuseAddress);
}
//----------------------------------------------------------------------------
int ServerSocket::Bind6(const SocketAddress& address, bool reuseAddress, bool ipV6Only)
{
	return GetImpl()->Bind6(address, reuseAddress, ipV6Only);
}
//----------------------------------------------------------------------------
int ServerSocket::Bind6(int16_t port, bool reuseAddress, bool ipV6Only)
{
	IPAddress wildcardAddr;
	SocketAddress address(wildcardAddr, port);
	return GetImpl()->Bind6(address, reuseAddress, ipV6Only);
}
//----------------------------------------------------------------------------
void ServerSocket::Listen(int backlog)
{
	GetImpl()->Listen(backlog);
}
//----------------------------------------------------------------------------
StreamSocket ServerSocket::AcceptConnection(SocketAddress& clientAddr)
{
	return StreamSocket(GetImpl()->AcceptConnection(clientAddr));
}
//----------------------------------------------------------------------------
StreamSocket ServerSocket::AcceptConnection()
{
	SocketAddress clientAddr;
	return StreamSocket(GetImpl()->AcceptConnection(clientAddr));
}
//----------------------------------------------------------------------------