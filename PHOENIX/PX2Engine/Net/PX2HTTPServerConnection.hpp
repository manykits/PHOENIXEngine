// PX2HTTPServerConnection.hpp

#ifndef PX2HTTPSERVERCONNECTION_HPP
#define PX2HTTPSERVERCONNECTION_HPP

#include "PX2NetPre.hpp"
#include "PX2TCPServerConnection.hpp"
#include "PX2HTTPResponse.hpp"
#include "PX2HTTPRequestHandlerFactory.hpp"
#include "PX2TCPServerParams.hpp"

namespace PX2
{

	class HTTPServerSession;

	class PX2_ENGINE_ITEM HTTPServerConnection : public TCPServerConnection
	{
	public:
		HTTPServerConnection(const StreamSocket& socket,
			HTTPServerParams *params, HTTPRequestHandlerFactory *factory);
		virtual ~HTTPServerConnection();

		virtual void Run();

	protected:
		void SendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status);
		void OnServerStopped(const bool& abortCurrent);

	private:
		HTTPServerParamsPtr          mParams;
		HTTPRequestHandlerFactoryPtr mFactory;
		bool mIsStopped;
		Mutex mMutex;
	};


}

#endif