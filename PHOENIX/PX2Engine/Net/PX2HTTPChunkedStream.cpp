// PX2HTTPChunkedStream.cpp

#include "PX2HTTPChunkedStream.hpp"
#include "PX2NumberFormatter.hpp"
#include "PX2NumberParser.hpp"
#include "PX2Ascii.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPChunkedStreamBuf::HTTPChunkedStreamBuf(HTTPSession& session, openmode mode) :
HTTPBasicStreamBuf(HTTPBufferAllocator::BUFFER_SIZE, mode),
mSession(session),
_mode(mode),
_chunk(0)
{
}
//----------------------------------------------------------------------------
HTTPChunkedStreamBuf::~HTTPChunkedStreamBuf()
{
}
//----------------------------------------------------------------------------
void HTTPChunkedStreamBuf::close()
{
	if (_mode & std::ios::out)
	{
		sync();
		mSession.write("0\r\n\r\n", 5);
	}
}
//----------------------------------------------------------------------------
int HTTPChunkedStreamBuf::ReadFromDevice(char* buffer, std::streamsize length)
{
	static const int eof = std::char_traits<char>::eof();

	if (_chunk == 0)
	{
		int ch = mSession.get();
		while (Ascii::IsSpace(ch)) ch = mSession.get();
		std::string chunkLen;
		while (Ascii::IsHexDigit(ch)) { chunkLen += (char)ch; ch = mSession.get(); }
		while (ch != eof && ch != '\n') ch = mSession.get();
		unsigned chunk;
		if (NumberParser::tryParseHex(chunkLen, chunk))
			_chunk = (std::streamsize) chunk;
		else
			return eof;
	}
	if (_chunk > 0)
	{
		if (length > _chunk) length = _chunk;
		int n = mSession.read(buffer, length);
		if (n > 0) _chunk -= n;
		return n;
	}
	else
	{
		int ch = mSession.get();
		while (ch != eof && ch != '\n') ch = mSession.get();
		return 0;
	}
}
//----------------------------------------------------------------------------
int HTTPChunkedStreamBuf::WriteToDevice(const char* buffer, std::streamsize length)
{
	_chunkBuffer.clear();
	NumberFormatter::appendHex(_chunkBuffer, (int)length);
	_chunkBuffer.append("\r\n", 2);
	_chunkBuffer.append(buffer, static_cast<std::string::size_type>(length));
	_chunkBuffer.append("\r\n", 2);
	mSession.write(_chunkBuffer.data(), static_cast<std::streamsize>(_chunkBuffer.size()));
	return static_cast<int>(length);
}
//----------------------------------------------------------------------------
HTTPChunkedIOS::HTTPChunkedIOS(HTTPSession& session, HTTPChunkedStreamBuf::openmode mode) :
_buf(session, mode)
{
	//poco_ios_init(&_buf);
}
//----------------------------------------------------------------------------
HTTPChunkedIOS::~HTTPChunkedIOS()
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
HTTPChunkedStreamBuf* HTTPChunkedIOS::rdbuf()
{
	return &_buf;
}
//----------------------------------------------------------------------------
MemoryPool HTTPChunkedInputStream::mPool(sizeof(HTTPChunkedInputStream));
//----------------------------------------------------------------------------
HTTPChunkedInputStream::HTTPChunkedInputStream(HTTPSession& session) :
HTTPChunkedIOS(session, std::ios::in),
std::istream(&_buf)
{
}
//----------------------------------------------------------------------------
HTTPChunkedInputStream::~HTTPChunkedInputStream()
{
}
//----------------------------------------------------------------------------
void* HTTPChunkedInputStream::operator new(std::size_t size)
{
	PX2_UNUSED(size);

	return mPool.Get();
}
//----------------------------------------------------------------------------
void HTTPChunkedInputStream::operator delete(void* ptr)
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
MemoryPool HTTPChunkedOutputStream::mPool(sizeof(HTTPChunkedOutputStream));
//----------------------------------------------------------------------------
HTTPChunkedOutputStream::HTTPChunkedOutputStream(HTTPSession& session) :
HTTPChunkedIOS(session, std::ios::out),
std::ostream(&_buf)
{
}
//----------------------------------------------------------------------------
HTTPChunkedOutputStream::~HTTPChunkedOutputStream()
{
}
//----------------------------------------------------------------------------
void* HTTPChunkedOutputStream::operator new(std::size_t size)
{
	PX2_UNUSED(size);

	return mPool.Get();
}
//----------------------------------------------------------------------------
void HTTPChunkedOutputStream::operator delete(void* ptr)
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