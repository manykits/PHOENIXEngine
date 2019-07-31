// PX2IPv6SocketAddressImpl.cpp

#include "PX2IPv6SocketAddressImpl.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
IPv6SocketAddressImpl::IPv6SocketAddressImpl(const struct sockaddr_in6* addr)
{
	std::memcpy(&mAddr, addr, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv6SocketAddressImpl::IPv6SocketAddressImpl(const void* addr, int16_t port)
{
	std::memset(&mAddr, 0, sizeof(mAddr));
	mAddr.sin6_family = AF_INET6;
	px2_set_sin6_len(&mAddr);
	std::memcpy(&mAddr.sin6_addr, addr, sizeof(mAddr.sin6_addr));
	mAddr.sin6_port = port;
}
//----------------------------------------------------------------------------
IPv6SocketAddressImpl::IPv6SocketAddressImpl (const void* addr, int16_t port, 
	int32_t scope)
{
	std::memset(&mAddr, 0, sizeof(mAddr));
	mAddr.sin6_family = AF_INET6;
	px2_set_sin6_len(&mAddr);
	std::memcpy(&mAddr.sin6_addr, addr, sizeof(mAddr.sin6_addr));
	mAddr.sin6_port = port;
	mAddr.sin6_scope_id = scope;
}
//----------------------------------------------------------------------------
IPAddress IPv6SocketAddressImpl::GetHost() const
{
	return IPAddress(&mAddr.sin6_addr, sizeof(mAddr.sin6_addr), mAddr.sin6_scope_id);
}
//----------------------------------------------------------------------------
int16_t IPv6SocketAddressImpl::GetPort() const
{
	return mAddr.sin6_port;
}
//----------------------------------------------------------------------------
px2_socklen_t IPv6SocketAddressImpl::GetAddrLength() const
{
	return sizeof(mAddr);
}
//----------------------------------------------------------------------------
const struct sockaddr* IPv6SocketAddressImpl::GetAddr() const
{
	return reinterpret_cast<const struct sockaddr*>(&mAddr);
}
//----------------------------------------------------------------------------
int IPv6SocketAddressImpl::GetAF () const
{
	return mAddr.sin6_family;
}
//----------------------------------------------------------------------------