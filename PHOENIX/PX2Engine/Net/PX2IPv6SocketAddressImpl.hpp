// PX2IPv6SocketAddressImpl.hpp

#ifndef PX2IPV6SOCKETADDRESSIMPL_HPP
#define PX2IPV6SOCKETADDRESSIMPL_HPP

#include "PX2SocketAddressImpl.hpp"

namespace PX2
{

	class IPv6SocketAddressImpl: public SocketAddressImpl
	{
	public:
		IPv6SocketAddressImpl (const struct sockaddr_in6* addr);
		IPv6SocketAddressImpl (const void* addr, int16_t port);
		IPv6SocketAddressImpl (const void* addr, int16_t port, int32_t scope);

		virtual IPAddress GetHost () const;
		virtual int16_t GetPort () const;
		virtual px2_socklen_t GetAddrLength () const;
		virtual const struct sockaddr* GetAddr () const;
		virtual int GetAF() const;

	private:
		struct sockaddr_in6 mAddr;
	};

}

#endif