// PX2WebSocketImpl.cpp

#include "PX2WebSocketImpl.hpp"
#include "PX2HTTPSession.hpp"
#include "PX2MemoryStream.hpp"
#include "PX2WebSocket.hpp"
#include "PX2BinaryWriter.hpp"
#include "PX2BinaryReader.hpp"
#include "PX2Exception.hpp"
#include "PX2NetException.hpp"
#include "PX2Format.hpp"
using namespace PX2;

#ifdef __GNUC__
#include <stdint.h>
#elif defined(_MSC_VER)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
typedef int int32_t;
typedef long long int int64_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
#endif

//----------------------------------------------------------------------------
WebSocketImpl::WebSocketImpl(StreamSocketImpl* pStreamSocketImpl, HTTPSession& session, bool mustMaskPayload) :
StreamSocketImpl(pStreamSocketImpl->GetSocket()),
_pStreamSocketImpl(pStreamSocketImpl),
_buffer(0),
_bufferOffset(0),
_frameFlags(0),
_mustMaskPayload(mustMaskPayload)
{
	assertion(0!=pStreamSocketImpl, "impl must not be null.");
	session.drainBuffer(_buffer);
}
//----------------------------------------------------------------------------
WebSocketImpl::~WebSocketImpl()
{
	Reset();
}
//----------------------------------------------------------------------------
int WebSocketImpl::SendBytes(const void* buffer, int length, int flags)
{
	DataBuffer<char> frame(length + MAX_HEADER_LENGTH);
	MemoryOutputStream ostr(frame.begin(), frame.size());
	BinaryWriter writer(ostr, BinaryWriter::NETWORK_BYTE_ORDER);

	if (flags == 0) flags = WebSocket::FRAME_BINARY;
	flags &= 0xff;
	writer << static_cast<uint8_t>(flags);
	uint8_t lengthByte(0);
	if (_mustMaskPayload)
	{
		lengthByte |= FRAME_FLAG_MASK;
	}
	if (length < 126)
	{
		lengthByte |= static_cast<uint8_t>(length);
		writer << lengthByte;
	}
	else if (length < 65536)
	{
		lengthByte |= 126;
		writer << lengthByte << static_cast<uint16_t>(length);
	}
	else
	{
		lengthByte |= 127;
		writer << lengthByte << static_cast<uint64_t>(length);
	}
	if (_mustMaskPayload)
	{
		const uint32_t mask = _rnd.Next();
		const char* m = reinterpret_cast<const char*>(&mask);
		const char* b = reinterpret_cast<const char*>(buffer);
		writer.writeRaw(m, 4);
		char* p = frame.begin() + ostr.charsWritten();
		for (int i = 0; i < length; i++)
		{
			p[i] = b[i] ^ m[i % 4];
		}
	}
	else
	{
		std::memcpy(frame.begin() + ostr.charsWritten(), buffer, length);
	}
	_pStreamSocketImpl->SendBytes(frame.begin(), length + static_cast<int>(ostr.charsWritten()));
	return length;
}
//----------------------------------------------------------------------------
int WebSocketImpl::receiveHeader(char mask[4], bool& useMask)
{
	char header[MAX_HEADER_LENGTH];
	int n = receiveNBytes(header, 2);
	if (n <= 0)
	{
		_frameFlags = 0;
		return n;
	}
	assertion(n == 2, "");
	uint8_t flags = static_cast<uint8_t>(header[0]);
	_frameFlags = flags;
	uint8_t lengthByte = static_cast<uint8_t>(header[1]);
	useMask = ((lengthByte & FRAME_FLAG_MASK) != 0);
	int payloadLength;
	lengthByte &= 0x7f;
	if (lengthByte == 127)
	{
		n = receiveNBytes(header + 2, 8);
		if (n <= 0)
		{
			_frameFlags = 0;
			return n;
		}
		MemoryInputStream istr(header + 2, 8);
		BinaryReader reader(istr, BinaryReader::NETWORK_BYTE_ORDER);
		uint64_t l = 0;
		reader >> l;
		payloadLength = static_cast<int>(l);
	}
	else if (lengthByte == 126)
	{
		n = receiveNBytes(header + 2, 2);
		if (n <= 0)
		{
			_frameFlags = 0;
			return n;
		}
		MemoryInputStream istr(header + 2, 2);
		BinaryReader reader(istr, BinaryReader::NETWORK_BYTE_ORDER);
		uint16_t l = 0;
		reader >> l;
		payloadLength = static_cast<int>(l);
	}
	else
	{
		payloadLength = lengthByte;
	}

	if (useMask)
	{
		n = receiveNBytes(mask, 4);
		if (n <= 0)
		{
			_frameFlags = 0;
			return n;
		}
	}

	return payloadLength;
}
//----------------------------------------------------------------------------
int WebSocketImpl::receivePayload(char *buffer, int payloadLength, 
	char mask[4], bool useMask)
{
	int received = receiveNBytes(reinterpret_cast<char*>(buffer), payloadLength);
	if (received <= 0) throw WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);

	if (useMask)
	{
		for (int i = 0; i < received; i++)
		{
			buffer[i] ^= mask[i % 4];
		}
	}
	return received;
}
//----------------------------------------------------------------------------
int WebSocketImpl::ReceiveBytes(void* buffer, int length, int)
{
	char mask[4];
	bool useMask;
	int payloadLength = receiveHeader(mask, useMask);
	if (payloadLength <= 0)
		return payloadLength;
	if (payloadLength > length)
		throw WebSocketException(PX2::Format("Insufficient buffer for payload size %hu", payloadLength), WebSocket::WS_ERR_PAYLOAD_TOO_BIG);
	return receivePayload(reinterpret_cast<char*>(buffer), payloadLength, mask, useMask);
}
//----------------------------------------------------------------------------
int WebSocketImpl::ReceiveBytes(DataBuffer<char>& buffer, int)
{
	char mask[4];
	bool useMask;
	int payloadLength = receiveHeader(mask, useMask);
	if (payloadLength <= 0)
		return payloadLength;
	int oldSize = buffer.size();
	buffer.resize(oldSize + payloadLength);
	return receivePayload(buffer.begin() + oldSize, payloadLength, mask, useMask);
}
//----------------------------------------------------------------------------
int WebSocketImpl::receiveNBytes(void* buffer, int bytes)
{
	int received = receiveSomeBytes(reinterpret_cast<char*>(buffer), bytes);
	if (received > 0)
	{
		while (received < bytes)
		{
			int n = receiveSomeBytes(reinterpret_cast<char*>(buffer)+received, bytes - received);
			if (n > 0)
				received += n;
			else
				throw WebSocketException("Incomplete frame received", WebSocket::WS_ERR_INCOMPLETE_FRAME);
		}
	}
	return received;
}
//----------------------------------------------------------------------------
int WebSocketImpl::receiveSomeBytes(char* buffer, int bytes)
{
	int n = _buffer.size() - _bufferOffset;
	if (n > 0)
	{
		if (bytes < n) n = bytes;
		std::memcpy(buffer, _buffer.begin() + _bufferOffset, n);
		_bufferOffset += n;
		return n;
	}
	else
	{
		return _pStreamSocketImpl->ReceiveBytes(buffer, bytes);
	}
}
//----------------------------------------------------------------------------
SocketImpl* WebSocketImpl::AcceptConnection(SocketAddress& clientAddr)
{
	return 0;
}
//----------------------------------------------------------------------------
int WebSocketImpl::ConnectB(const SocketAddress& address)
{
	return 0;
}
//----------------------------------------------------------------------------
int WebSocketImpl::ConnectB(const SocketAddress& address, const Timespan& timeout)
{
	return 0;
}
//----------------------------------------------------------------------------
int WebSocketImpl::ConnectNB(const SocketAddress& address)
{
	return 0;
}
//----------------------------------------------------------------------------
int WebSocketImpl::Bind(const SocketAddress& address, bool reuseAddress)
{
	return 0;
}
//----------------------------------------------------------------------------
int WebSocketImpl::Bind6(const SocketAddress& address, bool reuseAddress, bool ipV6Only)
{
	return 0;
}
//----------------------------------------------------------------------------
void WebSocketImpl::Listen(int backlog)
{
}
//----------------------------------------------------------------------------
void WebSocketImpl::Close()
{
	_pStreamSocketImpl->Close();
	Reset();
}
//----------------------------------------------------------------------------
void WebSocketImpl::ShutdownReceive()
{
	_pStreamSocketImpl->ShutdownReceive();
}
//----------------------------------------------------------------------------
void WebSocketImpl::ShutdownSend()
{
	_pStreamSocketImpl->ShutdownSend();
}
//----------------------------------------------------------------------------
void WebSocketImpl::Shutdown()
{
	_pStreamSocketImpl->Shutdown();
}
//----------------------------------------------------------------------------
int WebSocketImpl::SendTo(const void* buffer, int length, 
	const SocketAddress& address, int flags)
{
	return 0;
}
//----------------------------------------------------------------------------
int WebSocketImpl::ReceiveFrom(void* buffer, int length, 
	SocketAddress& address, int flags)
{
	return 0;
}
//----------------------------------------------------------------------------
void WebSocketImpl::SendUrgent(unsigned char data)
{
}
//----------------------------------------------------------------------------
bool WebSocketImpl::Secure() const
{
	return _pStreamSocketImpl->Secure();
}
//----------------------------------------------------------------------------
void WebSocketImpl::SetSendTimeout(const Timespan& timeout)
{
	_pStreamSocketImpl->SetSendTimeout(timeout);
}
//----------------------------------------------------------------------------
Timespan WebSocketImpl::GetSendTimeout()
{
	return _pStreamSocketImpl->GetSendTimeout();
}
//----------------------------------------------------------------------------
void WebSocketImpl::SetReceiveTimeout(const Timespan& timeout)
{
	_pStreamSocketImpl->SetReceiveTimeout(timeout);
}
//----------------------------------------------------------------------------
Timespan WebSocketImpl::GetReceiveTimeout()
{
	return _pStreamSocketImpl->GetReceiveTimeout();
}
//----------------------------------------------------------------------------
int WebSocketImpl::Available()
{
	int n = _buffer.size() - _bufferOffset;
	if (n > 0)
		return n + _pStreamSocketImpl->Available();
	else
		return _pStreamSocketImpl->Available();
}
//----------------------------------------------------------------------------