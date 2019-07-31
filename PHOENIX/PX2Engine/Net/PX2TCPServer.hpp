// PX2TCPServer.hpp

#ifndef PX2TCPSERVER_HPP
#define PX2TCPSERVER_HPP

#include "PX2NetPre.hpp"
#include "PX2Runnable.hpp"
#include "PX2TCPServerParams.hpp"
#include "PX2TCPServerConnectionFactory.hpp"
#include "PX2TCPServerDispatcher.hpp"
#include "PX2ThreadPool.hpp"
#include "PX2ServerSocket.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TCPServer : public Runnable
	{
	public:
		TCPServer(TCPServerConnectionFactory *factory, int portNumber = 0, TCPServerParams *params = 0);
		TCPServer(TCPServerConnectionFactory *factory, const ServerSocket& socket, TCPServerParams *params = 0);
		TCPServer(TCPServerConnectionFactory *factory, ThreadPool *threadPool, const ServerSocket& socket, TCPServerParams *params = 0);
		virtual ~TCPServer();

		const TCPServerParams& params() const;
		TCPServerDispatcher* GetDispatcher();

		void SetSingleThread(bool single);
		void SingleThreadUpdate();

		void Start();
		void Stop();

		int CurrentThreads() const;
		int GetNumMaxThreads() const;
		int GetNumTotalConnections() const;
		int GetNumCurrentConnections() const;
		int GetNumMaxConcurrentConnections() const;
		int GetNumQueuedConnections() const;
		int GetNumRefusedConnections() const;

		const ServerSocket& socket() const;
		int port() const;

	protected:
		virtual void Run();

		static std::string _ThreadName(const ServerSocket& socket);

	private:
		TCPServer();
		TCPServer(const TCPServer&);
		TCPServer& operator = (const TCPServer&);

		bool mIsSingleThread;

		ServerSocket mSocket;
		TCPServerDispatcher* mDispatcher;
		Thread mThread;
		bool mIsStopped;
		ThreadPoolPtr mThreadPool;
	};


}

#endif