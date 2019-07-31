// PX2HTTPChunkedStream.hpp

#ifndef PX2HTTPCHUNKEDSTREAM_HPP
#define PX2HTTPCHUNKEDSTREAM_HPP

#include "PX2HTTPBasicStreamBuf.hpp"
#include "PX2HTTPSession.hpp"
#include "PX2MemoryPool.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM HTTPChunkedStreamBuf : public HTTPBasicStreamBuf
	{
	public:
		typedef HTTPBasicStreamBuf::openmode openmode;

		HTTPChunkedStreamBuf(HTTPSession& session, openmode mode);
		~HTTPChunkedStreamBuf();
		void close();

	protected:
		int ReadFromDevice(char* buffer, std::streamsize length);
		int WriteToDevice(const char* buffer, std::streamsize length);

	private:
		HTTPSession&    mSession;
		openmode        _mode;
		std::streamsize _chunk;
		std::string     _chunkBuffer;
	};


	class PX2_ENGINE_ITEM HTTPChunkedIOS : public virtual std::ios
	{
	public:
		HTTPChunkedIOS(HTTPSession& session, HTTPChunkedStreamBuf::openmode mode);
		~HTTPChunkedIOS();
		HTTPChunkedStreamBuf* rdbuf();

	protected:
		HTTPChunkedStreamBuf _buf;
	};


	class PX2_ENGINE_ITEM HTTPChunkedInputStream : public HTTPChunkedIOS, public std::istream
	{
	public:
		HTTPChunkedInputStream(HTTPSession& session);
		~HTTPChunkedInputStream();

		void* operator new(std::size_t size);
		void operator delete(void* ptr);

	private:
		static MemoryPool mPool;
	};

	class PX2_ENGINE_ITEM HTTPChunkedOutputStream : public HTTPChunkedIOS, public std::ostream
	{
	public:
		HTTPChunkedOutputStream(HTTPSession& session);
		~HTTPChunkedOutputStream();

		void* operator new(std::size_t size);
		void operator delete(void* ptr);

	private:
		static MemoryPool mPool;
	};

}

#endif