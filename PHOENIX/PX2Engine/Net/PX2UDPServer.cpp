// PX2N_UDPServer.cpp

#include "PX2UDPServer.hpp"
#include "PX2Exception.hpp"
#include "PX2Log.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
UDPServer::UDPServer() :
mThread("UDPServer"),
mIsStop(false)
{
	//mSocket.Bind(SocketAddress(), true);
}
//----------------------------------------------------------------------------
UDPServer::UDPServer(const SocketAddress& sa) :
mThread("UDPServer"),
mIsStop(false)
{
	mSocket.Bind(sa, true);
}
//----------------------------------------------------------------------------
UDPServer::~UDPServer()
{
	Stop();
}
//----------------------------------------------------------------------------
void UDPServer::Bind(int port)
{
	SocketAddress addr(port);
	mSocket.Bind(addr, true);
}
//----------------------------------------------------------------------------
void UDPServer::Start()
{
	mThread.Start(*this);
	mReadEvent.Wait();
}
//----------------------------------------------------------------------------
void UDPServer::Stop()
{
	mUDPServerRecvCallbacks.clear();
	mRecvBufs.clear();
	mIsStop = true;
	mThread.Join();
}
//----------------------------------------------------------------------------
DatagramSocket &UDPServer::GetSocket()
{
	return mSocket;
}
//----------------------------------------------------------------------------
int UDPServer::GetPort() const
{
	return mSocket.GetAddress().GetPort();
}
//----------------------------------------------------------------------------
SocketAddress UDPServer::GetAddress() const
{
	return mSocket.GetAddress();
}
//----------------------------------------------------------------------------
void UDPServer::Update(float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

	ScopedCS cs(&mMutex);
	for (int i = 0; i < (int)mRecvBufs.size(); i++)
	{
		std::pair<SocketAddress, UDPRecvObj> &item = mRecvBufs[i];
		SocketAddress &sktAddr = item.first;

		std::string strBuf = item.second.Buffer;
		int sz = item.second.Size;
		OnRecv(sktAddr, strBuf, sz);
	}

	mRecvBufs.clear();
}
//----------------------------------------------------------------------------
bool UDPServer::IsHasRecvCallback(UDPServerRecvCallback callback)
{
	for (int i = 0; i < (int)mUDPServerRecvCallbacks.size(); i++)
	{
		if (callback == mUDPServerRecvCallbacks[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UDPServer::AddRecvCallback(UDPServerRecvCallback callback)
{
	mUDPServerRecvCallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
void UDPServer::OnRecv(SocketAddress &address, const std::string &buf,
	int length)
{
	std::string ip = address.GetHost().ToString();

	for (int i = 0; i < (int)mUDPServerRecvCallbacks.size(); i++)
	{
		UDPServerRecvCallback callBack = mUDPServerRecvCallbacks[i];
		if (callBack)
		{
			callBack(this, address, buf, length);
		}
	}

	for (int i = 0; i < (int)mOnRecvCallbacks.size(); i++)
	{
		const std::string &callback = mOnRecvCallbacks[i];
		if (!callback.empty())
		{
			std::string paramStr = ip + "&" + buf;

			PX2_SC_LUA->CallFunction(callback, this, paramStr);
		}
	}
}
//----------------------------------------------------------------------------
bool UDPServer::IsHasOnRecvCallback(const std::string &callback) const
{
	for (int i = 0; i < (int)mOnRecvCallbacks.size(); i++)
	{
		if (callback == mOnRecvCallbacks[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool UDPServer::AddOnRecvCallback(const std::string &callback)
{
	if (IsHasOnRecvCallback(callback))
		return false;

	mOnRecvCallbacks.push_back(callback);

	return true;
}
//----------------------------------------------------------------------------
bool UDPServer::RemoveOnRecvCallback(const std::string &callback)
{
	auto it = mOnRecvCallbacks.begin();
	for (; it != mOnRecvCallbacks.end(); it++)
	{
		if (*it == callback)
		{
			mOnRecvCallbacks.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void UDPServer::Run()
{
	Timespan span(250000);
	while (!mIsStop)
	{
		mReadEvent.Set();
		if (mSocket.Poll(span, Socket::SELECT_READ))
		{
			char buffer[65535];
			SocketAddress sender;
			int n = mSocket.ReceiveFrom(buffer, sizeof(buffer), sender);
			if (n > 0)
			{
				UDPRecvObj obj;
				obj.Size = n;
				obj.Buffer = std::string(buffer, n);

				ScopedCS cs(&mMutex);
				mRecvBufs.push_back(
					std::pair<SocketAddress, UDPRecvObj>(sender, obj));
			}
		}
	}
}
//----------------------------------------------------------------------------