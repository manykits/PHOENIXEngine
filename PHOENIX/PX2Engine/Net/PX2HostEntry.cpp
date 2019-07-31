// PX2HostEntry.cpp

#include "PX2HostEntry.hpp"
#include "PX2Assert.hpp"

#if defined __LINUX__
#include <net/if.h>
#endif

using namespace PX2;

//----------------------------------------------------------------------------
std::string HostEntry::mAndroidIP;
//----------------------------------------------------------------------------
std::string HostEntry::mAndroidMac;
//----------------------------------------------------------------------------
HostEntry::HostEntry()
{
}
//----------------------------------------------------------------------------
#if defined __LINUX__
//获取地址
//返回IP地址字符串
//返回：0=成功，-1=失败
int getlocalip(std::vector<std::string> &ips)
{
	int i = 0;
	int sockfd;
	struct ifconf ifconf;
	char buf[512];
	struct ifreq *ifreq;
	char* ip;
	//初始化ifconf
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		return -1;
	}

	ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息
	close(sockfd);

	//接下来一个一个的获取IP地址
	ifreq = (struct ifreq*)buf;
	for (i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; i--)
	{
		ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
		ips.push_back(ip);
		ifreq++;
	}

	return 0;
}
#endif
//----------------------------------------------------------------------------
HostEntry::HostEntry(struct hostent* entry)
{
	assertion(0!=entry, "entry must not be null.\n");

	mName = entry->h_name;	
	char** alias = entry->h_aliases;
	if (alias)
	{
		while (*alias)
		{
			mAliases.push_back(std::string(*alias));
			++alias;
		}
	}
#if defined __LINUX__
	char ipAddr[64];
	memset(ipAddr, 0, 64 * sizeof(char));
	std::vector<std::string > ips;
	getlocalip(ips);
	for (int i = 0; i < (int)ips.size(); i++)
	{
		std::string &ip = ips[i];
		mAddresses.push_back(IPAddress(ip));
	}

#else
	char** address = entry->h_addr_list;
	if (address)
	{
		while (*address)
		{
			mAddresses.push_back(IPAddress(*address, entry->h_length));
			++address;
		}
	}
#endif
}
//----------------------------------------------------------------------------
#if defined(PX2_HAVE_ADDRINFO)
HostEntry::HostEntry(struct addrinfo* ainfo)
{
	assertion(0!=ainfo, "ainfo must not be 0.\n");

	for (struct addrinfo* ai = ainfo; ai; ai = ai->ai_next)
	{
		if (ai->ai_canonname)
		{
			mName.assign(ai->ai_canonname);
		}
		if (ai->ai_addrlen && ai->ai_addr)
		{
			switch (ai->ai_addr->sa_family)
			{
			case AF_INET:
				mAddresses.push_back(IPAddress(
					&reinterpret_cast<struct sockaddr_in*>
					(ai->ai_addr)->sin_addr, sizeof(in_addr)));
				break;
#if defined(PX2_HAVE_IPV6)
			case AF_INET6:
				mAddresses.push_back(IPAddress(
					&reinterpret_cast<struct sockaddr_in6*>
					(ai->ai_addr)->sin6_addr, sizeof(in6_addr),
					reinterpret_cast<struct sockaddr_in6*>(ai->ai_addr)
					->sin6_scope_id));
				break;
#endif
			}
		}
	}
}
#endif
//----------------------------------------------------------------------------
HostEntry::HostEntry(const HostEntry& entry)
	:
mName(entry.mName),
mAliases(entry.mAliases),
mAddresses(entry.mAddresses)
{
}
//----------------------------------------------------------------------------
void HostEntry::SetAndroidIPMac(std::string ip, std::string mac)
{
	mAndroidIP = ip;
	mAndroidMac = mac;
}
//----------------------------------------------------------------------------
std::string HostEntry::GetAndroidIP()
{
	return mAndroidIP;
}
//----------------------------------------------------------------------------
std::string HostEntry::GetAndroidMac()
{
	return mAndroidMac;
}
//----------------------------------------------------------------------------
HostEntry& HostEntry::operator = (const HostEntry& entry)
{
	if (&entry != this)
	{
		mName = entry.mName;
		mAliases = entry.mAliases;
		mAddresses = entry.mAddresses;
	}

	return *this;
}
//----------------------------------------------------------------------------
void HostEntry::Swap(HostEntry& hostEntry)
{
	std::swap(mName, hostEntry.mName);
	std::swap(mAliases, hostEntry.mAliases);
	std::swap(mAddresses, hostEntry.mAddresses);
}
//----------------------------------------------------------------------------
HostEntry::~HostEntry()
{
}
//----------------------------------------------------------------------------
const std::string& HostEntry::GetName() const
{
	return mName;
}
//----------------------------------------------------------------------------
const HostEntry::AliasList& HostEntry::GetAliases() const
{
	return mAliases;
}
//----------------------------------------------------------------------------
const HostEntry::AddressList& HostEntry::GetAddresses() const
{
	return mAddresses;
}
//----------------------------------------------------------------------------