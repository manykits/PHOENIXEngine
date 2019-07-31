// PX2Socket.cpp

#include "PX2Socket.hpp"
#include "PX2NetDefine.hpp"
#include "PX2StreamSocketImpl.hpp"
#include "PX2Log.hpp"
#include "PX2NetError.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Socket::Socket() :
mImpl(new0 StreamSocketImpl())
{
}
//----------------------------------------------------------------------------
Socket::Socket (SocketImpl *impl) :
mImpl(impl)
{
	assertion(0!=mImpl, "_pImpl must not be 0.\n");
}
//----------------------------------------------------------------------------
Socket::Socket(const Socket& socket) :
mImpl(socket.mImpl)
{
		assertion(0!=mImpl, "mImpl must not be 0.\n");
}
//----------------------------------------------------------------------------
Socket::Socket(px2_socket_t socket) :
mImpl(new0 StreamSocketImpl(socket))
{
}
//----------------------------------------------------------------------------
Socket& Socket::operator = (const Socket& socket)
{
	if (&socket != this)
	{
		mImpl = socket.mImpl;
	}

	return *this;
}
//----------------------------------------------------------------------------
Socket::~Socket()
{
	mImpl = 0;
}
//----------------------------------------------------------------------------
void Socket::Close()
{
	if (mImpl)
		mImpl->Close();
}
//----------------------------------------------------------------------------
SocketAddress Socket::GetPeerAddress() const
{
	return mImpl->GetPeerAddress();
}
//----------------------------------------------------------------------------
SocketAddress Socket::GetAddress() const
{
	return mImpl->GetAddress();
}
//----------------------------------------------------------------------------
bool Socket::IsValid() const
{
	if (!mImpl)
		return false;

	return PX2_INVALID_SOCKET != mImpl->GetSocket();
}
//----------------------------------------------------------------------------
void Socket::SetNoDelay(bool flag)
{
	mImpl->SetNoDelay(flag);
}
//----------------------------------------------------------------------------
void Socket::SetSendTimeout(const Timespan& timeout)
{
	mImpl->SetSendTimeout(timeout);
}
//----------------------------------------------------------------------------
void Socket::SetReceiveTimeout(const Timespan& timeout)
{
	mImpl->SetReceiveTimeout(timeout);
}
//----------------------------------------------------------------------------
bool Socket::Poll(const Timespan& timeout, int mode) const
{
	return mImpl->Poll(timeout, mode);
}
//----------------------------------------------------------------------------
int Socket::Select (SocketList& readList, SocketList& writeList, 
	SocketList &exceptList, const Timespan& timeout)
{
	fd_set fdRead;
	fd_set fdWrite;
	fd_set fdExcept;
	int nfd = 0;
	FD_ZERO(&fdRead);
	for (SocketList::const_iterator it = readList.begin(); it != readList.end(); ++it)
	{
		px2_socket_t fd = it->GetSocket();
		if (fd != PX2_INVALID_SOCKET)
		{
			if (int(fd) > nfd)
				nfd = int(fd);
			FD_SET(fd, &fdRead);
		}
	}
	FD_ZERO(&fdWrite);
	for (SocketList::const_iterator it = writeList.begin(); it != writeList.end(); ++it)
	{
		px2_socket_t fd = it->GetSocket();
		if (fd != PX2_INVALID_SOCKET)
		{
			if (int(fd) > nfd)
				nfd = int(fd);
			FD_SET(fd, &fdWrite);
		}
	}
	FD_ZERO(&fdExcept);
	for (SocketList::const_iterator it = exceptList.begin(); it != exceptList.end(); ++it)
	{
		px2_socket_t fd = it->GetSocket();
		if (fd != PX2_INVALID_SOCKET)
		{
			if (int(fd) > nfd)
				nfd = int(fd);
			FD_SET(fd, &fdExcept);
		}
	}
	if (nfd == 0) return 0;
	Timespan remainingTime(timeout);
	int rc;
	struct timeval tv;
	tv.tv_sec  = (long) remainingTime.TotalSeconds();
	tv.tv_usec = (long) remainingTime.Useconds();
	Timestamp start;
	rc = ::select(nfd + 1, &fdRead, &fdWrite, &fdExcept, &tv);
	if (rc < 0) 
	{
		PX2_LOG_ERROR("select error");
		NetError::Error();
	}

	SocketList readyReadList;
	for (SocketList::const_iterator it = readList.begin(); it != readList.end(); ++it)
	{
		px2_socket_t fd = it->GetSocket();
		if (fd != PX2_INVALID_SOCKET)
		{
			if (FD_ISSET(fd, &fdRead))
				readyReadList.push_back(*it);
		}
	}
	std::swap(readList, readyReadList);
	SocketList readyWriteList;
	for (SocketList::const_iterator it = writeList.begin(); it != writeList.end(); ++it)
	{
		px2_socket_t fd = it->GetSocket();
		if (fd != PX2_INVALID_SOCKET)
		{
			if (FD_ISSET(fd, &fdWrite))
				readyWriteList.push_back(*it);
		}
	}
	std::swap(writeList, readyWriteList);
	SocketList readyExceptList;
	for (SocketList::const_iterator it = exceptList.begin(); it != exceptList.end(); ++it)
	{
		px2_socket_t fd = it->GetSocket();
		if (fd != PX2_INVALID_SOCKET)
		{
			if (FD_ISSET(fd, &fdExcept))
				readyExceptList.push_back(*it);
		}
	}
	std::swap(exceptList, readyExceptList);	
	return rc; 
}
//----------------------------------------------------------------------------