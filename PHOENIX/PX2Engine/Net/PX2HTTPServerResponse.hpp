// PX2HTTPServerResponse.hpp

#ifndef PX2HTTPSERVERRESPONSE_HPP
#define PX2HTTPSERVERRESPONSE_HPP

#include "PX2NetPre.hpp"
#include "PX2HTTPResponse.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM HTTPServerResponse : public HTTPResponse
	{
	public:
		HTTPServerResponse();
		~HTTPServerResponse();

		virtual void SendContinue() = 0;

		virtual std::ostream& Send() = 0;
		virtual void SendFile(const std::string& path, const std::string& mediaType) = 0;
		virtual void SendBuffer(const void* pBuffer, std::size_t length) = 0;
		virtual void Redirect(const std::string& uri, HTTPStatus status = HTTP_FOUND) = 0;
		virtual void RequireAuthentication(const std::string& realm) = 0;
		virtual bool Sent() const = 0;
	};


}

#endif