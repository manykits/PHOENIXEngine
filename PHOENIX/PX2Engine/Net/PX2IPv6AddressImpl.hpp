// PX2IPv6AddressImpl.hpp

#ifndef PX2IPV6ADDRESSIMPL_HPP
#define PX2IPV6ADDRESSIMPL_HPP

#include "PX2IPAddressImpl.hpp"

namespace PX2
{

	class IPv6AddressImpl : public IPAddressImpl
	{
	public:
		IPv6AddressImpl ();
		IPv6AddressImpl (const void* addr);
		IPv6AddressImpl (const void* addr, int32_t scope);
		IPv6AddressImpl (unsigned int prefix);
		virtual ~IPv6AddressImpl ();

		virtual std::string ToString() const;
		virtual px2_socklen_t GetAddrLength() const;
		virtual const void* GetAddr() const;
		virtual IPAddress::Family GetFamily() const;
		virtual int GetAF() const;
		virtual int32_t GetScope() const;
		virtual bool IsWildcard() const;
		virtual bool IsBroadcast() const;
		virtual bool IsLoopback() const;
		virtual bool IsMulticast() const;
		virtual bool IsLinkLocal() const;
		virtual bool IsSiteLocal() const;
		virtual bool IsIPv4Mapped() const;
		virtual bool IsIPv4Compatible() const;
		virtual bool IsWellKnownMC() const;
		virtual bool IsNodeLocalMC() const;
		virtual bool IsLinkLocalMC() const;
		virtual bool IsSiteLocalMC() const;
		virtual bool IsOrgLocalMC() const;
		virtual bool IsGlobalMC() const;
		virtual void Mask(const IPAddressImpl* mask, 
			const IPAddressImpl* set);
		virtual unsigned PrefixLength() const;

		virtual IPAddressImpl* Clone() const;

		IPv6AddressImpl operator & (const IPv6AddressImpl& GetAddr) const;
		IPv6AddressImpl operator | (const IPv6AddressImpl& GetAddr) const;
		IPv6AddressImpl operator ^ (const IPv6AddressImpl& GetAddr) const;
		IPv6AddressImpl operator ~ () const;

		static IPv6AddressImpl* Parse(const std::string& addr);

	private:
		IPv6AddressImpl(const IPv6AddressImpl& GetAddr);
		IPv6AddressImpl& operator = (const IPv6AddressImpl&);

		struct in6_addr mAddr;	
		int32_t mScope;
	};

}

#endif