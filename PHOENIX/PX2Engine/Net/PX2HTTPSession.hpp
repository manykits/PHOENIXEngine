// PX2HTTPSession.hpp

#ifndef PX2HTTPSESSION_HPP
#define PX2HTTPSESSION_HPP

#include "PX2NetPre.hpp"
#include "PX2StreamSocket.hpp"
#include "PX2Exception.hpp"
#include "PX2Any.hpp"
#include "PX2DataBuffer.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM HTTPSession
	{
	public:
		void SetKeepAlive(bool keepAlive);
		bool GetKeepAlive() const;

		void SetTimeout(const Timespan& timeout);
		Timespan GetTimeout() const;

		bool connected() const;

		virtual void abort();

		const Exception* GetNetworkException() const;

		void AttachSessionData(const Any& data);

		const Any& GetSessionData() const;

		enum
		{
			HTTP_PORT = 80
		};

		StreamSocket DetachSocket();
		StreamSocket& GetSocket();

		void drainBuffer(DataBuffer<char>& buffer);

	protected:
		HTTPSession();
		HTTPSession(const StreamSocket& socket);
		HTTPSession(const StreamSocket& socket, bool keepAlive);
		virtual ~HTTPSession();

		int get();
		int peek();
		virtual int read(char* buffer, std::streamsize length);
		virtual int write(const char* buffer, std::streamsize length);
		int receive(char* buffer, int length);
		int buffered() const;
		void refill();

		virtual void connect(const SocketAddress& address);
		void attachSocket(const StreamSocket& socket);
		void close();

		void setException(const Exception& exc);
		void clearException();

	private:
		enum
		{
			HTTP_DEFAULT_TIMEOUT = 60000000
		};

		HTTPSession(const HTTPSession&);
		HTTPSession& operator = (const HTTPSession&);

		StreamSocket     mSocket;
		char*            _pBuffer;
		char*            _pCurrent;
		char*            _pEnd;
		bool             mIsKeepAlive;
		Timespan   mTimeout;
		Exception* _pException;
		Any        _data;

		friend class HTTPStreamBuf;
		friend class HTTPHeaderStreamBuf;
		friend class HTTPFixedLengthStreamBuf;
		friend class HTTPChunkedStreamBuf;
	};


}

#endif