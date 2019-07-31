//PX2HTTPServerRequestImpl.hpp

#ifndef PX2HTTPSERVERREQUESTIMPL_HPP
#define PX2HTTPSERVERREQUESTIMPL_HPP

#include "PX2NetPre.hpp"
#include "PX2HTTPServerRequest.hpp"
#include "PX2HTTPServerResponseImpl.hpp"

namespace PX2
{
	
	class HTTPServerSession;
	class HTTPServerParams;
	class StreamSocket;


	class PX2_ENGINE_ITEM HTTPServerRequestImpl : public HTTPServerRequest
	{
	public:
		HTTPServerRequestImpl(HTTPServerResponseImpl& response, HTTPServerSession& session, HTTPServerParams* params);
		~HTTPServerRequestImpl();

		std::istream& stream();
		bool ExpectContinue() const;
		const SocketAddress& GetClientAddress() const;
		const SocketAddress& GerverAddress() const;
		const HTTPServerParams& GetServerParams() const;

		HTTPServerResponse& Response() const;

		StreamSocket& GetSocket();
		StreamSocket DetachSocket();
		HTTPServerSession &GetSession();

	protected:
		static const std::string EXPECT;

	private:
		HTTPServerResponseImpl& mResponse;
		HTTPServerSession& mSession;
		std::istream* mStream;
		HTTPServerParams* mParams;
		SocketAddress mClientAddress;
		SocketAddress mServerAddress;
	};

}

#endif