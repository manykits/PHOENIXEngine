// PX2BinaryWriter.hpp

#ifndef PX2BINARYWRITER_HPP
#define PX2BINARYWRITER_HPP

#include "PX2CorePre.hpp"
#include "PX2DataBuffer.hpp"
#include "PX2MemoryStream.hpp"

namespace PX2
{

	class TextEncoding;
	class TextConverter;

	class PX2_ENGINE_ITEM BinaryWriter
	{
	public:
		enum StreamByteOrder
		{
			NATIVE_BYTE_ORDER = 1, /// the host's native byte-order
			BIG_ENDIAN_BYTE_ORDER = 2, /// big-endian (network) byte-order
			NETWORK_BYTE_ORDER = 2, /// big-endian (network) byte-order
			LITTLE_ENDIAN_BYTE_ORDER = 3  /// little-endian byte-order
		};

		BinaryWriter(std::ostream& ostr, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
		BinaryWriter(std::ostream& ostr, TextEncoding& encoding, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
		~BinaryWriter();
		/// Destroys the BinaryWriter.

		BinaryWriter& operator << (bool value);
		BinaryWriter& operator << (char value);
		BinaryWriter& operator << (unsigned char value);
		BinaryWriter& operator << (signed char value);
		BinaryWriter& operator << (short value);
		BinaryWriter& operator << (unsigned short value);
		BinaryWriter& operator << (int value);
		BinaryWriter& operator << (unsigned int value);
		BinaryWriter& operator << (long value);
		BinaryWriter& operator << (unsigned long value);
		BinaryWriter& operator << (float value);
		BinaryWriter& operator << (double value);

#if defined(PX2_HAVE_INT64) && !defined(PX2_LONG_IS_64_BIT)
		BinaryWriter& operator << (int64_t value);
		BinaryWriter& operator << (uint64_t value);
#endif

		BinaryWriter& operator << (const std::string& value);
		BinaryWriter& operator << (const char* value);

		template <typename T>
		BinaryWriter& operator << (const std::vector<T>& value)
		{
			uint32_t size(static_cast<uint32_t>(value.size()));

			*this << size;
			for (typename std::vector<T>::const_iterator it = value.begin(); 
				it != value.end(); ++it)
			{
				*this << *it;
			}

			return *this;
		}

		void write7BitEncoded(uint32_t value);
#if defined(PX2_HAVE_INT64)
		void write7BitEncoded(uint64_t value);
#endif

		void writeRaw(const std::string& rawData);
		/// Writes the string as-is to the stream.

		void writeRaw(const char* buffer, std::streamsize length);
		/// Writes length raw bytes from the given buffer to the stream.

		void writeBOM();
		/// Writes a byte-order mark to the stream. A byte order mark is
		/// a 16-bit integer with a value of 0xFEFF, written in host byte-order. 
		/// A BinaryReader uses the byte-order mark to determine the byte-order 
		/// of the stream.

		void flush();
		/// Flushes the underlying stream.

		bool good();
		/// Returns _ostr.good();

		bool fail();
		/// Returns _ostr.fail();

		bool bad();
		/// Returns _ostr.bad();

		std::ostream& stream() const;
		/// Returns the underlying stream.

		StreamByteOrder byteOrder() const;
		/// Returns the byte ordering used by the writer, which is
		/// either BIG_ENDIAN_BYTE_ORDER or LITTLE_ENDIAN_BYTE_ORDER.

	private:
		std::ostream&  _ostr;
		bool           _flipBytes;
		TextConverter* _pTextConverter;
	};


	template <typename T>
	class BasicMemoryBinaryWriter : public BinaryWriter
		/// A convenient wrapper for using DataBuffer and MemoryStream with BinarWriter.
	{
	public:
		BasicMemoryBinaryWriter(DataBuffer<T>& data, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER) :
			BinaryWriter(_ostr, byteOrder),
			_data(data),
			_ostr(data.begin(), data.capacity())
		{
		}

		BasicMemoryBinaryWriter(DataBuffer<T>& data, TextEncoding& encoding, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER) :
			BinaryWriter(_ostr, encoding, byteOrder),
			_data(data),
			_ostr(data.begin(), data.capacity())
		{
		}

		~BasicMemoryBinaryWriter()
		{
			try
			{
				flush();
			}
			catch (...)
			{
				//poco_unexpected();
			}
		}

		DataBuffer<T>& data()
		{
			return _data;
		}

		const DataBuffer<T>& data() const
		{
			return _data;
		}

		const MemoryOutputStream& stream() const
		{
			return _ostr;
		}

		MemoryOutputStream& stream()
		{
			return _ostr;
		}

	private:
		DataBuffer<T>& _data;
		MemoryOutputStream _ostr;
	};


	typedef BasicMemoryBinaryWriter<char> MemoryBinaryWriter;


	//
	// inlines
	//


	inline std::ostream& BinaryWriter::stream() const
	{
		return _ostr;
	}


	inline bool BinaryWriter::good()
	{
		return _ostr.good();
	}


	inline bool BinaryWriter::fail()
	{
		return _ostr.fail();
	}


	inline bool BinaryWriter::bad()
	{
		return _ostr.bad();
	}


	inline BinaryWriter::StreamByteOrder BinaryWriter::byteOrder() const
	{
#if defined(PX2_ARCH_BIG_ENDIAN)
		return _flipBytes ? LITTLE_ENDIAN_BYTE_ORDER : BIG_ENDIAN_BYTE_ORDER;
#else
		return _flipBytes ? BIG_ENDIAN_BYTE_ORDER : LITTLE_ENDIAN_BYTE_ORDER;
#endif
	}


}

#endif
