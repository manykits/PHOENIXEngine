// PX2BinaryReader.hpp

#ifndef PX2BINARYREADER_HPP
#define PX2BINARYREADER_HPP

#include "PX2CorePre.hpp"
#include "PX2DataBuffer.hpp"
#include "PX2MemoryStream.hpp"

namespace PX2 
{

	class TextEncoding;
	class TextConverter;

	class PX2_ENGINE_ITEM BinaryReader
	{
	public:
		enum StreamByteOrder
		{
			NATIVE_BYTE_ORDER = 1,  /// the host's native byte-order
			BIG_ENDIAN_BYTE_ORDER = 2,  /// big-endian (network) byte-order
			NETWORK_BYTE_ORDER = 2,  /// big-endian (network) byte-order
			LITTLE_ENDIAN_BYTE_ORDER = 3,  /// little-endian byte-order
			UNSPECIFIED_BYTE_ORDER = 4   /// unknown, byte-order will be determined by reading the byte-order mark
		};

		BinaryReader(std::istream& istr, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
		BinaryReader(std::istream& istr, TextEncoding& encoding, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
		~BinaryReader();

		BinaryReader& operator >> (bool& value);
		BinaryReader& operator >> (char& value);
		BinaryReader& operator >> (unsigned char& value);
		BinaryReader& operator >> (signed char& value);
		BinaryReader& operator >> (short& value);
		BinaryReader& operator >> (unsigned short& value);
		BinaryReader& operator >> (int& value);
		BinaryReader& operator >> (unsigned int& value);
		BinaryReader& operator >> (long& value);
		BinaryReader& operator >> (unsigned long& value);
		BinaryReader& operator >> (float& value);
		BinaryReader& operator >> (double& value);

#if defined(PX2_HAVE_INT64) && !defined(PX2_LONG_IS_64_BIT)
		BinaryReader& operator >> (int64_t& value);
		BinaryReader& operator >> (uint64_t& value);
#endif

		BinaryReader& operator >> (std::string& value);

		template <typename T>
		BinaryReader& operator >> (std::vector<T>& value)
		{
			uint32_t size(0);
			T elem;

			*this >> size;
			if (!good()) return *this;
			value.reserve(size);
			while (this->good() && size-- > 0)
			{
				*this >> elem;
				value.push_back(elem);
			}
			return *this;
		}

		void read7BitEncoded(uint32_t& value);
		/// Reads a 32-bit unsigned integer in compressed format.
		/// See BinaryWriter::write7BitEncoded() for a description
		/// of the compression algorithm.

#if defined(PX2_HAVE_INT64)
		void read7BitEncoded(uint64_t& value);
		/// Reads a 64-bit unsigned integer in compressed format.
		/// See BinaryWriter::write7BitEncoded() for a description
		/// of the compression algorithm.
#endif

		void readRaw(std::streamsize length, std::string& value);
		/// Reads length bytes of raw data into value.

		void readRaw(char* buffer, std::streamsize length);
		/// Reads length bytes of raw data into buffer.

		void readBOM();
		/// Reads a byte-order mark from the stream and configures
		/// the reader for the encountered byte order.
		/// A byte-order mark is a 16-bit integer with a value of 0xFEFF,
		/// written in host byte order.

		bool good();
		/// Returns _istr.good();

		bool fail();
		/// Returns _istr.fail();

		bool bad();
		/// Returns _istr.bad();

		bool eof();
		/// Returns _istr.eof();

		std::istream& stream() const;
		/// Returns the underlying stream.

		StreamByteOrder byteOrder() const;
		/// Returns the byte-order used by the reader, which is
		/// either BIG_ENDIAN_BYTE_ORDER or LITTLE_ENDIAN_BYTE_ORDER.

		void setExceptions(std::ios_base::iostate st = (std::istream::failbit | std::istream::badbit));
		/// Sets the stream to throw exception on specified state (default failbit and badbit);

		std::streamsize available() const;
		/// Returns the number of available bytes in the stream.

	private:
		std::istream&  _istr;
		bool           _flipBytes;
		TextConverter* _pTextConverter;
	};


	template <typename T>
	class BasicMemoryBinaryReader : public BinaryReader
		/// A convenient wrapper for using DataBuffer and MemoryStream with BinaryReader.
	{
	public:
		BasicMemoryBinaryReader(const DataBuffer<T>& data, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER) :
			BinaryReader(_istr, byteOrder),
			_data(data),
			_istr(data.begin(), data.capacity())
		{
		}

		BasicMemoryBinaryReader(const DataBuffer<T>& data, TextEncoding& encoding, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER) :
			BinaryReader(_istr, encoding, byteOrder),
			_data(data),
			_istr(data.begin(), data.capacity())
		{
		}

		~BasicMemoryBinaryReader()
		{
		}

		const DataBuffer<T>& data() const
		{
			return _data;
		}

		const MemoryInputStream& stream() const
		{
			return _istr;
		}

		MemoryInputStream& stream()
		{
			return _istr;
		}

	private:
		const DataBuffer<T>& _data;
		MemoryInputStream _istr;
	};


	typedef BasicMemoryBinaryReader<char> MemoryBinaryReader;


	inline bool BinaryReader::good()
	{
		return _istr.good();
	}


	inline bool BinaryReader::fail()
	{
		return _istr.fail();
	}


	inline bool BinaryReader::bad()
	{
		return _istr.bad();
	}


	inline bool BinaryReader::eof()
	{
		return _istr.eof();
	}


	inline std::istream& BinaryReader::stream() const
	{
		return _istr;
	}


	inline BinaryReader::StreamByteOrder BinaryReader::byteOrder() const
	{
#if defined(PX2_ARCH_BIG_ENDIAN)
		return _flipBytes ? LITTLE_ENDIAN_BYTE_ORDER : BIG_ENDIAN_BYTE_ORDER;
#else
		return _flipBytes ? BIG_ENDIAN_BYTE_ORDER : LITTLE_ENDIAN_BYTE_ORDER;
#endif
	}

	inline void BinaryReader::setExceptions(std::ios_base::iostate st)
	{
		_istr.exceptions(st);
	}


	inline std::streamsize BinaryReader::available() const
	{
		return _istr.rdbuf()->in_avail();
	}

}

#endif
