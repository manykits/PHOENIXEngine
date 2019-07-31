// PX2NetBitStream.cpp

#include "PX2NetBitStream.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
BitStream::BitStream()
{
	mBufferPointer = 0;
	mLength = 0;
}
//----------------------------------------------------------------------------
BitStream::~BitStream()
{
}
//----------------------------------------------------------------------------
bool BitStream::SetBuffer(char *buffer)
{
	if (!buffer)
		return false;

	mBufferPointer = buffer;
	mLength = 0;

	return true;
}
//----------------------------------------------------------------------------
char *BitStream::GetBuffer()
{
	return mBufferPointer;
}
//----------------------------------------------------------------------------
bool BitStream::ReadInt32(int32_t *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(int32_t));

	mLength += sizeof(int32_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadUInt32(uint32_t *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(uint32_t));

	mLength += sizeof(uint32_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadByte(uint8_t *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(uint8_t));

	mLength += sizeof(uint8_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadBytes(uint8_t *data, uint32_t length)
{
	memcpy(data, mBufferPointer + mLength, length);

	mLength += length;

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadFloat(float *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(float));

	mLength += sizeof(float);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadInt64(int64_t *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(int64_t));

	mLength += sizeof(int64_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadInt16(int16_t *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(int16_t));

	mLength += sizeof(int16_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadUInt16(uint16_t *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(uint16_t));

	mLength += sizeof(uint16_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadBool(bool *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(bool));

	mLength += sizeof(bool);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::ReadString(std::string &data)
{
	uint16_t strLegth = 0;
	ReadUInt16(&strLegth);

	if (strLegth > 0)
	{
		data.resize(strLegth);
		ReadBytes((uint8_t*)data[0], (uint32_t)strLegth);
	}

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteInt32(int32_t data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(int32_t));

	mLength += sizeof(int32_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteUInt32(uint32_t data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(uint32_t));

	mLength += sizeof(uint32_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteByte(uint8_t data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(uint8_t));

	mLength += sizeof(uint8_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteBytes(uint8_t *data, uint32_t length)
{
	memcpy(mBufferPointer + mLength, data, length);

	mLength += length;

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteFloat(float data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(float));

	mLength += sizeof(float);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteInt64(int64_t data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(int64_t));

	mLength += sizeof(int64_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteInt16(int16_t data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(int16_t));

	mLength += sizeof(int16_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteUInt16(uint16_t data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(uint16_t));

	mLength += sizeof(uint16_t);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteBool(bool data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(bool));

	mLength += sizeof(bool);

	return true;
}
//----------------------------------------------------------------------------
bool BitStream::WriteString(const std::string &str)
{
	WriteUInt16((uint16_t)str.length());
	WriteBytes((uint8_t*)str.c_str(), (uint32_t)str.length());

	return true;
}
//----------------------------------------------------------------------------
uint32_t BitStream::ByteSize()
{
	return mLength;
}
//----------------------------------------------------------------------------
bool BitStream::SerializeToArray(void* data, int size) const
{
	memcpy(data, mBufferPointer, size);

	return true;
}
//----------------------------------------------------------------------------