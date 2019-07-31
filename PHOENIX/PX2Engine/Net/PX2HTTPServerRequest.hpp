// PX2HTTPServerRequest.hpp

#ifndef PX2HTTPSERVERREQUEST_HPP
#define PX2HTTPSERVERREQUEST_HPP

#include "PX2HTTPRequest.hpp"
#include "PX2SocketAddress.hpp"
#include "PX2HTTPServerParams.hpp"

namespace PX2
{
	class HTTPServerResponse;

	class PX2_ENGINE_ITEM HTTPServerRequest : public HTTPRequest
	{
	public:
		HTTPServerRequest();
		~HTTPServerRequest();

		virtual std::istream& stream() = 0;
		virtual bool ExpectContinue() const = 0;

		virtual const SocketAddress& GetClientAddress() const = 0;

		virtual const SocketAddress& GerverAddress() const = 0;

		virtual const HTTPServerParams& GetServerParams() const = 0;

		virtual HTTPServerResponse& Response() const = 0;
	};


}

#endif 