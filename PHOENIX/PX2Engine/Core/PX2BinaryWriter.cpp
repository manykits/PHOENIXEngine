// PX2BinaryWriter.cpp

#include "PX2BinaryWriter.hpp"
#include "PX2ByteOrder.hpp"
#include "PX2TextConverter.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
BinaryWriter::BinaryWriter(std::ostream& ostr, StreamByteOrder byteOrder) :
_ostr(ostr),
_pTextConverter(0)
{
#if defined(PX2_ARCH_BIG_ENDIAN)
	_flipBytes = (byteOrder == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (byteOrder == BIG_ENDIAN_BYTE_ORDER);
#endif
}
//----------------------------------------------------------------------------
BinaryWriter::~BinaryWriter()
{
	delete _pTextConverter;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (bool value)
{
	_ostr.write((const char*)&value, sizeof(value));
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (char value)
{
	_ostr.write((const char*)&value, sizeof(value));
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (unsigned char value)
{
	_ostr.write((const char*)&value, sizeof(value));
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (signed char value)
{
	_ostr.write((const char*)&value, sizeof(value));
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (short value)
{
	if (_flipBytes)
	{
		short fValue = ByteOrder::flipBytes(value);
		_ostr.write((const char*)&fValue, sizeof(fValue));
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (unsigned short value)
{
	if (_flipBytes)
	{
		unsigned short fValue = ByteOrder::flipBytes(value);
		_ostr.write((const char*)&fValue, sizeof(fValue));
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (int value)
{
	if (_flipBytes)
	{
		int fValue = ByteOrder::flipBytes(value);
		_ostr.write((const char*)&fValue, sizeof(fValue));
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (unsigned int value)
{
	if (_flipBytes)
	{
		unsigned int fValue = ByteOrder::flipBytes(value);
		_ostr.write((const char*)&fValue, sizeof(fValue));
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (long value)
{
	if (_flipBytes)
	{
#if defined(PX2_LONG_IS_64_BIT)
		long fValue = ByteOrder::flipBytes((int64_t)value);
#else
		long fValue = ByteOrder::flipBytes((int32_t)value);
#endif
		_ostr.write((const char*)&fValue, sizeof(fValue));
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (unsigned long value)
{
	if (_flipBytes)
	{
#if defined(PX2_LONG_IS_64_BIT)
		long fValue = ByteOrder::flipBytes((uint64_t)value);
#else
		long fValue = ByteOrder::flipBytes((uint32_t)value);
#endif
		_ostr.write((const char*)&fValue, sizeof(fValue));
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (float value)
{
	if (_flipBytes)
	{
		const char* ptr = (const char*)&value;
		ptr += sizeof(value);
		for (unsigned i = 0; i < sizeof(value); ++i)
			_ostr.write(--ptr, 1);
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (double value)
{
	if (_flipBytes)
	{
		const char* ptr = (const char*)&value;
		ptr += sizeof(value);
		for (unsigned i = 0; i < sizeof(value); ++i)
			_ostr.write(--ptr, 1);
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
#if defined(PX2_HAVE_INT64) && !defined(PX2_LONG_IS_64_BIT)
BinaryWriter& BinaryWriter::operator << (int64_t value)
{
	if (_flipBytes)
	{
		int64_t fValue = ByteOrder::flipBytes(value);
		_ostr.write((const char*)&fValue, sizeof(fValue));
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (uint64_t value)
{
	if (_flipBytes)
	{
		uint64_t fValue = ByteOrder::flipBytes(value);
		_ostr.write((const char*)&fValue, sizeof(fValue));
	}
	else
	{
		_ostr.write((const char*)&value, sizeof(value));
	}
	return *this;
}
#endif
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (const std::string& value)
{
	if (_pTextConverter)
	{
		std::string converted;
		_pTextConverter->convert(value, converted);
		uint32_t length = (uint32_t)converted.size();
		write7BitEncoded(length);
		_ostr.write(converted.data(), length);
	}
	else
	{
		uint32_t length = (uint32_t)value.size();
		write7BitEncoded(length);
		_ostr.write(value.data(), length);
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryWriter& BinaryWriter::operator << (const char* value)
{
	assertion(0!=value, "");

	if (_pTextConverter)
	{
		std::string converted;
		_pTextConverter->convert(value, static_cast<int>(std::strlen(value)), converted);
		uint32_t length = (uint32_t)converted.size();
		write7BitEncoded(length);
		_ostr.write(converted.data(), length);
	}
	else
	{
		uint32_t length = static_cast<uint32_t>(std::strlen(value));
		write7BitEncoded(length);
		_ostr.write(value, length);
	}
	return *this;
}
//----------------------------------------------------------------------------
void BinaryWriter::write7BitEncoded(uint32_t value)
{
	do
	{
		unsigned char c = (unsigned char)(value & 0x7F);
		value >>= 7;
		if (value) c |= 0x80;
		_ostr.write((const char*)&c, 1);
	} while (value);
}
//----------------------------------------------------------------------------
#if defined(PX2_HAVE_INT64)
void BinaryWriter::write7BitEncoded(uint64_t value)
{
	do
	{
		unsigned char c = (unsigned char)(value & 0x7F);
		value >>= 7;
		if (value) c |= 0x80;
		_ostr.write((const char*)&c, 1);
	} while (value);
}
#endif
//----------------------------------------------------------------------------
void BinaryWriter::writeRaw(const std::string& rawData)
{
	_ostr.write(rawData.data(), (std::streamsize) rawData.length());
}
//----------------------------------------------------------------------------
void BinaryWriter::writeRaw(const char* buffer, std::streamsize length)
{
	_ostr.write(buffer, length);
}
//----------------------------------------------------------------------------
void BinaryWriter::writeBOM()
{
	int64_t value = 0xFEFF;
	if (_flipBytes) value = ByteOrder::flipBytes(value);
	_ostr.write((const char*)&value, sizeof(value));
}
//----------------------------------------------------------------------------
void BinaryWriter::flush()
{
	_ostr.flush();
}
//----------------------------------------------------------------------------
