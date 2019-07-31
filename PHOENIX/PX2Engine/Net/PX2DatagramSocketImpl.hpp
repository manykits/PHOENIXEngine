// PX2DatagramSocketImpl.hpp

#ifndef PX2DTAAGRAMSOCKETIMPL_HPP
#define PX2DATAGRAMSOCKETIMPL_HPP

#include "PX2NetPre.hpp"
#include "PX2SocketImpl.hpp"

namespace PX2
{

	class DatagramSocketImpl : public SocketImpl
	{
	public:
		DatagramSocketImpl();
		explicit DatagramSocketImpl(IPAddress::Family family);

		DatagramSocketImpl(px2_socket_t sockfd);

	protected:
		virtual void Init(int af);

		~DatagramSocketImpl();
	};

}

#endif