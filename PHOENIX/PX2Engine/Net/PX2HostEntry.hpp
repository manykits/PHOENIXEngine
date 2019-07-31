// PX2HostEntry.hpp

#ifndef PX2HOSTENTRY_HPP
#define PX2HOSTENTRY_HPP

#include "PX2NetPre.hpp"
#include "PX2IPAddress.hpp"

namespace PX2
{

	/**
	* 存储“主机名”，别名，和IP地址列表
	*/
	class PX2_ENGINE_ITEM HostEntry
	{
	public:
		typedef std::vector<std::string> AliasList;
		typedef std::vector<IPAddress>   AddressList;

		HostEntry();
		HostEntry(struct hostent* entry);

		static void SetAndroidIPMac(std::string ip, std::string mac);
		static std::string GetAndroidIP();
		static std::string GetAndroidMac();

#if defined(PX2_HAVE_ADDRINFO)
		HostEntry(struct addrinfo* info);
#endif
		HostEntry(const HostEntry& entry);
		~HostEntry();

		HostEntry& operator = (const HostEntry& entry);
		void Swap(HostEntry& hostEntry);

		const std::string& GetName() const;
		const AliasList& GetAliases() const;
		const AddressList& GetAddresses() const;

	private:
		std::string mName;
		AliasList mAliases;
		AddressList mAddresses;

		static std::string mAndroidIP;
		static std::string mAndroidMac;
	};

}

#endif