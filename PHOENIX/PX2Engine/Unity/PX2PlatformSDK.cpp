// PX2PlatformSDK.cpp

#include "PX2PlatformSDK.hpp"
#include "PX2PlatformSDKEventType.hpp"
#include "PX2Log.hpp"
#include "PX2Serial.hpp"
#include "PX2StringHelp.hpp"
#include "PX2APoint.hpp"
#include "PX2InputManager.hpp"
#if defined (__ANDROID__)
#include "AppPlayJNI.hpp"
#endif
using namespace PX2;

//----------------------------------------------------------------------------
LoginData::LoginData() :
Result(LR_MAXTYPE),
APIID(-1)
{
}
//----------------------------------------------------------------------------
LoginData::~LoginData()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
BuyData::BuyData() :
Result(BR_MAXTYPE),
IsSyn(true),
Error(0)
{
}
//----------------------------------------------------------------------------
BuyData::~BuyData()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
PlatformSDK::PlatformSDK() :
mRinGun_PosX(0),
mRinGun_PosY(0),
mRinGun_IsFiring(false),
mAcceleratorX(0.5f),
mAcceleratorY(0.5f),
mAcceleratorZ(0.5f)
{
}
//----------------------------------------------------------------------------
PlatformSDK::~PlatformSDK()
{
}
//----------------------------------------------------------------------------
void PlatformSDK::SetPlatformSDK(const std::string &thirdname)
{
	mPlatformSDK = thirdname;
}
//----------------------------------------------------------------------------
const std::string &PlatformSDK::GetPlatformSDK() const
{
	return mPlatformSDK;
}
//----------------------------------------------------------------------------
bool PlatformSDK::IsThirdPlatform() const
{
	return !mPlatformSDK.empty();
}
//----------------------------------------------------------------------------
void PlatformSDK::PlatformSDKLogin()
{
#ifdef __ANDROID__
	PlatformSDKLoginJNI();
#endif
}
//----------------------------------------------------------------------------
void PlatformSDK::PlatformSDKLogout()
{
#ifdef __ANDROID__
	PlatformSDKLogoutJNI();
#endif
}
//----------------------------------------------------------------------------
void PlatformSDK::OnLoginSuc(int apiID, std::string loginUin, std::string sessionID,
	std::string nickname)
{
	LoginData data;
	data.APIID = apiID;
	data.Result = LoginData::LR_SUC;
	data.LoginUin = loginUin;
	data.SessionID = sessionID;
	data.Nickname = nickname;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Login);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::OnLoginCancel()
{
	LoginData data;
	data.Result = LoginData::LR_CANCEL;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Login);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::OnLoginFailed()
{
	LoginData data;
	data.Result = LoginData::LR_FAILED;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Login);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::OnGuestOfficialSuc()
{
	LoginData data;
	data.Result = LoginData::LR_GUESTOFFICIAL;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Login);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::SynPay(
	const std::string &productID, const std::string &productName,
	float productPrice, float productOriginPrice, int count,
	const std::string &payDescription)
{
	PX2_LOG_INFO("SynPay ProductID:%s ProductName:%s ProductPrice:%.2f ProductOriginPrice:%.2f Count:%d PayDesc:%s",
		productID.c_str(), productName.c_str(), productPrice, productOriginPrice, count, payDescription.c_str());

#ifdef __ANDROID__
	PlatformSDKSynPayJNI(
		productID.c_str(), productName.c_str(),
		productPrice, productOriginPrice, count, payDescription.c_str());
#endif
}
//----------------------------------------------------------------------------
void PlatformSDK::ASynPay(
	const std::string &productID, const std::string &productName,
	float productPrice, float productOriginPrice, int count,
	const std::string &payDescription)
{
	PX2_LOG_INFO("SynPay ProductID:%s ProductName:%s ProductPrice:%.2f ProductOriginPrice:%.2f Count:%d PayDesc:%s",
		productID.c_str(), productName.c_str(), productPrice, productOriginPrice, count, payDescription.c_str());

#ifdef __ANDROID__
	PlatformSDKASynPayJNI(
		productID.c_str(), productName.c_str(),
		productPrice, productOriginPrice, count, payDescription.c_str());
#endif
}
//----------------------------------------------------------------------------
void PlatformSDK::OnPaySuc(const std::string &serial, bool isSyn)
{
	PX2_LOG_INFO("OnPaySuc Serial:%s IsSyn:%d", serial.c_str(), isSyn ? 1 : 0);

	BuyData data;
	data.Result = BuyData::BR_SUC;
	data.Serial = serial;
	data.IsSyn = isSyn;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Buy);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::OnPayCancel(const std::string &serial, bool isSyn)
{
	PX2_LOG_INFO("OnPayCancel Serial:%s IsSyn:%d", serial.c_str(), isSyn ? 1 : 0);

	BuyData data;
	data.Result = BuyData::BR_CANCEL;
	data.Serial = serial;
	data.IsSyn = isSyn;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Buy);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::OnPayFailed(const std::string &serial, bool isSyn)
{
	PX2_LOG_INFO("OnPayFailed Serial:%s IsSyn:%d", serial.c_str(), isSyn ? 1 : 0);

	BuyData data;
	data.Result = BuyData::BR_FAILED;
	data.Serial = serial;
	data.IsSyn = isSyn;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Buy);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::OnPaySMSSent(const std::string &serial, bool isSyn)
{
	PX2_LOG_INFO("OnPaySMSSent Serial:%s IsSyn:%d", serial.c_str(), isSyn ? 1 : 0);

	BuyData data;
	data.Result = BuyData::BR_SMSSENT;
	data.Serial = serial;
	data.IsSyn = isSyn;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Buy);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::OnPayRequestSubmitted(const std::string &serial,
	bool isSyn)
{
	PX2_LOG_INFO("OnPayRequestSubmitted Serial:%s IsSyn:%d", serial.c_str(), isSyn ? 1 : 0);

	BuyData data;
	data.Result = BuyData::BR_REQUESTSUBMITTED;
	data.Serial = serial;
	data.IsSyn = isSyn;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Buy);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::OnPayError(const std::string &serial, int error,
	bool isSyn)
{
	PX2_LOG_INFO("OnPayError Serial:%s Error:%d IsSyn:%d", serial.c_str(), error, isSyn ? 1 : 0);

	BuyData data;
	data.Result = BuyData::BR_ERROR;
	data.Serial = serial;
	data.Error = error;
	data.IsSyn = isSyn;

	Event *ent = PlatformSDKSpace::CreateEventX(PlatformSDKSpace::Buy);
	ent->SetData(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void _PlatformSDKSerialReceiveCallback(Serial *ser, std::string recvVal,
	int length)
{
	PX2_PFSDK.RinGun_OnReceive(recvVal);
}
//----------------------------------------------------------------------------
void PlatformSDK::TVShowCursor(bool show)
{
	Event *ent = PX2_CREATEEVENTEX(PlatformSDKSpace, TVShowCorsor);
	ent->SetData(show);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void PlatformSDK::RinGun_Open()
{
	Serial *serial = Serial::GetDefaultSerial();
	if (serial)
	{
		serial->AddCallback(_PlatformSDKSerialReceiveCallback);
	}

	Event *ent = PX2_CREATEEVENTEX(PlatformSDKSpace, TVGameOpen);
	PX2_EW.BroadcastingLocalEvent(ent);

	TVShowCursor(true);
}
//----------------------------------------------------------------------------
void PlatformSDK::RinGun_Close()
{
	Serial *serial = Serial::GetDefaultSerial();
	if (serial)
	{
		serial->RemoveCallback(_PlatformSDKSerialReceiveCallback);
	}

	Event *ent = PX2_CREATEEVENTEX(PlatformSDKSpace, TVGameClose);
	PX2_EW.BroadcastingLocalEvent(ent);

	TVShowCursor(false);
}
//----------------------------------------------------------------------------
void PlatformSDK::RinGun_OnReceive(const std::string &string)
{
	mRinGun_CMD = string;

	std::string xStrL = string.substr(2 + 0, 2);
	std::string xStrH = string.substr(4 + 0, 2);
	std::string xStr = xStrH+xStrL;
	int xVal = StringHelp::HexToInt(xStr);
	mRinGun_PosX = xVal;

	std::string yStrL = string.substr(6 + 0, 2);
	std::string yStrH = string.substr(8 + 0, 2);
	std::string yStr = yStrH + yStrL;
	int yVal = StringHelp::HexToInt(yStr);
	mRinGun_PosY = yVal;

	Event *ent = PX2_CREATEEVENTEX(PlatformSDKSpace, TVSetCorsorPos);
	APoint pos(mRinGun_PosX*0.0001f, 1.0f-mRinGun_PosY*0.0001f, 0.0f);
	ent->SetData(pos);
	PX2_EW.BroadcastingLocalEvent(ent);

	int addVal = 0;
	std::string strFire = string.substr(0 + addVal, 2);
	PX2_LOG_INFO("Fire!!!!!!!:%s", strFire.c_str());
	bool isFire = (StringHelp::StringToInt(strFire)!=0);
	if (isFire != mRinGun_IsFiring)
	{
		if (isFire)
		{
			Event *ent = PX2_CREATEEVENTEX(PlatformSDKSpace, TVSetCorsorFirePressed);
			ent->SetData(pos);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
		else
		{
			Event *ent = PX2_CREATEEVENTEX(PlatformSDKSpace, TVSetCorsorFireReleased);
			ent->SetData(pos);
			PX2_EW.BroadcastingLocalEvent(ent);
		}

		mRinGun_IsFiring = isFire;
	}
}
//----------------------------------------------------------------------------
std::string PlatformSDK::RinGUN_GetCMD() const
{
	return mRinGun_CMD;
}
//----------------------------------------------------------------------------
int PlatformSDK::RinGun_GetPosX() const
{
	return mRinGun_PosX;
}
//----------------------------------------------------------------------------
int PlatformSDK::RinGun_GetPosY() const
{
	return mRinGun_PosY;
}
//----------------------------------------------------------------------------
int PlatformSDK::RinGun_IsFiring() const
{
	return mRinGun_IsFiring;
}
//----------------------------------------------------------------------------
void PlatformSDK::StartAccelerator()
{
#ifdef __ANDROID__
	PlatformSDKStartAccelerator();
#endif
}
//----------------------------------------------------------------------------
void PlatformSDK::RegistAccelerator()
{
#ifdef __ANDROID__
	PlatformSDKRegistAccelerator();
#endif
}
//----------------------------------------------------------------------------
void PlatformSDK::UnRegistAccelerator()
{
#ifdef __ANDROID__
	PlatformSDKUnRegistAccelerator();
#endif
}
//----------------------------------------------------------------------------
void PlatformSDK::SetAccelerator(float x, float y, float z)
{
	mAcceleratorX = x;
	mAcceleratorY = y;
	mAcceleratorZ = z;
}
//----------------------------------------------------------------------------
float PlatformSDK::GetAcceleratorX() const
{
	return mAcceleratorX;
}
//----------------------------------------------------------------------------
float PlatformSDK::GetAcceleratorY() const
{
	return mAcceleratorY;
}
//----------------------------------------------------------------------------
float PlatformSDK::GetAcceleratorZ() const
{
	return mAcceleratorZ;
}
//----------------------------------------------------------------------------