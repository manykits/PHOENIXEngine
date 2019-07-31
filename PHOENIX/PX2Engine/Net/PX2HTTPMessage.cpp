// PX2HTTPMessage.cpp

#include "PX2HTTPMessage.hpp"
#include "PX2NumberParser.hpp"
#include "PX2NumberFormatter.hpp"
using namespace PX2;

const std::string HTTPMessage::HTTP_1_0 = "HTTP/1.0";
const std::string HTTPMessage::HTTP_1_1 = "HTTP/1.1";
const std::string HTTPMessage::IDENTITY_TRANSFER_ENCODING = "identity";
const std::string HTTPMessage::CHUNKED_TRANSFER_ENCODING = "chunked";
const int         HTTPMessage::UNKNOWN_CONTENT_LENGTH = -1;
const std::string HTTPMessage::UNKNOWN_CONTENT_TYPE;
const std::string HTTPMessage::CONTENT_LENGTH = "Content-Length";
const std::string HTTPMessage::CONTENT_TYPE = "Content-Type";
const std::string HTTPMessage::TRANSFER_ENCODING = "Transfer-Encoding";
const std::string HTTPMessage::CONNECTION = "Connection";
const std::string HTTPMessage::CONNECTION_KEEP_ALIVE = "Keep-Alive";
const std::string HTTPMessage::CONNECTION_CLOSE = "Close";
const std::string HTTPMessage::EMPTY;

//----------------------------------------------------------------------------
HTTPMessage::HTTPMessage() :
_version(HTTP_1_0)
{
}
//----------------------------------------------------------------------------
HTTPMessage::HTTPMessage(const std::string& version) :
_version(version)
{
}
//----------------------------------------------------------------------------
HTTPMessage::~HTTPMessage()
{
}
//----------------------------------------------------------------------------
void HTTPMessage::SetVersion(const std::string& version)
{
	_version = version;
}
//----------------------------------------------------------------------------
void HTTPMessage::SetContentLength(std::streamsize length)
{
	if (length != UNKNOWN_CONTENT_LENGTH)
		set(CONTENT_LENGTH, NumberFormatter::format((int)length));
	else
		erase(CONTENT_LENGTH);
}
//----------------------------------------------------------------------------
std::streamsize HTTPMessage::GetContentLength() const
{
	const std::string& contentLength = get(CONTENT_LENGTH, EMPTY);
	if (!contentLength.empty())
	{
		if (sizeof(std::streamsize) == sizeof(int64_t))
			return static_cast<std::streamsize>(NumberParser::parse64(contentLength));
		else
			return static_cast<std::streamsize>(NumberParser::parse(contentLength));
	}
	else return UNKNOWN_CONTENT_LENGTH;
}
//----------------------------------------------------------------------------
void HTTPMessage::SetTransferEncoding(const std::string& transferEncoding)
{
	if (Icompare(transferEncoding, IDENTITY_TRANSFER_ENCODING) == 0)
		erase(TRANSFER_ENCODING);
	else
		set(TRANSFER_ENCODING, transferEncoding);
}
//----------------------------------------------------------------------------
const std::string& HTTPMessage::GetTransferEncoding() const
{
	return get(TRANSFER_ENCODING, IDENTITY_TRANSFER_ENCODING);
}
//----------------------------------------------------------------------------
void HTTPMessage::SetChunkedTransferEncoding(bool flag)
{
	if (flag)
		SetTransferEncoding(CHUNKED_TRANSFER_ENCODING);
	else
		SetTransferEncoding(IDENTITY_TRANSFER_ENCODING);
}
//----------------------------------------------------------------------------
bool HTTPMessage::GetChunkedTransferEncoding() const
{
	return Icompare(GetTransferEncoding(), CHUNKED_TRANSFER_ENCODING) == 0;
}
//----------------------------------------------------------------------------
void HTTPMessage::SetContentType(const std::string& mediaType)
{
	if (mediaType.empty())
		erase(CONTENT_TYPE);
	else
		set(CONTENT_TYPE, mediaType);
}
//----------------------------------------------------------------------------
const std::string& HTTPMessage::GetContentType() const
{
	return get(CONTENT_TYPE, UNKNOWN_CONTENT_TYPE);
}
//----------------------------------------------------------------------------
void HTTPMessage::SetKeepAlive(bool keepAlive)
{
	if (keepAlive)
		set(CONNECTION, CONNECTION_KEEP_ALIVE);
	else
		set(CONNECTION, CONNECTION_CLOSE);
}
//----------------------------------------------------------------------------
bool HTTPMessage::GetKeepAlive() const
{
	const std::string& connection = get(CONNECTION, EMPTY);
	if (!connection.empty())
		return Icompare(connection, CONNECTION_CLOSE) != 0;
	else
		return GetVersion() == HTTP_1_1;
}
//----------------------------------------------------------------------------
const std::string& HTTPMessage::GetVersion() const
{
	return _version;
}
//----------------------------------------------------------------------------
bool HTTPMessage::HasContentLength() const
{
	return has(CONTENT_LENGTH);
}
//----------------------------------------------------------------------------