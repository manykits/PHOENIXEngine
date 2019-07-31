// PX2HTTPServerRequestImpl.cpp

#include "PX2HTTPServerRequestImpl.hpp"
#include "PX2HTTPServerParams.hpp"
#include "PX2HTTPHeaderStream.hpp"
#include "PX2HTTPChunkedStream.hpp"
#include "PX2HTTPFixedLengthStream.hpp"
#include "PX2HTTPServerSession.hpp"
#include "PX2HTTPStream.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
const std::string HTTPServerRequestImpl::EXPECT("Expect");
//----------------------------------------------------------------------------
HTTPServerRequestImpl::HTTPServerRequestImpl(HTTPServerResponseImpl& response, 
	HTTPServerSession& session, HTTPServerParams* params) :
mResponse(response),
mSession(session),
mStream(0),
mParams(params)
{
	response.AttachRequest(this);

	HTTPHeaderInputStream hs(session);
	read(hs);

	// Now that we know socket is still connected, obtain addresses
	mClientAddress = session.GetClientAddress();
	mServerAddress = session.GerverAddress();

	if (GetChunkedTransferEncoding())
	{
		mStream = new HTTPChunkedInputStream(session);
	}
	else if (HasContentLength())
	{
		mStream = new HTTPFixedLengthInputStream(session, GetContentLength());
	}
	else if (GetMethod() == HTTPRequest::HTTP_GET ||
		GetMethod() == HTTPRequest::HTTP_HEAD || GetMethod() == HTTPRequest::HTTP_DELETE)
	{
		mStream = new HTTPFixedLengthInputStream(session, 0);
	}
	else
	{
		mStream = new HTTPInputStream(session);
	}
}
//----------------------------------------------------------------------------
HTTPServerRequestImpl::~HTTPServerRequestImpl()
{
	delete mStream;
}
//----------------------------------------------------------------------------
StreamSocket& HTTPServerRequestImpl::GetSocket()
{
	return mSession.GetSocket();
}
//----------------------------------------------------------------------------
StreamSocket HTTPServerRequestImpl::DetachSocket()
{
	return mSession.DetachSocket();
}
//----------------------------------------------------------------------------
HTTPServerSession &HTTPServerRequestImpl::GetSession()
{
	return mSession;
}
//----------------------------------------------------------------------------
bool HTTPServerRequestImpl::ExpectContinue() const
{
	const std::string& expect = get(EXPECT, EMPTY);
	return !expect.empty() && Icompare(expect, "100-continue") == 0;
}
//----------------------------------------------------------------------------
inline std::istream& HTTPServerRequestImpl::stream()
{
	//poco_check_ptr(mStream);

	return *mStream;
}
//----------------------------------------------------------------------------
inline const SocketAddress& HTTPServerRequestImpl::GetClientAddress() const
{
	return mClientAddress;
}
//----------------------------------------------------------------------------
inline const SocketAddress& HTTPServerRequestImpl::GerverAddress() const
{
	return mServerAddress;
}
//----------------------------------------------------------------------------
const HTTPServerParams& HTTPServerRequestImpl::GetServerParams() const
{
	return *mParams;
}
//----------------------------------------------------------------------------
HTTPServerResponse& HTTPServerRequestImpl::Response() const
{
	return mResponse;
}
//----------------------------------------------------------------------------