// PX2NetError.cpp

#include "PX2NetError.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
int NetError::LastError()
{
#if defined(_WIN32) || defined(WIN32)
	return WSAGetLastError();
#else
	return errno;
#endif
}
//----------------------------------------------------------------------------
void NetError::Error()
{
	int err = LastError();
	std::string empty;
	Error(err, empty);
}
//----------------------------------------------------------------------------
void NetError::Error(const std::string& arg)
{
	Error(LastError(), arg);
}
//----------------------------------------------------------------------------
void NetError::Error(int code)
{
	std::string arg;
	Error(code, arg);
}
//----------------------------------------------------------------------------
void NetError::Error(int code, const std::string& arg)
{
	PX2_UNUSED(arg);

	switch (code)
	{
	case PX2_ESYSNOTREADY:
		assertion(false, "Net subsystem not ready:%d\n", code);
		break;
	case PX2_ENOTINIT:
		assertion(false, "Net subsystem not IsInitialized:%d\n", code);
		break;
	case PX2_EINTR:
		assertion(false, "Interrupted:%d\n", code);
		break;
	case PX2_EACCES:
		assertion(false, "Permission denied:%d\n", code);
		break;
	case PX2_EFAULT:
		assertion(false, "Bad address:%d\n", code);
		break;
	case PX2_EINVAL:
		assertion(false, "Invalid Argument exception:%d\n", code);
		break;
	case PX2_EMFILE:
		assertion(false, "Too many open files:%d\n", code);
		break;
	case PX2_EWOULDBLOCK:
		assertion(false, "Operation would block:%d\n", code);
		break;
	case PX2_EINPROGRESS:
		assertion(false, "Operation now in progress:%d\n", code);
		break;
	case PX2_EALREADY:
		assertion(false, "Operation already in progress:%d\n", code);
		break;
	case PX2_ENOTSOCK:
		assertion(false, "Socket operation attempted on non-socket:%d\n", code);
		break;
	case PX2_EDESTADDRREQ:
		assertion(false, "Destination address required:%d\n", code);
		break;
	case PX2_EMSGSIZE:
		assertion(false, "Message too long:%d\n", code);
		break;
	case PX2_EPROTOTYPE:
		assertion(false, "Wrong protocol type:%d\n", code);
		break;
	case PX2_ENOPROTOOPT:
		assertion(false, "Protocol not available:%d\n", code);
		break;
	case PX2_EPROTONOSUPPORT:
		assertion(false, "Protocol not supported:%d\n", code);
		break;
	case PX2_ESOCKTNOSUPPORT:
		assertion(false, "Socket type not supported:%d\n", code);
		break;
	case PX2_ENOTSUP:
		assertion(false, "Operation not supported:%d\n", code);
		break;
	case PX2_EPFNOSUPPORT:
		assertion(false, "Protocol GetFamily not supported:%d\n", code);
		break;
	case PX2_EAFNOSUPPORT:
		assertion(false, "Address GetFamily not supported:%d\n", code);
		break;
	case PX2_EADDRINUSE:
		assertion(false, "Address %s already in use:%d\n", arg.c_str(), code);
		break;
	case PX2_EADDRNOTAVAIL:
#if !defined (__IOS__)
		assertion(false, "Cannot assign requested address:%s,%d\n", arg.c_str(), code);
#endif
		break;
	case PX2_ENETDOWN:
		assertion(false, "Network is down:%d\n", code);
		break;
	case PX2_ENETUNREACH:
#if !defined (__IOS__)
		assertion(false, "Network is unreachable:%d\n", code);
#endif
		break;
	case PX2_ENETRESET:
		assertion(false, "Network dropped connection on reset:%d\n", code);
		break;
	case PX2_ECONNABORTED:
		assertion(false, "ConnectionAborted:%d\n", code);
		break;
	case PX2_ECONNRESET:
		assertion(false, "ConnectionReset:%d\n", code);
		break;
	case PX2_ENOBUFS:
		assertion(false, "No buffer space available:%d\n", code);
		break;
	case PX2_EISCONN:
		assertion(false, "Socket is already connected:%d\n", code);
		break;
	case PX2_ENOTCONN:
		assertion(false, "Socket is not connected:%d\n", code);
		break;
	case PX2_ESHUTDOWN:
		assertion(false, "Cannot send after socket shutdown:%d\n", code);
		break;
	case PX2_ETIMEDOUT:
		assertion(false, "Timeout");
		break;
	case PX2_ECONNREFUSED:
		assertion(false, "ConnectionRefused");
		break;
	case PX2_EHOSTDOWN:
		assertion(false, "Host is down:%s,%d", arg, code);
		break;
	case PX2_EHOSTUNREACH:
		//assertion(false, "No route to host:%s,%d", arg.c_str(), code);
		break;
	default:
		//assertion(false, "Error:%s,%d", arg.c_str(), code);
		break;
	}
}
//----------------------------------------------------------------------------
