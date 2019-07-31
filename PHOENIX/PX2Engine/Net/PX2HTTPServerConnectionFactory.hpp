// PX2HTTPServerConnectionFactory.hpp

#ifndef PX2HTTPSERVERCONNECTIONFACTORY_HPP
#define PX2HTPPSERVERCONNECTIONFACTORY_HPP

#include "PX2CorePre.hpp"
#include "PX2TCPServerConnectionFactory.hpp"
#include "PX2HTTPServerParams.hpp"
#include "PX2HTTPRequestHandlerFactory.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM HTTPServerConnectionFactory : public TCPServerConnectionFactory
	{
	public:
		HTTPServerConnectionFactory(HTTPServerParams *params, HTTPRequestHandlerFactory *factory);
		virtual ~HTTPServerConnectionFactory();

		TCPServerConnection* CreateConnection(const StreamSocket& socket);

	private:
		HTTPServerParamsPtr mParams;
		HTTPRequestHandlerFactoryPtr mFactory;
	};

}

#endif