// PX2HTTPStream.hpp

#ifndef PX2HTTPSTREAM_HPP
#define PX2HTTPSTREAM_HPP

#include "PX2HTTPBasicStreamBuf.hpp"
#include "PX2MemoryPool.hpp"
#include "PX2HTTPSession.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM HTTPStreamBuf : public HTTPBasicStreamBuf
	{
	public:
		typedef HTTPBasicStreamBuf::openmode openmode;

		HTTPStreamBuf(HTTPSession& session, openmode mode);
		~HTTPStreamBuf();
		void close();

	protected:
		int ReadFromDevice(char* buffer, std::streamsize length);
		int WriteToDevice(const char* buffer, std::streamsize length);

	private:
		HTTPSession& mSession;
		openmode     _mode;
	};


	class PX2_ENGINE_ITEM HTTPIOS : public virtual std::ios
	{
	public:
		HTTPIOS(HTTPSession& session, HTTPStreamBuf::openmode mode);
		~HTTPIOS();
		HTTPStreamBuf* rdbuf();

	protected:
		HTTPStreamBuf _buf;
	};


	class PX2_ENGINE_ITEM HTTPInputStream : public HTTPIOS, public std::istream
	{
	public:
		HTTPInputStream(HTTPSession& session);
		~HTTPInputStream();

		void* operator new(std::size_t size);
		void operator delete(void* ptr);

	private:
		static MemoryPool mPool;
	};


	class PX2_ENGINE_ITEM HTTPOutputStream : public HTTPIOS, public std::ostream
	{
	public:
		HTTPOutputStream(HTTPSession& session);
		~HTTPOutputStream();

		void* operator new(std::size_t size);
		void operator delete(void* ptr);

	private:
		static MemoryPool mPool;
	};

}

#endif