// PX2TCPServerConnectionFactory.hpp

#ifndef PX2TCPSERVERCONNECTIONFACTORY_HPP
#define PX2TCPSERVERCONNECTIONFACTORY_HPP

#include "PX2NetPre.hpp"
#include "PX2TCPServerConnection.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TCPServerConnectionFactory
	{
	public:
		TCPServerConnectionFactory();
		virtual ~TCPServerConnectionFactory();
		virtual TCPServerConnection* CreateConnection(const StreamSocket& socket) = 0;

	private:
		TCPServerConnectionFactory(const TCPServerConnectionFactory&);
		TCPServerConnectionFactory& operator = (const TCPServerConnectionFactory&);
	};
	typedef Pointer0<TCPServerConnectionFactory> TCPServerConnectionFactoryPtr;

	template <class S>
	class TCPServerConnectionFactoryImpl : public TCPServerConnectionFactory
	{
	public:
		TCPServerConnectionFactoryImpl()
		{
		}

		~TCPServerConnectionFactoryImpl()
		{
		}

		TCPServerConnection* CreateConnection(const StreamSocket& socket)
		{
			return new0 S(socket);
		}
	};


}

#endif