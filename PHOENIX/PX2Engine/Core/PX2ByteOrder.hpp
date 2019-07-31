// PX2ByteOrder.hpp

#ifndef PX2BYTEORDER_HPP
#define PX2BYTEORDER_HPP

#include "PX2CorePre.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM ByteOrder
	{
	public:
		static int16_t flipBytes(int16_t value);
		static uint16_t flipBytes(uint16_t value);
		static int32_t flipBytes(int32_t value);
		static uint32_t flipBytes(uint32_t value);
#if defined(PX2_HAVE_INT64)
		static int64_t flipBytes(int64_t value);
		static uint64_t flipBytes(uint64_t value);
#endif

		static int16_t toBigEndian(int16_t value);
		static uint16_t toBigEndian(uint16_t value);
		static int32_t toBigEndian(int32_t value);
		static uint32_t toBigEndian(uint32_t value);
#if defined(PX2_HAVE_INT64)
		static int64_t toBigEndian(int64_t value);
		static uint64_t toBigEndian(uint64_t value);
#endif

		static int16_t fromBigEndian(int16_t value);
		static uint16_t fromBigEndian(uint16_t value);
		static int32_t fromBigEndian(int32_t value);
		static uint32_t fromBigEndian(uint32_t value);
#if defined(PX2_HAVE_INT64)
		static int64_t fromBigEndian(int64_t value);
		static uint64_t fromBigEndian(uint64_t value);
#endif

		static int16_t toLittleEndian(int16_t value);
		static uint16_t toLittleEndian(uint16_t value);
		static int32_t toLittleEndian(int32_t value);
		static uint32_t toLittleEndian(uint32_t value);
#if defined(PX2_HAVE_INT64)
		static int64_t toLittleEndian(int64_t value);
		static uint64_t toLittleEndian(uint64_t value);
#endif

		static int16_t fromLittleEndian(int16_t value);
		static uint16_t fromLittleEndian(uint16_t value);
		static int32_t fromLittleEndian(int32_t value);
		static uint32_t fromLittleEndian(uint32_t value);
#if defined(PX2_HAVE_INT64)
		static int64_t fromLittleEndian(int64_t value);
		static uint64_t fromLittleEndian(uint64_t value);
#endif

		static int16_t toNetwork(int16_t value);
		static uint16_t toNetwork(uint16_t value);
		static int32_t toNetwork(int32_t value);
		static uint32_t toNetwork(uint32_t value);
#if defined(PX2_HAVE_INT64)
		static int64_t toNetwork(int64_t value);
		static uint64_t toNetwork(uint64_t value);
#endif

		static int16_t fromNetwork(int16_t value);
		static uint16_t fromNetwork(uint16_t value);
		static int32_t fromNetwork(int32_t value);
		static uint32_t fromNetwork(uint32_t value);
#if defined(PX2_HAVE_INT64)
		static int64_t fromNetwork(int64_t value);
		static uint64_t fromNetwork(uint64_t value);
#endif
	};


#if !defined(PX2_NO_BYTESWAP_BUILTINS)
#if defined(_MSC_VER)
#if (PX2_MSVC_VERSION > 71)
#define PX2_HAVE_MSC_BYTESWAP 1
#endif
#elif defined(__clang__) 
#if __has_builtin(__builtin_bswap32)
#define PX2_HAVE_GCC_BYTESWAP 1
#endif
#elif defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
#define PX2_HAVE_GCC_BYTESWAP 1
#endif
#endif

	inline uint16_t ByteOrder::flipBytes(uint16_t value)
	{
#if defined(PX2_HAVE_MSC_BYTESWAP)
		return _byteswap_ushort(value);
#else
		return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
#endif
	}


	inline int16_t ByteOrder::flipBytes(int16_t value)
	{
		return int16_t(flipBytes(uint16_t(value)));
	}


	inline uint32_t ByteOrder::flipBytes(uint32_t value)
	{
#if defined(PX2_HAVE_MSC_BYTESWAP)
		return _byteswap_ulong(value);
#elif defined(PX2_HAVE_GCC_BYTESWAP)
		return __builtin_bswap32(value);
#else
		return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00)
			| ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
#endif
	}


	inline int32_t ByteOrder::flipBytes(int32_t value)
	{
		return int32_t(flipBytes(uint32_t(value)));
	}


#if defined(PX2_HAVE_INT64)
	inline uint64_t ByteOrder::flipBytes(uint64_t value)
	{
#if defined(PX2_HAVE_MSC_BYTESWAP)
		return _byteswap_uint64(value);
#elif defined(PX2_HAVE_GCC_BYTESWAP)
		return __builtin_bswap64(value);
#else
		uint32_t hi = uint32_t(value >> 32);
		uint32_t lo = uint32_t(value & 0xFFFFFFFF);
		return uint64_t(flipBytes(hi)) | (uint64_t(flipBytes(lo)) << 32);
#endif
	}


	inline int64_t ByteOrder::flipBytes(int64_t value)
	{
		return int64_t(flipBytes(uint64_t(value)));
	}
#endif // PX2_HAVE_INT64


	//
	// some macro trickery to automate the method implementation
	//
#define PX2_IMPLEMENT_BYTEORDER_NOOP_(op, type) \
	inline type ByteOrder::op(type value)		\
	{											\
	return value;							\
	}
#define PX2_IMPLEMENT_BYTEORDER_FLIP_(op, type) \
	inline type ByteOrder::op(type value)		\
	{											\
	return flipBytes(value);				\
	}


#if defined(PX2_HAVE_INT64)
#define PX2_IMPLEMENT_BYTEORDER_NOOP(op) \
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, int16_t)	\
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, uint16_t)	\
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, int32_t)	\
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, uint32_t)	\
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, int64_t)	\
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, uint64_t)
#define PX2_IMPLEMENT_BYTEORDER_FLIP(op) \
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, int16_t)	\
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, uint16_t)	\
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, int32_t)	\
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, uint32_t)	\
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, int64_t)	\
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, uint64_t)
#else
#define PX2_IMPLEMENT_BYTEORDER_NOOP(op) \
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, int16_t)	\
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, uint16_t)	\
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, int32_t)	\
	PX2_IMPLEMENT_BYTEORDER_NOOP_(op, uint32_t)
#define PX2_IMPLEMENT_BYTEORDER_FLIP(op) \
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, int16_t)	\
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, uint16_t)	\
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, int32_t)	\
	PX2_IMPLEMENT_BYTEORDER_FLIP_(op, uint32_t)
#endif


#if defined(PX2_ARCH_BIG_ENDIAN)
#define PX2_IMPLEMENT_BYTEORDER_BIG PX2_IMPLEMENT_BYTEORDER_NOOP
#define PX2_IMPLEMENT_BYTEORDER_LIT PX2_IMPLEMENT_BYTEORDER_FLIP
#else
#define PX2_IMPLEMENT_BYTEORDER_BIG PX2_IMPLEMENT_BYTEORDER_FLIP
#define PX2_IMPLEMENT_BYTEORDER_LIT PX2_IMPLEMENT_BYTEORDER_NOOP
#endif


	PX2_IMPLEMENT_BYTEORDER_BIG(toBigEndian)
		PX2_IMPLEMENT_BYTEORDER_BIG(fromBigEndian)
		PX2_IMPLEMENT_BYTEORDER_BIG(toNetwork)
		PX2_IMPLEMENT_BYTEORDER_BIG(fromNetwork)
		PX2_IMPLEMENT_BYTEORDER_LIT(toLittleEndian)
		PX2_IMPLEMENT_BYTEORDER_LIT(fromLittleEndian)

}

#endif