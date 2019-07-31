// PX2Wifi.hpp

#ifndef PX2WIFI_HPP
#define PX2WIFI_HPP

#include "PX2UnityPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM WifiES)
	PX2_EVENT(OnDisocveryNewDevice)
	PX2_EVENT(OnDiscoveryFinished)
	PX2_EVENT(OnConnected)
	PX2_EVENT(OnDisConnected)
	PX2_EVENT(OnConnectFailed)
	PX2_DECLARE_EVENT_END(WifiES)

	class PX2_ENGINE_ITEM Wifi : public Singleton<Wifi>
	{
	public:
		Wifi();
		~Wifi();

		void Clear();
		void Update(float elapsedSeconds);

		void DisConnect();
		void Connect(const std::string &addr, const std::string &password);
		void ReConnect();
		const std::string &GetLastConnectedDeviceStr() const;
		bool IsConnected() const;

		void DoDiscovery();
		void CancelDiscovery();
		void OnDisocveryNewDevice(const std::string &deviceStr);
		void OnDiscoveryFinished();

		void OnConnected();
		void OnConnectFailed();
		void OnDisConnected();

		void ClearScirptHandlers();
		bool IsHasScriptHandler(const std::string &scriptHandler);
		void AddScriptHandler(const std::string &scriptHandler);
		void RemoveScriptHandler(const std::string &scriptHandler);

	protected:
		std::vector<std::string> mDiscoveryDevices;

		std::string mTryConnectAddr;
		std::string mLastConnecttedAddr;
		std::string mTryConnectPassword;
		std::string mLastConnectedPassword;

		Mutex mRecvMutex;
		std::vector<std::string> mDiscoveryNewDevices;
		std::vector<std::string> mRecvs;

		bool mIsConnected;
		int mConnectedSignal;
		int mConnectFailedSignal;
		int mDisconnectedSignal;
		int mDiscoverFinishedSignal;

		std::vector<std::string> mScriptHandlers;
	};

#define PX2_WIFI Wifi::GetSingleton()

}

#endif