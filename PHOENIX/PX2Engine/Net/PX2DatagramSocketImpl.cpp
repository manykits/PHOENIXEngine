// PX2DatagramSocketImpl.cpp

#include "PX2DatagramSocketImpl.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
DatagramSocketImpl::DatagramSocketImpl()
{
	Init(AF_INET);
}
//----------------------------------------------------------------------------
DatagramSocketImpl::DatagramSocketImpl(IPAddress::Family family)
{
	if (family == IPAddress::IPv4)
		Init(AF_INET);
#if defined(PX2_HAVE_IPV6)
	else if (family == IPAddress::IPv6)
		Init(AF_INET6);
#endif
}
//----------------------------------------------------------------------------
DatagramSocketImpl::DatagramSocketImpl(px2_socket_t sockfd) : 
SocketImpl(sockfd)
{
}
//----------------------------------------------------------------------------
DatagramSocketImpl::~DatagramSocketImpl()
{
}
//----------------------------------------------------------------------------
void DatagramSocketImpl::Init(int af)
{
	InitSocket(af, SOCK_DGRAM);
}
//----------------------------------------------------------------------------