// PX2SocketAddressImpl.hpp

#ifndef PX2SOCKETADDRESSIMPL_HPP
#define PX2SOCKETADDRESSIMPL_HPP

#include "PX2NetPre.hpp"
#include "PX2IPAddress.hpp"
#include "PX2NetDefine.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class SocketAddressImpl
	{
	public:
		SocketAddressImpl ();
		virtual ~SocketAddressImpl ();

		virtual IPAddress GetHost () const = 0;
		virtual int16_t GetPort () const = 0;
		virtual px2_socklen_t GetAddrLength () const = 0;
		virtual const struct sockaddr* GetAddr () const = 0;
		virtual int GetAF() const = 0;

	private:
		SocketAddressImpl (const SocketAddressImpl&);
		SocketAddressImpl& operator = (const SocketAddressImpl&);
	};

	typedef Pointer0<SocketAddressImpl> SocketAddressImplPtr;

}

#endif