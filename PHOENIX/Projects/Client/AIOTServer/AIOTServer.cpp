
// AIOTServer.cpp

#include "AIOTServer.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2GeneralServer.hpp"
#include "PX2Log.hpp"
#include "AIOTUserManager.hpp"
#include "PX2JSONData.hpp"
#include "PX2StringTokenizer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AIOTServer::AIOTServer(Server::ServerType serverType, int port,
	int numMaxConnects, int numMaxMsgHandlers) :
	Server(serverType, port, numMaxConnects, numMaxMsgHandlers),
	mNextUID(1),
	mNextGroupID(0),
	mUpdateSeconds(0.0f)
{
	RegisterHandler(GeneralServerMsgID, (Server::MsgHandleFunc)&AIOTServer::OnString);
}
//----------------------------------------------------------------------------
AIOTServer::~AIOTServer()
{
}
//----------------------------------------------------------------------------
unsigned int AIOTServer::GetNextUID()
{
	return mNextUID++;
}
//----------------------------------------------------------------------------
void AIOTServer::Run(float elapsedTime)
{
	Server::Run(elapsedTime);

	mUpdateSeconds += elapsedTime;
	if (mUpdateSeconds > 1.0)
	{
		SendToAll("aiotheart");

		mUpdateSeconds = 0.0f;
	}
}
//----------------------------------------------------------------------------
int AIOTServer::OnProcessString(int clientid, const std::string &buf)
{
	int ret = 0;
	std::string cmd;
	StringTokenizer tokenizer(buf, " ");
	int count = tokenizer.Count();
	if (count > 0)
	{
		cmd = tokenizer[0];
		if ("quit" == cmd && count == 1)
		{
			ret = 1;
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
int AIOTServer::OnConnect(unsigned int clientid)
{
	PX2_LOG_INFO("client: %d connected", clientid);

	AIOTUser *player = V_PALYERM.AllocUser(clientid, GetNextUID());
	player->SetState(AUS_CONNECTED);

	std::string str("hello");
	SendToClient(clientid, str);

	return 0;
}
//----------------------------------------------------------------------------
void AIOTServer::SendToClient(unsigned int clientid, const std::string &str)
{
	SendMsgToClientBuffer(clientid, GeneralServerMsgID, str.c_str(),
		(int)str.length());
}
//----------------------------------------------------------------------------
void AIOTServer::SendToAll(const std::string &str)
{
	std::map<unsigned int, AIOTUser *> &map = V_PALYERM.GetClientMap();
	auto it = map.begin();
	for (; it != map.end(); it++) {
		unsigned int clientid = it->first;
		AIOTUser *user = it->second;

		SendToClient(clientid, str);
	}
}
//----------------------------------------------------------------------------
int AIOTServer::OnDisconnect(unsigned int clientid)
{
	PX2_LOG_INFO("client: %d disconnected", clientid);

	AIOTUser *usr = V_PALYERM.GetPlayerByClientID(clientid);
	if (usr)
	{
		if (0 != usr->AIOTUin) {
			AIOTUser *usrAIOT = V_PALYERM.GetPlayerByUIN(usr->AIOTUin);
			if (usrAIOT) {
				usrAIOT->RemoveAppUIN(usr->UIN);
			}
		}
		if (usr->APPUins.size() > 0)
		{
			for (int i = 0; i < usr->APPUins.size(); i++) {
				unsigned int appUIN = usr->APPUins[i];
				AIOTUser *usrAIOT = V_PALYERM.GetPlayerByUIN(appUIN);
				if (usrAIOT) {
					usrAIOT->AIOTUin = 0;
				}
			}
		}
	}

	V_PALYERM.FreeUser(usr);

	return 0;
}
//----------------------------------------------------------------------------
int AIOTServer::OnString(unsigned int clientid, const void *pbuffer,
	int buflen)
{
	AIOTUser *usr = V_PALYERM.GetPlayerByClientID(clientid);
	if (!usr)
		return 0;

	std::string str((char *)pbuffer, buflen);

	StringTokenizer stk(str, " ");
	int numCount = stk.Count();

	std::string from;
	std::string param0;
	std::string param1;
	std::string param2;
	std::string param3;
	std::string param4;
	std::string param5;

	if (numCount > 0) {
		from = stk[0];
	}
	if (numCount > 1) 
	{
		param0 = stk[1];
	}
	if (numCount > 2)
	{
		param1 = stk[2];
	}
	if (numCount > 3)
	{
		param2 = stk[3];
	}
	if (numCount > 4)
	{
		param3 = stk[4];
	}
	if (numCount > 5)
	{
		param4 = stk[5];
	}
	if (numCount > 6)
	{
		param5 = stk[6];
	}

	if ("app" == from)
	{
		bool isNeedRegenUIN = false;
		AIOTUser *aiotUser = 0;
		if (0 == usr->AIOTUin)
		{
			isNeedRegenUIN = true;
		}
		else
		{
			aiotUser = V_PALYERM.GetPlayerByUIN(usr->AIOTUin);
			if (!aiotUser)
			{
				isNeedRegenUIN = true;
			}
		}

		if ("connect" == param0) {
			if (isNeedRegenUIN)
			{
				std::string strKey = param1 + param2;
				FString key = FString(strKey.c_str());
				aiotUser = V_PALYERM.GetPlayerByUKey(key);
				if (aiotUser)
				{
					usr->AIOTUin = aiotUser->UIN;
					aiotUser->AddAppUIN(usr->UIN);
				}
			}
		}
		else {
			if (aiotUser) {
				PX2_LOG_INFO("SendToClient:%s", param0.c_str());
				unsigned int cleintID = aiotUser->ClientID;
				SendToClient(cleintID, param0);
			}
		}
	}
	else if ("aiot" == from)
	{
		if ("connect" == param0)
		{
			usr->Username_AIOT = param1;
			usr->Key_AIOT = param2;
			std::string strKey = usr->Username_AIOT + usr->Key_AIOT;
			usr->UKey = FString(strKey.c_str());

			PX2_LOG_INFO("aiot username:%s", param0.c_str());
			PX2_LOG_INFO("aiot key:%s", param1.c_str());
		}
		else if ("status" == param0)
		{
			std::string interp = param1;
			std::string perc = param2;
			std::string state = param3;
			std::string doorsate = param4;
			std::string err = param5;
			std::string sndStr = param0 + " " + param1 + " " + param2 + " " + param3 + " " + param4 + " " + param5;

			for (int i = 0; i < usr->APPUins.size(); i++) {
				unsigned int appUIN = usr->APPUins[i];
				AIOTUser *usr = V_PALYERM.GetPlayerByUIN(appUIN);
				if (usr) {
					//PX2_LOG_INFO("SendToClient:%s", sndStr.c_str());

					SendToClient(usr->ClientID, sndStr);
				}
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------