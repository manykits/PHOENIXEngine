package org.appplay.platformsdk;

public class PlatformSDKNatives
{
	public static void PlatformLogin()
	{
		
	}

	public static String GetPayPlatform()
	{
		return PlatformSDK.sThePlatformSDK.sPayPlatform;
	}
	
	public static String GetPlatformSource()
	{
		return PlatformSDK.sThePlatformSDK.sSource;
	}
	
	public static String GetHostName()
	{
		return PlatformSDK.sThePlatformSDK.sPayHostName;
	}
	
	public static native void SetPlatformSDK (String pfName);
	public static native void OnLoginSuc (int apiID, String loginUin, String sessionID, String nickname);	
	public static native void OnLoginCancel ();	
	public static native void OnLoginFailed ();
	public static native void OnGuestOfficialSuc ();
	
	public static native void OnPaySuc (String serial, boolean isSyn);
	public static native void OnPayCancel (String serial, boolean isSyn);
	public static native void OnPayFailed (String serial, boolean isSyn);
	public static native void OnPaySMSSent (String serial, boolean isSyn);
	public static native void OnPayRequestSubmitted (String serial, boolean isSyn);
	public static native void OnPayError (String serial, int error, boolean isSyn);
}
