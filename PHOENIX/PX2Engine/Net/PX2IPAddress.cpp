// PX2IPAddress.cpp

#include "PX2IPAddress.hpp"
#include "PX2IPv4AddressImpl.hpp"
#include "PX2IPv6AddressImpl.hpp"
#include "PX2Memory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
IPAddress::IPAddress()
	:
mImpl(new0 IPv4AddressImpl())
{
}
//----------------------------------------------------------------------------
IPAddress::IPAddress(const IPAddress& GetAddr)
	:
mImpl(GetAddr.mImpl)
{
}
//----------------------------------------------------------------------------
IPAddress::IPAddress (Family family)
{
	if (family == IPv4)
		mImpl = new0 IPv4AddressImpl();
#if defined(PX2_HAVE_IPV6)
	else if (family == IPv6)
		mImpl = new0 IPv6AddressImpl();
#endif
	else
	{
		assertion(false,
			"Invalid or unsupported address GetFamily passed to IPAddress()");
	}
}
//----------------------------------------------------------------------------
IPAddress::IPAddress(const std::string& addr)
{
	mImpl = IPv4AddressImpl::Parse(addr);
#if defined(PX2_HAVE_IPV6)
	if (!mImpl)
		mImpl = IPv6AddressImpl::Parse(addr);
#endif
	if (!mImpl)
	{
		assertion(false, "invalidAddress.\n");
	}
}
//----------------------------------------------------------------------------
IPAddress::IPAddress(const std::string& addr, Family family)
{
	if (family == IPv4)
		mImpl = IPv4AddressImpl::Parse(addr);
#if defined(PX2_HAVE_IPV6)
	else if (family == IPv6)
		mImpl = IPv6AddressImpl::Parse(addr);
#endif
	else
	{
		assertion(false,
			"Invalid or unsupported address GetFamily passed to IPAddress()");
	}

	if (!mImpl)
	{
		assertion(false, "invalidAddress.\n");
	}
}
//----------------------------------------------------------------------------
IPAddress::IPAddress(const void* addr, px2_socklen_t length)
{
	if (length == sizeof(struct in_addr))
		mImpl = new0 IPv4AddressImpl(addr);
#if defined(PX2_HAVE_IPV6)
	else if (length == sizeof(struct in6_addr))
		mImpl = new0 IPv6AddressImpl(addr);
#endif
	else
	{
		assertion(false, "Invalid address length passed to IPAddress()");
	}
}
//----------------------------------------------------------------------------
IPAddress::IPAddress(const void* addr, px2_socklen_t length, int32_t scope)
{
	if (length == sizeof(struct in_addr))
		mImpl = new0 IPv4AddressImpl(addr);
#if defined(PX2_HAVE_IPV6)
	else if (length == sizeof(struct in6_addr))
		mImpl = new0 IPv6AddressImpl(addr, scope);
#endif
	else
	{
		assertion(false, "Invalid address length passed to IPAddress()");
	}
}
//----------------------------------------------------------------------------
IPAddress::IPAddress(unsigned prefix, Family family)
{
	if (family == IPv4)
	{
		if (prefix <= 32)
		{
			mImpl = new0 IPv4AddressImpl(prefix);
		}
	}
#if defined(PX2_HAVE_IPV6)
	else if (family == IPv6)
	{
		if (prefix <= 128)
		{
			mImpl = new0 IPv6AddressImpl(prefix);
		}
	}
#endif
	else
	{
		assertion(false, 
			"Invalid or unsupported address GetFamily passed to IPAddress().\n");
	}

	if (!mImpl)
	{
		assertion(false, "Invalid prefix length passed to IPAddress()");
	}
}
//----------------------------------------------------------------------------
IPAddress::IPAddress(const struct sockaddr& sockaddr)
{
	unsigned short GetFamily = sockaddr.sa_family;
	if (GetFamily == AF_INET)
		mImpl = new0 IPv4AddressImpl(
		&reinterpret_cast<const struct sockaddr_in*>(&sockaddr)->sin_addr);
#if defined(PX2_HAVE_IPV6)
	else if (GetFamily == AF_INET6)
		mImpl = new0 IPv6AddressImpl(
		&reinterpret_cast<const struct sockaddr_in6*>(&sockaddr)->sin6_addr,
		reinterpret_cast<const struct sockaddr_in6*>(&sockaddr)
		->sin6_scope_id);
#endif
	else 
	{
		assertion(false,
			"Invalid or unsupported address GetFamily passed to IPAddress().\n");
	}
}
//----------------------------------------------------------------------------
IPAddress::~IPAddress()
{
	mImpl = 0;
}
//----------------------------------------------------------------------------
IPAddress& IPAddress::operator = (const IPAddress& GetAddr)
{
	if (&GetAddr != this)
	{
		mImpl = GetAddr.mImpl;
	}

	return *this;
}
//----------------------------------------------------------------------------
void IPAddress::Swap(IPAddress& address)
{
	Pointer0<IPAddressImpl> temp = mImpl;
	mImpl = address.mImpl;
	address.mImpl = temp;
}
//----------------------------------------------------------------------------
IPAddress::Family IPAddress::GetFamily() const
{
	return mImpl->GetFamily();
}
//----------------------------------------------------------------------------
int32_t IPAddress::GetScope() const
{
	return mImpl->GetScope();
}
//----------------------------------------------------------------------------
std::string IPAddress::ToString() const
{
	return mImpl->ToString();
}
//----------------------------------------------------------------------------
bool IPAddress::IsWildcard() const
{
	return mImpl->IsWildcard();
}
//----------------------------------------------------------------------------
bool IPAddress::IsBroadcast() const
{
	return mImpl->IsBroadcast();
}
//----------------------------------------------------------------------------
bool IPAddress::IsLoopback() const
{
	return mImpl->IsLoopback();
}
//----------------------------------------------------------------------------
bool IPAddress::IsMulticast() const
{
	return mImpl->IsMulticast();
}
//----------------------------------------------------------------------------
bool IPAddress::isUnicast() const
{
	return !IsWildcard() && !IsBroadcast() && !IsMulticast();
}
//----------------------------------------------------------------------------
bool IPAddress::IsLinkLocal() const
{
	return mImpl->IsLinkLocal();
}
//----------------------------------------------------------------------------
bool IPAddress::IsSiteLocal() const
{
	return mImpl->IsSiteLocal();
}
//----------------------------------------------------------------------------
bool IPAddress::IsIPv4Compatible() const
{
	return mImpl->IsIPv4Compatible();
}
//----------------------------------------------------------------------------
bool IPAddress::IsIPv4Mapped() const
{
	return mImpl->IsIPv4Mapped();
}
//----------------------------------------------------------------------------
bool IPAddress::IsWellKnownMC() const
{
	return mImpl->IsWellKnownMC();
}
//----------------------------------------------------------------------------
bool IPAddress::IsNodeLocalMC() const
{
	return mImpl->IsNodeLocalMC();
}
//----------------------------------------------------------------------------
bool IPAddress::IsLinkLocalMC() const
{
	return mImpl->IsLinkLocalMC();
}
//----------------------------------------------------------------------------
bool IPAddress::IsSiteLocalMC() const
{
	return mImpl->IsSiteLocalMC();
}
//----------------------------------------------------------------------------
bool IPAddress::IsOrgLocalMC() const
{
	return mImpl->IsOrgLocalMC();
}
//----------------------------------------------------------------------------
bool IPAddress::IsGlobalMC() const
{
	return mImpl->IsGlobalMC();
}
//----------------------------------------------------------------------------
bool IPAddress::operator == (const IPAddress& a) const
{
	px2_socklen_t l1 = GetAddrLength();
	px2_socklen_t l2 = a.GetAddrLength();
	if (l1 == l2)
		return std::memcmp(GetAddr(), a.GetAddr(), l1) == 0;
	else
		return false;
}
//----------------------------------------------------------------------------
bool IPAddress::operator != (const IPAddress& a) const
{
	px2_socklen_t l1 = GetAddrLength();
	px2_socklen_t l2 = a.GetAddrLength();
	if (l1 == l2)
		return std::memcmp(GetAddr(), a.GetAddr(), l1) != 0;
	else
		return true;
}
//----------------------------------------------------------------------------
bool IPAddress::operator < (const IPAddress& a) const
{
	px2_socklen_t l1 = GetAddrLength();
	px2_socklen_t l2 = a.GetAddrLength();
	if (l1 == l2)
		return std::memcmp(GetAddr(), a.GetAddr(), l1) < 0;
	else
		return l1 < l2;
}
//----------------------------------------------------------------------------
bool IPAddress::operator <= (const IPAddress& a) const
{
	px2_socklen_t l1 = GetAddrLength();
	px2_socklen_t l2 = a.GetAddrLength();
	if (l1 == l2)
		return std::memcmp(GetAddr(), a.GetAddr(), l1) <= 0;
	else
		return l1 < l2;
}
//----------------------------------------------------------------------------
bool IPAddress::operator > (const IPAddress& a) const
{
	px2_socklen_t l1 = GetAddrLength();
	px2_socklen_t l2 = a.GetAddrLength();
	if (l1 == l2)
		return std::memcmp(GetAddr(), a.GetAddr(), l1) > 0;
	else
		return l1 > l2;
}
//----------------------------------------------------------------------------
bool IPAddress::operator >= (const IPAddress& a) const
{
	px2_socklen_t l1 = GetAddrLength();
	px2_socklen_t l2 = a.GetAddrLength();
	if (l1 == l2)
		return std::memcmp(GetAddr(), a.GetAddr(), l1) >= 0;
	else
		return l1 > l2;
}
//----------------------------------------------------------------------------
IPAddress IPAddress::operator & (const IPAddress& other) const
{
	if (GetFamily() == other.GetFamily())
	{
		if (GetFamily() == IPv4)
		{
			IPv4AddressImpl t(mImpl->GetAddr());
			IPv4AddressImpl o(other.mImpl->GetAddr());
			return IPAddress((t & o).GetAddr(), sizeof(struct in_addr));
		}
#if defined(PX2_HAVE_IPV6)
		else if (GetFamily() == IPv6)
		{
			IPv6AddressImpl t(mImpl->GetAddr());
			IPv6AddressImpl o(other.mImpl->GetAddr());
			return IPAddress((t & o).GetAddr(), sizeof(struct in6_addr));
		}
#endif
		else
		{
			assertion(false,
				"Invalid or unsupported address GetFamily passed to IPAddress().\n");
		}
	}
	else
	{
		assertion(false,
			"Invalid or unsupported address GetFamily passed to IPAddress().\n");
	}

	return *this;
}
//----------------------------------------------------------------------------
IPAddress IPAddress::operator | (const IPAddress& other) const
{
	if (GetFamily() == other.GetFamily())
	{
		if (GetFamily() == IPv4)
		{
			IPv4AddressImpl t(mImpl->GetAddr());
			IPv4AddressImpl o(other.mImpl->GetAddr());
			return IPAddress((t | o).GetAddr(), sizeof(struct in_addr));
		}
#if defined(PX2_HAVE_IPV6)
		else if (GetFamily() == IPv6)
		{
			IPv6AddressImpl t(mImpl->GetAddr());
			IPv6AddressImpl o(other.mImpl->GetAddr());
			return IPAddress((t | o).GetAddr(), sizeof(struct in6_addr));
		}
#endif
		else
		{
			assertion(false,
				"Invalid or unsupported address GetFamily passed to IPAddress().\n");
		}
	}
	else
	{
		assertion(false,
			"Invalid or unsupported address GetFamily passed to IPAddress().\n");
	}

	return *this;
}
//----------------------------------------------------------------------------
IPAddress IPAddress::operator ^ (const IPAddress& other) const
{
	if (GetFamily() == other.GetFamily())
	{
		if (GetFamily() == IPv4)
		{
			IPv4AddressImpl t(mImpl->GetAddr());
			IPv4AddressImpl o(other.mImpl->GetAddr());
			return IPAddress((t ^ o).GetAddr(), sizeof(struct in_addr));
		}
#if defined(PX2_HAVE_IPV6)
		else if (GetFamily() == IPv6)
		{
			IPv6AddressImpl t(mImpl->GetAddr());
			IPv6AddressImpl o(other.mImpl->GetAddr());
			return IPAddress((t ^ o).GetAddr(), sizeof(struct in6_addr));
		}
#endif
		else
		{
			assertion(false,
				"Invalid or unsupported address GetFamily passed to IPAddress()");
		}
	}
	else
	{
		assertion(false, 
			"Invalid or unsupported address GetFamily passed to IPAddress()");
	}

	return *this;
}
//----------------------------------------------------------------------------
IPAddress IPAddress::operator ~ () const
{
	if (GetFamily() == IPv4)
	{
		IPv4AddressImpl self(this->mImpl->GetAddr());
		return IPAddress((~self).GetAddr(), sizeof(struct in_addr));
	}
#if defined(PX2_HAVE_IPV6)
	else if (GetFamily() == IPv6)
	{
		IPv6AddressImpl self(this->mImpl->GetAddr());
		return IPAddress((~self).GetAddr(), sizeof(struct in6_addr));
	}
#endif
	else
	{
		assertion(false, 
			"Invalid or unsupported address GetFamily passed to IPAddress()");
	}

	return *this;
}
//----------------------------------------------------------------------------
px2_socklen_t IPAddress::GetAddrLength() const
{
	return mImpl->GetAddrLength();
}
//----------------------------------------------------------------------------
const void* IPAddress::GetAddr() const
{
	return mImpl->GetAddr();
}
//----------------------------------------------------------------------------
int IPAddress::GetAF() const
{
	return mImpl->GetAF();
}
//----------------------------------------------------------------------------
unsigned IPAddress::PrefixLength() const
{
	return mImpl->PrefixLength();
}
//----------------------------------------------------------------------------
void IPAddress::Init(IPAddressImpl *impl)
{
	mImpl = impl;
}
//----------------------------------------------------------------------------
IPAddress IPAddress::Parse(const std::string& addr)
{
	return IPAddress(addr);
}
//----------------------------------------------------------------------------
bool IPAddress::TryParse(const std::string &addr, IPAddress& result)
{
	IPAddressImpl *impl = IPv4AddressImpl::Parse(addr);

#if defined(PX2_HAVE_IPV6)
	if (!impl)
	{
		impl = IPv6AddressImpl::Parse(addr);
	}
#endif

	if (impl)
	{
		result.Init(impl);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void IPAddress::Mask(const IPAddress& mask)
{
	IPAddressImpl* pClone = mImpl->Clone();
	mImpl = pClone;
	IPAddress null;
	mImpl->Mask(mask.mImpl, null.mImpl);
}
//----------------------------------------------------------------------------
void IPAddress::Mask(const IPAddress& mask, const IPAddress& set)
{
	IPAddressImpl* pClone = mImpl->Clone();
	mImpl = pClone;
	mImpl->Mask(mask.mImpl, set.mImpl);
}
//----------------------------------------------------------------------------
IPAddress IPAddress::MakeWildcard(Family family)
{
	return IPAddress(family);
}
//----------------------------------------------------------------------------
IPAddress IPAddress::MakeBroadcast()
{
	struct in_addr ia;
	ia.s_addr = INADDR_NONE;
	return IPAddress(&ia, sizeof(ia));
}
//----------------------------------------------------------------------------