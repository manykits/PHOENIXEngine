// PX2Arduino.cpp

#include "PX2Arduino.hpp"
#include "PX2Log.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2StringHelp.hpp"
#include "PX2UDPServer.hpp"
#include "PX2System.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Math.hpp"
#include "PX2System.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

const uint8_t START_BYTE = 0x12;
const uint8_t STOP_BYTE = 0x13;
const uint8_t ESCAPE_BYTE = 0x7D;

PX2_IMPLEMENT_RTTI(PX2, Object, Arduino);
PX2_IMPLEMENT_STREAM(Arduino);
PX2_IMPLEMENT_DEFAULT_STREAM(Object, Arduino);
PX2_IMPLEMENT_FACTORY(Arduino);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, Arduino);

//----------------------------------------------------------------------------
void _ArduinoProcessStr1(Arduino *ardu, std::string recvVal)
{
	StringTokenizer stk(recvVal, " ");
	std::string cmdStr;
	std::string paramStr;
	std::string paramStr1;
	std::string paramStr2;
	std::string paramStr3;
	std::string paramStr4;
	std::string paramStr5;
	std::string paramStr6;

	if (stk.Count() > 0)
		cmdStr = stk[0];
	if (stk.Count() > 1)
		paramStr = stk[1];
	if (stk.Count() > 2)
		paramStr1 = stk[2];
	if (stk.Count() > 3)
		paramStr2 = stk[3];
	if (stk.Count() > 4)
		paramStr3 = stk[4];
	if (stk.Count() > 5)
		paramStr4 = stk[5];
	if (stk.Count() > 6)
		paramStr5 = stk[6];
	if (stk.Count() > 7)
		paramStr6 = stk[7];

	if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DIST] == cmdStr)
	{
		float dist = StringHelp::StringToFloat(paramStr);
		ardu->_SetDist(dist);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DR] == cmdStr)
	{
		int pin = StringHelp::StringToInt(paramStr);
		int val = StringHelp::StringToInt(paramStr1);
		ardu->_SetPinVal((Arduino::Pin)(Arduino::P_0 + pin), val);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_AR] == cmdStr)
	{
		int pin = StringHelp::StringToInt(paramStr);
		int val = StringHelp::StringToInt(paramStr1);
		ardu->_SetPinVal((Arduino::Pin)(Arduino::P_0 + pin), val);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_IR] == cmdStr) 
	{
		int val = StringHelp::StringToInt(paramStr);
		ardu->_SetIRReceive(val);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETRUN_RC] == cmdStr)
	{
		int val = StringHelp::StringToInt(paramStr);
		ardu->_SetRCReceive(val);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_NETID] == cmdStr)
	{
		int val = StringHelp::StringToInt(paramStr);
		ardu->SetNetID(val);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_MOTOSPD] == cmdStr)
	{
		int speedL = StringHelp::StringToInt(paramStr);
		int dirValL = StringHelp::StringToInt(paramStr1);
		int speedR = StringHelp::StringToInt(paramStr2);
		int dirValR = StringHelp::StringToInt(paramStr3);

		ardu->_SetSpeedLR(speedL, dirValL, speedR, dirValR);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_HX711] == cmdStr)
	{
		int index = StringHelp::StringToInt(paramStr);
		float val = (float)StringHelp::StringToInt(paramStr1);
		ardu->_SetWeight(index, val);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DSTMAT] == cmdStr)
	{
		int index = StringHelp::StringToInt(paramStr);
		float val = (float)StringHelp::StringToInt(paramStr1);
		ardu->_SetDistMat(index, val);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_AXIS] == cmdStr)
	{
		unsigned long time = (unsigned long)atol(paramStr.c_str());
		float yAxis = StringHelp::StringToFloat(paramStr1);
		float xAxis = StringHelp::StringToFloat(paramStr2);
		float zAxis = StringHelp::StringToFloat(paramStr3);
		float pitch = StringHelp::StringToFloat(paramStr4);
		float roll = StringHelp::StringToFloat(paramStr5);
		float yaw = StringHelp::StringToFloat(paramStr6);

		//yAxis = 0.0f;
		//xAxis = 0.0f;
		//zAxis = 0.0f;
		//pitch = 0.0f;
		//roll = 0.0f;
		//yaw = 0.0f;
		ardu->_SetAxis(time, yAxis, xAxis, zAxis, pitch, roll, yaw);
	}
	else if (Arduino::sOptTypeStr[Arduino::OT_RETURN_DHTTEMP] == cmdStr)
	{
		float temp = StringHelp::StringToFloat(paramStr);
		float humi = StringHelp::StringToFloat(paramStr1);
		ardu->_SetTemperature(temp);
		ardu->_SetHumidity(humi);
	}
}
//----------------------------------------------------------------------------
void Arduino::_SetAxis(unsigned long timeMilliseconds, float yAxis, float xAxis, 
	float zAxis, float pitch, float roll, float yaw)
{
	mYAxis = yAxis;
	mXAxis = xAxis;
	mZAxis = zAxis;
	mPitch = pitch;
	mRoll = roll;
	mYaw = yaw;

	int iNum = (int)Mathf::Floor(Mathf::FAbs(mYaw) / 180.0f);
	if (mYaw >= 0.0f)
	{
		if (iNum % 2 == 0)
			mYaw = mYaw - iNum*180.0f;
		else
			mYaw = -(180.0f - (mYaw - iNum*180.0f));
	}
	else
	{
		if (iNum % 2 == 0)
			mYaw = mYaw + iNum*180.0f;
		else
			mYaw = 180.0f + (mYaw + iNum*180.0f);
	}

	AxisObj axisObj;
	axisObj.TStamp = mArduinoSetTime + Timestamp::TimeDiff(timeMilliseconds*Timespan::MILLISECONDS);
	axisObj.IsValied = true;
	axisObj.AX = mXAxis;
	axisObj.AY = mYAxis;
	axisObj.AZ = mZAxis;
	axisObj.P = mPitch;
	axisObj.R = mRoll;
	axisObj.Y = mYaw;
	mCurAxisObj = axisObj;
	mAxisObjs.push_back(axisObj);
	Timestamp::TimeVal t = axisObj.TStamp.EpochMicroseconds();

	if (mAxisObjs.size() > 60)
	{
		std::vector<AxisObj> axisObjVec;
		for (int i = 10; i < (int)mAxisObjs.size(); i++)
		{
			axisObjVec.push_back(mAxisObjs[i]);
		}
		mAxisObjs = axisObjVec;
	}
}
//----------------------------------------------------------------------------
void _Less4Process(Arduino *ardu, std::string recvVal)
{
	if (recvVal.empty())
		return;

	if (recvVal.length() < 4)
		return;

	std::string useStr = recvVal.substr(4);
	_ArduinoProcessStr1(ardu, useStr);
}
//----------------------------------------------------------------------------
void _Less4ProcessSerial(Serial *ser, std::string recvVal)
{
	Arduino *ardu = (Arduino*)(ser->GetUserDataPointer("Arduino"));
	_Less4Process(ardu, recvVal);
}
//----------------------------------------------------------------------------
void _Less4ProcessClientConnector(ClientConnector *cn, std::string recvVal)
{
	Arduino *ardu = (Arduino*)(cn->GetUserDataPointer("Arduino"));
	_Less4Process(ardu, recvVal);
}
//----------------------------------------------------------------------------
void _Less4ProcessBluetooth(std::string recvVal)
{
	Arduino *ardu = (Arduino*)(PX2_BLUETOOTH.GetUserDataPointer("Arduino"));
	_Less4Process(ardu, recvVal);
}
//----------------------------------------------------------------------------
std::string Arduino::PinStr[P_MAX_TYPE] = 
{
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	"13",
	"30",
	"31",
	"32",
	"33",
	"34",
	"35",
	"36"
};
//----------------------------------------------------------------------------
std::string Arduino::PModeStr[PM_MAX_TYPE] = 
{
	"0",
	"1"
};
//---------------------------------------------------------------------------
std::string Arduino::sOptTypeStr[OT_MAX_TYPE] =
{
	"100", //OT_TOGET_NETID
	"101", //OT_RETRUN_NETID
	"0",  //OT_PM
	"1",  //OT_DW
	"2",  //OT_AW
	"3",  //OT_RETURN_DR
	"4",  //OT_RETURN_AR
	"5",  //OT_SVR_I
	"6",  //OT_SVR_W
	"7",  //OT_DST_I
	"8",  //OT_DST_T
	"9",  //OT_RETURN_DIST
	"10", //OT_MOTO_I
	"11", //OT_MOTO_RUN
	"12", //OT_MOTO_RUNSIMPLE
	"13", //OT_MOTO_STOP
	"14", //OT_MOTO_I_SPD
	"15", //OT_RETURN_MOTOSPD
	"16", //OT_MOTO_I_DRIVER4567
	"17", //OT_MOTO_I_DRIVER298N
	"18", //OT_MP3_INIT
	"19", //OT_MP3_DO
	"20", //OT_MP3_PLAYFOLDER
	"21", //OT_MP3_SETVOLUME
	"24", //OT_IR_INIT
	"25", //OT_IR_SEND
	"26", //OT_RETURN_IR
	"27", //OT_HX711_I
	"28", //OT_HX711_TEST
	"29", //OT_RETURN_HX711
	"30", //OT_DSTMAT_I,
	"31", //OT_RETURN_DSTMAT,
	"32", //OT_AXIS_I,
	"33", //OT_RETURN_AXIS
	"34", //OT_SET_TIME
	"35", //OT_RC_INIT
	"36", //OT_RC_SEND
	"37", //OT_RETRUN_RC
	"38", //OT_DHT_I
	"39", //OT_RETURN_DHTTEMP
	"40", //OT_RETURN_DHTHUMI
	"41", //OT_LEDSTRIP_I
	"42", //OT_LEDSTRIP_SET
	"43", //OT_SEGMENT_INIT
	"44", //OT_SEGMENT_BRIGHTNESS
	"45", //OT_SEGMENT_CLEAR
	"46", //OT_SEGMENT_DISPLAY
	"47", // OT_LEDMATRIX_I
	"48", // OT_LEDMATRIX_BRIGHTNESS
	"49", // OT_LEDMATRIX_CLEARSCREEN
	"50", // OT_LEDMATRIX_LIGHTAT
	"51", // OT_STEPMOTO_I
	"52", // OT_STEPMOTO_ENABLE
	"53", // OT_STEPMOTO_DIR
	"54", // OT_STEPMOTO_STEP
	"150", //OT_MC_INTERNAL_LIGHT
	"151", //OT_MC_LIGHT
	"152", //OT_MC_SEGMENT
	"153", //OT_MC_MOTO
	"154", //OT_MC_DISTTEST
	"161", //OT_MB_MOTO
	"162", //OT_MB_SEND
	"163", //OT_MB_BUZZER
	"200" //OT_VERSION
};
//---------------------------------------------------------------------------
std::string Arduino::sDirectionTypeStr[DT_MAX_TYPE] =
{
	"0",
	"1",
	"2"
};
//---------------------------------------------------------------------------
std::string Arduino::sSimpleDirectionTypeStr[SDT_MAX_TYPE] =
{
	"0",
	"1",
	"2",
	"3",
	"4",
};
//---------------------------------------------------------------------------
void Arduino::_SetDist(float dist)
{
	if (dist > 500)
		return;

	mDist = dist;

	_OnCallback(ART_RECV_DIST, (int)mDist);

	mIsBlockLoopDoBreak = true;
	mBlockLoopUpdateTimes = 0;
}
//---------------------------------------------------------------------------
void Arduino::_SetIRReceive(int irReceive)
{
	mIRReceive = irReceive;

	_OnCallback(ART_RECV_IR, mIRReceive);
}
//---------------------------------------------------------------------------
void Arduino::_SetRCReceive(int val)
{
	mRCReceiveVal = val;

	_OnCallback(ART_RECV_RC, mRCReceiveVal);
}
//---------------------------------------------------------------------------
void Arduino::_SetHumidity(float humi)
{
	mDHTHumidity = humi;
}
//---------------------------------------------------------------------------
void Arduino::_SetTemperature(float temp)
{
	mDHTTemperature = temp;
}
//---------------------------------------------------------------------------
void Arduino::_SetPinVal(Pin pin, int val)
{
	if (0 <= pin && pin < P_MAX_TYPE)
		mPinValue[pin] = val;

	mIsBlockLoopDoBreak = true;
	mBlockLoopUpdateTimes = 0;
}
//---------------------------------------------------------------------------
void Arduino::_SetWeight(int index, float weight)
{
	if (0 <= index && index < 4)
	{
		mWeight[index] = weight;
	}

	mIsBlockLoopDoBreak = true;
	mBlockLoopUpdateTimes = 0;
}
//---------------------------------------------------------------------------
void Arduino::_SetDistMat(int index, float val)
{
	if (0 <= index && index < NUMDISTMAT)
	{
		mDistMat[index] = val;
	}
}
//---------------------------------------------------------------------------
void Arduino::_SetSpeedLR(int left, int dirL, int right, int dirR)
{
	mSpeed[0] = left;
	mSpeed[1] = right;
	mSpeedDir[0] = dirL;
	mSpeedDir[1] = dirR;

	//PX2_LOG_INFO("%d,%d¡ª%d,%d", left, dirL, right, dirR);
}
//---------------------------------------------------------------------------
Arduino::Arduino() :
mIsBlockLoopDoBreak(true),
mBlockLoopUpdateTimes(0),
mMode(M_BLUETOOTH),
mNetID(0),
mTargetRobotID(0),
mRobotUDPPort(0),
mTcpPort(9000),
mDist(0.0f),
mIRReceive(0),
mServer(0),
mYAxis(0.0f),
mXAxis(0.0f),
mZAxis(0.0f),
mPitch(0.0f),
mRoll(0.0f),
mYaw(0.0f),
mDHTTemperature(0.0f),
mDHTHumidity(0.0f),
mIsEverSetTime(false),
mEverSetTime(0.0f)
{
	mEndStr = "\n";

	mSerial.AddCMDCallback(_Less4ProcessSerial);
	mSerial.SetUserDataPointer("Arduino", this);

	Reset();

	mMutexThread = new0 Mutex();

	mIsThreadRun = true;
	mThread = new0 Thread();
	mThread->Start(*this);
}
//---------------------------------------------------------------------------
Arduino::~Arduino()
{
	if (mThread)
	{
		mIsThreadRun = false;
		System::SleepSeconds(0.2f);
		mThread = 0;
	}

	if (mMutexThread)
	{
		delete0(mMutexThread);
	}

	if (mSerial.IsOpened())
	{
		mSerial.Close();
	}

	if (mServer)
	{
		mServer->Shutdown();
		mServer = 0;
	}
}
//---------------------------------------------------------------------------
Arduino::Mode Arduino::GetMode() const
{
	return mMode;
}
//---------------------------------------------------------------------------
bool Arduino::AddArduinoReceiveCallbacks(ArduinoReceiveCallback callback)
{
	if (IsHasArduinoReceiveCallback(callback))
		return false;

	mCallbacks.push_back(callback);

	return true;
}
//---------------------------------------------------------------------------
bool Arduino::IsHasArduinoReceiveCallback(ArduinoReceiveCallback callback) const
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (callback == mCallbacks[i])
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool Arduino::AddScriptHandler(const std::string &handler)
{
	for (int i = 0; i < (int)mRecvHandlers.size(); i++)
	{
		if (handler == mRecvHandlers[i])
			return false;
	}

	mRecvHandlers.push_back(handler);
	return true;
}
//---------------------------------------------------------------------------
Serial &Arduino::GetSerial()
{
	return mSerial;
}
//---------------------------------------------------------------------------
bool Arduino::Initlize(Mode mode, const std::string &port, int baudrate)
{
	mMode = mode;

	Reset();

	if (M_SERIAL == mMode)
	{
		if (0 == mSerial.Open(port, baudrate))
		{
			mSerial.SetUserDataPointer("Arduino", this);
			mSerial.AddCMDCallback(_Less4ProcessSerial);

			return true;
		}
		else
		{
			PX2_LOG_ERROR("Serial open failed.");
			return false;
		}
	}
	else if (M_BLUETOOTH == mMode)
	{
		/*_*/
		PX2_BLUETOOTH.AddCMDCallback(_Less4ProcessBluetooth);
		PX2_BLUETOOTH.SetUserDataPointer("Arduino", this);
		PX2_LOG_INFO("Arduino Initlize BLUETOOTH");
	}

	return true;
}
//---------------------------------------------------------------------------
bool Arduino::InitlizeSocketUDP_Broadcast(int targetRobotID, int udpPort)
{
	Reset();

	mMode = M_SOCKETUDP_Broadcast;
	mTargetRobotID = targetRobotID;
	mRobotUDPPort = udpPort;

	return true;
}
//---------------------------------------------------------------------------
static int _DoConnect(ClientConnector *conn, const char *ip, int port, 
	const char *servername)
{
	if (conn->ConnectNB(ip, (int16_t)port) < 0)
	{
		printf("connect %s failed\n", servername);
		return -1;
	}
	else printf("connect %s begin...\n", servername);

	int times = 0;
	while (conn->GetConnectState() == CONNSTATE_TRYCONNECT)
	{
		if (times > 10)
			return - 1;

		times++;

		System::SleepSeconds(0.02f);
		if (conn->Update(0.0f) < 0)
		{
			printf("connect %s failed\n", servername);
			return -1;
		}
	}

	if (conn->GetConnectState() == CONNSTATE_INIT)
	{
		printf("connect %s failed\n", servername);
		return -1;
	}
	else printf("connected\n");

	return 0;
}
//---------------------------------------------------------------------------
void _ClientConnectorRecvCallback(ClientConnector *cn, 
	const std::string &recvStr)
{
	Arduino *ardu = (Arduino*)cn->GetUserDataPointer("Arduino");

	if (recvStr.empty())
		return;

	std::string cmdStr;
	int index = 0;
	for (index = 0; index < (int)recvStr.size(); index++)
	{
		char chara = recvStr[index];
		if ('\r' == chara)
		{
			continue;
		}
		else if ('\n' == chara)
		{
			if (!recvStr.empty())
			{
				_Less4ProcessClientConnector(cn, ardu->NetRecvStr);
				ardu->NetRecvStr.clear();
			}
		}
		else
		{
			ardu->NetRecvStr += chara;
		}
	}
}
//---------------------------------------------------------------------------
bool Arduino::InitlizeESPSocketTCP_Connector(const std::string &ip, int port)
{
	Reset();

	mMode = M_ESP_SOCKETTCP_Connector;

	if (mConnector)
	{
		mConnector->Disconnect();
		mConnector = 0;
	}

	mTcpIP = ip;
	mTcpPort = port;
	mConnector = new0 ClientConnector(2);
	mConnector->AddRecvCallback(_ClientConnectorRecvCallback);
	int connectRet = _DoConnect(mConnector, mTcpIP.c_str(), port, "");
	if (0 != connectRet)
	{
		PX2_LOG_INFO("DoConnect failed.");
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool Arduino::InitlizeSocketTCP_Connector(ClientConnector *clientConnector)
{
	Reset();

	mMode = M_SOCKETTCP_Connector;

	mConnector = clientConnector;

	return true;
}
//---------------------------------------------------------------------------
bool Arduino::InitlizeEmpty()
{
	Reset();

	mMode = M_EMPTY;

	return true;
}
//---------------------------------------------------------------------------
bool Arduino::IsInitlized()
{
	if (M_SERIAL == mMode)
	{
		return mSerial.IsOpened();
	}
	else if (M_BLUETOOTH == mMode)
	{
		return PX2_BLUETOOTH.IsConnected();
	}
	else if (M_ESP_SOCKETTCP_Connector == mMode)
	{
		return (mConnector->GetConnectState() == CONNSTATE_CONNECTED);
	}

	return true;
}
//---------------------------------------------------------------------------
void Arduino::Terminate()
{
	if (mSerial.IsOpened())
	{
		mSerial.Close();
	}

	if (mConnector)
	{
		mConnector->Disconnect();
		mConnector = 0;
	}
}
//---------------------------------------------------------------------------
void Arduino::Reset()
{
	mIsBlockLoopDoBreak = true;
	mBlockLoopUpdateTimes = 0;

	mDist = 0.0f;
	mIRReceive = 0;

	mDHTTemperature = 0.0f;
	mDHTHumidity = 0.0f;

	for (int i = 0; i < NUMSERVO; i++)
	{
		mLastServoValue[i] = -1;
	}

	for (int i = 0; i < P_MAX_TYPE; i++)
	{
		mPinValue[i] = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		mSpeed[i] = 0;
		mSpeedDir[0] = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		mWeight[i] = 0.0f;
	}

	for (int i = 0; i < NUMDISTMAT; i++)
	{
		mDistMat[i] = 0;
	}

	for (int i = 0; i < NumMaxStepMoto; i++)
	{
		mStepMotoEnable[i] = false;
		mStepMotoDir[i] = false;
		mStepMotoRunDelay[i] = 0;
	}

	Bluetooth *bth = Bluetooth::GetSingletonPtr();
	if (bth)
	{
		bth->SetUserDataPointer("Arduino", this);
		bth->AddCMDCallback(_Less4ProcessBluetooth);
	}

	mIsEverSetTime = false;
	mEverSetTime = 0.0f;
}
//----------------------------------------------------------------------------
void Arduino::_SetTime()
{
	std::string opStr = sOptTypeStr[OT_SET_TIME];
	std::string sendStr = opStr;

	_Send(sendStr + mEndStr);
	mArduinoSetTime = Timestamp();
}
//----------------------------------------------------------------------------
Arduino::AxisObj::AxisObj()
{
	IsValied = false;

	AX = 0.0f;
	AY = 0.0f;
	AZ = 0.0f;
	P = 0.0f;
	R = 0.0f;
	Y = 0.0f;
}
//----------------------------------------------------------------------------
Arduino::AxisObj::~AxisObj()
{
}
//---------------------------------------------------------------------------
std::vector<Arduino::AxisObj> &Arduino::GetAxisObjs()
{
	return mAxisObjs;
}
//---------------------------------------------------------------------------
Arduino::AxisObj Arduino::GetCurAxisObj()
{
	return mCurAxisObj;
}
//---------------------------------------------------------------------------
void Arduino::_OnCallback(ArduinoRecvType type, int value)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (mCallbacks[i])
		{
			mCallbacks[i](type, value);
		}
	}

	for (int i = 0; i < (int)mRecvHandlers.size(); i++)
	{
		if (!mRecvHandlers[i].empty())
		{
			PX2_SC_LUA->CallFunction(mRecvHandlers[i], this, (int)type);
		}
	}
}
//---------------------------------------------------------------------------
void Arduino::_OnToSendCallback(const std::string &str)
{
	for (int i = 0; i < (int)mToSendCallbacks.size(); i++)
	{
		if (mToSendCallbacks[i])
		{
			mToSendCallbacks[i](str);
		}
	}
}
//---------------------------------------------------------------------------
void Arduino::SendToGetNetID()
{
	std::string opStr = sOptTypeStr[OT_TOGET_NETID];
	std::string sendStr = opStr;

	_Send(sendStr + mEndStr);
}
//---------------------------------------------------------------------------
void Arduino::SetNetID(int id)
{
	mNetID = id;
}
//---------------------------------------------------------------------------
int Arduino::GetNetID() const
{
	return mNetID;
}
//---------------------------------------------------------------------------
void Arduino::Run()
{
	while (mIsThreadRun)
	{
		std::vector<std::string> sendCmds;
		{
			ScopedCS cs(mMutexThread);
			sendCmds = mThreadSend0;
			mThreadSend0.clear();
		}

		for (int i = 0; i < (int)sendCmds.size(); i++)
		{
			std::string &sendCmd = sendCmds[i];
			mSerial.Write(sendCmd);
		}
	}
}
//---------------------------------------------------------------------------
void Arduino::Update(float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

	if (mSerial.IsOpened())
	{
		mSerial.Update();

		if (!mIsEverSetTime)
		{
			mEverSetTime += elapsedSeconds;
			if (mEverSetTime > 2.0f)
			{
				_SetTime();
				mIsEverSetTime = true;
			}
		}
	}

	if (mConnector)
	{
		mConnector->Update(elapsedSeconds);

		if (CONNSTATE_CONNECTED == mConnector->GetConnectState())
		{
			if (!mIsEverSetTime)
			{
				_SetTime();
				mIsEverSetTime = true;
			}
		}
	}
}
//----------------------------------------------------------------------------
std::string Arduino::_Pin2Str(Pin pin)
{
	return PinStr[pin];
}
//----------------------------------------------------------------------------
std::string Arduino::_PinMode2Str(PMode pm)
{
	return PModeStr[pm];
}
//----------------------------------------------------------------------------
std::string Arduino::_Bool2Str(bool bVal)
{
	return bVal ? "1" : "0";
}
//----------------------------------------------------------------------------
std::string Arduino::_Int2Str(int val)
{
	return StringHelp::IntToString(val);
}
//----------------------------------------------------------------------------
std::string Arduino::_DirectionType2Str(DirectionType dt)
{
	int type = (int)dt;
	return _Int2Str(type);
}
//----------------------------------------------------------------------------
Arduino::Pin Arduino::_NetStr2Pin(const std::string &str)
{
	if ("0" == str)
		return Arduino::P_0;
	else if ("1" == str)
		return Arduino::P_1;
	else if ("2" == str)
		return Arduino::P_2;
	else if ("3" == str)
		return Arduino::P_3;
	else if ("4" == str)
		return Arduino::P_4;
	else if ("5" == str)
		return Arduino::P_5;
	else if ("6" == str)
		return Arduino::P_6;
	else if ("7" == str)
		return Arduino::P_7;
	else if ("8" == str)
		return Arduino::P_8;
	else if ("9" == str)
		return Arduino::P_9;
	else if ("10" == str)
		return Arduino::P_10;
	else if ("11" == str)
		return Arduino::P_11;
	else if ("12" == str)
		return Arduino::P_12;
	else if ("13" == str)
		return Arduino::P_13;
	else if ("A0" == str)
		return Arduino::P_A0;
	else if ("A1" == str)
		return Arduino::P_A1;
	else if ("A2" == str)
		return Arduino::P_A2;
	else if ("A3" == str)
		return Arduino::P_A3;
	else if ("A4" == str)
		return Arduino::P_A4;
	else if ("A5" == str)
		return Arduino::P_A5;
	else if ("A6" == str)
		return Arduino::P_A6;

	return Arduino::P_0;
}
//----------------------------------------------------------------------------
Arduino::PMode Arduino::_NetStr2PinMode(const std::string &str)
{
	if ("INPUT"==str || "input" == str)
		return Arduino::PM_INPUT;

	return Arduino::PM_OUTPUT;
}
//----------------------------------------------------------------------------
bool Arduino::_NetStr2Bool(const std::string &str)
{
	return str == "TRUE" ||str=="true" ? true : false;
}
//----------------------------------------------------------------------------
bool Arduino::_HighLow2Bool(const std::string &str)
{
	if ("HIGH" == str || "high" == str)
		return true;
	else if ("LOW" == str || "low" == str)
		return false;

	int intVal = StringHelp::StringToInt(str);
	if (intVal > 0)
		return true;

	return false;
}
//----------------------------------------------------------------------------
int Arduino::_NetStr2Int(const std::string &str)
{
	return StringHelp::StringToInt(str);
}
//----------------------------------------------------------------------------
float Arduino::_NetStr2Float(const std::string &str)
{
	return StringHelp::StringToFloat(str);
}
 //----------------------------------------------------------------------------
 Arduino::DirectionType Arduino::_NetStr2Dir(const std::string &str)
 {
	 if ("none" == str)
	 {
		 return DT_NONE;
	 }
	 if ("forward" == str)
	 {
		 return DT_FORWARD;
	 }
	 else if ("backward" == str)
	 {
		 return DT_BACKWARD;
	 }

	 return DT_NONE;
 }
 //----------------------------------------------------------------------------
 Arduino::SimpleDirectionType Arduino::_NetStr2SimpleDir(const std::string &str)
 {
	 if ("none" == str)
	 {
		 return SDT_NONE;
	 }
	 if ("go" == str)
	 {
		 return SDT_FORWARD;
	 }
	 else if ("back" == str)
	 {
		 return SDT_BACKWARD;
	 }
	 else if ("left" == str)
	 {
		 return SDT_LEFT;
	 }
	 else if ("right" == str)
	 {
		 return SDT_RIGHT;
	 }

	 return SDT_NONE;
 }
