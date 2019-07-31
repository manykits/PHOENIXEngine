// PX2IPAddressImpl.hpp

#ifndef PX2IPADDRESSIMPL_HPP
#define PX2IPADDRESSIMPL_HPP

#include "PX2NetPre.hpp"
#include "PX2NetDefine.hpp"
#include "PX2IPAddress.hpp"

namespace PX2
{

	class IPAddressImpl
	{
	public:
		IPAddressImpl ();
		virtual ~IPAddressImpl ();

		virtual std::string ToString() const = 0;
		virtual px2_socklen_t GetAddrLength() const = 0;
		virtual const void* GetAddr() const = 0;
		virtual IPAddress::Family GetFamily() const = 0;
		virtual int GetAF() const = 0;
		virtual int32_t GetScope() const = 0;

		virtual bool IsWildcard() const	= 0;
		virtual bool IsBroadcast() const = 0;
		virtual bool IsLoopback() const = 0;
		virtual bool IsMulticast() const = 0;
		virtual bool IsLinkLocal() const = 0;
		virtual bool IsSiteLocal() const = 0;
		virtual bool IsIPv4Mapped() const = 0;
		virtual bool IsIPv4Compatible() const = 0;
		virtual bool IsWellKnownMC() const = 0;
		virtual bool IsNodeLocalMC() const = 0;
		virtual bool IsLinkLocalMC() const = 0;
		virtual bool IsSiteLocalMC() const = 0;
		virtual bool IsOrgLocalMC() const = 0;
		virtual bool IsGlobalMC() const = 0;
		virtual void Mask(const IPAddressImpl* mask, const IPAddressImpl* set) = 0;
		virtual unsigned PrefixLength() const = 0;

		virtual IPAddressImpl* Clone() const = 0;

	private:
		IPAddressImpl(const IPAddressImpl&);
		IPAddressImpl& operator = (const IPAddressImpl&);
	};

}

#endif