// PX2NetBitStream.hpp

#ifndef PX2NETBITSTREAM_HPP
#define PX2NETBITSTREAM_HPP

#include "PX2NetPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM BitStream
	{
	public:
		BitStream();
		~BitStream();

	private:
		char *mBufferPointer;
		uint32_t mLength;

	public:
		bool SetBuffer(char *buffer);
		char *GetBuffer();

		bool ReadInt32(int32_t *data);
		bool ReadUInt32(uint32_t *data);
		bool ReadByte(uint8_t *data);
		bool ReadBytes(uint8_t *data, uint32_t length);
		bool ReadFloat(float *data);
		bool ReadInt64(int64_t *data);
		bool ReadUInt16(uint16_t *data);
		bool ReadInt16(int16_t *data);
		bool ReadBool(bool *data);
		bool ReadString(std::string &data);

		bool WriteInt32(int32_t data);
		bool WriteUInt32(uint32_t data);
		bool WriteByte(uint8_t data);
		bool WriteBytes(uint8_t *data, uint32_t length);
		bool WriteFloat(float data);
		bool WriteInt64(int64_t data);
		bool WriteUInt16(uint16_t data);
		bool WriteInt16(int16_t data);
		bool WriteBool(bool data);
		bool WriteString(const std::string &str);

		uint32_t ByteSize();

		bool SerializeToArray(void* data, int size) const;
	};


}

#endif