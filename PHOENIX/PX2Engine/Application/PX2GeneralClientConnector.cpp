// PX2GeneralClientConnector.cpp

#include "PX2GeneralClientConnector.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
bool GeneralClientConnector::sIsDownloadingFile = false;
//----------------------------------------------------------------------------
GeneralClientConnector::GeneralClientConnector() :
ClientConnector(10),
mIsServerConnected(false)
{
	RegisterHandler(GeneralClientConnectorMsgID, 
		(ServerMsgHandleFunc)&GeneralClientConnector::OnStringMsg);
}
//----------------------------------------------------------------------------
GeneralClientConnector::~GeneralClientConnector()
{
}
//----------------------------------------------------------------------------
int GeneralClientConnector::Update(float elapsedSeconds)
{
	int ret = ClientConnector::Update(elapsedSeconds);

	if (!mIsServerConnected)
	{
		if (CONNSTATE_CONNECTED == GetConnectState())
		{
			OnConnectedToServer();
			mIsServerConnected = true;
		}
	}
	else
	{
		if (CONNSTATE_CONNECTED != GetConnectState())
		{
			Disconnect();

			OnDisConnectedToServer();
			mIsServerConnected = false;
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
bool GeneralClientConnector::IsConnected() const
{
	return mIsServerConnected;
}
//----------------------------------------------------------------------------
void GeneralClientConnector::OnConnectedToServer()
{
	for (int i = 0; i < (int)mOnConnectCallbacks.size(); i++)
	{
		const std::string &strCallBack = mOnConnectCallbacks[i];
		if (!strCallBack.empty())
		{
			PX2_SC_LUA->CallFunction(strCallBack, this);
		}
	}
}
//----------------------------------------------------------------------------
void GeneralClientConnector::OnDisConnectedToServer()
{
	for (int i = 0; i < (int)mOnDisconnectCallbacks.size(); i++)
	{
		const std::string &strCallBack = mOnDisconnectCallbacks[i];
		if (!strCallBack.empty())
		{
			PX2_SC_LUA->CallFunction(strCallBack, this);
		}
	}
}
//----------------------------------------------------------------------------
void GeneralClientConnector::SendString(const std::string &str)
{
	if (!str.empty())
	{
		SendMsgToServerBuffer(GeneralClientConnectorMsgID, str.c_str(),
			(int)str.length());
	}
}
//----------------------------------------------------------------------------
void GeneralClientConnector::SendRawString(const std::string &str)
{
	if (!str.empty())
	{
		SendMsgToServerBuffer(GeneralClientConnectorMsgID, str.c_str(),
			(int)str.length());
	}
}
//----------------------------------------------------------------------------
bool GeneralClientConnector::IsHasOnConnectCallback(const std::string &strCallback) const
{
	for (int i = 0; i < (int)mOnConnectCallbacks.size(); i++)
	{
		const std::string &ck = mOnConnectCallbacks[i];
		if (ck == strCallback)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void GeneralClientConnector::AddOnConnectCallback(const std::string &callback)
{
	if (IsHasOnConnectCallback(callback))
		return;

	mOnConnectCallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
void GeneralClientConnector::RemoveOnConnectCallback(const std::string &callback)
{
	auto it = std::find(mOnConnectCallbacks.begin(), mOnConnectCallbacks.end(),
		callback);
	if (it != mOnConnectCallbacks.end())
	{
		mOnConnectCallbacks.erase(it);
	}
}
//----------------------------------------------------------------------------
bool GeneralClientConnector::IsHasOnDisConnectCallback(const std::string &strCallback) const
{
	for (int i = 0; i < (int)mOnDisconnectCallbacks.size(); i++)
	{
		const std::string &ck = mOnDisconnectCallbacks[i];
		if (ck == strCallback)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void GeneralClientConnector::AddOnDisconnectCallback(const std::string &callback)
{
	if (IsHasOnDisConnectCallback(callback))
		return;

	mOnDisconnectCallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
void GeneralClientConnector::RemoveOnDisconnectCallback(const std::string &callback)
{
	auto it = std::find(mOnDisconnectCallbacks.begin(), mOnDisconnectCallbacks.end(),
		callback);
	if (it != mOnDisconnectCallbacks.end())
	{
		mOnDisconnectCallbacks.erase(it);
	}
}
//----------------------------------------------------------------------------
bool GeneralClientConnector::IsHasOnRecvCallback(const std::string &strCallback) const
{
	for (int i = 0; i < (int)mOnRecvCallbacks.size(); i++)
	{
		const std::string &ck = mOnRecvCallbacks[i];
		if (ck == strCallback)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void GeneralClientConnector::AddOnRecvCallback(const std::string &callback)
{
	if (IsHasOnRecvCallback(callback))
		return;

	mOnRecvCallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
void GeneralClientConnector::RemoveOnRecvCallback(const std::string &callback)
{
	auto it = std::find(mOnRecvCallbacks.begin(), mOnRecvCallbacks.end(),
		callback);
	if (it != mOnRecvCallbacks.end())
	{
		mOnRecvCallbacks.erase(it);
	}
}
//----------------------------------------------------------------------------
int GeneralClientConnector::OnStringMsg(const void *pbuffer, int buflen)
{
	std::string strBuf((const char*)pbuffer, buflen);

	for (int i = 0; i < (int)mOnRecvCallbacks.size(); i++)
	{
		const std::string &strCallBack = mOnRecvCallbacks[i];
		if (!strCallBack.empty())
		{
			PX2_SC_LUA->CallFunction(strCallBack, this, strBuf);
		}
	}

	return 0;
}
//----------------------------------------------------------------------------