// PX2WebSocket.hpp

#ifndef PX2WEBSOCKET_HPP
#define PX2WEBSOCKET_HPP

#include "PX2NetPre.hpp"
#include "PX2Socket.hpp"
#include "PX2StreamSocket.hpp"
#include "PX2DataBuffer.hpp"

namespace PX2
{

	class WebSocketImpl;
	class HTTPServerRequest;
	class HTTPServerResponse;
	class HTTPClientSession;
	class HTTPRequest;
	class HTTPResponse;

	class PX2_ENGINE_ITEM WebSocket : public StreamSocket
	{
	public:
		enum Mode
		{
			WS_SERVER, /// Server-side WebSocket.
			WS_CLIENT  /// Client-side WebSocket.
		};

		enum FrameFlags
		{
			FRAME_FLAG_FIN = 0x80, /// FIN bit: final fragment of a multi-fragment message.
			FRAME_FLAG_RSV1 = 0x40, /// Reserved for future use. Must be zero.
			FRAME_FLAG_RSV2 = 0x20, /// Reserved for future use. Must be zero.
			FRAME_FLAG_RSV3 = 0x10  /// Reserved for future use. Must be zero.
		};

		enum FrameOpcodes
		{
			FRAME_OP_CONT = 0x00, /// Continuation frame.
			FRAME_OP_TEXT = 0x01, /// Text frame.
			FRAME_OP_BINARY = 0x02, /// Binary frame.
			FRAME_OP_CLOSE = 0x08, /// Close connection.
			FRAME_OP_PING = 0x09, /// Ping frame.
			FRAME_OP_PONG = 0x0a, /// Pong frame.
			FRAME_OP_BITMASK = 0x0f, /// Bit mask for opcodes. 
			FRAME_OP_SETRAW = 0x100 /// Set raw flags (for use with sendBytes() and FRAME_OP_CONT).
		};

		enum SendFlags
		{
			FRAME_TEXT = FRAME_FLAG_FIN | FRAME_OP_TEXT,
			/// Use this for sending a single text (UTF-8) payload frame.
			FRAME_BINARY = FRAME_FLAG_FIN | FRAME_OP_BINARY
			/// Use this for sending a single binary payload frame.
		};

		enum StatusCodes
		{
			WS_NORMAL_CLOSE = 1000,
			WS_ENDPOINT_GOING_AWAY = 1001,
			WS_PROTOCOL_ERROR = 1002,
			WS_PAYLOAD_NOT_ACCEPTABLE = 1003,
			WS_RESERVED = 1004,
			WS_RESERVED_NO_STATUS_CODE = 1005,
			WS_RESERVED_ABNORMAL_CLOSE = 1006,
			WS_MALFORMED_PAYLOAD = 1007,
			WS_POLICY_VIOLATION = 1008,
			WS_PAYLOAD_TOO_BIG = 1009,
			WS_EXTENSION_REQUIRED = 1010,
			WS_UNEXPECTED_CONDITION = 1011,
			WS_RESERVED_TLS_FAILURE = 1015
		};

		enum ErrorCodes
		{
			WS_ERR_NO_HANDSHAKE = 1,
			/// No Connection: Upgrade or Upgrade: websocket header in handshake request.
			WS_ERR_HANDSHAKE_NO_VERSION = 2,
			/// No Sec-WebSocket-Version header in handshake request.
			WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION = 3,
			/// Unsupported WebSocket version requested by client.
			WS_ERR_HANDSHAKE_NO_KEY = 4,
			/// No Sec-WebSocket-Key header in handshake request.
			WS_ERR_HANDSHAKE_ACCEPT = 5,
			/// No Sec-WebSocket-Accept header or wrong value.
			WS_ERR_UNAUTHORIZED = 6,
			/// The server rejected the username or password for authentication.
			WS_ERR_PAYLOAD_TOO_BIG = 10,
			/// Payload too big for supplied buffer.
			WS_ERR_INCOMPLETE_FRAME = 11
			/// Incomplete frame received.
		};

		WebSocket(HTTPServerRequest& request, HTTPServerResponse& response);
		WebSocket(const Socket& socket);
		virtual ~WebSocket();

		WebSocket& operator = (const Socket& socket);

		void shutdown();
		void shutdown(int16_t statusCode, const std::string& statusMessage = "");
		int sendFrame(const void* buffer, int length, int flags = FRAME_TEXT);
		int receiveFrame(void* buffer, int length, int& flags);
		int receiveFrame(DataBuffer<char>& buffer, int& flags);
		Mode mode() const;
		/// Returns WS_SERVER if the WebSocket is a server-side
		/// WebSocket, or WS_CLIENT otherwise.

		static const std::string WEBSOCKET_VERSION;
		/// The WebSocket protocol version supported (13).

	protected:
		static WebSocketImpl* _Accept(HTTPServerRequest& request, HTTPServerResponse& response);
		static std::string _ComputeAccept(const std::string& key);

	private:
		WebSocket();

		static const std::string WEBSOCKET_GUID;
	};

}

#endif