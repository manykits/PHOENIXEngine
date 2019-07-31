// PX2HTTPStream.cpp

#include "PX2HTTPStream.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPStreamBuf::HTTPStreamBuf(HTTPSession& session, openmode mode) :
HTTPBasicStreamBuf(HTTPBufferAllocator::BUFFER_SIZE, mode),
mSession(session),
_mode(mode)
{
}
//----------------------------------------------------------------------------
HTTPStreamBuf::~HTTPStreamBuf()
{
}
//----------------------------------------------------------------------------
void HTTPStreamBuf::close()
{
	if (_mode & std::ios::out)
	{
		sync();
		mSession.GetSocket().ShutdownSend();
	}
}
//----------------------------------------------------------------------------
int HTTPStreamBuf::ReadFromDevice(char* buffer, std::streamsize length)
{
	return mSession.read(buffer, length);
}
//----------------------------------------------------------------------------
int HTTPStreamBuf::WriteToDevice(const char* buffer, std::streamsize length)
{

	return mSession.write(buffer, length);
}
//----------------------------------------------------------------------------
HTTPIOS::HTTPIOS(HTTPSession& session, HTTPStreamBuf::openmode mode) :
_buf(session, mode)
{
	//poco_ios_init(&_buf);
}
//----------------------------------------------------------------------------
HTTPIOS::~HTTPIOS()
{
	try
	{
		_buf.close();
	}
	catch (...)
	{
	}
}
//----------------------------------------------------------------------------
HTTPStreamBuf* HTTPIOS::rdbuf()
{
	return &_buf;
}
//----------------------------------------------------------------------------
MemoryPool HTTPInputStream::mPool(sizeof(HTTPInputStream));
//----------------------------------------------------------------------------
HTTPInputStream::HTTPInputStream(HTTPSession& session) :
HTTPIOS(session, std::ios::in),
std::istream(&_buf)
{
}
//----------------------------------------------------------------------------
HTTPInputStream::~HTTPInputStream()
{
}
//----------------------------------------------------------------------------
void* HTTPInputStream::operator new(std::size_t size)
{
	PX2_UNUSED(size);

	return mPool.Get();
}
//----------------------------------------------------------------------------
void HTTPInputStream::operator delete(void* ptr)
{
	try
	{
		mPool.Release(ptr);
	}
	catch (...)
	{
		//poco_unexpected();
	}
}
//----------------------------------------------------------------------------
MemoryPool HTTPOutputStream::mPool(sizeof(HTTPOutputStream));
//----------------------------------------------------------------------------
HTTPOutputStream::HTTPOutputStream(HTTPSession& session) :
HTTPIOS(session, std::ios::out),
std::ostream(&_buf)
{
}
//----------------------------------------------------------------------------
HTTPOutputStream::~HTTPOutputStream()
{
}
//----------------------------------------------------------------------------
void* HTTPOutputStream::operator new(std::size_t size)
{
	PX2_UNUSED(size);

	return mPool.Get();
}
//----------------------------------------------------------------------------
void HTTPOutputStream::operator delete(void* ptr)
{
	try
	{
		mPool.Release(ptr);
	}
	catch (...)
	{
		//poco_unexpected();
	}
}
//----------------------------------------------------------------------------