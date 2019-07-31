// PX2ApplicationUDPNetInfo.hpp

#ifndef PX2APPLICATIONUDPNETINFO_HPP
#define PX2APPLICATIONUDPNETINFO_HPP

#include "PX2CorePre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class UDPServer;

	class PX2_ENGINE_ITEM UDPNetInfo
	{
	public:
		UDPNetInfo();
		~UDPNetInfo();

		enum Type
		{
			T_DEVICE,
			T_CAMERA,
			T_MAX_TYPE
		};

		Type TheType;

		std::string Name;
		std::string IP;

		const std::string &GetName() const;
		const std::string &GetIP() const;

		int ClientID;
		bool IsConnected;

		float UpdateTiming;

		static void UDPServerRecvCallback(UDPServer *sever, SocketAddress &address,
			const std::string &buf, int length);
	};
	typedef Pointer0<UDPNetInfo> UDPNetInfoPtr;

}

#endif