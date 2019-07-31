// PX2GeneralServer.cpp

#include "PX2GeneralServer.hpp"
#include "PX2Log.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
GeneralServerCallData::GeneralServerCallData() :
TheCallType(GSCT_MAX_TYPE),
ClientID(0)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
GeneralServer::GeneralServer(Server::ServerType serverType, int port,
	int numMaxConnects, int numMaxMsgHandlers) :
	Server(serverType, port, numMaxConnects, numMaxMsgHandlers)
{
	RegisterHandler(GeneralServerMsgID, (Server::MsgHandleFunc)
		&GeneralServer::OnString);
}
//----------------------------------------------------------------------------
GeneralServer::~GeneralServer()
{
}
//----------------------------------------------------------------------------
int GeneralServer::OnConnect(unsigned int clientid)
{
	ClientContext *context = GetClientContext(clientid);
	if (context)
	{
		SocketAddress addr = context->TheSocket.GetImpl()->GetPeerAddress();
		std::string ip = addr.ToString();

		GeneralServerCallData data;
		data.TheCallType = GSCT_ON_CONNECT;
		data.ClientID = clientid;
		data.strData = ip;

		for (int i = 0; i < (int)mCallbacks.size(); i++)
		{	
			mCallbacks[i](data);
		}

		for (int i = 0; i <(int)mOnConnectCallbacks.size(); i++)
		{
			const std::string &callback = mOnConnectCallbacks[i];
			if (!callback.empty())
			{
				PX2_SC_LUA->CallFunction(callback, this, clientid);
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int GeneralServer::OnDisconnect(unsigned int clientid)
{
	GeneralServerCallData data;
	data.TheCallType = GSCT_ON_DISCONNECT;
	data.ClientID = clientid;

	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		mCallbacks[i](data);
	}

	for (int i = 0; i < (int)mOnDisconnectCallbacks.size(); i++)
	{
		const std::string &callback = mOnDisconnectCallbacks[i];
		if (!callback.empty())
		{
			PX2_SC_LUA->CallFunction(callback, this, clientid);
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int GeneralServer::OnString(unsigned int clientid, const void *pbuffer, 
	int buflen)
{
	std::string strBuf((const char*)pbuffer, buflen);

	GeneralServerCallData data;
	data.TheCallType = GSCT_ON_RECVEDMSG;
	data.ClientID = clientid;
	data.strData = strBuf;

	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		mCallbacks[i](data);
	}

	for (int i = 0; i < (int)mOnRecvCallbacks.size(); i++)
	{
		const std::string &callback = mOnRecvCallbacks[i];
		if (!callback.empty())
		{
			PX2_SC_LUA->CallFunction(callback, this, clientid, strBuf);
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
void GeneralServer::SendString(int clientid, const std::string &str)
{
	if (!str.empty() && clientid > 0)
	{
		SendMsgToClientBuffer(clientid, GeneralServerMsgID, str.c_str(),
			str.length());
	}
}
//----------------------------------------------------------------------------
void GeneralServer::SendRawString(int clientid, const std::string &str)
{
	if (!str.empty() && clientid > 0)
	{
		SendMsgToClientRawBuffer(clientid, str.c_str(), str.length());
	}
}
//----------------------------------------------------------------------------
void GeneralServer::AddCallback(GeneralServerCallback callBack)
{
	mCallbacks.push_back(callBack);
}
//----------------------------------------------------------------------------
bool GeneralServer::IsHasOnConnectCallback(const std::string &strCallback) const
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
void GeneralServer::AddOnConnectCallback(const std::string &callback)
{
	if (IsHasOnConnectCallback(callback))
		return;

	mOnConnectCallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
void GeneralServer::RemoveOnConnectCallback(const std::string &callback)
{
	auto it = std::find(mOnConnectCallbacks.begin(), mOnConnectCallbacks.end(),
		callback);
	if (it != mOnConnectCallbacks.end())
	{
		mOnConnectCallbacks.erase(it);
	}
}
//----------------------------------------------------------------------------
bool GeneralServer::IsHasOnDisConnectCallback(const std::string &strCallback) const
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
void GeneralServer::AddOnDisconnectCallback(const std::string &callback)
{
	if (IsHasOnDisConnectCallback(callback))
		return;

	mOnDisconnectCallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
void GeneralServer::RemoveOnDisconnectCallback(const std::string &callback)
{
	auto it = std::find(mOnDisconnectCallbacks.begin(), mOnDisconnectCallbacks.end(),
		callback);
	if (it != mOnDisconnectCallbacks.end())
	{
		mOnDisconnectCallbacks.erase(it);
	}
}
//----------------------------------------------------------------------------
bool GeneralServer::IsHasOnRecvCallback(const std::string &strCallback) const
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
void GeneralServer::AddOnRecvCallback(const std::string &callback)
{
	if (IsHasOnRecvCallback(callback))
		return;

	mOnRecvCallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
void GeneralServer::RemoveOnRecvCallback(const std::string &callback)
{
	auto it = std::find(mOnRecvCallbacks.begin(), mOnRecvCallbacks.end(),
		callback);
	if (it != mOnRecvCallbacks.end())
	{
		mOnRecvCallbacks.erase(it);
	}
}
//----------------------------------------------------------------------------