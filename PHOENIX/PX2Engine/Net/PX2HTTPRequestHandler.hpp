// PX2HTTPRequestHandler.hpp

#ifndef PX2HTTPREQUESTHANDLER_HPP
#define PX2HTTPREQUESTHANDLER_HPP

#include "PX2NetPre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class HTTPServerRequest;
	class HTTPServerResponse;
	
	class PX2_ENGINE_ITEM HTTPRequestHandler
	{
	public:
		HTTPRequestHandler();
		virtual ~HTTPRequestHandler();

		virtual void HandleRequest(HTTPServerRequest& request, HTTPServerResponse& response) = 0;

	private:
		HTTPRequestHandler(const HTTPRequestHandler&);
		HTTPRequestHandler& operator = (const HTTPRequestHandler&);
	};
	typedef Pointer0<HTTPRequestHandler> HTTPRequestHandlerPtr;

}

#endif