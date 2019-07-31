// PX2HTTPServerResponseImpl.cpp

#include "PX2HTTPServerResponseImpl.hpp"
#include "PX2HTTPHeaderStream.hpp"
#include "PX2HTTPFixedLengthStream.hpp"
#include "PX2HTTPRequest.hpp"
#include "PX2HTTPServerSession.hpp"
#include "PX2HTTPChunkedStream.hpp"
#include "PX2HTTPServerRequestImpl.hpp"
#include "PX2HTTPStream.hpp"
#include "PX2DateTimeFormat.hpp"
#include "PX2DateTimeFormatter.hpp"
#include "PX2CountingStream.hpp"
#include "PX2StreamCopier.hpp"
#include "PX2File.hpp"
#include "PX2FileIO.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPServerResponseImpl::HTTPServerResponseImpl(HTTPServerSession& session) :
mSession(session),
mRequest(0),
mStream(0)
{
}
//----------------------------------------------------------------------------
HTTPServerResponseImpl::~HTTPServerResponseImpl()
{
	delete mStream;
}
//----------------------------------------------------------------------------
void HTTPServerResponseImpl::SendContinue()
{
	HTTPHeaderOutputStream hs(mSession);
	hs << GetVersion() << " 100 Continue\r\n\r\n";
}
//----------------------------------------------------------------------------
std::ostream& HTTPServerResponseImpl::Send()
{
	assert(!mStream);

	if ((mRequest && mRequest->GetMethod() == HTTPRequest::HTTP_HEAD) ||
		GetStatus() < 200 ||
		GetStatus() == HTTPResponse::HTTP_NO_CONTENT ||
		GetStatus() == HTTPResponse::HTTP_NOT_MODIFIED)
	{
		CountingOutputStream cs;
		write(cs);
		mStream = new HTTPFixedLengthOutputStream(mSession, cs.chars());
		write(*mStream);
	}
	else if (GetChunkedTransferEncoding())
	{
		HTTPHeaderOutputStream hs(mSession);
		write(hs);
		mStream = new HTTPChunkedOutputStream(mSession);
	}
	else if (HasContentLength())
	{
		CountingOutputStream cs;
		write(cs);
		mStream = new HTTPFixedLengthOutputStream(mSession, GetContentLength() + cs.chars());
		write(*mStream);
	}
	else
	{
		mStream = new HTTPOutputStream(mSession);
		SetKeepAlive(false);
		write(*mStream);
	}
	return *mStream;
}
//----------------------------------------------------------------------------
void HTTPServerResponseImpl::SendFile(const std::string& path, 
	const std::string& mediaType)
{
	assert(!mStream);

	File f(path);
	Timestamp dateTime = f.getLastModified();
	
	File::FileSize length = f.getSize();
	set("Last-Modified", DateTimeFormatter::format(dateTime, DateTimeFormat::HTTP_FORMAT));
	SetContentLength(static_cast<int>(length));
	SetContentType(mediaType);
	SetChunkedTransferEncoding(false);

	int bufferSize = 0;
	char *buffer = 0;
	if (FileIO::Load(path, true, bufferSize, buffer))
	{
		std::string str(buffer, bufferSize);
		std::istringstream iStr(str);

		mStream = new HTTPHeaderOutputStream(mSession);
		write(*mStream);
		if (mRequest && mRequest->GetMethod() != HTTPRequest::HTTP_HEAD)
		{
			StreamCopier::CopyStream(iStr, *mStream);
		}
	}
	else
	{
		throw OpenFileException(path);
	}
}
//----------------------------------------------------------------------------
void HTTPServerResponseImpl::SendBuffer(const void* pBuffer, std::size_t length)
{
	assert(!mStream);

	SetContentLength(static_cast<int>(length));
	SetChunkedTransferEncoding(false);

	mStream = new HTTPHeaderOutputStream(mSession);
	write(*mStream);
	if (mRequest && mRequest->GetMethod() != HTTPRequest::HTTP_HEAD)
	{
		mStream->write(static_cast<const char*>(pBuffer), static_cast<std::streamsize>(length));
	}
}
//----------------------------------------------------------------------------
void HTTPServerResponseImpl::Redirect(const std::string& uri, HTTPStatus status)
{
	assert(!mStream);

	SetContentLength(0);
	SetChunkedTransferEncoding(false);

	SetStatusAndReason(status);
	set("Location", uri);

	mStream = new HTTPHeaderOutputStream(mSession);
	write(*mStream);
}
//----------------------------------------------------------------------------
void HTTPServerResponseImpl::RequireAuthentication(const std::string& realm)
{
	assert(!mStream);

	SetStatusAndReason(HTTPResponse::HTTP_UNAUTHORIZED);
	std::string auth("Basic realm=\"");
	auth.append(realm);
	auth.append("\"");
	set("WWW-Authenticate", auth);
}
//----------------------------------------------------------------------------
bool HTTPServerResponseImpl::Sent() const
{
	return mStream != 0;
}
//----------------------------------------------------------------------------
void HTTPServerResponseImpl::AttachRequest(HTTPServerRequestImpl* pRequest)
{
	mRequest = pRequest;
}
//----------------------------------------------------------------------------