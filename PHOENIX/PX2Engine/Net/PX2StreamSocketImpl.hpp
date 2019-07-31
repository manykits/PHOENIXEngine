// PX2StreamSocketImpl.hpp

#ifndef PX2STREAMSOCKETIMPL_HPP
#define PX2STREAMSOCKETIMPL_HPP

#include "PX2NetPre.hpp"
#include "PX2SocketImpl.hpp"

namespace PX2
{

	class StreamSocketImpl: public SocketImpl
	{
	public:
		StreamSocketImpl();
		explicit StreamSocketImpl(IPAddress::Family addressFamily);
		StreamSocketImpl(px2_socket_t sockfd);

		virtual int SendBytes(const void* buffer, int length, int flags = 0);

	protected:
		virtual ~StreamSocketImpl();

		virtual void Init (int af);
	};

}

#endif