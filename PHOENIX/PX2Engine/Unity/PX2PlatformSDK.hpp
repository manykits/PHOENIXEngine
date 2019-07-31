// PX2PlatformSDK.hpp

#ifndef PX2PLATFORMSDK_HPP
#define PX2PLATFORMSDK_HPP

#include "PX2CorePre.hpp"
#include "PX2Singleton.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LoginData
	{
	public:
		LoginData();
		~LoginData();

		enum LoginResult
		{
			LR_SUC,
			LR_CANCEL,
			LR_FAILED,
			LR_GUESTOFFICIAL,
			LR_MAXTYPE
		};

		LoginResult Result;

		int APIID;
		std::string LoginUin;
		std::string SessionID;
		std::string Nickname;
	};

	class PX2_ENGINE_ITEM BuyData
	{
	public:
		BuyData();
		~BuyData();

		enum BuyResult
		{
			BR_SUC,
			BR_CANCEL,
			BR_FAILED,
			BR_SMSSENT,
			BR_REQUESTSUBMITTED,
			BR_ERROR,
			BR_MAXTYPE
		};

		BuyResult Result;
		std::string Serial;
		bool IsSyn;
		int Error;
	};

	class PX2_ENGINE_ITEM PlatformSDK
	{
		PX2_SINGLETION(PlatformSDK);

	public:
		void SetPlatformSDK(const std::string &thirdname);
		const std::string &GetPlatformSDK() const;
		bool IsThirdPlatform() const;

		void PlatformSDKLogin();
		void PlatformSDKLogout();

		void OnLoginSuc(int apiID, std::string loginUin, std::string sessionID, std::string nickname);
		void OnLoginCancel();
		void OnLoginFailed();
		void OnGuestOfficialSuc();

		void SynPay(const std::string &productID, const std::string &productName,
			float productPrice, float productOriginPrice, int count,
			const std::string &payDescription);
		void ASynPay(const std::string &productID, const std::string &productName,
			float productPrice, float productOriginPrice, int count,
			const std::string &payDescription);

		void OnPaySuc(const std::string &serial, bool isSyn);
		void OnPayCancel(const std::string &serial, bool isSyn);
		void OnPayFailed(const std::string &serial, bool isSyn);
		void OnPaySMSSent(const std::string &serial, bool isSyn);
		void OnPayRequestSubmitted(const std::string &serial, bool isSyn);
		void OnPayError(const std::string &serial, int error, bool isSyn);

	private:
		std::string mPlatformSDK;

	public:
		void TVShowCursor(bool show);
		void RinGun_Open();
		void RinGun_Close();
		void RinGun_OnReceive(const std::string &strCMD);
		std::string RinGUN_GetCMD() const;
		int RinGun_GetPosX() const;
		int RinGun_GetPosY() const;
		int RinGun_IsFiring() const;

	private:
		std::string mRinGun_CMD;
		int mRinGun_PosX;
		int mRinGun_PosY;
		bool mRinGun_IsFiring;

	public:
		void StartAccelerator();

		void RegistAccelerator();
		void UnRegistAccelerator();

		void SetAccelerator(float x, float y, float z);
		float GetAcceleratorX() const;
		float GetAcceleratorY() const;
		float GetAcceleratorZ() const;

	private:
		float mAcceleratorX;
		float mAcceleratorY;
		float mAcceleratorZ;
	};

#define PX2_PFSDK PlatformSDK::GetSingleton()

}

#endif