// PX2HTTPHeaderStream.hpp

#ifndef PX2HTTPHEADERSTREAM_HPP
#define PX2HTTPHEADERSTREAM_HPP

#include "PX2HTTPBasicStreamBuf.hpp"
#include "PX2MemoryPool.hpp"
#include "PX2HTTPSession.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM HTTPHeaderStreamBuf : public HTTPBasicStreamBuf
	{
	public:
		typedef HTTPBasicStreamBuf::openmode openmode;

		HTTPHeaderStreamBuf(HTTPSession& session, openmode mode);
		~HTTPHeaderStreamBuf();

	protected:
		int ReadFromDevice(char* buffer, std::streamsize length);
		int WriteToDevice(const char* buffer, std::streamsize length);

	private:
		HTTPSession& mSession;
		bool         _end;
	};


	class PX2_ENGINE_ITEM HTTPHeaderIOS : public virtual std::ios
	{
	public:
		HTTPHeaderIOS(HTTPSession& session, HTTPHeaderStreamBuf::openmode mode);
		~HTTPHeaderIOS();
		HTTPHeaderStreamBuf* rdbuf();

	protected:
		HTTPHeaderStreamBuf _buf;
	};


	class PX2_ENGINE_ITEM HTTPHeaderInputStream : public HTTPHeaderIOS, public std::istream
	{
	public:
		HTTPHeaderInputStream(HTTPSession& session);
		~HTTPHeaderInputStream();

		void* operator new(std::size_t size);
		void operator delete(void* ptr);

	private:
		static MemoryPool mPool;
	};


	class PX2_ENGINE_ITEM HTTPHeaderOutputStream : public HTTPHeaderIOS, public std::ostream
	{
	public:
		HTTPHeaderOutputStream(HTTPSession& session);
		~HTTPHeaderOutputStream();

		void* operator new(std::size_t size);
		void operator delete(void* ptr);

	private:
		static MemoryPool mPool;
	};

}

#endif