//----------------------------------------------------------------------------
std::string Arduino::_SimpleDirectionType2Str(SimpleDirectionType dt)
{
	int type = (int)dt;
	return _Int2Str(type);
}
//----------------------------------------------------------------------------
void Arduino::PinMode(Pin pin, PMode mode)
{
	std::string opStr = sOptTypeStr[OT_PM];
	std::string pinStr = _Pin2Str(pin);
	std::string valStr = _PinMode2Str(mode);

	std::string sendStr = opStr + " " + pinStr + " " + valStr;

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::DigitalWrite(Pin pin, bool isHigh)
{
	_SetPinVal(pin, isHigh ? 1 : 0);

	std::string opStr = sOptTypeStr[OT_DW];
	std::string pinStr = _Pin2Str(pin);
	std::string valStr = _Bool2Str(isHigh);

	std::string sendStr = opStr + " " + pinStr + " " + valStr;

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::AnalogWrite(Pin pin, int val)
{
	_SetPinVal(pin, val);

	std::string opStr = sOptTypeStr[OT_AW];
	std::string pinStr = _Pin2Str(pin);
	std::string valStr = _Int2Str(val);

	std::string sendStr = opStr + " " + pinStr + " " + valStr;

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
int Arduino::DigitalRead(Pin pin)
{
	std::string opStr = sOptTypeStr[OT_RETURN_DR];
	std::string pinStr = _Pin2Str(pin);

	std::string sendStr = opStr + " " + pinStr;

	_Send(sendStr + mEndStr);

	mIsBlockLoopDoBreak = false;

	while (!mIsBlockLoopDoBreak && mBlockLoopUpdateTimes<1000)
	{
		Update(0.001f);
		System::SleepSeconds(0.001f);
		mBlockLoopUpdateTimes++;

	}

	return mPinValue[pin]>0 ? 1:0;
}
//----------------------------------------------------------------------------
int Arduino::AnalogRead(Pin pin)
{
	std::string opStr = sOptTypeStr[OT_RETURN_AR];
	std::string pinStr = _Pin2Str(pin);

	std::string sendStr = opStr + " " + pinStr;

	_Send(sendStr + mEndStr);

	mIsBlockLoopDoBreak = false;

	while (!mIsBlockLoopDoBreak && mBlockLoopUpdateTimes < 1000)
	{
		Update(0.001f);
		System::SleepSeconds(0.001f);
		mBlockLoopUpdateTimes++;
	}

	return mPinValue[pin];
}
//----------------------------------------------------------------------------
void Arduino::VehicleInitMotoBoard()
{
	std::string sendStr = sOptTypeStr[OT_MOTO_I];

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::VehicleInitMotoBoard4567()
{
	std::string sendStr = sOptTypeStr[OT_MOTO_I_DRIVER4567];

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::VehicleInitMotoBoard298N(Pin pinLA, Pin pinLB, Pin pinLS, 
	Pin pinRA, Pin pinRB, Pin pinRS)
{
	std::string sendStr = sOptTypeStr[OT_MOTO_I_DRIVER298N];

	sendStr += " " + _Pin2Str(pinLA) + " " + _Pin2Str(pinLB) + ' ' + _Pin2Str(pinLS) +
		" " + _Pin2Str(pinRA) + " " + _Pin2Str(pinRB) + ' ' + _Pin2Str(pinRS);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::VehicleSpeedInit(Pin pinLA, Pin pinLB, Pin pinRA, Pin pinRB)
{
	std::string sendStr = sOptTypeStr[OT_MOTO_I_SPD];

	sendStr += " " + _Pin2Str(pinLA) + " " + _Pin2Str(pinLB) + 
		" " + _Pin2Str(pinRA) + " " + _Pin2Str(pinRB);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::Run(int motoIndex, DirectionType dt, int speed)
{
	if (0 <= motoIndex && motoIndex < 2)
	{
		std::string optStr = sOptTypeStr[OT_MOTO_RUN];
		std::string sendStr = optStr + " " + _Int2Str(motoIndex) + " " +
			_DirectionType2Str(dt) + " " +
			_Int2Str(speed);
		
		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
void Arduino::RunSpeed(int motoIndex, int speed)
{
	DirectionType type = DT_NONE;
	if (speed == 0)
	{
		type = DT_NONE;
	}
	else if (speed > 0)
	{
		type = DT_FORWARD;
	}
	else if (speed < 0)
	{
		type = DT_BACKWARD;
	}

	int spdVal = Math<int>::FAbs(speed);
	Run(motoIndex, type, spdVal);
}
//----------------------------------------------------------------------------
void Arduino::RunSpeed(int motoIndex, float speedReal, float radius,
	int reduction)
{
	float roundLength = Mathf::TWO_PI * radius;
	float numWheelRound = speedReal / roundLength;
	float numMotoRound = numWheelRound * (float)reduction;
	int numSpeedVal = (int)(numMotoRound * 2.0f);

	RunSpeed(motoIndex, numSpeedVal);
}
//----------------------------------------------------------------------------
void Arduino::Run(SimpleDirectionType dt, int speed)
{
	std::string optStr = sOptTypeStr[OT_MOTO_RUNSIMPLE];
	std::string sendStr = optStr + " " + 
		_SimpleDirectionType2Str(dt) + " " +
		_Int2Str(speed);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
int Arduino::GetSpeed(int motoIndex) const
{
	if (0 <= motoIndex && motoIndex<4)
		return mSpeed[motoIndex];

	return 0;
}
//----------------------------------------------------------------------------
int Arduino::GetSpeedDir(int motoIndex) const
{
	if (0 <= motoIndex && motoIndex < 4)
		return mSpeedDir[motoIndex];

	return 0;
}
//----------------------------------------------------------------------------
void Arduino::Stop()
{
	std::string sendStr = sOptTypeStr[OT_MOTO_STOP];

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::MP3Init(Pin pinR, Pin pinT)
{
	std::string opStr = sOptTypeStr[OT_MP3_INIT];
	std::string pinRStr = _Pin2Str(pinR);
	std::string pinTStr = _Pin2Str(pinT);

	std::string sendStr = opStr + " " + pinRStr + " " + pinTStr;

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::MP3DO(MP3PlayType mpt)
{
	std::string opStr = sOptTypeStr[OT_MP3_DO];

	std::string sendStr = opStr + " " + _Int2Str((int)mpt);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::MP3Play(int folder, int index)
{
	std::string opStr = sOptTypeStr[OT_MP3_PLAYFOLDER];

	std::string sendStr = opStr + " " + _Int2Str(folder) + 
		" " + _Int2Str(index);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::MP3SetVolume(int volume)
{
	std::string opStr = sOptTypeStr[OT_MP3_SETVOLUME];

	std::string sendStr = opStr + " " + _Int2Str(volume);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::IRInit(Pin pinR)
{
	mIRReceive = 0;

	std::string opStr = sOptTypeStr[OT_IR_INIT];
	std::string pinRStr = _Pin2Str(pinR);
	std::string sendStr = opStr + " " + pinRStr;

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::IRSend(int val)
{
	std::string opStr = sOptTypeStr[OT_IR_SEND];
	std::string sendStr = opStr + " " + _Int2Str(val);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::DistInit(Pin pinTrig, Pin pinEcho)
{
	std::string sendStr = sOptTypeStr[OT_DST_I];

	sendStr += " " + _Pin2Str(pinTrig) + " " + _Pin2Str(pinEcho);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::DistTest()
{
	std::string sendStr = sOptTypeStr[OT_DST_T];

	_Send(sendStr + mEndStr);

	mIsBlockLoopDoBreak = false;

	while (!mIsBlockLoopDoBreak && mBlockLoopUpdateTimes < 1000)
	{
		Update(0.001f);
		System::SleepSeconds(0.001f);
		mBlockLoopUpdateTimes++;
	}
}
//----------------------------------------------------------------------------
float Arduino::GetDist() const
{
	return mDist;
}
//----------------------------------------------------------------------------
void Arduino::ServoInit(int i, Pin pin)
{
	if (0 <= i && i < NUMSERVO)
	{
		std::string sendStr = sOptTypeStr[OT_SVR_I];
		sendStr += " " + _Int2Str(i) + " " + _Pin2Str(pin);

		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
void Arduino::ServoWrite(int i, int val)
{
	if (0 <= i && i < NUMSERVO)
	{
		if (val == mLastServoValue[i])
			return;

		mLastServoValue[i] = val;

		std::string sendStr = sOptTypeStr[OT_SVR_W] + " " +
			_Int2Str(i) + " " +
			_Int2Str(val);

		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
int Arduino::GetIRReceive() const
{
	return mIRReceive;
}
//----------------------------------------------------------------------------
void Arduino::WeightInit(int i, Pin pinOut, Pin pinClk)
{
	if (0 <= i && i < 4)
	{
		std::string sendStr = sOptTypeStr[OT_HX711_I];
		sendStr += " " + _Int2Str(i) + " " + _Pin2Str(pinOut) + 
			" " + _Pin2Str(pinClk);

		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
void Arduino::WeightTest(int i)
{
	if (0 <= i && i < 4)
	{
		std::string sendStr = sOptTypeStr[OT_HX711_TEST];
		sendStr += " " + _Int2Str(i);

		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
float Arduino::GetWeight(int i)
{
	if (0 <= i && i < 4)
	{
		return mWeight[i];
	}

	return 0.0f;
}
//----------------------------------------------------------------------------
void Arduino::DistMatInit(int i, Pin pinTrig, Pin pinEcho)
{
	std::string sendStr = sOptTypeStr[OT_DSTMAT_I];

	sendStr += _Int2Str(i) + " " + _Pin2Str(pinTrig) + " " 
		+ _Pin2Str(pinEcho);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
float Arduino::GetMatDist(int i) const
{
	if (0 <= i && i < NUMDISTMAT)
	{
		return mDistMat[i];
	}

	return 0.0f;
}
//----------------------------------------------------------------------------
void Arduino::DHTInit(Pin pin)
{
	mDHTHumidity = 0.0f;
	mDHTTemperature = 0.0f;

	std::string sendStr = sOptTypeStr[OT_DHT_I];

	sendStr += " " + _Pin2Str(pin);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
float Arduino::GetHumidity() const
{
	return mDHTHumidity;
}
//----------------------------------------------------------------------------
float Arduino::GetTemperature() const
{
	return mDHTTemperature;
}
//----------------------------------------------------------------------------
void Arduino::RGBLEDInit(Pin pin, int num)
{
	std::string sendStr = sOptTypeStr[OT_LEDSTRIP_I];

	sendStr += " " + _Pin2Str(pin) + " " + _Int2Str(num);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::RGBLEDSetColor(int index, int r, int g, int b)
{
	std::string sendStr = sOptTypeStr[OT_LEDSTRIP_SET];

	sendStr += " " + _Int2Str(index) +
		" " + _Int2Str(r) +
		" " + _Int2Str(g) + 
		" " + _Int2Str(b);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::SegmentInit(Pin clickPin, Pin dataPin)
{
	std::string sendStr = sOptTypeStr[OT_SEGMENT_I];

	sendStr += " " + _Pin2Str(clickPin) + " " + _Pin2Str(dataPin);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::SegmentSetBrightness(int brightness)
{
	std::string sendStr = sOptTypeStr[OT_SEGMENT_BRIGHTNESS];

	sendStr += " " + _Int2Str(brightness);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::SegmentClear()
{
	std::string sendStr = sOptTypeStr[OT_SEGMENT_CLEAR];

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::SegmentDisplayInt(int val)
{
	std::string sendStr = sOptTypeStr[OT_SEGMENT_DISPLAY];
	sendStr = sendStr + " " + "1" + " " + _Int2Str(val);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::SegmentDisplayFloat(float val)
{
	std::string sendStr = sOptTypeStr[OT_SEGMENT_DISPLAY];
	sendStr = sendStr + " " + "2" + " " + StringHelp::FloatToString(val, 4);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::LEDMatrixInit(Pin pinClk, Pin pinData)
{
	std::string sendStr = sOptTypeStr[OT_LEDMATRIX_I];

	sendStr += " " + _Pin2Str(pinClk) + " " + _Pin2Str(pinData);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::LEDMatrixSetBrightness(int brightness)
{
	std::string sendStr = sOptTypeStr[OT_LEDMATRIX_BRIGHTNESS];

	sendStr += " " + _Int2Str(brightness);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::LEDMatrixClearScreen()
{
	std::string sendStr = sOptTypeStr[OT_LEDMATRIX_CLEARSCREEN];
	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::LEDMatrixLightAt(int x, int y, int width, bool onOff)
{
	std::string sendStr = sOptTypeStr[OT_LEDMATRIX_LIGHTAT];
	sendStr = sendStr + " " + _Int2Str(x) +
		" " + _Int2Str(y) + 
		" " + _Int2Str(width) + 
		" " + _Bool2Str(onOff);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::StepMotoInit(int index, Pin pinVCC, Pin pincPLS, Pin pinDir,
	Pin pinEnable)
{
	if (0 <= index && index < NUMSTEPMOTO)
	{
		std::string sendStr = sOptTypeStr[OT_STEPMOTO_I];
		sendStr += " " + _Int2Str(index) + 
			" " + _Pin2Str(pinVCC) +
			" " + _Pin2Str(pincPLS) +
			" " + _Pin2Str(pinDir) +
			" " + _Pin2Str(pinEnable);

		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
void Arduino::StepMotoEnable(int index, bool enable)
{
	if (0 <= index && index < NUMSTEPMOTO)
	{
		if (mStepMotoEnable[index] == enable)
		{
			return;
		}

		mStepMotoEnable[index] = enable;

		std::string sendStr = sOptTypeStr[OT_STEPMOTO_ENABLE];
		sendStr = sendStr + " " + _Int2Str(index) +
			" " + _Bool2Str(enable);

		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
void Arduino::StepMotoDir(int index, bool forward)
{
	if (0 <= index && index < NUMSTEPMOTO)
	{
		if (mStepMotoDir[index] == forward)
		{
			return;
		}

		mStepMotoDir[index] = forward;


		std::string sendStr = sOptTypeStr[OT_STEPMOTO_DIR];
		sendStr = sendStr + " " + _Int2Str(index) +
			" " + _Bool2Str(forward);

		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
void Arduino::StepMotoStep(int index, int delay)
{
	if (0 <= index && index < NUMSTEPMOTO)
	{
		if (mStepMotoRunDelay[index] == delay)
		{
			return;
		}

		mStepMotoRunDelay[index] = delay;

		std::string sendStr = sOptTypeStr[OT_STEPMOTO_STEP];
		sendStr = sendStr + " " + _Int2Str(index) +
			" " + _Int2Str(delay);

		_Send(sendStr + mEndStr);
	}
}
//----------------------------------------------------------------------------
void Arduino::AxisInit()
{
	mYAxis = 0.0f;
	mXAxis = 0.0f;
	mZAxis = 0.0f; 
	mPitch = 0.0f;
	mRoll = 0.0f;
	mYaw = 0.0f;

	std::string sendStr = sOptTypeStr[OT_AXIS_I];

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
float Arduino::AxisGetYAxis() const
{
	return mYAxis;
}
//----------------------------------------------------------------------------
float Arduino::AxisGetXAxis() const
{
	return mXAxis;
}
//----------------------------------------------------------------------------
float Arduino::AxisGetZAxis() const
{
	return mZAxis;
}
//----------------------------------------------------------------------------
float Arduino::AxisGetPitch() const
{
	return mPitch;
}
//----------------------------------------------------------------------------
float Arduino::AxisGetRoll() const
{
	return mRoll;
}
//----------------------------------------------------------------------------
float Arduino::AxisGetYaw() const
{
	return mYaw;
}
//----------------------------------------------------------------------------
void Arduino::RCInit(Pin pin)
{
	std::string sendStr = sOptTypeStr[OT_RC_INIT];
	sendStr += " " + _Pin2Str(pin);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::RCSend(long val)
{
	std::string sendStr = sOptTypeStr[OT_RC_SEND];
	sendStr += " " + StringHelp::LongToString(val);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
int Arduino::GetRCReceive() const
{
	return mRCReceiveVal;
}
//----------------------------------------------------------------------------
bool Arduino::AddArduinoToSendCallback(ArduinoToSendCallback callback)
{
	if (IsHasArduinoToSendCallback(callback))
		return false;

	mToSendCallbacks.push_back(callback);

	return true;
}
//----------------------------------------------------------------------------
bool Arduino::IsHasArduinoToSendCallback(ArduinoToSendCallback callback) const
{
	for (int i = 0; i < (int)mToSendCallbacks.size(); i++)
	{
		if (callback == mToSendCallbacks[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
const std::string &Arduino::GetLastSendContentString() const
{
	return mLastSendContentString;
}
//----------------------------------------------------------------------------
const std::string &Arduino::GetLastSendString() const
{
	return 	mLastSendString;
}
//----------------------------------------------------------------------------
void Arduino::MCSegmentSet(int pin, int val)
{
	std::string sendStr = sOptTypeStr[OT_MC_SEGMENT];
	sendStr += " " + _Int2Str(pin) + " " + _Int2Str(val);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::MCMoto(int pin, int speed)
{
	std::string sendStr = sOptTypeStr[OT_MC_MOTO];
	sendStr += " " + _Int2Str(pin) + " " + _Int2Str(speed);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::MCTestDist(int pin)
{
	std::string sendStr = sOptTypeStr[OT_MC_DISTTEST];
	sendStr += " " + _Int2Str(pin);

	_Send(sendStr + mEndStr);
}
//----------------------------------------------------------------------------
void Arduino::MBotInit()
{

}
//----------------------------------------------------------------------------
void Arduino::MBotMoto(int leftRight, int speed)
{
	PX2_UNUSED(leftRight);
	PX2_UNUSED(speed);
}
//----------------------------------------------------------------------------
void Arduino::MBotIRSend(int val)
{
	PX2_UNUSED(val);
}
//----------------------------------------------------------------------------
void Arduino::MBotIRBuzzer(bool on)
{
	PX2_UNUSED(on);
}
//----------------------------------------------------------------------------
void Arduino::MBotIsButtonPressed() const
{
}
//----------------------------------------------------------------------------
int Arduino::MBotGetLightSensorValue() const
{
	return 0;
}
//----------------------------------------------------------------------------
void Arduino::_Send(const std::string &cmdStr)
{
	mLastSendContentString = cmdStr;
	std::string lastCmdStr = "0000" + cmdStr;
	mLastSendString = lastCmdStr;

	if (M_SERIAL == mMode)
	{
		if (mSerial.IsOpened())
		{
			ScopedCS cs(mMutexThread);
			mThreadSend0.push_back(lastCmdStr);
			//mSerial.Write(lastCmdStr);
		}
	}
	else if (M_BLUETOOTH == mMode)
	{
		PX2_BLUETOOTH.Send(lastCmdStr);
	}
	else if (M_SOCKETUDP_Broadcast == mMode)
	{
		int allLength = 2 + cmdStr.length();
		*(unsigned short *)(&lastCmdStr[0]) = (unsigned short)allLength; // length
		*(unsigned short *)(&lastCmdStr[2]) = (unsigned short)2; // GeneralServerMsgID

		if (0 != mRobotUDPPort)
		{
			SocketAddress sktAddr("255.255.255.255", (int16_t)mRobotUDPPort);
			std::string sendStr = _Int2Str(mRobotUDPPort) + "$" +
				std::string("rs") + "$" + _Int2Str(mTargetRobotID)
				+ "$" + lastCmdStr;

			DatagramSocket udpSocket;
			udpSocket.SetBroadcast(true);
			udpSocket.SendTo(sendStr.c_str(), sendStr.length(), sktAddr);
		}
	}
	else if (M_ESP_SOCKETTCP_Connector == mMode)
	{
		if (mConnector && !lastCmdStr.empty())
		{
			mConnector->GetSocket().SendBytes(lastCmdStr.c_str(), (int)lastCmdStr.length());
		}
	}
	else if (M_SOCKETTCP_Connector == mMode)
	{
		if (mConnector && !mLastSendContentString.empty())
		{
			mConnector->SendMsgToServerBuffer(Arduino_SocketTCP_MsgID,
				mLastSendContentString.c_str(),
				(int)mLastSendContentString.length());
			mConnector->Update(0.0f);
		}
	}

	_OnToSendCallback(lastCmdStr);
}
//----------------------------------------------------------------------------