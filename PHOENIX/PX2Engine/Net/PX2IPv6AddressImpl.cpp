// PX2IPv6AddressImpl.cpp

#include "PX2IPv6AddressImpl.hpp"
#include "PX2Assert.hpp"
#include "PX2StringHelp.hpp"
#include "PX2NumberFormatter.hpp"
using namespace PX2;

template <typename T>
unsigned MaskBits(T val, unsigned size)
{
	unsigned count = 0;

	if (val)
	{
		val = (val ^ (val - 1)) >> 1;
		for (count = 0; val; ++count)
		{
			val >>= 1;
		}
	}
	else
	{
		count = size;
	}

	return size - count;
}

//----------------------------------------------------------------------------
IPv6AddressImpl::IPv6AddressImpl()
	:
mScope(0)
{
	std::memset(&mAddr, 0, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv6AddressImpl::IPv6AddressImpl(const void* addr)
	:
mScope(0)
{
	std::memcpy(&mAddr, addr, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv6AddressImpl::IPv6AddressImpl(const void* addr,  int32_t scope)
	:
mScope(scope)
{
	std::memcpy(&mAddr, addr, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv6AddressImpl::IPv6AddressImpl(unsigned int prefix)
	:
mScope(0)
{
	unsigned i = 0;
    
#if defined(_WIN32) || defined(WIN32)
	for (; prefix >= 16; ++i, prefix -= 16) 
	{
		mAddr.s6_addr16[i] = 0xffff;
	}

	if (prefix > 0)
		mAddr.s6_addr16[i++] = htons(~(0xffff >> prefix));

	while (i < 8)
	{
		mAddr.s6_addr16[i++] = 0;
	}
#elif defined (__ANDROID__)
	for (; prefix >= 32; ++i, prefix -= 32)
	{
		mAddr.s6_addr32[i] = 0xffffffff;
	}

	if (prefix > 0)
		mAddr.s6_addr32[i++] = htonl(~(0xffffffffU >> prefix));

	while (i < 4)
	{
		mAddr.s6_addr32[i++] = 0;
	}
#endif
}
//----------------------------------------------------------------------------
IPv6AddressImpl::IPv6AddressImpl(const IPv6AddressImpl& addr)
	:
mScope(0)
{
	std::memcpy((void*) &mAddr, (void*) &addr.mAddr, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv6AddressImpl::~IPv6AddressImpl ()
{
}
//----------------------------------------------------------------------------
std::string IPv6AddressImpl::ToString() const
{
	assertion(false, "Not implemented.");
	return "";
}
//----------------------------------------------------------------------------
px2_socklen_t IPv6AddressImpl::GetAddrLength() const
{
	return sizeof(mAddr);
}
//----------------------------------------------------------------------------
const void* IPv6AddressImpl::GetAddr() const
{
	return &mAddr;
}
//----------------------------------------------------------------------------
IPAddress::Family IPv6AddressImpl::GetFamily() const
{
	return IPAddress::IPv6;
}
//----------------------------------------------------------------------------
int IPv6AddressImpl::GetAF() const
{
	return AF_INET6;
}
//----------------------------------------------------------------------------
unsigned IPv6AddressImpl::PrefixLength() const
{
	unsigned bits = 0;
	unsigned bitPos = 128;

#if defined(_WIN32) || defined(WIN32)
	for (int i = 7; i >= 0; --i)
	{
		unsigned short addr = ntohs(mAddr.s6_addr16[i]);
		if ((bits = MaskBits(addr, 16)) > 0)
			return (bitPos - (16 - bits));
		bitPos -= 16;
	}
	return 0;
#elif defined __ANDROID__ 
    for (int i = 3; i >= 0; --i)
	{
		unsigned addr = ntohl(mAddr.s6_addr32[i]);
		if ((bits = MaskBits(addr, 32)) > 0)
			return (bitPos - (32 - bits));
		bitPos -= 32;
	}
	return 0;
#endif
    
    return 0;
}
//----------------------------------------------------------------------------
int32_t IPv6AddressImpl::GetScope() const
{
	return mScope;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsWildcard() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0 && 
		words[4] == 0 && words[5] == 0 && words[6] == 0 && words[7] == 0;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsBroadcast() const
{
	return false;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsLoopback() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0 && 
		words[4] == 0 && words[5] == 0 && words[6] == 0 && ntohs(words[7]) == 0x0001;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsMulticast() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return (ntohs(words[0]) & 0xFFE0) == 0xFF00;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsLinkLocal() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return (ntohs(words[0]) & 0xFFE0) == 0xFE80;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsSiteLocal() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return ((ntohs(words[0]) & 0xFFE0) == 0xFEC0) || ((ntohs(words[0]) & 0xFF00) == 0xFC00);
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsIPv4Compatible() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0 && words[4] == 0 && words[5] == 0;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsIPv4Mapped() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0 && words[4] == 0 && ntohs(words[5]) == 0xFFFF;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsWellKnownMC() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return (ntohs(words[0]) & 0xFFF0) == 0xFF00;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsNodeLocalMC() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return (ntohs(words[0]) & 0xFFEF) == 0xFF01;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsLinkLocalMC() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return (ntohs(words[0]) & 0xFFEF) == 0xFF02;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsSiteLocalMC() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return (ntohs(words[0]) & 0xFFEF) == 0xFF05;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsOrgLocalMC() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return (ntohs(words[0]) & 0xFFEF) == 0xFF08;
}
//----------------------------------------------------------------------------
bool IPv6AddressImpl::IsGlobalMC() const
{
	const int16_t* words = reinterpret_cast<const int16_t*>(&mAddr);
	return (ntohs(words[0]) & 0xFFEF) == 0xFF0F;
}
//----------------------------------------------------------------------------
IPv6AddressImpl* IPv6AddressImpl::Parse(const std::string& addr)
{
	if (addr.empty())
		return 0;

	assertion(false, "not implemented yet.\n");

	return 0;
}
//----------------------------------------------------------------------------
void IPv6AddressImpl::Mask(const IPAddressImpl* mask, const IPAddressImpl* set)
{
	PX2_UNUSED(mask);
	PX2_UNUSED(set);
	assertion(false, "Mask() is only supported for IPv4 addresses.\n");
}
//----------------------------------------------------------------------------
IPAddressImpl* IPv6AddressImpl::Clone() const
{
	return new0 IPv6AddressImpl(&mAddr, mScope);
}
//----------------------------------------------------------------------------
IPv6AddressImpl IPv6AddressImpl::operator & (const IPv6AddressImpl& addr) const
{
	IPv6AddressImpl result(&mAddr);

#if defined(_WIN32) || defined(WIN32)
	result.mAddr.s6_addr16[0] &= addr.mAddr.s6_addr16[0];
	result.mAddr.s6_addr16[1] &= addr.mAddr.s6_addr16[1];
	result.mAddr.s6_addr16[2] &= addr.mAddr.s6_addr16[2];
	result.mAddr.s6_addr16[3] &= addr.mAddr.s6_addr16[3];
	result.mAddr.s6_addr16[4] &= addr.mAddr.s6_addr16[4];
	result.mAddr.s6_addr16[5] &= addr.mAddr.s6_addr16[5];
	result.mAddr.s6_addr16[6] &= addr.mAddr.s6_addr16[6];
	result.mAddr.s6_addr16[7] &= addr.mAddr.s6_addr16[7];
#elif defined __ANDROID__
	result.mAddr.s6_addr32[0] &= addr.mAddr.s6_addr32[0];
	result.mAddr.s6_addr32[1] &= addr.mAddr.s6_addr32[1];
	result.mAddr.s6_addr32[2] &= addr.mAddr.s6_addr32[2];
	result.mAddr.s6_addr32[3] &= addr.mAddr.s6_addr32[3];
#endif

	return result;
}
//----------------------------------------------------------------------------
IPv6AddressImpl IPv6AddressImpl::operator | (const IPv6AddressImpl& addr) const
{
	IPv6AddressImpl result(&mAddr);

#if defined(_WIN32) || defined(WIN32)
	result.mAddr.s6_addr16[0] |= addr.mAddr.s6_addr16[0];
	result.mAddr.s6_addr16[1] |= addr.mAddr.s6_addr16[1];
	result.mAddr.s6_addr16[2] |= addr.mAddr.s6_addr16[2];
	result.mAddr.s6_addr16[3] |= addr.mAddr.s6_addr16[3];
	result.mAddr.s6_addr16[4] |= addr.mAddr.s6_addr16[4];
	result.mAddr.s6_addr16[5] |= addr.mAddr.s6_addr16[5];
	result.mAddr.s6_addr16[6] |= addr.mAddr.s6_addr16[6];
	result.mAddr.s6_addr16[7] |= addr.mAddr.s6_addr16[7];
#elif defined __ANDROID__
	result.mAddr.s6_addr32[0] |= addr.mAddr.s6_addr32[0];
	result.mAddr.s6_addr32[1] |= addr.mAddr.s6_addr32[1];
	result.mAddr.s6_addr32[2] |= addr.mAddr.s6_addr32[2];
	result.mAddr.s6_addr32[3] |= addr.mAddr.s6_addr32[3];
#endif

	return result;
}
//----------------------------------------------------------------------------
IPv6AddressImpl IPv6AddressImpl::operator ^ (const IPv6AddressImpl& addr) const
{
	IPv6AddressImpl result(&mAddr);

#if defined(_WIN32) || defined(WIN32)
	result.mAddr.s6_addr16[0] ^= addr.mAddr.s6_addr16[0];
	result.mAddr.s6_addr16[1] ^= addr.mAddr.s6_addr16[1];
	result.mAddr.s6_addr16[2] ^= addr.mAddr.s6_addr16[2];
	result.mAddr.s6_addr16[3] ^= addr.mAddr.s6_addr16[3];
	result.mAddr.s6_addr16[4] ^= addr.mAddr.s6_addr16[4];
	result.mAddr.s6_addr16[5] ^= addr.mAddr.s6_addr16[5];
	result.mAddr.s6_addr16[6] ^= addr.mAddr.s6_addr16[6];
	result.mAddr.s6_addr16[7] ^= addr.mAddr.s6_addr16[7];
#elif defined __ANDROID__
	result.mAddr.s6_addr32[0] ^= addr.mAddr.s6_addr32[0];
	result.mAddr.s6_addr32[1] ^= addr.mAddr.s6_addr32[1];
	result.mAddr.s6_addr32[2] ^= addr.mAddr.s6_addr32[2];
	result.mAddr.s6_addr32[3] ^= addr.mAddr.s6_addr32[3];
#endif

	return result;
}
//----------------------------------------------------------------------------
IPv6AddressImpl IPv6AddressImpl::operator ~ () const
{
	IPv6AddressImpl result(&mAddr);

#if defined(_WIN32) || defined(WIN32)
	result.mAddr.s6_addr16[0] ^= 0xffff;
	result.mAddr.s6_addr16[1] ^= 0xffff;
	result.mAddr.s6_addr16[2] ^= 0xffff;
	result.mAddr.s6_addr16[3] ^= 0xffff;
	result.mAddr.s6_addr16[4] ^= 0xffff;
	result.mAddr.s6_addr16[5] ^= 0xffff;
	result.mAddr.s6_addr16[6] ^= 0xffff;
	result.mAddr.s6_addr16[7] ^= 0xffff;
#elif defined __ANDROID__
	result.mAddr.s6_addr32[0] ^= 0xffffffff;
	result.mAddr.s6_addr32[1] ^= 0xffffffff;
	result.mAddr.s6_addr32[2] ^= 0xffffffff;
	result.mAddr.s6_addr32[3] ^= 0xffffffff;
#endif

	return result;
}
//----------------------------------------------------------------------------