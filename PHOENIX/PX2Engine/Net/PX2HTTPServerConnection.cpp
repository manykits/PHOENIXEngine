// PX2HTTPServerConnection.cpp

#include "PX2HTTPServerConnection.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2NetException.hpp"
#include "PX2HTTPServerSession.hpp"
#include "PX2HTTPServerRequestImpl.hpp"
#include "PX2HTTPServerResponseImpl.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPServerConnection::HTTPServerConnection(const StreamSocket& socket,
	HTTPServerParams *params, HTTPRequestHandlerFactory *factory) :
TCPServerConnection(socket),
mParams(params),
mFactory(factory),
mIsStopped(false)
{
	assert(factory);

	//mFactory->serverStopped += PX2::delegate(this, &HTTPServerConnection::OnServerStopped);
}
//----------------------------------------------------------------------------
HTTPServerConnection::~HTTPServerConnection()
{
	try
	{
		//mFactory->serverStopped -= PX2::delegate(this, &HTTPServerConnection::OnServerStopped);
	}
	catch (...)
	{
		//poco_unexpected();
	}
}
//----------------------------------------------------------------------------
void HTTPServerConnection::Run()
{
	std::string server = mParams->GetSoftwareVersion();

	HTTPServerSession session(GetSocket(), mParams);

	while (!mIsStopped && session.HasMoreRequests())
	{
		HTTPServerResponseImpl response(session);
		HTTPServerRequestImpl request(response, session, mParams);

		Timestamp now;
		response.SetDate(now);
		response.SetVersion(request.GetVersion());
		response.SetKeepAlive(mParams->GetKeepAlive() && request.GetKeepAlive() && session.CanKeepAlive());
		if (!server.empty())
		{
			response.set("Server", server);
		}

		HTTPRequestHandlerPtr handler = mFactory->CreateRequestHandler(request);
		if (handler)
		{
			if (request.ExpectContinue())
				response.SendContinue();

			std::cout << "A" << std::endl;

			handler->HandleRequest(request, response);

			std::cout << "B" << std::endl;

			bool isKeepAlive = mParams->GetKeepAlive() &&
				response.GetKeepAlive() && session.CanKeepAlive();
			session.SetKeepAlive(isKeepAlive);
		}
		else
		{
			SendErrorResponse(session, HTTPResponse::HTTP_NOT_IMPLEMENTED);
		}

		std::cout << "Z" << std::endl;
	}
}
//----------------------------------------------------------------------------
void HTTPServerConnection::SendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status)
{
	HTTPServerResponseImpl response(session);
	response.SetVersion(HTTPMessage::HTTP_1_1);
	response.SetStatusAndReason(status);
	response.SetKeepAlive(false);
	response.Send();
	session.SetKeepAlive(false);
}
//----------------------------------------------------------------------------
void HTTPServerConnection::OnServerStopped(const bool& abortCurrent)
{
	mIsStopped = true;
	if (abortCurrent)
	{
		try
		{
			// Note: On Windows, select() will not return if one of its socket is being
			// shut down. Therefore we have to call close(), which works better.
			// On other platforms, we do the more graceful thing.
#if defined(_WIN32)
			GetSocket().Close();
#else
			GetSocket().Shutdown();
#endif
		}
		catch (...)
		{
		}
	}
	else
	{
		ScopedCS cs(&mMutex);

		try
		{
#if defined(_WIN32)
			GetSocket().Close();
#else
			GetSocket().Shutdown();
#endif
		}
		catch (...)
		{
		}
	}
}
//----------------------------------------------------------------------------