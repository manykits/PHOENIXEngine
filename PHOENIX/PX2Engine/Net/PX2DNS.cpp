// PX2DNS.cpp

#include "PX2DNS.hpp"
#include "PX2SocketAddress.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HostEntry DNS::GetHostByName (const std::string &hostname)
{
#ifdef __ANDROID__
	struct hostent* he = gethostbyname(hostname.c_str());
	if (he)
	{
		return HostEntry(he);
	}
#else
	struct hostent* he = gethostbyname(hostname.c_str());
	if (he)
	{
		return HostEntry(he);
	}
#endif

	Error(LastError(), hostname);

	assertion(false, "GetHostByName error.\n");

	return HostEntry();
}
//----------------------------------------------------------------------------
HostEntry DNS::GetHostByAddress (const IPAddress &address)
{
#if defined(PX2_HAVE_ADDRINFO)
	SocketAddress sa(address, 0);
	static char fqname[1024];
	int rc = getnameinfo(sa.GetAddr(), sa.GetAddrLength(), fqname, 
		sizeof(fqname), NULL,
		0, NI_NAMEREQD); 
	if (rc == 0)
	{
		struct addrinfo* pAI;
		struct addrinfo hints;
		std::memset(&hints, 0, sizeof(hints));
		hints.ai_flags = AI_CANONNAME | AI_ADDRCONFIG;
		rc = getaddrinfo(fqname, NULL, &hints, &pAI);
		if (rc == 0)
		{
			HostEntry result(pAI);
			freeaddrinfo(pAI);
			return result;
		}
		else
		{
			Aierror(rc, address.ToString());
		}
	}
	else
	{
		Aierror(rc, address.ToString());
	}
#else

#ifndef __ANDROID__
	struct hostent* he = gethostbyaddr(
		reinterpret_cast<const char*>(address.GetAddr()), 
		address.GetAddrLength(), address.GetAF());
	if (he)
	{
		return HostEntry(he);
	}
#else
	return HostEntry();
#endif


#endif
	int err = LastError();
	Error(err, address.ToString());
	assertion(false, "");
	return HostEntry();
}
//----------------------------------------------------------------------------
HostEntry DNS::Resolve(const std::string &address)
{
	IPAddress ip;

	if (IPAddress::TryParse(address, ip))
		return GetHostByAddress(ip);

	return GetHostByName(address);
}
//----------------------------------------------------------------------------
IPAddress DNS::ResolveOne (const std::string &address)
{
	const HostEntry& entry = Resolve(address);

	if (!entry.GetAddresses().empty())
		return entry.GetAddresses()[0];

	assertion(false, "ResolveOne failed.\n");
	PX2_LOG_ERROR("ResolveOne failed.\n");

	return IPAddress();
}
//----------------------------------------------------------------------------
HostEntry DNS::GetThisHost()
{
	return GetHostByName(GetHostName());
}
//----------------------------------------------------------------------------
std::string DNS::GetHostName ()
{
	char buffer[256];
	memset(buffer, 0, 256 * sizeof(char));
	int rc = gethostname(buffer, sizeof(buffer));
	if (rc == 0)
	{
		PX2_LOG_INFO("Host name is %s", buffer);

		return std::string(buffer);
	}

	assertion(false, "Cannot get host name.\n");
	PX2_LOG_ERROR("Cannot get host name.");

	return "";
}
//----------------------------------------------------------------------------
int DNS::LastError()
{
#if defined(_WIN32) || defined(WIN32)
	return ::GetLastError();
#elif defined (__ANDROID__)
	return 0;
#elif defined (__IOS__)
	return h_errno;
#endif
}
//----------------------------------------------------------------------------
void DNS::Error (int code, const std::string& arg)
{
	PX2_UNUSED(arg);
	PX2_UNUSED(code);

	switch (code)
	{
	case PX2_ESYSNOTREADY:
		assertion(false, "Net subsystem not ready.\n");
		PX2_LOG_ERROR("Net subsystem not ready.");
		break;
	case PX2_ENOTINIT:
		assertion(false, "Net subsystem not initialized.\n");
		PX2_LOG_ERROR("Net subsystem not initialized.");
		break;
#ifndef __ANDROID__
	case PX2_HOST_NOT_FOUND:
		assertion(false, "Host not found.\n");
		PX2_LOG_ERROR("Host not found.");
		break;
	case PX2_TRY_AGAIN:
		assertion(false, "Temporary DNS error while resolving.\n");
		PX2_LOG_ERROR("Temporary DNS error while resolving.");
		break;
	case PX2_NO_RECOVERY:
		assertion(false, "Non recoverable DNS error while resolving.\n");
		PX2_LOG_ERROR("Non recoverable DNS error while resolving.");
		break;
	case PX2_NO_DATA:
		assertion(false, "No address found.\n");
		PX2_LOG_ERROR("No address found.");
		break;
#endif
	default:
		assertion(false, "%d \n", code);
		PX2_LOG_ERROR("%d", code);
		break;
	}
}
//----------------------------------------------------------------------------
void DNS::Aierror (int code, const std::string& arg)
{
	PX2_UNUSED(arg);
	PX2_UNUSED(code);

#if defined(PX2_HAVE_IPV6) || defined(PX2_HAVE_ADDRINFO)
	switch (code)
	{
#ifndef __ANDROID__
	case EAI_AGAIN:
		assertion(false, "Temporary DNS error while resolving.\n");
		PX2_LOG_ERROR("Temporary DNS error while resolving.");
		break;
	case EAI_FAIL:
		assertion(false, "Non recoverable DNS error while resolving.\n");
		PX2_LOG_ERROR("Non recoverable DNS error while resolving.");
#endif // !(__ANDROID__)

#if !defined(_WIN32) && !defined(WIN32)
#if defined(EAI_NODATA)
	case EAI_NODATA:
		assertion(false, "no address found.\n");
		PX2_LOG_ERROR("no address found.");
		break;
#endif
#endif

#ifndef __ANDROID__
	case EAI_NONAME:
		assertion(false, "Hose not found.\n");
		PX2_LOG_ERROR("Hose not found.");
		break;
#endif

#if defined(EAI_SYSTEM)
	case EAI_SYSTEM:
		Error(LastError(), arg);
		break;
#endif
            
#if defined(_WIN32) || defined(WIN32)
	case WSANO_DATA:
		assertion(false, "Host not found.\n");
		PX2_LOG_ERROR("Host not found.");
		break;
#endif
	default:
		assertion(false, "EAI:%d", code);
		PX2_LOG_ERROR("EAI:%d", code);
		break;
	}
#endif // PX2_HAVE_IPV6
}
//----------------------------------------------------------------------------