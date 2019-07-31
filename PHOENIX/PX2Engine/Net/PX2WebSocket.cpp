// PX2WebSocket.cpp

#include "PX2WebSocket.hpp"
#include "PX2WebSocketImpl.hpp"
#include "PX2HTTPServerRequest.hpp"
#include "PX2HTTPServerResponse.hpp"
#include "PX2HTTPServerRequestImpl.hpp"
#include "PX2HTTPServerSession.hpp"
#include "PX2NetException.hpp"
#include "PX2SHA1Engine.hpp"
#include "PX2Base64.hpp"
#include "PX2MemoryStream.hpp"
#include "PX2BinaryWriter.hpp"
#include "PX2Base64.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
const std::string WebSocket::WEBSOCKET_GUID("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
const std::string WebSocket::WEBSOCKET_VERSION("13");
//----------------------------------------------------------------------------
WebSocket::WebSocket(HTTPServerRequest& request, HTTPServerResponse& response) :
StreamSocket(_Accept(request, response))
{
}
//----------------------------------------------------------------------------
WebSocket::WebSocket(const Socket& socket) :
StreamSocket(socket)
{
	if (!dynamic_cast<WebSocketImpl*>(GetImpl()))
	{
		assertion(false, "must be WebSocketImpl");
	}
}
//----------------------------------------------------------------------------
WebSocket::~WebSocket()
{
}
//----------------------------------------------------------------------------
WebSocket& WebSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<WebSocketImpl*>(socket.GetImpl()))
		Socket::operator = (socket);
	else
	{
		assertion(false, "Cannot assign incompatible socket");
	}

	return *this;
}
//----------------------------------------------------------------------------
void WebSocket::shutdown()
{
	shutdown(WS_NORMAL_CLOSE);
}
//----------------------------------------------------------------------------
void WebSocket::shutdown(int16_t statusCode, const std::string& statusMessage)
{
	DataBuffer<char> buffer(statusMessage.size() + 2);
	MemoryOutputStream ostr(buffer.begin(), buffer.size());
	
	BinaryWriter writer(ostr, BinaryWriter::NETWORK_BYTE_ORDER);

	writer << statusCode;
	writer.writeRaw(statusMessage);
	sendFrame(buffer.begin(), static_cast<int>(ostr.charsWritten()), 
		FRAME_FLAG_FIN | FRAME_OP_CLOSE);
}
//----------------------------------------------------------------------------
int WebSocket::sendFrame(const void* buffer, int length, int flags)
{
	flags |= FRAME_OP_SETRAW;
	return static_cast<WebSocketImpl*>(GetImpl())->SendBytes(buffer, length, flags);
}
//----------------------------------------------------------------------------
int WebSocket::receiveFrame(void* buffer, int length, int& flags)
{
	int n = static_cast<WebSocketImpl*>(GetImpl())->ReceiveBytes(buffer, length, 0);
	flags = static_cast<WebSocketImpl*>(GetImpl())->FrameFlags();
	return n;
}
//----------------------------------------------------------------------------
int WebSocket::receiveFrame(DataBuffer<char>& buffer, int& flags)
{
	int n = static_cast<WebSocketImpl*>(GetImpl())->ReceiveBytes(buffer, 0);
	flags = static_cast<WebSocketImpl*>(GetImpl())->FrameFlags();
	return n;
}
//----------------------------------------------------------------------------
WebSocket::Mode WebSocket::mode() const
{
	return static_cast<WebSocketImpl*>(GetImpl())->MustMaskPayload() ? WS_CLIENT : WS_SERVER;
}
//----------------------------------------------------------------------------
WebSocketImpl* WebSocket::_Accept(HTTPServerRequest& request, HTTPServerResponse& response)
{
	if (request.hasToken("Connection", "upgrade") && Icompare(request.get("Upgrade", ""), "websocket") == 0)
	{
		std::string version = request.get("Sec-WebSocket-Version", "");
		if (version.empty())
		{
			assertion(false, "Missing Sec-WebSocket-Version in handshake request");
		}
		
		if (version != WEBSOCKET_VERSION)
		{
			assertion(false, "Unsupported WebSocket version requested");
		}

		std::string key = request.get("Sec-WebSocket-Key", "");
		TrimInPlace(key);
		if (key.empty())
		{
			assertion(false, "Missing Sec-WebSocket-Key in handshake request");
		}

		response.SetStatusAndReason(HTTPResponse::HTTP_SWITCHING_PROTOCOLS);
		response.set("Upgrade", "websocket");
		response.set("Connection", "Upgrade");
		std::string cmpAccept = _ComputeAccept(key);
		response.set("Sec-WebSocket-Accept", cmpAccept);
		response.SetContentLength(0);
		response.Send().flush();

		HTTPServerRequestImpl& requestImpl = static_cast<HTTPServerRequestImpl&>(request);

		return new0 WebSocketImpl(static_cast<StreamSocketImpl*>(
			requestImpl.DetachSocket().GetImpl()), requestImpl.GetSession(), false);
	}
	else
	{
		throw WebSocketException("No WebSocket handshake", WS_ERR_NO_HANDSHAKE);
	}
}
//----------------------------------------------------------------------------
std::string WebSocket::_ComputeAccept(const std::string& key)
{
	std::string keyStr = key;
	//keyStr = "wkMO4FmEuOLpV6BMnNZNpQ==";

	std::string accept(keyStr);
	accept += WEBSOCKET_GUID;
	SHA1Engine sha1;
	sha1.update(accept);
	DigestEngine::Digest d = sha1.digest();

	const char *dBuf = reinterpret_cast<char*>(&d[0]);
	int size = d.size();
	std::string retStr = Base64::Encode(dBuf, size);

	//std::ostringstream ostr;
	//Base64Encoder base64(ostr);
	//const char *dBuf = reinterpret_cast<const char*>(&d[0]);
	//base64.write(dBuf, d.size());
	//base64.close();

	//std::string retStr = ostr.str();
	return retStr;
}
//----------------------------------------------------------------------------