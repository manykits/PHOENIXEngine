// PX2HTTPRequestHandlerFactory.hpp

#ifndef PX2HTTPREQUESTHANDLERFACTORY_HPP
#define PX2HTTPREQUESTHANDLERFACTORY_HPP

#include "PX2NetPre.hpp"
#include "PX2HTTPServerRequest.hpp"
#include "PX2HTTPRequestHandler.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM HTTPRequestHandlerFactory
	{
	public:
		HTTPRequestHandlerFactory();
		virtual ~HTTPRequestHandlerFactory();

		virtual HTTPRequestHandler* CreateRequestHandler(const HTTPServerRequest& request) = 0;

	private:
		HTTPRequestHandlerFactory(const HTTPRequestHandlerFactory&);
		HTTPRequestHandlerFactory& operator = (const HTTPRequestHandlerFactory&);

		friend class HTTPServer;
		friend class HTTPServerConnection;
	};

	typedef Pointer0<HTTPRequestHandlerFactory> HTTPRequestHandlerFactoryPtr;

}

#endif