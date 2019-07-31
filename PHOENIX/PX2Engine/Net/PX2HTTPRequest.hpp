// PX2HTTPRequest.hpp

#ifndef PX2HTTPREQUEST_HPP
#define PX2HTTPREQUEST_HPP

#include "PX2HTTPMessage.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM HTTPRequest : public HTTPMessage
	{
	public:
		HTTPRequest();
		/// Creates a GET / HTTP/1.0 HTTP request.

		// HTTP/1.0 or HTTP/1.1
		HTTPRequest(const std::string& version);
		HTTPRequest(const std::string& method, const std::string& uri);
		HTTPRequest(const std::string& method, const std::string& uri, const std::string& version);

		virtual ~HTTPRequest();

		void SetMethod(const std::string& method);
		const std::string& GetMethod() const;

		void SetURI(const std::string& uri);
		const std::string& GetURI() const;

		void SetHost(const std::string& host);
		void SetHost(const std::string& host, int port);
		const std::string& GetHost() const;
		///

		void SetCookies(const NameValueCollection& cookies);
		void GetCookies(NameValueCollection& cookies) const;

		bool HasCredentials() const;
		void GetCredentials(std::string& scheme, std::string& authInfo) const;
		void SetCredentials(const std::string& scheme, const std::string& authInfo);

		bool HasProxyCredentials() const;
		void GetProxyCredentials(std::string& scheme, std::string& authInfo) const;
		void SetProxyCredentials(const std::string& scheme, const std::string& authInfo);

		void write(std::ostream& ostr) const;
		void read(std::istream& istr);

		static const std::string HTTP_GET;
		static const std::string HTTP_HEAD;
		static const std::string HTTP_PUT;
		static const std::string HTTP_POST;
		static const std::string HTTP_OPTIONS;
		static const std::string HTTP_DELETE;
		static const std::string HTTP_TRACE;
		static const std::string HTTP_CONNECT;
		static const std::string HTTP_PATCH;

		static const std::string HOST;
		static const std::string COOKIE;
		static const std::string AUTHORIZATION;
		static const std::string PROXY_AUTHORIZATION;
		static const std::string UPGRADE;

	protected:
		void GetCredentials(const std::string& header, std::string& scheme, std::string& authInfo) const;
		void SetCredentials(const std::string& header, const std::string& scheme, const std::string& authInfo);

	private:
		enum Limits
		{
			MAX_METHOD_LENGTH = 32,
			MAX_URI_LENGTH = 16384,
			MAX_VERSION_LENGTH = 8
		};

		std::string _method;
		std::string _uri;

		HTTPRequest(const HTTPRequest&);
		HTTPRequest& operator = (const HTTPRequest&);
	};

}

#endif