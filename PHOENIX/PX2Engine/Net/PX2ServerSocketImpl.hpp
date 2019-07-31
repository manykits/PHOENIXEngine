// PX2ServerSocketImpl.hpp

#ifndef PX2SERVERSOCKETIMPL_HPP
#define PX2SERVERSOCKETIMPL_HPP

#include "PX2SocketImpl.hpp"

namespace PX2
{

	class ServerSocketImpl : public SocketImpl
	{
	public:
		ServerSocketImpl();

	protected:
		virtual ~ServerSocketImpl();
	};

}

#endif