// PX2HTTPFixedLengthStream.cpp

#include "PX2HTTPFixedLengthStream.hpp"
#include "PX2HTTPSession.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPFixedLengthStreamBuf::HTTPFixedLengthStreamBuf(HTTPSession& session, 
	ContentLength length, openmode mode) :
HTTPBasicStreamBuf(HTTPBufferAllocator::BUFFER_SIZE, mode),
mSession(session),
_length(length),
_count(0)
{
}
//----------------------------------------------------------------------------
HTTPFixedLengthStreamBuf::~HTTPFixedLengthStreamBuf()
{
}
//----------------------------------------------------------------------------
int HTTPFixedLengthStreamBuf::ReadFromDevice(char* buffer, std::streamsize length)
{
	int n = 0;
	if (_count < _length)
	{
		if (_count + length > _length)
			length = static_cast<std::streamsize>(_length - _count);
		n = mSession.read(buffer, length);
		if (n > 0) _count += n;
	}
	return n;
}
//----------------------------------------------------------------------------
int HTTPFixedLengthStreamBuf::WriteToDevice(const char* buffer, std::streamsize length)
{
	int n = 0;
	if (_count < _length)
	{
		if (_count + length > _length)
			length = static_cast<std::streamsize>(_length - _count);
		n = mSession.write(buffer, length);
		if (n > 0) _count += n;
	}
	return n;
}
//----------------------------------------------------------------------------
HTTPFixedLengthIOS::HTTPFixedLengthIOS(HTTPSession& session,
	HTTPFixedLengthStreamBuf::ContentLength length, HTTPFixedLengthStreamBuf::openmode mode) :
_buf(session, length, mode)
{
	//poco_ios_init(&_buf);
}
//----------------------------------------------------------------------------
HTTPFixedLengthIOS::~HTTPFixedLengthIOS()
{
	try
	{
		_buf.sync();
	}
	catch (...)
	{
	}
}
//----------------------------------------------------------------------------
HTTPFixedLengthStreamBuf* HTTPFixedLengthIOS::rdbuf()
{
	return &_buf;
}
//----------------------------------------------------------------------------
MemoryPool HTTPFixedLengthInputStream::mPool(sizeof(HTTPFixedLengthInputStream));
//----------------------------------------------------------------------------
HTTPFixedLengthInputStream::HTTPFixedLengthInputStream(HTTPSession& session,
	HTTPFixedLengthStreamBuf::ContentLength length) :
HTTPFixedLengthIOS(session, length, std::ios::in),
std::istream(&_buf)
{
}
//----------------------------------------------------------------------------
HTTPFixedLengthInputStream::~HTTPFixedLengthInputStream()
{
}
//----------------------------------------------------------------------------
void* HTTPFixedLengthInputStream::operator new(std::size_t size)
{
	PX2_UNUSED(size);
	return mPool.Get();
}
//----------------------------------------------------------------------------
void HTTPFixedLengthInputStream::operator delete(void* ptr)
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
MemoryPool HTTPFixedLengthOutputStream::mPool(sizeof(HTTPFixedLengthOutputStream));
//----------------------------------------------------------------------------
HTTPFixedLengthOutputStream::HTTPFixedLengthOutputStream(HTTPSession& session,
	HTTPFixedLengthStreamBuf::ContentLength length) :
HTTPFixedLengthIOS(session, length, std::ios::out),
std::ostream(&_buf)
{
}
//----------------------------------------------------------------------------
HTTPFixedLengthOutputStream::~HTTPFixedLengthOutputStream()
{
}
//----------------------------------------------------------------------------
void* HTTPFixedLengthOutputStream::operator new(std::size_t size)
{
	PX2_UNUSED(size);
	return mPool.Get();
}
//----------------------------------------------------------------------------
void HTTPFixedLengthOutputStream::operator delete(void* ptr)
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