// PX2HTTPFixedLengthStream.hpp

#ifndef PX2HTTPFIXEDLENGTHSTREAM_HPP
#define PX2HTTPFIXEDLENGTHSTREAM_HPP

#include "PX2NetPre.hpp"
#include "PX2HTTPBasicStreamBuf.hpp"
#include "PX2MemoryPool.hpp"

namespace PX2
{

	class HTTPSession;

	class PX2_ENGINE_ITEM HTTPFixedLengthStreamBuf : public HTTPBasicStreamBuf
	{
	public:
		typedef HTTPBasicStreamBuf::openmode openmode;
		typedef std::streamsize ContentLength;

		HTTPFixedLengthStreamBuf(HTTPSession& session,
			ContentLength length, openmode mode);
		~HTTPFixedLengthStreamBuf();

	protected:
		int ReadFromDevice(char* buffer, std::streamsize length);
		int WriteToDevice(const char* buffer, std::streamsize length);

	private:
		HTTPSession&    mSession;
		ContentLength _length;
		ContentLength _count;
	};

	class PX2_ENGINE_ITEM HTTPFixedLengthIOS : public virtual std::ios
	{
	public:
		HTTPFixedLengthIOS(HTTPSession& session, 
			HTTPFixedLengthStreamBuf::ContentLength length,
			HTTPFixedLengthStreamBuf::openmode mode);
		~HTTPFixedLengthIOS();
		HTTPFixedLengthStreamBuf* rdbuf();

	protected:
		HTTPFixedLengthStreamBuf _buf;
	};

	class PX2_ENGINE_ITEM HTTPFixedLengthInputStream : 
		public HTTPFixedLengthIOS, public std::istream
	{
	public:
		HTTPFixedLengthInputStream(HTTPSession& session, 
			HTTPFixedLengthStreamBuf::ContentLength length);
		~HTTPFixedLengthInputStream();

		void* operator new(std::size_t size);
		void operator delete(void* ptr);

	private:
		static MemoryPool mPool;
	};

	class PX2_ENGINE_ITEM HTTPFixedLengthOutputStream :
		public HTTPFixedLengthIOS, public std::ostream
	{
	public:
		HTTPFixedLengthOutputStream(HTTPSession& session, 
			HTTPFixedLengthStreamBuf::ContentLength length);
		~HTTPFixedLengthOutputStream();

		void* operator new(std::size_t size);
		void operator delete(void* ptr);

	private:
		static MemoryPool mPool;
	};


}

#endif