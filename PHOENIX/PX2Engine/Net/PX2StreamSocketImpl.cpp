// PX2StreamSocketImpl.cpp

#include "PX2StreamSocketImpl.hpp"
#include "PX2Thread.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
StreamSocketImpl::StreamSocketImpl()
{
}
//----------------------------------------------------------------------------
StreamSocketImpl::StreamSocketImpl(IPAddress::Family family)
{
	if (family == IPAddress::IPv4)
	{
		Init(AF_INET);
	}
#if defined(PX2_HAVE_IPV6)
	else if (family == IPAddress::IPv6)
	{
		Init(AF_INET6);
	}
#endif
	else
	{
		assertion(false,
			"Invalid or unsupported address family passed to StreamSocketImpl");
	}
}
//----------------------------------------------------------------------------
StreamSocketImpl::StreamSocketImpl(px2_socket_t sockfd) :
SocketImpl(sockfd)
{
}
//----------------------------------------------------------------------------
StreamSocketImpl::~StreamSocketImpl()
{
}
//----------------------------------------------------------------------------
int StreamSocketImpl::SendBytes(const void* buffer, int length, int flags)
{
	const char* p = reinterpret_cast<const char*>(buffer);
	int remaining = length;
	int sent = 0;

	bool blocking = IsBlocking();

	while (remaining > 0)
	{
		int n = SocketImpl::SendBytes(p, remaining, flags);
		assertion(n>=0, "n must > 0.\n");
		p += n; 
		sent += n;
		remaining -= n;
		if (blocking && remaining > 0)
		{
			Thread::DoYield();
		}
		else
		{
			break;
		}
	}
	return sent;
}
//----------------------------------------------------------------------------
void StreamSocketImpl::Init (int af)
{
	InitSocket(af, SOCK_STREAM, IPPROTO_TCP);
}
//----------------------------------------------------------------------------