// PX2DatagramSocket.hpp

#ifndef PX2DATAGRAMSOCKET_HPP
#define PX2DATAGRAMSOCKET_HPP

#include "PX2NetPre.hpp"
#include "PX2Socket.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DatagramSocket : public Socket
	{
	public:
		DatagramSocket();
		explicit DatagramSocket(IPAddress::Family family);
		DatagramSocket(const SocketAddress& address, 
			bool reuseAddress = false);
		DatagramSocket(const Socket& socket);
		DatagramSocket(const std::string &ip, int port, bool reuseAddress = false);
		~DatagramSocket();

		DatagramSocket& operator = (const Socket& socket);

		void ConnectB(const SocketAddress& address);
		void Bind(const SocketAddress& address, bool reuseAddress = false);
		int SendString(const std::string &str);
		int SendBytes(const void* buffer, int length, int flags = 0);
		int ReceiveBytes(void* buffer, int length, int flags = 0);
		int SendTo(const void* buffer, int length,
			const SocketAddress& address, int flags = 0);
		int ReceiveFrom(void* buffer, int length, SocketAddress& address,
			int flags = 0);

		static void Bradcast(int port, const std::string &str);

		void SetBroadcast(bool flag);
		bool IsBroadcast() const;

	protected:
		DatagramSocket(SocketImpl* pImpl);
	};

}

#endif