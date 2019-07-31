// PX2UDPServer.hpp

#ifndef PX2UDPSERVER_HPP
#define PX2UDPSERVER_HPP

#include "PX2Runnable.hpp"
#include "PX2SocketAddress.hpp"
#include "PX2DatagramSocket.hpp"
#include "PX2Thread.hpp"
#include "PX2Eventt.hpp"
#include "PX2Mutex.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class UDPServer;

	struct UDPRecvObj{
		int Size;
		std::string Buffer;
	};

	typedef void(*UDPServerRecvCallback) (UDPServer *server, SocketAddress &address,
		const std::string &buf, int size);

	class PX2_ENGINE_ITEM UDPServer : public Object, public Runnable
	{
		PX2_NEW(UDPServer);

	public:
		UDPServer();
		UDPServer(const SocketAddress& sa);
		~UDPServer();

		void Bind(int port);
		void Start();
		void Stop();

		DatagramSocket &GetSocket();
		int GetPort() const;
		SocketAddress GetAddress() const;
		void Update(float elapsedSeconds);

		virtual void Run();

		bool IsHasRecvCallback(UDPServerRecvCallback callback);
		void AddRecvCallback(UDPServerRecvCallback callback);
		void OnRecv(SocketAddress &address, const std::string &buf, int length);

		bool IsHasOnRecvCallback(const std::string &callback) const;
		bool AddOnRecvCallback(const std::string &callback);
		bool RemoveOnRecvCallback(const std::string &callback);

	private:
		DatagramSocket mSocket;
		Thread mThread;
		Eventt mReadEvent;
		bool mIsStop;

		Mutex mMutex;
		std::vector<std::pair<SocketAddress, UDPRecvObj> > mRecvBufs;
		std::vector<UDPServerRecvCallback> mUDPServerRecvCallbacks;

		std::vector<std::string> mOnRecvCallbacks;
	};
	typedef Pointer0<UDPServer> UDPServerPtr;

}

#endif