// PX2NetStreamSocket.hpp

#ifndef PX2STREAMSOCKET_HPP
#define PX2STREAMSOCKET_HPP

#include "PX2NetPre.hpp"
#include "PX2Socket.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM StreamSocket : public Socket
	{
	public:
		StreamSocket();
		explicit StreamSocket(const SocketAddress& address);
		explicit StreamSocket(IPAddress::Family family);
		StreamSocket(const Socket& socket);
		StreamSocket(px2_socket_t socket);
		StreamSocket(SocketImpl* impl);
		virtual ~StreamSocket();

		StreamSocket& operator = (const Socket& socket);

		int ConnectB(const SocketAddress& address);
		int ConnectB(const SocketAddress& address, const Timespan& timeout);
		int ConnectNB(const SocketAddress& address);

		void ShutdownReceive();
		void ShutdownSend();
		void Shutdown();

		int SendBytes(const void* buffer, int length, int flags = 0);
		int ReceiveBytes(void* buffer, int length, int flags = 0);

		void SendUrgent(unsigned char data);

	private:
		enum
		{
			BUFFER_SIZE = 1024
		};

		friend class ServerSocket;
		friend class SocketIOS;
	};

}

#endif