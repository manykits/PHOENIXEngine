// PX2IPv4AddressImpl.hpp

#ifndef PX2IPV4ADDRESSIMPL_HPP
#define PX2IPV4ADDRESSIMPL_HPP

#include "PX2IPAddressImpl.hpp"

namespace PX2
{

	class IPv4AddressImpl : public IPAddressImpl
	{
	public:
		IPv4AddressImpl ();
		IPv4AddressImpl (const void* addr);
		IPv4AddressImpl (unsigned int prefix);
		virtual ~IPv4AddressImpl ();

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

		IPv4AddressImpl operator & (const IPv4AddressImpl& GetAddr) const;
		IPv4AddressImpl operator | (const IPv4AddressImpl& GetAddr) const;
		IPv4AddressImpl operator ^ (const IPv4AddressImpl& GetAddr) const;
		IPv4AddressImpl operator ~ () const;

		static IPv4AddressImpl* Parse(const std::string& addr);

	private:	
		IPv4AddressImpl (const IPv4AddressImpl& GetAddr);
		IPv4AddressImpl& operator = (const IPv4AddressImpl&);

		struct in_addr mAddr;
	};

}

#endif