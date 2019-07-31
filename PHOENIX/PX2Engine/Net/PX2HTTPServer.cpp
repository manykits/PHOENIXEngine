// PX2HTTPServer.cpp

#include "PX2HTTPServer.hpp"
#include "PX2HTTPServerConnection.hpp"
#include "PX2HTTPServerConnectionFactory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPServer::HTTPServer(HTTPRequestHandlerFactory *factory, 
	int portNumber, HTTPServerParams *params) :
TCPServer(new0 HTTPServerConnectionFactory(params, factory), portNumber, params),
mFactory(factory)
{
}
//----------------------------------------------------------------------------
HTTPServer::HTTPServer(HTTPRequestHandlerFactory *factory, 
	const ServerSocket& socket, HTTPServerParams *params) :
TCPServer(new0 HTTPServerConnectionFactory(params, factory), socket, params),
mFactory(factory)
{
}
//----------------------------------------------------------------------------
HTTPServer::HTTPServer(HTTPRequestHandlerFactory *factory, 
	ThreadPool *threadPool, const ServerSocket& socket, 
	HTTPServerParams *params) :
TCPServer(new0 HTTPServerConnectionFactory(params, factory), threadPool, socket, params),
mFactory(factory)
{
}
//----------------------------------------------------------------------------
HTTPServer::~HTTPServer()
{
}
//----------------------------------------------------------------------------
void HTTPServer::StopAll(bool abortCurrent)
{
	Stop();
	//mFactory->ServerStopped(this, abortCurrent);
}
//----------------------------------------------------------------------------