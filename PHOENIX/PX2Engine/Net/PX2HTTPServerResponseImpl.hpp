// PX2HTTPServerResponseImpl.hpp

#ifndef PX2HTTPSERVERRESPONSEIMPL_HPP
#define PX2HTTPSERVERRESPONSEIMPL_HPP

#include "PX2NetPre.hpp"
#include "PX2HTTPServerResponse.hpp"
#include "PX2HTTPServerParams.hpp"

namespace PX2
{

	class HTTPServerSession;
	class HTTPServerRequestImpl;


	class PX2_ENGINE_ITEM HTTPServerResponseImpl : public HTTPServerResponse
	{
	public:
		HTTPServerResponseImpl(HTTPServerSession& session);
		~HTTPServerResponseImpl();

		void SendContinue();
		std::ostream& Send();
		void SendFile(const std::string& path, const std::string& mediaType);
		void SendBuffer(const void* pBuffer, std::size_t length);
		void Redirect(const std::string& uri, HTTPStatus status = HTTP_FOUND);
		void RequireAuthentication(const std::string& realm);
		bool Sent() const;

	protected:
		void AttachRequest(HTTPServerRequestImpl* pRequest);

	private:
		HTTPServerSession& mSession;
		HTTPServerRequestImpl* mRequest;
		std::ostream* mStream;

		friend class HTTPServerRequestImpl;
	};

}


#endif