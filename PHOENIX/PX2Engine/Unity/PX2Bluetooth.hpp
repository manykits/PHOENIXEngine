// PX2Bluetooth.hpp

#ifndef PX2BLUETOOTH_HPP
#define PX2BLUETOOTH_HPP

#include "PX2UnityPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2EventSystem.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM BluetoothES)
	PX2_EVENT(OnConnected)
	PX2_EVENT(OnDisConnected)
	PX2_EVENT(OnConnectFailed)
	PX2_EVENT(OnDisocveryNewDevice)
	PX2_EVENT(OnDiscoveryFinished)
	PX2_DECLARE_EVENT_END(BluetoothES)

	typedef void(*BluetoothReceiveCallback) (std::string recvVal);

	class PX2_ENGINE_ITEM Bluetooth : public Object, public Singleton<Bluetooth>
	{
	public:
		Bluetooth();
		~Bluetooth();

		enum Type
		{
			T_2,
			T_4,
			T_MAX_TYPE
		};
		void SetType(Type t);
		Type GetType() const;

		// Hex–≠“È
		void SetDataProtocolHex(bool isUseHex);
		bool IsDataProtocolHex() const;

		void Clear();
		void Update(float elapsedSeconds);

		void ClearSendReceives();

		bool IsAvailable() const;
		bool IsConnected() const;

		std::vector<std::string> GetPairedDevices();

		int GetNumPairedDevices() const;
		std::string GetPairedDevice(int i) const;

		void SetWriteServiceID(const std::string &serviceID);
		const std::string &GetWriteServiceID() const;
		void SetWriteCharaID(const std::string &charaID);
		const std::string &GetWriteCharaID() const;

		void SetReadServiceID(const std::string &serviceID);
		const std::string &GetReadServiceID() const;
		void SetReadCharaID(const std::string &charaID);
		const std::string &GetReadCharaID() const;

		void DisConnect();
		void Connect(const std::string &addr);
		void ReConnect();

		void Send(const std::string &str, bool withRead=false);
		void SendHex(const std::string &hexStr, bool withRead=false);
		
		void DoDiscovery();
		void CancelDiscovery();
		void OnDisocveryNewDevice(const std::string &deviceStr);
		void OnDiscoveryFinished();
		void OnConnected();
		void OnConnectFailed();
		void OnDisConnected();
		void OnReceive(const std::string &recvBuffer);

		void ClearCMDCallbacks();
		bool IsHasCMDCallback(BluetoothReceiveCallback callBack);
		void AddCMDCallback(BluetoothReceiveCallback callBack);
		void RemoveCMDCallback(BluetoothReceiveCallback callback);
		
		void ClearScirptHandlers();
		bool IsHasScriptHandler(const std::string &scriptHandler);
		void AddScriptHandler(const std::string &scriptHandler);
		void RemoveScriptHandler(const std::string &scriptHandler);

		void ClearScriptHandlersHex();
		bool IsHasScriptHandlerHex(const std::string &scriptHandler);
		void AddScriptHandlerHex(const std::string &scriptHandler);
		void RemoveScriptHandlerHex(const std::string &scriptHandler);

		int Rssi100(int rssi);

		void BeginHex(const std::string &headVal);
		void AddHex(int val);
		void AddHex(int bit0, int bit1, int bit2, int bit3);
		void AddHex(int bit0, int bit1, int bit2, int bit3, int bit4, int bit5, int bit6, int bit7);
		std::string EndHexStr(bool isDoAddCal);

	protected:
		void _OnReceive(const std::string &recvStr);

		Type mType;
		bool mIsDataProtocolHex;

		std::string mTryConnectAddr;
		std::string mLastConnecttedAddr;

		float mSendUpdateSeconds;
		std::vector<std::string> mSendBufs;

		std::vector<std::string> mPairedDevices;
		std::vector<std::string> mDiscoveryDevices;
		std::string mWriteServiceID;
		std::string mWriteCharaID;
		std::string mReadServiceID;
		std::string mReadCharaID;

		Mutex mRecvMutex;
		std::vector<std::string> mDiscoveryNewDevices;
		std::vector<std::string> mRecvs;

		int mConnectedSignal;
		int mConnectFailedSignal;
		int mDisconnectedSignal;
		int mDiscoverFinishedSignal;

		std::vector<BluetoothReceiveCallback> mCallbacks;
		std::vector<std::string> mScriptHandlers;
		std::vector<std::string> mScriptHandlersHex;

		std::string mHexStr;
		std::vector<int> mHexAll;
	};

#define PX2_BLUETOOTH Bluetooth::GetSingleton()

}

#endif
