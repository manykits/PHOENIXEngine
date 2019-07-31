// PX2LP_HandlerFactory.hpp

#ifndef PX2LP_HANDLERFACTORY_HPP
#define PX2LP_HANDLERFACTORY_HPP

#include "PX2HTTPRequestHandlerFactory.hpp"
#include "PX2HTTPRequestHandler.hpp"
#include "PX2HTTPServerResponse.hpp"
#include "PX2HTTPServerRequest.hpp"

namespace PX2
{

	class HandlerFactory : public HTTPRequestHandlerFactory
	{
	public:
		HandlerFactory();

		HTTPRequestHandler * CreateRequestHandler(const HTTPServerRequest &request);
	};

	class LP_RequestHandler : public HTTPRequestHandler
	{
	public:
		LP_RequestHandler();
		virtual ~LP_RequestHandler();

		virtual void HandleRequest(HTTPServerRequest& request,
			HTTPServerResponse& response);
	};

}

#endif