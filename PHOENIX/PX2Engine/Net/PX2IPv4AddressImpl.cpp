// PX2IPv4AddressImpl.cpp

#include "PX2IPv4AddressImpl.hpp"
#include "PX2Assert.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

namespace 
{

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

} // namespace
//----------------------------------------------------------------------------
IPv4AddressImpl::IPv4AddressImpl()
{
	std::memset(&mAddr, 0, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv4AddressImpl::IPv4AddressImpl(const void* addr)
{
	std::memcpy(&mAddr, addr, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv4AddressImpl::IPv4AddressImpl(unsigned int prefix)
{
	int32_t addr = (prefix == 32) ? 0xffffffff : ~(0xffffffff >> prefix);
	mAddr.s_addr = htonl(addr);
}
//----------------------------------------------------------------------------
IPv4AddressImpl::IPv4AddressImpl(const IPv4AddressImpl& GetAddr)
{
	std::memcpy(&mAddr, &GetAddr.mAddr, sizeof(mAddr));
}
//----------------------------------------------------------------------------
IPv4AddressImpl::~IPv4AddressImpl ()
{
}
//----------------------------------------------------------------------------
std::string IPv4AddressImpl::ToString() const
{
	const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&mAddr);
	std::string result;
	result.reserve(16);
	result = result + StringHelp::IntToString(bytes[0]);
	result.append(".");
	result = result + StringHelp::IntToString(bytes[1]);
	result.append(".");
	result = result + StringHelp::IntToString(bytes[2]);
	result.append(".");
	result = result + StringHelp::IntToString(bytes[3]);
	return result;
}
//----------------------------------------------------------------------------
px2_socklen_t IPv4AddressImpl::GetAddrLength() const
{
	return sizeof(mAddr);
}
//----------------------------------------------------------------------------
const void* IPv4AddressImpl::GetAddr() const
{
	return &mAddr;
}
//----------------------------------------------------------------------------
IPAddress::Family IPv4AddressImpl::GetFamily() const
{
	return IPAddress::IPv4;
}
//----------------------------------------------------------------------------
int IPv4AddressImpl::GetAF() const
{
	return AF_INET;
}
//----------------------------------------------------------------------------
unsigned IPv4AddressImpl::PrefixLength() const
{
	return MaskBits(ntohl(mAddr.s_addr), 32);
}
//----------------------------------------------------------------------------
int32_t IPv4AddressImpl::GetScope() const
{
	return 0;
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsWildcard() const
{
	return mAddr.s_addr == INADDR_ANY;
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsBroadcast() const
{
	return mAddr.s_addr == INADDR_NONE;
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsLoopback() const
{
	return (ntohl(mAddr.s_addr) & 0xFF000000) == 0x7F000000; // 127.0.0.1 to 127.255.255.255
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsMulticast() const
{
	return (ntohl(mAddr.s_addr) & 0xF0000000) == 0xE0000000; // 224.0.0.0/24 to 239.0.0.0/24
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsLinkLocal() const
{
	return (ntohl(mAddr.s_addr) & 0xFFFF0000) == 0xA9FE0000; // 169.254.0.0/16
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsSiteLocal() const
{
	uint32_t GetAddr = ntohl(mAddr.s_addr);
	return (GetAddr & 0xFF000000) == 0x0A000000 ||     // 10.0.0.0/24
		(GetAddr & 0xFFFF0000) == 0xC0A80000 ||        // 192.68.0.0/16
		(GetAddr >= 0xAC100000 && GetAddr <= 0xAC1FFFFF); // 172.16.0.0 to 172.31.255.255
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsIPv4Compatible() const
{
	return true;
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsIPv4Mapped() const
{
	return true;
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsWellKnownMC() const
{
	return (ntohl(mAddr.s_addr) & 0xFFFFFF00) == 0xE0000000; // 224.0.0.0/8
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsNodeLocalMC() const
{
	return false;
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsLinkLocalMC() const
{
	return (ntohl(mAddr.s_addr) & 0xFF000000) == 0xE0000000; // 244.0.0.0/24
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsSiteLocalMC() const
{
	return (ntohl(mAddr.s_addr) & 0xFFFF0000) == 0xEFFF0000; // 239.255.0.0/16
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsOrgLocalMC() const
{
	return (ntohl(mAddr.s_addr) & 0xFFFF0000) == 0xEFC00000; // 239.192.0.0/16
}
//----------------------------------------------------------------------------
bool IPv4AddressImpl::IsGlobalMC() const
{
	uint32_t GetAddr = ntohl(mAddr.s_addr);
	return GetAddr >= 0xE0000100 && GetAddr <= 0xEE000000; // 224.0.1.0 to 238.255.255.255
}
//----------------------------------------------------------------------------
IPv4AddressImpl* IPv4AddressImpl::Parse(const std::string &GetAddr)
{
	if (GetAddr.empty()) return 0;		
#if defined(_WIN32) || defined(WIN32)
	struct in_addr ia;
	ia.s_addr = inet_addr(GetAddr.c_str());
	if (ia.s_addr == INADDR_NONE && GetAddr != "255.255.255.255")
		return 0;
	else
	{
		return new0 IPv4AddressImpl(&ia);
	}

#else
	struct in_addr ia;
	if (inet_aton(GetAddr.c_str(), &ia))
		return new0 IPv4AddressImpl(&ia);
	else
	{
		return 0;
	}
#endif
}
//----------------------------------------------------------------------------
void IPv4AddressImpl::Mask(const IPAddressImpl* mask, const IPAddressImpl* set)
{
	assertion(mask->GetAF() == AF_INET && set->GetAF() == AF_INET,
		"mask->GetAF() == AF_INET && set->GetAF() == AF_INET");

	mAddr.s_addr &= static_cast<const IPv4AddressImpl*>(mask)->mAddr.s_addr;
	mAddr.s_addr |= static_cast<const IPv4AddressImpl*>(set)->mAddr.s_addr 
		& ~static_cast<const IPv4AddressImpl*>(mask)->mAddr.s_addr;
}
//----------------------------------------------------------------------------
IPAddressImpl* IPv4AddressImpl::Clone() const
{
	return new0 IPv4AddressImpl(&mAddr);
}
//----------------------------------------------------------------------------
IPv4AddressImpl IPv4AddressImpl::operator & (const IPv4AddressImpl& GetAddr) const
{
	IPv4AddressImpl result(&mAddr);
	result.mAddr.s_addr &= GetAddr.mAddr.s_addr;
	return result;
}
//----------------------------------------------------------------------------
IPv4AddressImpl IPv4AddressImpl::operator | (const IPv4AddressImpl& GetAddr) const
{
	IPv4AddressImpl result(&mAddr);
	result.mAddr.s_addr |= GetAddr.mAddr.s_addr;
	return result;
}
//----------------------------------------------------------------------------
IPv4AddressImpl IPv4AddressImpl::operator ^ (const IPv4AddressImpl& GetAddr) const
{
	IPv4AddressImpl result(&mAddr);
	result.mAddr.s_addr ^= GetAddr.mAddr.s_addr;
	return result;
}
//----------------------------------------------------------------------------
IPv4AddressImpl IPv4AddressImpl::operator ~ () const
{
	IPv4AddressImpl result(&mAddr);
	result.mAddr.s_addr ^= 0xffffffff;
	return result;
}
//----------------------------------------------------------------------------