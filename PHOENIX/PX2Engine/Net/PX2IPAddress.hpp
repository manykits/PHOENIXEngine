// PX2IPAddress.hpp

#ifndef PX2IPADDRESS_HPP
#define PX2IPADDRESS_HPP

#include "PX2NetPre.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2NetDefine.hpp"

namespace PX2
{

	class IPAddressImpl;

	class PX2_ENGINE_ITEM IPAddress
	{
	public:
		enum Family
		{
			IPv4,
			IPv6
		};

		IPAddress ();
		IPAddress (const IPAddress& GetAddr);
		explicit IPAddress (Family family);
		explicit IPAddress (const std::string &addr);
		IPAddress (const std::string& addr, Family family);
		IPAddress (const void* addr, px2_socklen_t length);
		IPAddress (const void* addr, px2_socklen_t length, int32_t scope);
		IPAddress (unsigned prefix, Family family);
		IPAddress (const struct sockaddr& sockaddr);
		~IPAddress ();

		IPAddress& operator = (const IPAddress& GetAddr);
		void Swap (IPAddress& address);

		bool operator == (const IPAddress& GetAddr) const;	
		bool operator != (const IPAddress& GetAddr) const;
		bool operator <  (const IPAddress& GetAddr) const;
		bool operator <= (const IPAddress& GetAddr) const;
		bool operator >  (const IPAddress& GetAddr) const;
		bool operator >= (const IPAddress& GetAddr) const;
		IPAddress operator & (const IPAddress& GetAddr) const;
		IPAddress operator | (const IPAddress& GetAddr) const;
		IPAddress operator ^ (const IPAddress& GetAddr) const;
		IPAddress operator ~ () const;

		Family GetFamily () const;
		int32_t GetScope () const;
		std::string ToString() const;
		bool IsWildcard () const;
		bool IsBroadcast () const;
		bool IsLoopback () const;
		bool IsMulticast () const;
		bool isUnicast () const;
		bool IsLinkLocal () const;
		bool IsSiteLocal () const;
		bool IsIPv4Compatible () const;
		bool IsIPv4Mapped () const;
		bool IsWellKnownMC () const;
		bool IsNodeLocalMC () const;
		bool IsLinkLocalMC () const;
		bool IsSiteLocalMC () const;
		bool IsOrgLocalMC () const;
		bool IsGlobalMC () const;

		px2_socklen_t GetAddrLength() const;

		const void* GetAddr() const;
		int GetAF() const;
		unsigned PrefixLength() const;

		void Mask(const IPAddress& mask);
		void Mask(const IPAddress& mask, const IPAddress& set);

		static IPAddress Parse (const std::string& addr);
		static bool TryParse (const std::string& addr, IPAddress& result);
		static IPAddress MakeWildcard (Family family = IPv4);
		static IPAddress MakeBroadcast ();

		enum
		{
			MAX_ADDRESS_LENGTH = 
#if defined(PX2_HAVE_IPV6)
			sizeof(struct in6_addr)
#else
			sizeof(struct in_addr)
#endif
		};

	protected:
		void Init (IPAddressImpl *impl);

	private:
		Pointer0<IPAddressImpl> mImpl;
	};

}

#endif