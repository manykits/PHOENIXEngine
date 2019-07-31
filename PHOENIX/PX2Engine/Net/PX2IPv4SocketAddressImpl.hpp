// PX2IPv4SocketAddressImpl.hpp

#ifndef PX2IP4SOCKETADDRESSIMPL_HPP
#define PX2IP4SOCKETADDRESSIMPL_HPP

#include "PX2SocketAddressImpl.hpp"

namespace PX2
{

	class IPv4SocketAddressImpl : public SocketAddressImpl
	{
	public:
		IPv4SocketAddressImpl ();
		IPv4SocketAddressImpl (const struct sockaddr_in* addr);
		IPv4SocketAddressImpl (const void* addr, int16_t port);

		virtual IPAddress GetHost () const;
		virtual int16_t GetPort () const;
		virtual px2_socklen_t GetAddrLength () const;
		virtual const struct sockaddr* GetAddr () const;
		virtual int GetAF () const;

	private:
		struct sockaddr_in mAddr;
	};

}

#endif