// CSA_HandlerFactory.cpp

#include "PX2LP_HandlerFactory.hpp"
#include "PX2DateTime.hpp"
#include "PX2Base64.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2FileIO.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HandlerFactory::HandlerFactory()
{
}
//----------------------------------------------------------------------------
HTTPRequestHandler * HandlerFactory::CreateRequestHandler(
	const HTTPServerRequest &request)
{
	return new0 LP_RequestHandler();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
LP_RequestHandler::LP_RequestHandler()
{
}
//----------------------------------------------------------------------------
LP_RequestHandler::~LP_RequestHandler()
{
}
//----------------------------------------------------------------------------
void LP_RequestHandler::HandleRequest(HTTPServerRequest& request,
	HTTPServerResponse& response)
{
	std::string method = request.GetMethod();
	std::string url = request.GetURI();
	if (url.empty())
		return;

	if ('/' == url[0])
		url = url.substr(1, url.length() - 1);

	response.SendFile(url, "file");
}
//----------------------------------------------------------------------------