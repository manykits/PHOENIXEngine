// PX2TCPServer.cpp

#include "PX2TCPServer.hpp"
#include "PX2System.hpp"
#include "PX2NetException.hpp"
#include "PX2ErrorHandler.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
TCPServer::TCPServer(TCPServerConnectionFactory *factory, int portNumber,
	TCPServerParams *params) :
mSocket(ServerSocket((int16_t)portNumber)),
mThread(_ThreadName(mSocket)),
mIsStopped(true),
mIsSingleThread(false)
{
	mThreadPool = new0 ThreadPool();

	if (params)
	{
		int toAdd = params->GetNumMaxThreads() - mThreadPool->Capacity();
		if (toAdd > 0) mThreadPool->AddCapacity(toAdd);
	}
	mDispatcher = new0 TCPServerDispatcher(factory, mThreadPool, params);
}
//----------------------------------------------------------------------------
TCPServer::TCPServer(TCPServerConnectionFactory *factory, 
	const ServerSocket &socket, TCPServerParams *params) :
mSocket(socket),
mThread(_ThreadName(socket)),
mIsStopped(true),
mIsSingleThread(false)
{
	mThreadPool = new0 ThreadPool();
	if (params)
	{
		int toAdd = params->GetNumMaxThreads() - mThreadPool->Capacity();
		if (toAdd > 0) mThreadPool->AddCapacity(toAdd);
	}
	mDispatcher = new0 TCPServerDispatcher(factory, mThreadPool, params);
}
//----------------------------------------------------------------------------
TCPServer::TCPServer(TCPServerConnectionFactory *factory, ThreadPool *threadPool, 
	const ServerSocket& socket, TCPServerParams *params) :
mSocket(socket),
mDispatcher(new0 TCPServerDispatcher(factory, threadPool, params)),
mThread(_ThreadName(socket)),
mIsStopped(true),
mIsSingleThread(false)
{
}
//----------------------------------------------------------------------------
TCPServer::~TCPServer()
{
	Stop();
	delete0(mDispatcher);
	mDispatcher = 0;
	mThreadPool = 0;
}
//----------------------------------------------------------------------------
const TCPServerParams& TCPServer::params() const
{
	return *mDispatcher->params();
}
//----------------------------------------------------------------------------
TCPServerDispatcher* TCPServer::GetDispatcher()
{
	return mDispatcher;
}
//----------------------------------------------------------------------------
void TCPServer::SetSingleThread(bool single)
{
	mIsSingleThread = single;
}
//----------------------------------------------------------------------------
void TCPServer::SingleThreadUpdate()
{

}
//----------------------------------------------------------------------------
void TCPServer::Start()
{
	mIsStopped = false;	
	mThread.Start(*this);
}
//----------------------------------------------------------------------------
void TCPServer::Stop()
{
	if (!mIsStopped)
	{
		mIsStopped = true;

		System::SleepSeconds(0.2f);
		mThread.Join();
		mDispatcher->Stop();

		mThreadPool->StopAll();
	}
}
//----------------------------------------------------------------------------
void TCPServer::Run()
{
	while (!mIsStopped)
	{
		Timespan timeout(250000);
		try
		{
			if (mSocket.Poll(timeout, Socket::SELECT_READ))
			{
				try
				{

					StreamSocket ss = mSocket.AcceptConnection();

					// enabe nodelay per default: OSX really needs that
					ss.SetNoDelay(true);

					mDispatcher->Enqueue(ss);
				}
				catch (Exception& exc)
				{
					ErrorHandler::Handle(exc);
				}
				catch (std::exception& exc)
				{
					ErrorHandler::Handle(exc);
				}
				catch (...)
				{
					ErrorHandler::Handle();
				}
			}
		}
		catch (Exception& exc)
		{
			ErrorHandler::Handle(exc);
			// possibly a resource issue since poll() failed;
			// give some time to recover before trying again
			System::SleepSeconds(0.05f);
		}
	}
}
//----------------------------------------------------------------------------
int TCPServer::CurrentThreads() const
{
	return mDispatcher->CurrentThreads();
}
//----------------------------------------------------------------------------
int TCPServer::GetNumMaxThreads() const
{
	return mDispatcher->GetNumMaxThreads();
}
//----------------------------------------------------------------------------
int TCPServer::GetNumTotalConnections() const
{
	return mDispatcher->GetNumTotalConnections();
}
//----------------------------------------------------------------------------
int TCPServer::GetNumCurrentConnections() const
{
	return mDispatcher->GetNumCurrentConnections();
}
//----------------------------------------------------------------------------
int TCPServer::GetNumMaxConcurrentConnections() const
{
	return mDispatcher->GetNumMaxConcurrentConnections();
}
//----------------------------------------------------------------------------
int TCPServer::GetNumQueuedConnections() const
{
	return mDispatcher->GetNumQueuedConnections();
}
//----------------------------------------------------------------------------
int TCPServer::GetNumRefusedConnections() const
{
	return mDispatcher->GetNumRefusedConnections();
}
//----------------------------------------------------------------------------
std::string TCPServer::_ThreadName(const ServerSocket& socket)
{
	std::string name("TCPServer: ");
	name.append(socket.GetAddress().ToString());
	
	return name;
}
//----------------------------------------------------------------------------