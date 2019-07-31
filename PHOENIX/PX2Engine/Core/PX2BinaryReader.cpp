// PX2BinaryReader.cpp

#include "PX2BinaryReader.hpp"
#include "PX2TextConverter.hpp"
#include "PX2ByteOrder.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
BinaryReader::BinaryReader(std::istream& istr, StreamByteOrder byteOrder) :
_istr(istr),
_pTextConverter(0)
{
#if defined(PX2_ARCH_BIG_ENDIAN)
	_flipBytes = (byteOrder == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (byteOrder == BIG_ENDIAN_BYTE_ORDER);
#endif
}
//----------------------------------------------------------------------------
BinaryReader::BinaryReader(std::istream& istr, TextEncoding& encoding, 
	StreamByteOrder byteOrder) :
_istr(istr),
_pTextConverter(new TextConverter(encoding, TextEncoding::global()))
{
#if defined(PX2_ARCH_BIG_ENDIAN)
	_flipBytes = (byteOrder == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (byteOrder == BIG_ENDIAN_BYTE_ORDER);
#endif
}
//----------------------------------------------------------------------------
BinaryReader::~BinaryReader()
{
	delete _pTextConverter;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (bool& value)
{
	_istr.read((char*)&value, sizeof(value));
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (char& value)
{
	_istr.read((char*)&value, sizeof(value));
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (unsigned char& value)
{
	_istr.read((char*)&value, sizeof(value));
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (signed char& value)
{
	_istr.read((char*)&value, sizeof(value));
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (short& value)
{
	_istr.read((char*)&value, sizeof(value));
	if (_flipBytes) value = ByteOrder::flipBytes(value);
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (unsigned short& value)
{
	_istr.read((char*)&value, sizeof(value));
	if (_flipBytes) value = ByteOrder::flipBytes(value);
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (int& value)
{
	_istr.read((char*)&value, sizeof(value));
	if (_flipBytes) value = ByteOrder::flipBytes(value);
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (unsigned int& value)
{
	_istr.read((char*)&value, sizeof(value));
	if (_flipBytes) value = ByteOrder::flipBytes(value);
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (long& value)
{
	_istr.read((char*)&value, sizeof(value));
#if defined(PX2_LONG_IS_64_BIT)
	if (_flipBytes) value = ByteOrder::flipBytes((int64_t)value);
#else
	if (_flipBytes) value = ByteOrder::flipBytes((int32_t)value);
#endif
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (unsigned long& value)
{
	_istr.read((char*)&value, sizeof(value));
#if defined(PX2_LONG_IS_64_BIT)
	if (_flipBytes) value = ByteOrder::flipBytes((uint64_t)value);
#else
	if (_flipBytes) value = ByteOrder::flipBytes((uint32_t)value);
#endif
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (float& value)
{
	if (_flipBytes)
	{
		char* ptr = (char*)&value;
		ptr += sizeof(value);
		for (unsigned i = 0; i < sizeof(value); ++i)
			_istr.read(--ptr, 1);
	}
	else
	{
		_istr.read((char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (double& value)
{
	if (_flipBytes)
	{
		char* ptr = (char*)&value;
		ptr += sizeof(value);
		for (unsigned i = 0; i < sizeof(value); ++i)
			_istr.read(--ptr, 1);
	}
	else
	{
		_istr.read((char*)&value, sizeof(value));
	}
	return *this;
}
//----------------------------------------------------------------------------
#if defined(PX2_HAVE_INT64) && !defined(PX2_LONG_IS_64_BIT)
BinaryReader& BinaryReader::operator >> (int64_t& value)
{
	_istr.read((char*)&value, sizeof(value));
	if (_flipBytes) value = ByteOrder::flipBytes(value);
	return *this;
}
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (uint64_t& value)
{
	_istr.read((char*)&value, sizeof(value));
	if (_flipBytes) value = ByteOrder::flipBytes(value);
	return *this;
}
#endif
//----------------------------------------------------------------------------
BinaryReader& BinaryReader::operator >> (std::string& value)
{
	uint32_t size = 0;
	read7BitEncoded(size);
	value.clear();
	if (!_istr.good()) return *this;
	value.reserve(size);
	while (size--)
	{
		char c;
		if (!_istr.read(&c, 1).good()) break;
		value += c;
	}
	if (_pTextConverter)
	{
		std::string converted;
		_pTextConverter->convert(value, converted);
		std::swap(value, converted);
	}
	return *this;
}
//----------------------------------------------------------------------------
void BinaryReader::read7BitEncoded(uint32_t& value)
{
	char c;
	value = 0;
	int s = 0;
	do
	{
		c = 0;
		_istr.read(&c, 1);
		uint32_t x = (c & 0x7F);
		x <<= s;
		value += x;
		s += 7;
	} while (c & 0x80);
}
//----------------------------------------------------------------------------
#if defined(PX2_HAVE_INT64)
void BinaryReader::read7BitEncoded(uint64_t& value)
{
	char c;
	value = 0;
	int s = 0;
	do
	{
		c = 0;
		_istr.read(&c, 1);
		uint64_t x = (c & 0x7F);
		x <<= s;
		value += x;
		s += 7;
	} while (c & 0x80);
}
#endif
//----------------------------------------------------------------------------
void BinaryReader::readRaw(std::streamsize length, std::string& value)
{
	value.clear();
	value.reserve(static_cast<std::string::size_type>(length));
	while (length--)
	{
		char c;
		if (!_istr.read(&c, 1).good()) break;
		value += c;
	}
}
//----------------------------------------------------------------------------
void BinaryReader::readRaw(char* buffer, std::streamsize length)
{
	_istr.read(buffer, length);
}
//----------------------------------------------------------------------------
void BinaryReader::readBOM()
{
	uint16_t bom;
	_istr.read((char*)&bom, sizeof(bom));
	_flipBytes = bom != 0xFEFF;
}
//----------------------------------------------------------------------------