// PX2DNS.hpp

#ifndef PX2DNS_HPP
#define PX2DNS_HPP

#include "PX2IPAddress.hpp"
#include "PX2HostEntry.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DNS
	{
	public:
		static HostEntry GetHostByName (const std::string &hostname);
		static HostEntry GetHostByAddress (const IPAddress &address);
		static HostEntry Resolve (const std::string &address);
		static IPAddress ResolveOne (const std::string &address);
		static HostEntry GetThisHost ();
		static std::string GetHostName ();

	protected:
		static int LastError ();
		static void Error (int code, const std::string& arg);
		static void Aierror (int code, const std::string& arg);
	};

}

#endif