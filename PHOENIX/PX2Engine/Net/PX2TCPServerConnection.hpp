// PX2TCPServerConnection.hpp

#ifndef PX2TCPSERVERCONNECTION_HPP
#define PX2TCPSERVERCONNECTION_HPP

#include "PX2NetPre.hpp"
#include "PX2Runnable.hpp"
#include "PX2StreamSocket.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TCPServerConnection : public Runnable
	{
	public:
		TCPServerConnection();
		TCPServerConnection(const StreamSocket& socket);
		virtual ~TCPServerConnection();

	protected:
		StreamSocket& GetSocket();

		void Start();

	private:
		TCPServerConnection(const TCPServerConnection&);
		TCPServerConnection& operator = (const TCPServerConnection&);

		StreamSocket mSocket;

		friend class TCPServerDispatcher;
	};
	typedef Pointer0<TCPServerConnection> TCPServerConnectionPtr;

}

#endif