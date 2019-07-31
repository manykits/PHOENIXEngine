// PX2IPv4SocketAddressImpl.cpp

#include "PX2IPv4SocketAddressImpl.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
IPv4SocketAddressImpl::IPv4SocketAddressImpl()
{
	std::memset(&mAddr, 0, sizeof(mAddr));
	mAddr.sin_family = AF_INET;
	px2_set_sin_len(&mAddr);
}
//----------------------------------------------------------------------------
IPv4SocketAddressImpl::IPv4SocketAddressImpl(const struct sockaddr_in* addr)
{
	std::memcpy(&mAddr, addr, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv4SocketAddressImpl::IPv4SocketAddressImpl(const void* addr, int16_t port)
{
	std::memset(&mAddr, 0, sizeof(mAddr));
	mAddr.sin_family = AF_INET;
	std::memcpy(&mAddr.sin_addr, addr, sizeof(mAddr.sin_addr));
	mAddr.sin_port = port;
}
//----------------------------------------------------------------------------
IPAddress IPv4SocketAddressImpl::GetHost() const
{
	return IPAddress(&mAddr.sin_addr, sizeof(mAddr.sin_addr));
}
//----------------------------------------------------------------------------
int16_t IPv4SocketAddressImpl::GetPort() const
{
	return mAddr.sin_port;
}
//----------------------------------------------------------------------------
px2_socklen_t IPv4SocketAddressImpl::GetAddrLength() const
{
	return sizeof(mAddr);
}
//----------------------------------------------------------------------------
const struct sockaddr* IPv4SocketAddressImpl::GetAddr() const
{
	return reinterpret_cast<const struct sockaddr*>(&mAddr);
}
//----------------------------------------------------------------------------
int IPv4SocketAddressImpl::GetAF() const
{
	return mAddr.sin_family;
}
//----------------------------------------------------------------------------