// PX2NetDefine.hpp

#ifndef PX2NETDEFINE_HPP
#define PX2NETDEFINE_HPP

#include "PX2NetPre.hpp"

#if defined(_WIN32) || defined(WIN32)

#define PX2_INVALID_SOCKET  INVALID_SOCKET
#define px2_socket_t        SOCKET
#define px2_socklen_t       int
#define px2_ioctl_request_t int
#define px2_closesocket(s)  closesocket(s)
#define PX2_EINTR           WSAEINTR
#define PX2_EACCES          WSAEACCES
#define PX2_EFAULT          WSAEFAULT
#define PX2_EINVAL          WSAEINVAL
#define PX2_EMFILE          WSAEMFILE
#define PX2_EAGAIN          WSAEWOULDBLOCK
#define PX2_EWOULDBLOCK     WSAEWOULDBLOCK
#define PX2_EINPROGRESS     WSAEINPROGRESS
#define PX2_EALREADY        WSAEALREADY
#define PX2_ENOTSOCK        WSAENOTSOCK
#define PX2_EDESTADDRREQ    WSAEDESTADDRREQ
#define PX2_EMSGSIZE        WSAEMSGSIZE
#define PX2_EPROTOTYPE      WSAEPROTOTYPE
#define PX2_ENOPROTOOPT     WSAENOPROTOOPT
#define PX2_EPROTONOSUPPORT WSAEPROTONOSUPPORT
#define PX2_ESOCKTNOSUPPORT WSAESOCKTNOSUPPORT
#define PX2_ENOTSUP         WSAEOPNOTSUPP
#define PX2_EPFNOSUPPORT    WSAEPFNOSUPPORT
#define PX2_EAFNOSUPPORT    WSAEAFNOSUPPORT
#define PX2_EADDRINUSE      WSAEADDRINUSE
#define PX2_EADDRNOTAVAIL   WSAEADDRNOTAVAIL
#define PX2_ENETDOWN        WSAENETDOWN
#define PX2_ENETUNREACH     WSAENETUNREACH
#define PX2_ENETRESET       WSAENETRESET
#define PX2_ECONNABORTED    WSAECONNABORTED
#define PX2_ECONNRESET      WSAECONNRESET
#define PX2_ENOBUFS         WSAENOBUFS
#define PX2_EISCONN         WSAEISCONN
#define PX2_ENOTCONN        WSAENOTCONN
#define PX2_ESHUTDOWN       WSAESHUTDOWN
#define PX2_ETIMEDOUT       WSAETIMEDOUT
#define PX2_ECONNREFUSED    WSAECONNREFUSED
#define PX2_EHOSTDOWN       WSAEHOSTDOWN
#define PX2_EHOSTUNREACH    WSAEHOSTUNREACH
#define PX2_ESYSNOTREADY    WSASYSNOTREADY
#define PX2_ENOTINIT        WSANOTINITIALISED
#define PX2_HOST_NOT_FOUND  WSAHOST_NOT_FOUND
#define PX2_TRY_AGAIN       WSATRY_AGAIN
#define PX2_NO_RECOVERY     WSANO_RECOVERY
#define PX2_NO_DATA         WSANO_DATA
#ifndef ADDRESS_FAMILY
#define ADDRESS_FAMILY USHORT
#endif

#elif defined(__LINUX__) || defined(__APPLE__) || defined(__ANDROID__)

#define PX2_INVALID_SOCKET  -1
#define px2_socket_t        int
#define px2_socklen_t       socklen_t
#define px2_fcntl_request_t int
#define px2_ioctl_request_t int
#define px2_closesocket(s)  close(s)
#define PX2_EINTR           EINTR
#define PX2_EACCES          EACCES
#define PX2_EFAULT          EFAULT
#define PX2_EINVAL          EINVAL
#define PX2_EMFILE          EMFILE
#define PX2_EAGAIN          EAGAIN
#define PX2_EWOULDBLOCK     EWOULDBLOCK
#define PX2_EINPROGRESS     EINPROGRESS
#define PX2_EALREADY        EALREADY
#define PX2_ENOTSOCK        ENOTSOCK
#define PX2_EDESTADDRREQ    EDESTADDRREQ
#define PX2_EMSGSIZE        EMSGSIZE
#define PX2_EPROTOTYPE      EPROTOTYPE
#define PX2_ENOPROTOOPT     ENOPROTOOPT
#define PX2_EPROTONOSUPPORT EPROTONOSUPPORT
#if defined(ESOCKTNOSUPPORT)
#define PX2_ESOCKTNOSUPPORT ESOCKTNOSUPPORT
#else
#define PX2_ESOCKTNOSUPPORT -1
#endif
#define PX2_ENOTSUP         ENOTSUP
#define PX2_EPFNOSUPPORT    EPFNOSUPPORT
#define PX2_EAFNOSUPPORT    EAFNOSUPPORT
#define PX2_EADDRINUSE      EADDRINUSE
#define PX2_EADDRNOTAVAIL   EADDRNOTAVAIL
#define PX2_ENETDOWN        ENETDOWN
#define PX2_ENETUNREACH     ENETUNREACH
#define PX2_ENETRESET       ENETRESET
#define PX2_ECONNABORTED    ECONNABORTED
#define PX2_ECONNRESET      ECONNRESET
#define PX2_ENOBUFS         ENOBUFS
#define PX2_EISCONN         EISCONN
#define PX2_ENOTCONN        ENOTCONN
#if defined(ESHUTDOWN)
#define PX2_ESHUTDOWN   ESHUTDOWN
#else
#define PX2_ESHUTDOWN   -2
#endif
#define PX2_ETIMEDOUT       ETIMEDOUT
#define PX2_ECONNREFUSED    ECONNREFUSED
#if defined(EHOSTDOWN)
#define PX2_EHOSTDOWN   EHOSTDOWN
#else
#define PX2_EHOSTDOWN   -3
#endif
#define PX2_EHOSTUNREACH    EHOSTUNREACH
#define PX2_ESYSNOTREADY    -4
#define PX2_ENOTINIT        -5
#define PX2_HOST_NOT_FOUND  HOST_NOT_FOUND
#define PX2_TRY_AGAIN       TRY_AGAIN
#define PX2_NO_RECOVERY     NO_RECOVERY
#define PX2_NO_DATA         NO_DATA

#endif

// È±Ê¡Ö§³ÖIPv6
#if !defined(PX2_NET_NO_IPV6) && !defined (PX2_HAVE_IPV6)
	#define PX2_HAVE_IPV6
#elif defined(PX2_NET_NO_IPV6) && defined (PX2_HAVE_IPV6)
	#undef PX2_HAVE_IPV6
#endif

#if !defined(s6_addr16)
	#if defined(_WIN32) || defined(WIN32)
		#define s6_addr16 u.Word
	#elif defined(__LINUX__) || defined(__APPLE__) || defined(__ANDROID__)
		#define s6_addr16 __u6_addr.__u6_addr16
	#endif
#endif

#if defined(PX2_HAVE_SALEN)
	#define px2_set_sa_len(sa, len)  (sa)->sa_len   = (len)
	#define px2_set_sin_len(sa)      (sa)->sin_len  = sizeof(struct sockaddr_in)
	#if defined(PX2_HAVE_IPV6)
		#define px2_set_sin6_len(sa) (sa)->sin6_len = sizeof(struct sockaddr_in6)
	#endif
#else
	#define px2_set_sa_len(sa, len) (void)0
	#define px2_set_sin_len(sa)     (void)0
	#define px2_set_sin6_len(sa)    (void)0
#endif

#endif