// PX2HTTPHeaderStream.cpp

#include "PX2HTTPHeaderStream.hpp"
using namespace PX2;


//----------------------------------------------------------------------------
HTTPHeaderStreamBuf::HTTPHeaderStreamBuf(HTTPSession& session, openmode mode) :
HTTPBasicStreamBuf(HTTPBufferAllocator::BUFFER_SIZE, mode),
mSession(session),
_end(false)
{
}
//----------------------------------------------------------------------------
HTTPHeaderStreamBuf::~HTTPHeaderStreamBuf()
{
}
//----------------------------------------------------------------------------
int HTTPHeaderStreamBuf::ReadFromDevice(char* buffer, std::streamsize length)
{
	// read line-by-line; an empty line denotes the end of the headers.
	static const int eof = std::char_traits<char>::eof();

	if (_end) return 0;

	int n = 0;
	int ch = mSession.get();
	while (ch != eof && ch != '\n' && n < length - 1)
	{
		*buffer++ = (char)ch; ++n;
		ch = mSession.get();
	}
	if (ch != eof)
	{
		*buffer++ = (char)ch; ++n;
		if (n == 2) _end = true;
	}
	return n;
}
//----------------------------------------------------------------------------
int HTTPHeaderStreamBuf::WriteToDevice(const char* buffer, std::streamsize length)
{
	return mSession.write(buffer, length);
}
//----------------------------------------------------------------------------
HTTPHeaderIOS::HTTPHeaderIOS(HTTPSession& session, HTTPHeaderStreamBuf::openmode mode) :
_buf(session, mode)
{
	//poco_ios_init(&_buf);
}
//----------------------------------------------------------------------------
HTTPHeaderIOS::~HTTPHeaderIOS()
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
HTTPHeaderStreamBuf* HTTPHeaderIOS::rdbuf()
{
	return &_buf;
}
//----------------------------------------------------------------------------
MemoryPool HTTPHeaderInputStream::mPool(sizeof(HTTPHeaderInputStream));
//----------------------------------------------------------------------------
HTTPHeaderInputStream::HTTPHeaderInputStream(HTTPSession& session) :
HTTPHeaderIOS(session, std::ios::in),
std::istream(&_buf)
{
}
//----------------------------------------------------------------------------
HTTPHeaderInputStream::~HTTPHeaderInputStream()
{
}
//----------------------------------------------------------------------------
void* HTTPHeaderInputStream::operator new(std::size_t size)
{
	PX2_UNUSED(size);

	return mPool.Get();
}
//----------------------------------------------------------------------------
void HTTPHeaderInputStream::operator delete(void* ptr)
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
MemoryPool HTTPHeaderOutputStream::mPool(sizeof(HTTPHeaderOutputStream));
//----------------------------------------------------------------------------
HTTPHeaderOutputStream::HTTPHeaderOutputStream(HTTPSession& session) :
HTTPHeaderIOS(session, std::ios::out),
std::ostream(&_buf)
{
}
//----------------------------------------------------------------------------
HTTPHeaderOutputStream::~HTTPHeaderOutputStream()
{
}
//----------------------------------------------------------------------------
void* HTTPHeaderOutputStream::operator new(std::size_t size)
{
	PX2_UNUSED(size);

	return mPool.Get();
}
//----------------------------------------------------------------------------
void HTTPHeaderOutputStream::operator delete(void* ptr)
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