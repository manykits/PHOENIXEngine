// PX2EngineServer.hpp

#include "PX2EngineServer.hpp"
#include "PX2EngineCanvas.hpp"
#include "PX2StringHelp.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2Application.hpp"
#include "PX2Dir.hpp"
#include "PX2EngineNetCmdProcess.hpp"
#include "PX2EngineNetEvent.hpp"
#include "PX2EngineNetDefine.hpp"
#include "PX2Log.hpp"
#include "PX2Robot.hpp"
#include "PX2RobotDatas.hpp"
#include "PX2OutStream.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
_ConnectObj::_ConnectObj()
{
	ClientID = 0;
	HeartTiming = 0.0f;
}
//----------------------------------------------------------------------------
_ConnectObj::~_ConnectObj()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
EngineServer::EngineServer(Server::ServerType serverType, int port) :
	Server(serverType, port, 10, 10),
	mBroadCastMapTiming(0.0f)
{
	RegisterHandler(EngineServerMsgID, (Server::MsgHandleFunc)
		&EngineServer::OnString);
	RegisterHandler(EngineServerArduinoMsgID, (Server::MsgHandleFunc)
		&EngineServer::OnArduinoString);
}
//----------------------------------------------------------------------------
EngineServer::~EngineServer()
{

}
//----------------------------------------------------------------------------
void EngineServer::Run(float elapsedTime)
{
	Server::Run(elapsedTime);

	auto it = mConnections.begin();
	for (; it != mConnections.end();)
	{
		_ConnectObj *obj = it->second;
		obj->HeartTiming += elapsedTime;

		if (obj->HeartTiming > EngineServerHeartTime)
		{
			obj->HeartTiming = 0.0f;

			int clientID = (int)it->first;
			DisconnectClient(it->first);
			it = mConnections.erase(it);

			PX2_LOG_INFO("Heart disconnect client %d", clientID);
		}
		else
		{
			it++;
		}
	}

	Robot *robot = Robot::GetSingletonPtr();
	if (robot)
	{
		mBroadCastMapTiming += elapsedTime;
		if (mBroadCastMapTiming > 0.1f)
		{
			Robot::RoleType rt = robot->GetRoleType();
			if (Robot::RT_MASTER == rt)
				BroadCastRobotMap();

			if (Robot::RT_MASTER_ONLY_SENDLIDAR == rt)
				BroadCastLidarData();

			mBroadCastMapTiming = 0;
		}
	}
}
//----------------------------------------------------------------------------
void EngineServer::SendString(int clientid, const std::string &str)
{
	if (!str.empty() && clientid > 0)
	{
		SendMsgToClientBuffer(clientid, EngineServerMsgID, str.c_str(),
			str.length());
	}
}
//----------------------------------------------------------------------------
void EngineServer::BroadCastString(const std::string &str)
{
	auto it = mConnections.begin();
	for (; it != mConnections.end(); it++)
	{
		unsigned int clientID = it->first;
		SendString(clientID, str);
	}
}
//----------------------------------------------------------------------------
void EngineServer::SendRobotMap(int clientid)
{
	RobotMapData *curMapData = PX2_ROBOT.GetCurMapData();
;
	RobotMapDataStruct stru = curMapData->MapStruct;
	SendMsgToClientBuffer(clientid, EngineServerSendMapInfoMsgID,
		(const char *)&stru, sizeof(stru));

	std::vector<unsigned char> mapDatas = curMapData->Map2DOrigin;
	std::vector<unsigned char> dstDatas;
	dstDatas.resize(mapDatas.size());
	memset(&dstDatas[0], 0, (int)dstDatas.size());

	unsigned long destLength = dstDatas.size();
	PX2_RM.ZipCompress(&(dstDatas[0]), &destLength, &(mapDatas[0]),
		mapDatas.size());

	int sendSize = 1024;
	int sendedSize = 0;
	int toSendSize = 0;
	int leftSize = destLength - sendedSize;
	while (leftSize > 0)
	{
		int thisTimeSendSize =  leftSize > sendSize ? sendSize : leftSize;
		SendMsgToClientBuffer(clientid, EngineServerSendMapMsgID,
			(const char *)&dstDatas[0] + sendedSize, thisTimeSendSize);
		
		sendedSize += thisTimeSendSize;
		leftSize = destLength - sendedSize;
	}

	std::string strEnd = "mapend";
	SendMsgToClientBuffer(clientid, EngineServerSendMapEndMsgID,
		strEnd.c_str(), strEnd.length());
}
//----------------------------------------------------------------------------
void EngineServer::BroadCastRobotMap()
{
	auto it = mConnections.begin();
	for (; it != mConnections.end(); it++)
	{
		unsigned int clientID = it->first;
		SendRobotMap(clientID);
	}
}
//----------------------------------------------------------------------------
void EngineServer::SendLidarData(int clientid)
{
	LiDar *liDar = PX2_ROBOT.GetLidar();
	if (!liDar)
		return;

	std::vector<RslidarDataComplete> lData = liDar->GetLiDarData();
	int size = lData.size();

	OutStream outStream;

	NetLidarData data;
	data.Datas = lData;

	int bufSize = 0;
	char *buf = 0;

	outStream.Insert(&data);
	outStream.Save(bufSize, buf);

	std::vector<unsigned char> dstDatas;
	dstDatas.resize(bufSize);
	memset(&dstDatas[0], 0, bufSize);

	unsigned long destLength = bufSize;
	PX2_RM.ZipCompress(&(dstDatas[0]), &destLength, (const unsigned char*)buf, bufSize);

	if (destLength > 0)
	{
		//PX2_LOG_INFO("try to send lidardata: %d", destLength);
		SendMsgToClientBuffer(clientid, EngineServerSendLidarMsgID,
			(const char *)&(dstDatas[0]), destLength);
		//PX2_LOG_INFO("end send lidardata");
	}

	delete1(buf);
}
//----------------------------------------------------------------------------
void EngineServer::BroadCastLidarData()
{
	auto it = mConnections.begin();
	for (; it != mConnections.end(); it++)
	{
		unsigned int clientID = it->first;
		SendLidarData(clientID);
	}
}
//----------------------------------------------------------------------------
int EngineServer::OnConnect(unsigned int clientid)
{
	ClientContext *clentContext = GetClientContext(clientid);
	std::string ip = clentContext->TheSocket.GetAddress().GetHost()
		.ToString();

	_ConnectObj *cntObj = new0 _ConnectObj();
	cntObj->ClientID = clientid;
	cntObj->IP = ip;
	mConnections[clientid] = cntObj;

	Event *ent = PX2_CREATEEVENTEX(EngineNetES, OnEngineServerBeConnected);
	ent->SetDataStr0(StringHelp::IntToString((int)clientid));
	ent->SetDataStr1(ip);
	PX2_EW.BroadcastingLocalEvent(ent);

	return 0;
}
//----------------------------------------------------------------------------
int EngineServer::OnDisconnect(unsigned int clientid)
{
	_ConnectObj *obj = mConnections[clientid];
	std::string ip = obj->IP;
	mConnections.erase(clientid);

	Event *ent = PX2_CREATEEVENTEX(EngineNetES, OnEngineServerBeDisConnected);
	ent->SetDataStr0(StringHelp::IntToString((int)clientid));
	ent->SetDataStr1(ip);
	PX2_EW.BroadcastingLocalEvent(ent);

	return 0;
}
//----------------------------------------------------------------------------
int EngineServer::OnString(unsigned int clientid, const void *pbuffer,
	int buflen)
{
	std::string strBuf((const char*)pbuffer, buflen);

	EngineCanvas *engineCanvas = EngineCanvas::GetSingletonPtr();
	if (engineCanvas)
	{
		UIList *list = engineCanvas->GetEngineInfoList();
		list->AddItem("OnString clientid:" + StringHelp::IntToString(clientid) +
			" " + "Str:" + strBuf);
	}

	StringTokenizer stk(strBuf, " ");
	int numTok = stk.Count();
	
	std::string cmdStr;
	std::string paramStr0;
	std::string paramStr1;
	if (numTok >= 1)
		cmdStr = stk[0];
	if (numTok >= 2)
		paramStr0 = stk[1];
	if (numTok >= 3)
		paramStr1 = stk[2];

	if ("heart" == cmdStr)
	{
		mConnections[clientid]->HeartTiming = 0.0f;
	}

	EngineNetCmdProcess::OnCmd(mConnections[clientid]->IP, 
		cmdStr, paramStr0, paramStr1);

	return 0;
}
//----------------------------------------------------------------------------
int EngineServer::OnArduinoString(unsigned int clientid, 
	const void *pbuffer, int buflen)
{
	std::string strBuf((const char*)pbuffer, buflen);

	Robot *robot = Robot::GetSingletonPtr();
	if (robot)
	{
		Arduino *arduino = robot->GetArduino();
		if (arduino)
		{
			arduino->_Send(strBuf);
		}
	}

	return 0;
}
//----------------------------------------------------------------------------