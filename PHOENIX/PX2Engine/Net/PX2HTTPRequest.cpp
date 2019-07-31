// PX2HTTPRequest.cpp

#include "PX2HTTPRequest.hpp"
#include "PX2Ascii.hpp"
#include "PX2NumberFormatter.hpp"
#include "PX2NetException.hpp"
using namespace PX2;

const std::string HTTPRequest::HTTP_GET = "GET";
const std::string HTTPRequest::HTTP_HEAD = "HEAD";
const std::string HTTPRequest::HTTP_PUT = "PUT";
const std::string HTTPRequest::HTTP_POST = "POST";
const std::string HTTPRequest::HTTP_OPTIONS = "OPTIONS";
const std::string HTTPRequest::HTTP_DELETE = "DELETE";
const std::string HTTPRequest::HTTP_TRACE = "TRACE";
const std::string HTTPRequest::HTTP_CONNECT = "CONNECT";
const std::string HTTPRequest::HTTP_PATCH = "PATCH";
const std::string HTTPRequest::HOST = "Host";
const std::string HTTPRequest::COOKIE = "Cookie";
const std::string HTTPRequest::AUTHORIZATION = "Authorization";
const std::string HTTPRequest::PROXY_AUTHORIZATION = "Proxy-Authorization";
const std::string HTTPRequest::UPGRADE = "Upgrade";

//----------------------------------------------------------------------------
HTTPRequest::HTTPRequest() :
_method(HTTP_GET),
_uri("/")
{
}
//----------------------------------------------------------------------------
HTTPRequest::HTTPRequest(const std::string& version) :
HTTPMessage(version),
_method(HTTP_GET),
_uri("/")
{
}
//----------------------------------------------------------------------------
HTTPRequest::HTTPRequest(const std::string& method, const std::string& uri) :
_method(method),
_uri(uri)
{
}
//----------------------------------------------------------------------------
HTTPRequest::HTTPRequest(const std::string& method, const std::string& uri,
	const std::string& version) :
