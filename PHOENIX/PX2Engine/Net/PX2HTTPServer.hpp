// PX2HTTPServer.hpp

#ifndef PX2HTTPSERVER_HPP
#define PX2HTTPSERVER_HPP

#include "PX2TCPServer.hpp"
#include "PX2HTTPRequestHandlerFactory.hpp"
#include "PX2HTTPServerParams.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM HTTPServer : public TCPServer
	{
	public:
		HTTPServer(HTTPRequestHandlerFactory *factory, int portNumber = 80, 
			HTTPServerParams *params = new0 HTTPServerParams);
		HTTPServer(HTTPRequestHandlerFactory *factory,
			const ServerSocket& socket, HTTPServerParams *params);
		HTTPServer(HTTPRequestHandlerFactory *factory, ThreadPool *threadPool, 
			const ServerSocket& socket, HTTPServerParams *params);
		virtual ~HTTPServer();

		void StopAll(bool abortCurrent = false);

	private:
		HTTPRequestHandlerFactoryPtr mFactory;
	};
	typedef Pointer0<HTTPServer> HTTPServerPtr;

}

#endif