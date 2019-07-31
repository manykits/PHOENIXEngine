// PX2Bluetooth.cpp

#include "PX2Bluetooth.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2Log.hpp"
#include "PX2StringHelp.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2GraphicsRoot.hpp"

#if defined(__ANDROID__)
#include "AppPlayJNI.hpp"
#elif defined (__IOS__)
#include "AppPlayIOS.hpp"
#endif

using namespace PX2;

PX2_IMPLEMENT_EVENT(BluetoothES);

//----------------------------------------------------------------------------
Bluetooth::Bluetooth()
{
	mType = T_MAX_TYPE;
	mIsDataProtocolHex = false;
	Clear();
}
//----------------------------------------------------------------------------
Bluetooth::~Bluetooth()
{
}
//----------------------------------------------------------------------------
void Bluetooth::SetType(Type t)
{
	mType = t;

	if (T_2 == mType)
	{
#if defined(__ANDROID__)
		BluetoothSetBle(false);
#elif defined (__IOS__)
        AppPlay::BluetoothSetBle(false);
#endif
	}
	else if (T_4 == mType)
	{
#if defined(__ANDROID__)
		BluetoothSetBle(true);
#elif defined (__IOS__)
        AppPlay::BluetoothSetBle(true);
#endif
	}
}
//----------------------------------------------------------------------------
Bluetooth::Type Bluetooth::GetType() const
{
	return mType;
}
//----------------------------------------------------------------------------
void Bluetooth::SetDataProtocolHex(bool isUseHex)
{
	mIsDataProtocolHex = isUseHex;

#if defined(__ANDROID__)
	BluetoothSetDataProtocolHex(isUseHex);
#elif defined (__IOS__)
    AppPlay::BluetoothSetDataProtocolHex(isUseHex);
#endif
}
//----------------------------------------------------------------------------
bool Bluetooth::IsDataProtocolHex() const
{
	return mIsDataProtocolHex;
}
//----------------------------------------------------------------------------
void Bluetooth::Clear()
{
	mSendUpdateSeconds = 0.0f;

	ClearCMDCallbacks();
	ClearScirptHandlers();

	mPairedDevices.clear();
	mRecvs.clear();
	mDiscoveryDevices.clear();

	mDiscoverFinishedSignal = 0;
	mConnectedSignal = 0;
	mConnectFailedSignal = 0;
	mDisconnectedSignal = 0;
}
//----------------------------------------------------------------------------
void Bluetooth::ClearSendReceives()
{
	mSendUpdateSeconds = 0.0f;
	mSendBufs.clear();
	mRecvs.clear();
}
//----------------------------------------------------------------------------
void Bluetooth::Update(float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

	{
		ScopedCS cs(&mRecvMutex);

		for (int i = 0; i < (int)mRecvs.size(); i++)
		{
			_OnReceive(mRecvs[i]);
		}
		mRecvs.clear();

		for (int i = 0; i < (int)mDiscoveryNewDevices.size(); i++)
		{
			std::string str = mDiscoveryNewDevices[i];
			mDiscoveryDevices.push_back(str);

			Event *ent = PX2_CREATEEVENTEX(BluetoothES, OnDisocveryNewDevice);
			ent->SetDataStr0(str);
			ent->SetData<std::string>(str);
			PX2_EW.BroadcastingLocalEvent(ent);

			PX2_GR.LogInfo("BleNewDevice:" + str);
		}
		mDiscoveryNewDevices.clear();

		if (1 == mDiscoverFinishedSignal)
		{
			Event *ent = PX2_CREATEEVENTEX(BluetoothES, OnDiscoveryFinished);
			PX2_EW.BroadcastingLocalEvent(ent);

			mDiscoverFinishedSignal = 0;
		}

		if (1 == mConnectedSignal)
		{
			Event *ent = PX2_CREATEEVENTEX(BluetoothES, OnConnected);
			PX2_EW.BroadcastingLocalEvent(ent);

			PX2_GR.LogInfo("BleOnConnected");

			mConnectedSignal = 0;
		}

		if (1 == mConnectFailedSignal)
		{
			Event *ent = PX2_CREATEEVENTEX(BluetoothES, OnConnectFailed);
			PX2_EW.BroadcastingLocalEvent(ent);


			PX2_GR.LogInfo("BleOnConnectFailed");

			mConnectFailedSignal = 0;
		}

		if (1 == mDisconnectedSignal)
		{
			Event *ent = PX2_CREATEEVENTEX(BluetoothES, OnDisConnected);
			PX2_EW.BroadcastingLocalEvent(ent);

			mDisconnectedSignal = 0;
		}
	}

	if (T_2 == mType)
	{
		if (0.0f == mSendUpdateSeconds)
		{
			auto it = mSendBufs.begin();
			if (it != mSendBufs.end())
			{
				mSendUpdateSeconds += elapsedSeconds;

				std::string sendStr = *it;

#if defined(__ANDROID__)
				BluetoothSend(sendStr.c_str(), (int)sendStr.length(), false);
#elif defined (__IOS__)
                AppPlay::BluetoothSend(sendStr.c_str(), (int)sendStr.length(), false);
#endif

				mSendBufs.erase(it);
				
				return;
			}
		}
		else
		{
			mSendUpdateSeconds += elapsedSeconds;
			if (mSendUpdateSeconds > 0.1f)
			{
				mSendUpdateSeconds = 0.0f;
			}
		}
	}
}
//----------------------------------------------------------------------------
bool Bluetooth::IsAvailable() const
{
#if defined(__ANDROID__)
	return BluetoothIsAvailable();
#elif defined (__IOS__)
    
#endif

	return false;
}
//----------------------------------------------------------------------------
bool Bluetooth::IsConnected() const
{
#if defined(__ANDROID__)
	return BluetoothIsConnected();
#elif defined(__IOS__)
    return AppPlay::BluetoothIsConnected();
#endif
	return false;
}
//----------------------------------------------------------------------------
std::vector<std::string> Bluetooth::GetPairedDevices()
{
	mPairedDevices.clear();

	std::string allDeviceStr;

#if defined(__ANDROID__)
	allDeviceStr = BluetoothGetPairedDevices();

	StringTokenizer token(allDeviceStr, ";");
	for (int i = 0; i < token.Count(); i++)
	{
		std::string deviceStr = token[i];
		mPairedDevices.push_back(deviceStr);
	}

#endif

	return mPairedDevices;
}
//----------------------------------------------------------------------------
int Bluetooth::GetNumPairedDevices() const
{
	return (int)mPairedDevices.size();
}
//----------------------------------------------------------------------------
std::string Bluetooth::GetPairedDevice(int i) const
{
	if (0 <= i && i < (int)mPairedDevices.size())
		return mPairedDevices[i];

	return "";
}
//----------------------------------------------------------------------------
void Bluetooth::SetWriteServiceID(const std::string &serviceID)
{
	mWriteServiceID = serviceID;

#if defined(__ANDROID__)
	BluetoothSetWriteServiceID(serviceID.c_str());
#elif defined (__IOS__)
    AppPlay::BluetoothSetWriteServiceID(serviceID.c_str());
#endif
}
//----------------------------------------------------------------------------
const std::string &Bluetooth::GetWriteServiceID() const
{
	return mWriteServiceID;
}
//----------------------------------------------------------------------------
void Bluetooth::SetWriteCharaID(const std::string &charaID)
{
	mWriteCharaID = charaID;

#if defined(__ANDROID__)
	BluetoothSetWriteCharaID(charaID.c_str());
#elif defined (__IOS__)
    AppPlay::BluetoothSetWriteCharaID(charaID.c_str());
#endif
}
//----------------------------------------------------------------------------
const std::string &Bluetooth::GetWriteCharaID() const
{
	return mWriteCharaID;
}
//----------------------------------------------------------------------------
void Bluetooth::SetReadServiceID(const std::string &serviceID)
{
	mReadServiceID = serviceID;

#if defined(__ANDROID__)
	BluetoothSetReadServiceID(serviceID.c_str());
#elif defined (__IOS__)
    AppPlay::BluetoothSetReadServiceID(serviceID.c_str());
#endif
}
//----------------------------------------------------------------------------
const std::string &Bluetooth::GetReadServiceID() const
{
	return mReadServiceID;
}
//----------------------------------------------------------------------------
void Bluetooth::SetReadCharaID(const std::string &charaID)
{
	mReadCharaID = charaID;

#if defined(__ANDROID__)
	BluetoothSetReadCharaID(charaID.c_str());
#elif defined (__IOS__)
    AppPlay::BluetoothSetReadCharaID(charaID.c_str());
#endif
}
//----------------------------------------------------------------------------
const std::string &Bluetooth::GetReadCharaID() const
{
	return mReadCharaID;
}
//----------------------------------------------------------------------------
void Bluetooth::DisConnect()
{
#if defined(__ANDROID__)
	BluetoothDisConnect();
#elif defined (__IOS__)
    AppPlay::BluetoothDisConnect();
#endif

	
}
//----------------------------------------------------------------------------
void Bluetooth::Connect(const std::string &addr)
{
	PX2_UNUSED(addr);

#if defined(__ANDROID__)
	BluetoothConnect(addr.c_str());
#elif defined (__IOS__)
    AppPlay::BluetoothConnect(addr.c_str());
#endif

	mTryConnectAddr = addr;
}
//----------------------------------------------------------------------------
void Bluetooth::ReConnect()
{
	if (!mLastConnecttedAddr.empty())
	{
		Connect(mLastConnecttedAddr);
	}
}
//----------------------------------------------------------------------------
void Bluetooth::Send(const std::string &str, bool withRead)
{
	PX2_UNUSED(withRead);

	int length = (int)str.length();
	if (str.length() >= 64)
	{
		PX2_LOG_INFO("bluetooth size bigger%d", length);
	}
	
	if (mType == T_2)
	{
		mSendBufs.push_back(str);
	}
	else
	{
#if defined(__ANDROID__)
		BluetoothSend(str.c_str(), (int)str.length(), withRead);
#elif defined (__IOS__)
        AppPlay::BluetoothSend(str.c_str(), (int)str.length(), withRead);
#endif
	}
}
//----------------------------------------------------------------------------
void Bluetooth::SendHex(const std::string &hexStr, bool withRead)
{
	PX2_LOG_INFO("Bluetooth SendHex:%d Str:%s", (int)hexStr.length(), 
		hexStr.c_str());

	char strSend[64];
	int length = 0;
	StringHelp::HexToStr(hexStr.c_str(), strSend, length);
	std::string str(strSend, length);

	Send(str, withRead);
}
//----------------------------------------------------------------------------
void Bluetooth::DoDiscovery()
{
	mDiscoveryDevices.clear();

#if defined(__ANDROID__)
	BluetoothDoDiscovery();
#elif defined (__IOS__)
    AppPlay::BluetoothDoDiscovery();
#endif
}
//----------------------------------------------------------------------------
void Bluetooth::CancelDiscovery()
{
#if defined(__ANDROID__)
	BluetoothCancelDiscovery();
#elif defined (__IOS__)
    AppPlay::BluetoothCancelDiscovery();
#endif
}
//----------------------------------------------------------------------------
void Bluetooth::OnConnected()
{
	ScopedCS cs(&mRecvMutex);
	mConnectedSignal = 1;

	mLastConnecttedAddr = mTryConnectAddr;
}
//----------------------------------------------------------------------------
void Bluetooth::OnConnectFailed()
{
	ScopedCS cs(&mRecvMutex);
	mConnectFailedSignal = 1;
}
//----------------------------------------------------------------------------
void Bluetooth::OnDisConnected()
{
	ScopedCS cs(&mRecvMutex);
	mDisconnectedSignal = 1;
	mRecvs.clear();

	PX2_LOG_INFO("BlueToothOnDisConnected");
}
//----------------------------------------------------------------------------
void Bluetooth::OnDisocveryNewDevice(const std::string &deviceStr)
{
	ScopedCS cs(&mRecvMutex);

	for (int i = 0; i < (int)mDiscoveryDevices.size(); i++)
	{
		if (deviceStr == mDiscoveryDevices[i])
			return;
	}

	mDiscoveryNewDevices.push_back(deviceStr);
}
//----------------------------------------------------------------------------
void Bluetooth::OnDiscoveryFinished()
{
	mDiscoverFinishedSignal = 1;
}
//----------------------------------------------------------------------------
void Bluetooth::OnReceive(const std::string &recvBuffer)
{
	PX2_LOG_INFO("Bluetooth Recvd:%s", recvBuffer.c_str());
	ScopedCS cs(&mRecvMutex);
	mRecvs.push_back(recvBuffer);
}
//----------------------------------------------------------------------------
void Bluetooth::ClearCMDCallbacks()
{
	mCallbacks.clear();
}
//----------------------------------------------------------------------------
bool Bluetooth::IsHasCMDCallback(BluetoothReceiveCallback callBack)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (callBack == mCallbacks[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Bluetooth::AddCMDCallback(BluetoothReceiveCallback callBack)
{
	if (IsHasCMDCallback(callBack))
		return;

	mCallbacks.push_back(callBack);
}
//----------------------------------------------------------------------------
void Bluetooth::RemoveCMDCallback(BluetoothReceiveCallback callback)
{
	auto it = mCallbacks.begin();
	for (; it != mCallbacks.end();)
	{
		if (*it == callback)
		{
			it = mCallbacks.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
void Bluetooth::ClearScirptHandlers()
{
	mScriptHandlers.clear();
}
//----------------------------------------------------------------------------
bool Bluetooth::IsHasScriptHandler(const std::string &scriptHandler)
{
	for (int i = 0; i < (int)mScriptHandlers.size(); i++)
	{
		if (scriptHandler == mScriptHandlers[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Bluetooth::AddScriptHandler(const std::string &scriptHandler)
{
	if (IsHasScriptHandler(scriptHandler))
		return;

	mScriptHandlers.push_back(scriptHandler);
}
//----------------------------------------------------------------------------
void Bluetooth::RemoveScriptHandler(const std::string &scriptHandler)
{
	auto it = mScriptHandlers.begin();
	for (; it != mScriptHandlers.end();)
	{
		if (scriptHandler == (*it))
		{
			it = mScriptHandlers.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
void Bluetooth::ClearScriptHandlersHex()
{
	mScriptHandlersHex.clear();
}
//----------------------------------------------------------------------------
bool Bluetooth::IsHasScriptHandlerHex(const std::string &scriptHandler)
{
	for (int i = 0; i < (int)mScriptHandlersHex.size(); i++)
	{
		if (scriptHandler == mScriptHandlersHex[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Bluetooth::AddScriptHandlerHex(const std::string &scriptHandler)
{
	if (IsHasScriptHandlerHex(scriptHandler))
		return;

	mScriptHandlersHex.push_back(scriptHandler);
}
//----------------------------------------------------------------------------
void Bluetooth::RemoveScriptHandlerHex(const std::string &scriptHandler)
{
	auto it = mScriptHandlersHex.begin();
	for (; it != mScriptHandlersHex.end();)
	{
		if (scriptHandler == (*it))
		{
			it = mScriptHandlersHex.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
int Bluetooth::Rssi100(int rssi)
{
	int signalStrength = rssi + 100;

	if (signalStrength < 0)
	{
		signalStrength = 0;
	}
	else if (signalStrength > 100)
	{
		signalStrength = 100;
	}

	return signalStrength;
}
//----------------------------------------------------------------------------
std::string dec2hex(int i, int width)
{
	std::stringstream ioss; //定义字符串流
	std::string s_temp; //存放转化后字符
	ioss << std::hex << i; //以十六制形式输出
	ioss >> s_temp;
	std::string s(width - s_temp.size(), '0'); //补0
	s += s_temp; //合并
	return s;
}
//----------------------------------------------------------------------------
void Bluetooth::BeginHex(const std::string &headVal)
{
	mHexStr = headVal;
	mHexAll.clear();
}
//----------------------------------------------------------------------------
void Bluetooth::AddHex(int val)
{
	mHexStr += dec2hex(val, 2);
	mHexAll.push_back(val);
}
//----------------------------------------------------------------------------
void Bluetooth::AddHex(int bit0, int bit1, int bit2, int bit3)
{
	int val = 0;

	if (bit0 > 0)
		val |= 0x01;
	else
		val &= ~0x01;

	if (bit1 > 0)
		val |= 0x02;
	else
		val &= ~0x02;

	if (bit2 > 0)
		val |= 0x04;
	else
		val &= ~0x04;

	if (bit3 > 0)
		val |= 0x08;
	else
		val &= ~0x08;

	AddHex(val);
}
//----------------------------------------------------------------------------
void Bluetooth::AddHex(int bit0, int bit1, int bit2, int bit3, int bit4,
	int bit5, int bit6, int bit7)
{
	int val = 0;

	if (bit0 > 0)
		val |= 0x01;
	else
		val &= ~0x01;

	if (bit1 > 0)
		val |= 0x02;
	else
		val &= ~0x02;

	if (bit2 > 0)
		val |= 0x04;
	else
		val &= ~0x04;

	if (bit3 > 0)
		val |= 0x08;
	else
		val &= ~0x08;

	if (bit4 > 0)
		val |= 0x10;
	else
		val &= ~0x10;

	if (bit5 > 0)
		val |= 0x20;
	else
		val &= ~0x20;
	
	if (bit6 > 0)
		val |= 0x40;
	else
		val &= ~0x40;

	if (bit7 > 0)
		val |= 0x80;
	else
		val &= ~0x80;

	AddHex(val);
}
//----------------------------------------------------------------------------
std::string Bluetooth::EndHexStr(bool isDoAddCal)
{
	if (isDoAddCal)
	{
		unsigned char valAll = 0;
		for (int i = 0; i < (int)mHexAll.size(); i++)
		{
			valAll += mHexAll[i];
		}
		AddHex(valAll);
	}

	return mHexStr;
}
//----------------------------------------------------------------------------
void Bluetooth::_OnReceive(const std::string &recvStr)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (mCallbacks[i])
			mCallbacks[i](recvStr);
	}

	for (int i = 0; i < (int)mScriptHandlers.size(); i++)
	{
		const std::string &scriptStr = mScriptHandlers[i];
		if (!scriptStr.empty())
		{
			PX2_SC_LUA->CallString(scriptStr + "(\"" + recvStr + "\")");
		}
	}

	if ((int)mScriptHandlersHex.size() > 0)
	{
		std::string hexBuf = StringHelp::StrToHex(recvStr);
		PX2_LOG_INFO("Bluetooth Recvd Hex:%s", hexBuf.c_str());

		for (int i = 0; i < (int)mScriptHandlersHex.size(); i++)
		{
			const std::string &scriptStrHex = mScriptHandlersHex[i];
			if (!scriptStrHex.empty())
			{
				std::string strBuf = scriptStrHex + "(\"" + hexBuf + "\")";
				PX2_SC_LUA->CallString(strBuf);
			}
		}
	}
}
//----------------------------------------------------------------------------
