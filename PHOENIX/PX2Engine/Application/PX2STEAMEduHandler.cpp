// PX2STEAMEduHandler.cpp

#include "PX2STEAMEdu.hpp"
#include "PX2DateTime.hpp"
#include "PX2Base64.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2FileIO.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Arduino.hpp"
#include "PX2STEAMEdu.hpp"
#include "PX2WebSocket.hpp"
#include "PX2NetException.hpp"
#include "PX2PlatformSDK.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Application.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
STEAMEduHandlerFactory::STEAMEduHandlerFactory()
{
}
//----------------------------------------------------------------------------
HTTPRequestHandler * STEAMEduHandlerFactory::CreateRequestHandler(
	const HTTPServerRequest &request)
{
	if (request.find("Upgrade") != request.end() 
		&& Icompare(request["Upgrade"], "websocket") == 0)
		return new0 WebSocketRequestHandler;
	else
	{
		return new0 Snap_RequestHandler();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
Snap_RequestHandler::Snap_RequestHandler()
{
}
//----------------------------------------------------------------------------
Snap_RequestHandler::~Snap_RequestHandler()
{
}
//----------------------------------------------------------------------------
void _StrToPin(const std::string &paramStr, int &pin, bool &isA)
{
	if ('a' == paramStr[0] || 'A' == paramStr[0])
	{
		isA = true;
		std::string subStr = paramStr.substr(1, paramStr.length() - 1);
		pin = StringHelp::StringToInt(subStr);
	}
	else
	{
		pin = StringHelp::StringToInt(paramStr);
	}
}
//----------------------------------------------------------------------------
void Snap_RequestHandler::HandleRequest(HTTPServerRequest& request,
	HTTPServerResponse& response)
{
	if (0==PX2_STEAMEDU.CurReq && 0==PX2_STEAMEDU.CurResp)
	{
		PX2_STEAMEDU.CurReq = &request;
		PX2_STEAMEDU.CurResp = &response;

		PX2_STEAMEDU.TheSemaphore->Wait();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
WebSocketRequestHandler::WebSocketRequestHandler()
{
}
//----------------------------------------------------------------------------
WebSocketRequestHandler::~WebSocketRequestHandler()
{
}
//----------------------------------------------------------------------------
void WebSocketRequestHandler::HandleRequest(HTTPServerRequest& request,
	HTTPServerResponse& response)
{
	WebSocket ws(request, response);

	char buffer[1024];
	int flags;
	int n;
	do
	{
		n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		if (n > 0)
		{
			ws.sendFrame(buffer, n, flags);
		}
	} while (n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) !=
		WebSocket::FRAME_OP_CLOSE);
}
//----------------------------------------------------------------------------