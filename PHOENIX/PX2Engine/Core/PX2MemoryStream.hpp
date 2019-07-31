// PX2MemoryStream.hpp

#ifndef PX2MEMORYSTREAM_HPP
#define PX2MEMORYSTREAM_HPP

#include "PX2CorePre.hpp"
#include "PX2Assert.hpp"

namespace PX2 {


	template <typename ch, typename tr>
	class BasicMemoryStreamBuf : public std::basic_streambuf<ch, tr>
	{
	protected:
		typedef std::basic_streambuf<ch, tr> Base;
		typedef std::basic_ios<ch, tr> IOS;
		typedef ch char_type;
		typedef tr char_traits;
		typedef typename Base::int_type int_type;
		typedef typename Base::pos_type pos_type;
		typedef typename Base::off_type off_type;

	public:
		BasicMemoryStreamBuf(char_type* pBuffer, std::streamsize bufferSize) :
			_pBuffer(pBuffer),
			_bufferSize(bufferSize)
		{
			this->setg(_pBuffer, _pBuffer, _pBuffer + _bufferSize);
			this->setp(_pBuffer, _pBuffer + _bufferSize);
		}

		~BasicMemoryStreamBuf()
		{
		}

		virtual int_type overflow(int_type /*c*/)
		{
			return char_traits::eof();
		}

		virtual int_type underflow()
		{
			return char_traits::eof();
		}

		virtual pos_type seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
		{
			const pos_type fail = off_type(-1);
			off_type newoff = off_type(-1);

			if ((which & std::ios_base::in) != 0)
			{
				if (this->gptr() == 0)
					return fail;

				if (way == std::ios_base::beg)
				{
					newoff = 0;
				}
				else if (way == std::ios_base::cur)
				{
					// cur is not valid if both in and out are specified (Condition 3)
					if ((which & std::ios_base::out) != 0)
						return fail;
					newoff = this->gptr() - this->eback();
				}
				else if (way == std::ios_base::end)
				{
					newoff = this->egptr() - this->eback();
				}
				else
				{
					assertion(false, "");
				}

				if ((newoff + off) < 0 || (this->egptr() - this->eback()) < (newoff + off))
					return fail;
				this->setg(this->eback(), this->eback() + newoff + off, this->egptr());
			}

			if ((which & std::ios_base::out) != 0)
			{
				if (this->pptr() == 0)
					return fail;

				if (way == std::ios_base::beg)
				{
					newoff = 0;
				}
				else if (way == std::ios_base::cur)
				{
					// cur is not valid if both in and out are specified (Condition 3)
					if ((which & std::ios_base::in) != 0)
						return fail;
					newoff = this->pptr() - this->pbase();
				}
				else if (way == std::ios_base::end)
				{
					newoff = this->epptr() - this->pbase();
				}
				else
				{
					assertion(false, "");
				}

				if (newoff + off < 0 || (this->epptr() - this->pbase()) < newoff + off)
					return fail;
				this->pbump((int)(newoff + off - (this->pptr() - this->pbase())));
			}

			return newoff;
		}

		virtual int sync()
		{
			return 0;
		}

		std::streamsize charsWritten() const
		{
			return static_cast<std::streamsize>(this->pptr() - this->pbase());
		}

		void reset()
			/// Resets the buffer so that current read and write positions
			/// will be set to the beginning of the buffer.
		{
			this->setg(_pBuffer, _pBuffer, _pBuffer + _bufferSize);
			this->setp(_pBuffer, _pBuffer + _bufferSize);
		}

	private:
		char_type*      _pBuffer;
		std::streamsize _bufferSize;
	};

	typedef BasicMemoryStreamBuf<char, std::char_traits<char> > MemoryStreamBuf;

	class PX2_ENGINE_ITEM MemoryIOS : public virtual std::ios
	{
	public:
		MemoryIOS(char* pBuffer, std::streamsize bufferSize);
		~MemoryIOS();

		MemoryStreamBuf* rdbuf();

	protected:
		MemoryStreamBuf _buf;
	};

	class PX2_ENGINE_ITEM MemoryInputStream : public MemoryIOS, public std::istream
	{
	public:
		MemoryInputStream(const char* pBuffer, std::streamsize bufferSize);
		~MemoryInputStream();
	};

	class PX2_ENGINE_ITEM MemoryOutputStream : public MemoryIOS, public std::ostream
	{
	public:
		MemoryOutputStream(char* pBuffer, std::streamsize bufferSize);
		~MemoryOutputStream();

		std::streamsize charsWritten() const;
	};

	inline MemoryStreamBuf* MemoryIOS::rdbuf()
	{
		return &_buf;
	}

	inline std::streamsize MemoryOutputStream::charsWritten() const
	{
		return _buf.charsWritten();
	}

}

#endif