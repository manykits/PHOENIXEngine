// PX2HTTPServerConnectionFactory.cpp

#include "PX2HTTPServerConnectionFactory.hpp"
#include "PX2HTTPServerConnection.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPServerConnectionFactory::HTTPServerConnectionFactory(HTTPServerParams *params, 
	HTTPRequestHandlerFactory *factory) :
mParams(params),
mFactory(factory)
{
	//poco_check_ptr(factory);
}
//----------------------------------------------------------------------------
HTTPServerConnectionFactory::~HTTPServerConnectionFactory()
{
	mParams = 0;
}
//----------------------------------------------------------------------------
TCPServerConnection* HTTPServerConnectionFactory::CreateConnection(const StreamSocket& socket)
{
	return new0 HTTPServerConnection(socket, mParams, mFactory);
}
//----------------------------------------------------------------------------