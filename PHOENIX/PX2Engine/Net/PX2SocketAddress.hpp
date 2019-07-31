// PX2SocketAddress.hpp

#ifndef PX2SOCKETADDRESS_HPP
#define PX2SOCKETADDRESS_HPP

#include "PX2IPAddress.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class SocketAddressImpl;

	class PX2_ENGINE_ITEM SocketAddress
	{
	public:
		SocketAddress ();
		SocketAddress (const IPAddress &host, int16_t port);
		SocketAddress (int16_t port);
		SocketAddress (const std::string &host, int16_t port);
		SocketAddress (const std::string &host, const std::string& port);

		/// Examples:
		///     192.168.1.10:80
		///     [::ffff:192.168.1.120]:2040
		///     www.appinf.com:8080
		explicit SocketAddress (const std::string &hostAndPort);
		SocketAddress (const SocketAddress &addr);
		SocketAddress (const struct sockaddr *addr, px2_socklen_t length);
		~SocketAddress ();

		SocketAddress& operator = (const SocketAddress &addr);
		void Swap(SocketAddress &addr);

		IPAddress GetHost() const;
		int16_t GetPort() const;
		px2_socklen_t GetAddrLength() const;
		const struct sockaddr* GetAddr() const;
		int GetAF() const;
		std::string ToString() const;
		IPAddress::Family GetFamily() const;

		bool operator < (const SocketAddress& addr) const;
		bool operator == (const SocketAddress& addr) const;
		bool operator != (const SocketAddress& addr) const;

		enum
		{
			MAX_ADDRESS_LENGTH = 
#if defined(PX2_HAVE_IPV6)
			sizeof(struct sockaddr_in6)
#else
			sizeof(struct sockaddr_in)
#endif
		};

	protected:
		void Init (const IPAddress &host, int16_t port);
		void Init (const std::string &host, int16_t port);
		int16_t ResolveService(const std::string &service);

	private:
		Pointer0<SocketAddressImpl> mImpl;
	};

}

#endif