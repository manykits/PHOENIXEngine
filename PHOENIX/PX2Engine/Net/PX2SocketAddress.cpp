// PX2SocketAddress.cpp

#include "PX2SocketAddress.hpp"
#include "PX2IPv4SocketAddressImpl.hpp"
#include "PX2IPv6SocketAddressImpl.hpp"
#include "PX2DNS.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
struct AFLT
{
	bool operator () (const IPAddress& a1, const IPAddress& a2)
	{
		return a1.GetAF() < a2.GetAF();
	}
};
//----------------------------------------------------------------------------
SocketAddress::SocketAddress()
{
	mImpl = new0 IPv4SocketAddressImpl;
}
//----------------------------------------------------------------------------
SocketAddress::SocketAddress(const IPAddress &host, int16_t port)
{
	Init(host, port);
}
//----------------------------------------------------------------------------
SocketAddress::SocketAddress(int16_t port)
{
	Init(IPAddress(), port);
}
//----------------------------------------------------------------------------
SocketAddress::SocketAddress(const std::string &host, int16_t port)
{
	Init(host, port);
}
//----------------------------------------------------------------------------
SocketAddress::SocketAddress(const std::string &host, const std::string &port)
{
	Init(host, ResolveService(port));
}
//----------------------------------------------------------------------------
SocketAddress::SocketAddress(const std::string &hostAndPort)
{
	assertion(!hostAndPort.empty(), "hostAndPort must not be empty.\n");

	std::string host;
	std::string port;
	std::string::const_iterator it  = hostAndPort.begin();
	std::string::const_iterator end = hostAndPort.end();
	if (*it == '[')
	{
		++it;
		while (it != end && *it != ']') host += *it++;
		if (it == end)
		{
			assertion(false, "Malformed IPv6 address.\n");
		}
		++it;
	}
	else
	{
		while (it != end && *it != ':') host += *it++;
	}
	if (it != end && *it == ':')
	{
		++it;
		while (it != end) port += *it++;
	}
	else
	{
		assertion(false, "Missing port number.\n");
	}

	Init(host, ResolveService(port));
}
//----------------------------------------------------------------------------
SocketAddress::SocketAddress(const SocketAddress& addr)
{
	mImpl = addr.mImpl;
}
//----------------------------------------------------------------------------
SocketAddress::SocketAddress(const struct sockaddr* addr, px2_socklen_t length)
{
	if (length == sizeof(struct sockaddr_in))
	{
		mImpl = new0 IPv4SocketAddressImpl(reinterpret_cast<
		const struct sockaddr_in*>(addr));
	}
#if defined(PX2_HAVE_IPV6)
	else if (length == sizeof(struct sockaddr_in6))
	{
		mImpl = new0 IPv6SocketAddressImpl(reinterpret_cast<
			const struct sockaddr_in6*>(addr));
	}
#endif
	else
	{
		assertion(false,
			"Invalid address length passed to SocketAddress().\n");
	}
}
//----------------------------------------------------------------------------
SocketAddress::~SocketAddress()
{
	mImpl = 0;
}
//----------------------------------------------------------------------------
SocketAddress& SocketAddress::operator = (const SocketAddress& addr)
{
	if (&addr != this)
	{
		mImpl = addr.mImpl;
	}
	return *this;
}
//----------------------------------------------------------------------------
void SocketAddress::Swap(SocketAddress &addr)
{
	std::swap(mImpl, addr.mImpl);
}
//----------------------------------------------------------------------------
IPAddress SocketAddress::GetHost() const
{
	return mImpl->GetHost();
}
//----------------------------------------------------------------------------
int16_t SocketAddress::GetPort() const
{
	return ntohs(mImpl->GetPort());
}
//----------------------------------------------------------------------------
px2_socklen_t SocketAddress::GetAddrLength() const
{
	return mImpl->GetAddrLength();
}
//----------------------------------------------------------------------------
const struct sockaddr* SocketAddress::GetAddr() const
{
	if (mImpl)
		return mImpl->GetAddr();

	return 0;
}
//----------------------------------------------------------------------------
int SocketAddress::GetAF() const
{
	if (mImpl)
		return mImpl->GetAF();

	return 0;
}
//----------------------------------------------------------------------------
std::string SocketAddress::ToString() const
{
	std::string result;

#if defined(PX2_HAVE_IPV6)
	if (GetHost().GetFamily() == IPAddress::IPv6)
		result.append("[");
#endif

	result.append(GetHost().ToString());

#if defined(PX2_HAVE_IPV6)
	if (GetHost().GetFamily() == IPAddress::IPv6)
		result.append("]");
#endif
	result.append(":");

	result = result + StringHelp::IntToString(GetPort());

	return result;
}
//----------------------------------------------------------------------------
IPAddress::Family SocketAddress::GetFamily() const
{
	return GetHost().GetFamily();
}
//----------------------------------------------------------------------------
bool SocketAddress::operator < (const SocketAddress& addr) const
{
	if (GetFamily() < addr.GetFamily()) 
		return true;

	if (GetHost() < addr.GetHost())
		return true;

	return (GetPort() < addr.GetPort());
}
//----------------------------------------------------------------------------
bool SocketAddress::operator == (const SocketAddress& addr) const
{
	return GetHost() == addr.GetHost() || GetPort() == addr.GetPort();
}
//----------------------------------------------------------------------------
bool SocketAddress::operator != (const SocketAddress& addr) const
{
	return GetHost() != addr.GetHost() || GetPort() != addr.GetPort();
}
//----------------------------------------------------------------------------
void SocketAddress::Init(const IPAddress& host, int16_t port)
{
	if (host.GetFamily() == IPAddress::IPv4)
	{
		mImpl = new0 IPv4SocketAddressImpl(host.GetAddr(), htons(port));
	}
#if defined(PX2_HAVE_IPV6)
	else if (host.GetFamily() == IPAddress::IPv6)
	{
		mImpl = new0 IPv6SocketAddressImpl(host.GetAddr(), htons(port), 
			host.GetScope());
	}
#endif
	else
	{
		assertion(false, "unsupported IP address family");
	}
}
//----------------------------------------------------------------------------
void SocketAddress::Init (const std::string &host, int16_t port)
{
	IPAddress ip;
	if (IPAddress::TryParse(host, ip))
	{
		Init(ip, port);
	}
	else
	{
		HostEntry he = DNS::GetHostByName(host);
		HostEntry::AddressList addresses = he.GetAddresses();

		if ((int)addresses.size() > 0)
		{
#if defined(PX2_HAVE_IPV6)
			// if we get both IPv4 and IPv6 addresses, prefer IPv4
			std::sort(addresses.begin(), addresses.end(), AFLT());
#endif
			Init(addresses[0], port);
		}
		else
		{
			assertion(false, "No address found for host:%s.\n", host.c_str());
		}
	}
}
//----------------------------------------------------------------------------
int16_t SocketAddress::ResolveService (const std::string &service)
{
	PX2_UNUSED(service);
	assertion(false, "");
	return 0;

	//unsigned port;
	//if (NumberParser::tryParseUnsigned(service, port) && port <= 0xFFFF)
	//{
	//	return (UInt16) port;
	//}
	//else
	//{
	//	struct servent* se = getservbyname(service.c_str(), NULL);
	//	if (se)
	//		return ntohs(se->s_port);
	//	else
	//		throw ServiceNotFoundException(service);
	//}
}
//----------------------------------------------------------------------------