HTTPMessage(version),
_method(method),
_uri(uri)
{
}
//----------------------------------------------------------------------------
HTTPRequest::~HTTPRequest()
{
}
//----------------------------------------------------------------------------
void HTTPRequest::SetMethod(const std::string& method)
{
	_method = method;
}
//----------------------------------------------------------------------------
void HTTPRequest::SetURI(const std::string& uri)
{
	_uri = uri;
}
//----------------------------------------------------------------------------
void HTTPRequest::SetHost(const std::string& host)
{
	set(HOST, host);
}
//----------------------------------------------------------------------------
void HTTPRequest::SetHost(const std::string& host, int port)
{
	std::string value;
	if (host.find(':') != std::string::npos)
	{
		// IPv6 address
		value.append("[");
		value.append(host);
		value.append("]");
	}
	else
	{
		value.append(host);
	}

	if (port != 80 && port != 443)
	{
		value.append(":");
		NumberFormatter::append(value, port);
	}
	SetHost(value);
}
//----------------------------------------------------------------------------
const std::string& HTTPRequest::GetHost() const
{
	return get(HOST);
}
//----------------------------------------------------------------------------
void HTTPRequest::SetCookies(const NameValueCollection& cookies)
{
	std::string cookie;
	cookie.reserve(64);
	for (NameValueCollection::ConstIterator it = cookies.begin(); 
		it != cookies.end(); ++it)
	{
		if (it != cookies.begin())
			cookie.append("; ");
		cookie.append(it->first);
		cookie.append("=");
		cookie.append(it->second);
	}
	add(COOKIE, cookie);
}
//----------------------------------------------------------------------------
void HTTPRequest::GetCookies(NameValueCollection& cookies) const
{
	NameValueCollection::ConstIterator it = find(COOKIE);
	while (it != end() && Icompare(it->first, COOKIE) == 0)
	{
		splitParameters(it->second.begin(), it->second.end(), cookies);
		++it;
	}
}
//----------------------------------------------------------------------------
bool HTTPRequest::HasCredentials() const
{
	return has(AUTHORIZATION);
}
//----------------------------------------------------------------------------
void HTTPRequest::GetCredentials(std::string& scheme,
	std::string& authInfo) const
{
	GetCredentials(AUTHORIZATION, scheme, authInfo);
}
//----------------------------------------------------------------------------
void HTTPRequest::SetCredentials(const std::string& scheme, 
	const std::string& authInfo)
{
	SetCredentials(AUTHORIZATION, scheme, authInfo);
}
//----------------------------------------------------------------------------
bool HTTPRequest::HasProxyCredentials() const
{
	return has(PROXY_AUTHORIZATION);
}
//----------------------------------------------------------------------------
void HTTPRequest::GetProxyCredentials(std::string& scheme,
	std::string& authInfo) const
{
	GetCredentials(PROXY_AUTHORIZATION, scheme, authInfo);
}
//----------------------------------------------------------------------------
void HTTPRequest::SetProxyCredentials(const std::string& scheme, 
	const std::string& authInfo)
{
	SetCredentials(PROXY_AUTHORIZATION, scheme, authInfo);
}
//----------------------------------------------------------------------------
void HTTPRequest::write(std::ostream& ostr) const
{
	ostr << _method << " " << _uri << " " << GetVersion() << "\r\n";
	HTTPMessage::write(ostr);
	ostr << "\r\n";
}
//----------------------------------------------------------------------------
void HTTPRequest::read(std::istream& istr)
{
	static const int eof = std::char_traits<char>::eof();

	std::string method;
	std::string uri;
	std::string version;
	method.reserve(16);
	uri.reserve(64);
	version.reserve(16);
	int ch = istr.get();
	if (istr.bad()) throw NetException("Error reading HTTP request header");
	if (ch == eof) throw NoMessageException();
	while (PX2::Ascii::IsSpace(ch)) ch = istr.get();
	if (ch == eof) throw MessageException("No HTTP request header");
	while (!PX2::Ascii::IsSpace(ch) && ch != eof && method.length() < MAX_METHOD_LENGTH) { method += (char)ch; ch = istr.get(); }
	if (!PX2::Ascii::IsSpace(ch)) throw MessageException("HTTP request method invalid or too long");
	while (PX2::Ascii::IsSpace(ch)) ch = istr.get();
	while (!PX2::Ascii::IsSpace(ch) && ch != eof && uri.length() < MAX_URI_LENGTH) { uri += (char)ch; ch = istr.get(); }
	if (!PX2::Ascii::IsSpace(ch)) throw MessageException("HTTP request URI invalid or too long");
	while (PX2::Ascii::IsSpace(ch)) ch = istr.get();
	while (!PX2::Ascii::IsSpace(ch) && ch != eof && version.length() < MAX_VERSION_LENGTH) { version += (char)ch; ch = istr.get(); }
	if (!PX2::Ascii::IsSpace(ch)) throw MessageException("Invalid HTTP version string");
	while (ch != '\n' && ch != eof) { ch = istr.get(); }

	HTTPMessage::read(istr);
	ch = istr.get();
	while (ch != '\n' && ch != eof) { ch = istr.get(); }
	SetMethod(method);
	SetURI(uri);
	SetVersion(version);
}
//----------------------------------------------------------------------------
void HTTPRequest::GetCredentials(const std::string& header, 
	std::string& scheme, std::string& authInfo) const
{
	scheme.clear();
	authInfo.clear();
	if (has(header))
	{
		const std::string& auth = get(header);
		std::string::const_iterator it = auth.begin();
		std::string::const_iterator end = auth.end();
		while (it != end && PX2::Ascii::IsSpace(*it)) ++it;
		while (it != end && !PX2::Ascii::IsSpace(*it)) scheme += *it++;
		while (it != end && PX2::Ascii::IsSpace(*it)) ++it;
		while (it != end) authInfo += *it++;
	}
	else throw NotAuthenticatedException();
}
//----------------------------------------------------------------------------
void HTTPRequest::SetCredentials(const std::string& header, 
	const std::string& scheme, const std::string& authInfo)
{
	std::string auth(scheme);
	auth.append(" ");
	auth.append(authInfo);
	set(header, auth);
}
//----------------------------------------------------------------------------
const std::string& HTTPRequest::GetMethod() const
{
	return _method;
}
//----------------------------------------------------------------------------
const std::string& HTTPRequest::GetURI() const
{
	return _uri;
}
//----------------------------------------------------------------------------
