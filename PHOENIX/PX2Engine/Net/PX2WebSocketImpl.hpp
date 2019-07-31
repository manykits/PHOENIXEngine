// PX2WebSocketImpl.hpp

#ifndef PX2WEBSOCKETIMPL_HPP
#define PX2WEBSOCKETIMPL_HPP

#include "PX2NetPre.hpp"
#include "PX2Socket.hpp"
#include "PX2StreamSocketImpl.hpp"
#include "PX2DataBuffer.hpp"
#include "PX2Random.hpp"

namespace PX2
{

	class HTTPSession;

	class  WebSocketImpl : public StreamSocketImpl
	{
	public:
		WebSocketImpl(StreamSocketImpl* pStreamSocketImpl, HTTPSession& session, bool mustMaskPayload);

		virtual int SendBytes(const void* buffer, int length, int flags);
		virtual int ReceiveBytes(void* buffer, int length, int flags);
		virtual int ReceiveBytes(DataBuffer<char>& buffer, int flags);

		virtual SocketImpl* AcceptConnection(SocketAddress &clientAddr);
		virtual int ConnectB(const SocketAddress &address);
		virtual int ConnectB(const SocketAddress &address,
			const Timespan& timeout);
		virtual int ConnectNB(const SocketAddress& address);
		virtual int Bind(const SocketAddress& address,
			bool reuseAddress = false);
		virtual int Bind6(const SocketAddress& address,
			bool reuseAddress = false, bool ipV6Only = false);
		virtual void Listen(int backlog = 64);
		virtual void Close();
		virtual void ShutdownReceive();
		virtual void ShutdownSend();
		virtual void Shutdown();
		virtual int SendTo(const void* buffer, int length, const SocketAddress& address, int flags = 0);
		virtual int ReceiveFrom(void* buffer, int length, SocketAddress& address, int flags = 0);
		virtual void SendUrgent(unsigned char data);
		virtual int Available();
		virtual bool Secure() const;
		virtual void SetSendTimeout(const Timespan& timeout);
		virtual Timespan GetSendTimeout();
		virtual void SetReceiveTimeout(const Timespan& timeout);
		virtual Timespan GetReceiveTimeout();

		// Internal
		int FrameFlags() const;
		bool MustMaskPayload() const;

	protected:
		enum
		{
			FRAME_FLAG_MASK = 0x80,
			MAX_HEADER_LENGTH = 14
		};

		int receiveHeader(char mask[4], bool& useMask);
		int receivePayload(char *buffer, int payloadLength, char mask[4], bool useMask);
		int receiveNBytes(void* buffer, int bytes);
		int receiveSomeBytes(char* buffer, int bytes);
		virtual ~WebSocketImpl();

	private:
		WebSocketImpl();

		StreamSocketImpl* _pStreamSocketImpl;
		DataBuffer<char> _buffer;
		int _bufferOffset;
		int _frameFlags;
		bool _mustMaskPayload;
		Random _rnd;
	};

	inline int WebSocketImpl::FrameFlags() const
	{
		return _frameFlags;
	}


	inline bool WebSocketImpl::MustMaskPayload() const
	{
		return _mustMaskPayload;
	}

}

#endif