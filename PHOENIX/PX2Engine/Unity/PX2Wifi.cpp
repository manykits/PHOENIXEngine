// PX2Wifi.hpp

#include "PX2Wifi.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Log.hpp"
using namespace PX2;

#if defined(__ANDROID__)
#include "AppPlayJNI.hpp"
#elif defined(__IOS__)
#include "AppPlayIOS.hpp"
#endif

using namespace PX2;

PX2_IMPLEMENT_EVENT(WifiES);

//----------------------------------------------------------------------------
Wifi::Wifi()
{
	Clear();

	mDiscoverFinishedSignal = 0;
}
//----------------------------------------------------------------------------
Wifi::~Wifi()
{
}
//----------------------------------------------------------------------------
void Wifi::Clear()
{
	ClearScirptHandlers();

	mIsConnected = false;

	mDiscoveryDevices.clear();

	mDiscoverFinishedSignal = 0;
	mConnectedSignal = 0;
	mConnectFailedSignal = 0;
	mDisconnectedSignal = 0;
}
//----------------------------------------------------------------------------
void Wifi::Update(float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

	{
		ScopedCS cs(&mRecvMutex);

		for (int i = 0; i < (int)mDiscoveryNewDevices.size(); i++)
		{
			std::string str = mDiscoveryNewDevices[i];
			if (!str.empty())
			{
				mDiscoveryDevices.push_back(str);

				Event *ent = PX2_CREATEEVENTEX(WifiES, OnDisocveryNewDevice);
				ent->SetDataStr0(str);
				ent->SetData<std::string>(str);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
		}
		mDiscoveryNewDevices.clear();

		if (1 == mDiscoverFinishedSignal)
		{
			Event *ent = PX2_CREATEEVENTEX(WifiES, OnDiscoveryFinished);
			PX2_EW.BroadcastingLocalEvent(ent);

			mDiscoverFinishedSignal = 0;
		}

		if (1 == mConnectedSignal)
		{
			Event *ent = PX2_CREATEEVENTEX(WifiES, OnConnected);
			PX2_EW.BroadcastingLocalEvent(ent);

			mConnectedSignal = 0;
		}

		if (1 == mConnectFailedSignal)
		{
			Event *ent = PX2_CREATEEVENTEX(WifiES, OnConnectFailed);
			PX2_EW.BroadcastingLocalEvent(ent);

			mConnectFailedSignal = 0;
		}

		if (1 == mDisconnectedSignal)
		{
			Event *ent = PX2_CREATEEVENTEX(WifiES, OnDisConnected);
			PX2_EW.BroadcastingLocalEvent(ent);

			mDisconnectedSignal = 0;
		}
	}
}
//----------------------------------------------------------------------------
void Wifi::DisConnect()
{
#if defined(__ANDROID__)
	WifiDisConnect();
#endif

	mLastConnecttedAddr = "";
	mLastConnectedPassword = "";

	mIsConnected = false;
	mDisconnectedSignal = 1;
}
//----------------------------------------------------------------------------
void Wifi::Connect(const std::string &addr, const std::string &password)
{
	mTryConnectAddr = addr;
	mTryConnectPassword = password;
	
	PX2_LOG_INFO("Wifi Connect:%s,%s", addr.c_str(), password.c_str());

#if defined(__ANDROID__)
	WifiConnect(mTryConnectAddr.c_str(), mTryConnectPassword.c_str());
#endif
}
//----------------------------------------------------------------------------
void Wifi::ReConnect()
{
	Connect(mLastConnecttedAddr, mLastConnectedPassword);
}
//----------------------------------------------------------------------------
const std::string &Wifi::GetLastConnectedDeviceStr() const
{
	return mLastConnecttedAddr;
}
//----------------------------------------------------------------------------
bool Wifi::IsConnected() const
{
#if defined (_WIN32) || defined (_WIN32)
    return true;
#elif defined (__ANDROID__)
    return mIsConnected;
#elif defined (__IOS__)
    AppPlay::WifiIsConnected();
#endif
    
	return mIsConnected;
}
//----------------------------------------------------------------------------
void Wifi::OnConnected()
{
	mLastConnecttedAddr = mTryConnectAddr;
	mTryConnectAddr = "";

	mLastConnectedPassword = mTryConnectPassword;
	mTryConnectPassword = "";

	mConnectedSignal = 1;

	mIsConnected = true;
}
//----------------------------------------------------------------------------
void Wifi::OnConnectFailed()
{
	mConnectFailedSignal = 1;

	mIsConnected = false;
}
//----------------------------------------------------------------------------
void Wifi::OnDisConnected()
{
	mDisconnectedSignal = 1;

	mIsConnected = false;
}
//----------------------------------------------------------------------------
void Wifi::DoDiscovery()
{
	mDiscoveryDevices.clear();

#if defined(__ANDROID__)
	WifiDoDiscovery();
#endif

	mDisconnectedSignal = true;
}
//----------------------------------------------------------------------------
void Wifi::CancelDiscovery()
{
#if defined(__ANDROID__)
	WifiCancelDiscovery();
#endif
}
//----------------------------------------------------------------------------
void Wifi::OnDisocveryNewDevice(const std::string &deviceStr)
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
void Wifi::OnDiscoveryFinished()
{
	mDiscoverFinishedSignal = 1;
}
//----------------------------------------------------------------------------
void Wifi::ClearScirptHandlers()
{
	mScriptHandlers.clear();
}
//----------------------------------------------------------------------------
bool Wifi::IsHasScriptHandler(const std::string &scriptHandler)
{
	for (int i = 0; i < (int)mScriptHandlers.size(); i++)
	{
		if (scriptHandler == mScriptHandlers[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Wifi::AddScriptHandler(const std::string &scriptHandler)
{
	if (IsHasScriptHandler(scriptHandler))
		return;

	mScriptHandlers.push_back(scriptHandler);
}
//----------------------------------------------------------------------------
void Wifi::RemoveScriptHandler(const std::string &scriptHandler)
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
