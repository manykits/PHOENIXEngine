// PX2HTTPSession.cpp

#include "PX2HTTPSession.hpp"
#include "PX2HTTPBufferAllocator.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPSession::HTTPSession() :
_pBuffer(0),
_pCurrent(0),
_pEnd(0),
mIsKeepAlive(false),
mTimeout(HTTP_DEFAULT_TIMEOUT),
_pException(0)
{
}
//----------------------------------------------------------------------------
HTTPSession::HTTPSession(const StreamSocket& socket) :
mSocket(socket),
_pBuffer(0),
_pCurrent(0),
_pEnd(0),
mIsKeepAlive(false),
mTimeout(HTTP_DEFAULT_TIMEOUT),
_pException(0)
{
}
//----------------------------------------------------------------------------
HTTPSession::HTTPSession(const StreamSocket& socket, bool keepAlive) :
mSocket(socket),
_pBuffer(0),
_pCurrent(0),
_pEnd(0),
mIsKeepAlive(keepAlive),
mTimeout(HTTP_DEFAULT_TIMEOUT),
_pException(0)
{
}
//----------------------------------------------------------------------------
HTTPSession::~HTTPSession()
{
	try
	{
		if (_pBuffer) HTTPBufferAllocator::Deallocate(_pBuffer, HTTPBufferAllocator::BUFFER_SIZE);
	}
	catch (...)
	{
		//poco_unexpected();
	}
	try
	{
		close();
	}
	catch (...)
	{
	}
	delete _pException;
}
//----------------------------------------------------------------------------
void HTTPSession::SetKeepAlive(bool keepAlive)
{
	mIsKeepAlive = keepAlive;
}
//----------------------------------------------------------------------------
void HTTPSession::SetTimeout(const Timespan& timeout)
{
	mTimeout = timeout;
}
//----------------------------------------------------------------------------
int HTTPSession::get()
{
	if (_pCurrent == _pEnd)
		refill();

	if (_pCurrent < _pEnd)
		return *_pCurrent++;
	else
		return std::char_traits<char>::eof();
}
//----------------------------------------------------------------------------
int HTTPSession::peek()
{
	if (_pCurrent == _pEnd)
		refill();

	if (_pCurrent < _pEnd)
		return *_pCurrent;
	else
		return std::char_traits<char>::eof();
}
//----------------------------------------------------------------------------
int HTTPSession::read(char* buffer, std::streamsize length)
{
	if (_pCurrent < _pEnd)
	{
		int n = (int)(_pEnd - _pCurrent);
		if (n > length) n = (int)length;
		std::memcpy(buffer, _pCurrent, n);
		_pCurrent += n;
		return n;
	}
	else return receive(buffer, (int)length);
}
//----------------------------------------------------------------------------
int HTTPSession::write(const char* buffer, std::streamsize length)
{
	try
	{
		return mSocket.SendBytes(buffer, (int)length);
	}
	catch (Exception& exc)
	{
		setException(exc);
		throw;
	}
}
//----------------------------------------------------------------------------
int HTTPSession::receive(char* buffer, int length)
{
	try
	{
		return mSocket.ReceiveBytes(buffer, length);
	}
	catch (Exception& exc)
	{
		setException(exc);
		throw;
	}
}
//----------------------------------------------------------------------------
void HTTPSession::refill()
{
	if (!_pBuffer)
	{
		_pBuffer = HTTPBufferAllocator::Allocate(HTTPBufferAllocator::BUFFER_SIZE);
	}
	_pCurrent = _pEnd = _pBuffer;
	int n = receive(_pBuffer, HTTPBufferAllocator::BUFFER_SIZE);
	_pEnd += n;
}
//----------------------------------------------------------------------------
bool HTTPSession::connected() const
{
	return mSocket.GetImpl()->IsInitialized();
}
//----------------------------------------------------------------------------
void HTTPSession::connect(const SocketAddress& address)
{
	mSocket.ConnectB(address, mTimeout);
	mSocket.SetReceiveTimeout(mTimeout);
	mSocket.SetNoDelay(true);
	// There may be leftover data from a previous (failed) request in the buffer,
	// so we clear it.
	_pCurrent = _pEnd = _pBuffer;
}
//----------------------------------------------------------------------------
void HTTPSession::abort()
{
	mSocket.Shutdown();
	close();
}
//----------------------------------------------------------------------------
void HTTPSession::close()
{
	mSocket.Close();
}
//----------------------------------------------------------------------------
void HTTPSession::setException(const Exception& exc)
{
	delete _pException;
	_pException = exc.clone();
}
//----------------------------------------------------------------------------
void HTTPSession::clearException()
{
	delete _pException;
	_pException = 0;
}
//----------------------------------------------------------------------------
StreamSocket HTTPSession::DetachSocket()
{
	StreamSocket oldSocket(mSocket);
	StreamSocket newSocket;
	mSocket = newSocket;
	return oldSocket;
}
//----------------------------------------------------------------------------
StreamSocket& HTTPSession::GetSocket()
{
	return mSocket;
}
//----------------------------------------------------------------------------
void HTTPSession::drainBuffer(DataBuffer<char>& buffer)
{
	buffer.assign(_pCurrent, static_cast<std::size_t>(_pEnd - _pCurrent));
	_pCurrent = _pEnd;
}
//----------------------------------------------------------------------------
void HTTPSession::attachSocket(const StreamSocket& socket)
{
	mSocket = socket;
}
//----------------------------------------------------------------------------
void HTTPSession::AttachSessionData(const Any& data)
{
	_data = data;
}
//----------------------------------------------------------------------------
bool HTTPSession::GetKeepAlive() const
{
	return mIsKeepAlive;
}
//----------------------------------------------------------------------------
Timespan HTTPSession::GetTimeout() const
{
	return mTimeout;
}
//----------------------------------------------------------------------------
const Exception* HTTPSession::GetNetworkException() const
{
	return _pException;
}
//----------------------------------------------------------------------------
int HTTPSession::buffered() const
{
	return static_cast<int>(_pEnd - _pCurrent);
}
//----------------------------------------------------------------------------
const Any& HTTPSession::GetSessionData() const
{
	return _data;
}
//----------------------------------------------------------------------------