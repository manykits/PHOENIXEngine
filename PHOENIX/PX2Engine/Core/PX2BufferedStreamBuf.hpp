// PX2BufferedStreamBuf.hpp

#ifndef PX2BUFFEREDSTREAMBUF_HPP
#define PX2BUFFEREDSTREAMBUF_HPP

#include "PX2CorePre.hpp"
#include "PX2BufferAllocator.hpp"

namespace PX2
{

	template <typename ch, typename tr, typename ba = BufferAllocator<ch> >
	class BasicBufferedStreamBuf : public std::basic_streambuf<ch, tr>
	{
	protected:
		typedef std::basic_streambuf<ch, tr> Base;
		typedef std::basic_ios<ch, tr> IOS;
		typedef ch char_type;
		typedef tr char_traits;
		typedef ba BufferAllocator;
		typedef typename Base::int_type int_type;
		typedef typename Base::pos_type pos_type;
		typedef typename Base::off_type off_type;
		typedef typename IOS::openmode openmode;

	public:
		BasicBufferedStreamBuf(std::streamsize bufferSize, openmode mode) :
			_bufsize(bufferSize),
			_pBuffer(BufferAllocator::Allocate(_bufsize)),
			_mode(mode)
		{
			this->setg(_pBuffer + 4, _pBuffer + 4, _pBuffer + 4);
			this->setp(_pBuffer, _pBuffer + (_bufsize - 1));
		}

		~BasicBufferedStreamBuf()
		{
			BufferAllocator::Deallocate(_pBuffer, _bufsize);
		}

		virtual int_type overflow(int_type c)
		{
			if (!(_mode & IOS::out)) return char_traits::eof();

			if (c != char_traits::eof())
			{
				*this->pptr() = char_traits::to_char_type(c);
				this->pbump(1);
			}
			if (flushBuffer() == std::streamsize(-1)) return char_traits::eof();

			return c;
		}

		virtual int_type underflow()
		{
			if (!(_mode & IOS::in)) return char_traits::eof();

			if (this->gptr() && (this->gptr() < this->egptr()))
				return char_traits::to_int_type(*this->gptr());

			int putback = int(this->gptr() - this->eback());
			if (putback > 4) putback = 4;

			char_traits::move(_pBuffer + (4 - putback), this->gptr() - putback, putback);

			int n = ReadFromDevice(_pBuffer + 4, _bufsize - 4);
			if (n <= 0) return char_traits::eof();

			this->setg(_pBuffer + (4 - putback), _pBuffer + 4, _pBuffer + 4 + n);

			// return next character
			return char_traits::to_int_type(*this->gptr());
		}

		virtual int sync()
		{
			if (this->pptr() && this->pptr() > this->pbase())
			{
				if (flushBuffer() == -1) return -1;
			}
			return 0;
		}

	protected:
		void setMode(openmode mode)
		{
			_mode = mode;
		}

		openmode getMode() const
		{
			return _mode;
		}

	private:
		virtual int ReadFromDevice(char_type* /*buffer*/, std::streamsize /*length*/)
		{
			return 0;
		}

		virtual int WriteToDevice(const char_type* /*buffer*/, std::streamsize /*length*/)
		{
			return 0;
		}

		int flushBuffer()
		{
			int n = int(this->pptr() - this->pbase());
			if (WriteToDevice(this->pbase(), n) == n)
			{
				this->pbump(-n);
				return n;
			}
			return -1;
		}

		std::streamsize _bufsize;
		char_type*      _pBuffer;
		openmode        _mode;

		BasicBufferedStreamBuf(const BasicBufferedStreamBuf&);
		BasicBufferedStreamBuf& operator = (const BasicBufferedStreamBuf&);
	};

	typedef BasicBufferedStreamBuf<char, std::char_traits<char> > BufferedStreamBuf;

}

#endif