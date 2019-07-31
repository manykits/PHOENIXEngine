package org.appplay.platformsdk;

import java.util.UUID;

import android.app.Activity;

abstract public class PlatformSDK
{
	public static PlatformSDK sThePlatformSDK = null;
	public static Activity sTheActivtiy = null;
	
	public static String sPayPlatform;
	public static String sPayHostName;
	public static String sSource;
	private String mSerial;
	
	public PlatformSDK(Activity act)
	{
		sTheActivtiy = act;
	}

	public abstract void Init();
	
	public abstract void Term();
	
	// app to background to call
	public abstract void OnResume ();
	
	// press but "back" to call
	public abstract void OnExist();
	public abstract void OnLogoutExist();
	
	// login logout
	public abstract void Login_Guest();
	public abstract void Login();
	public abstract void IsLogined();
	
	public abstract void Logout();
	
	// pay
	public abstract void SynPay (
			String productID,
			String productName,
			float productPrice,
			float productOrginalPrice,
			int count,
			String payDescription);
	
	public abstract void ASynPay (
			String productID,
			String productName,
			float productPrice,
			float productOrginalPrice,
			int count,
			String payDescription);
	
	public String GetSerial() { return mSerial; }
	
	public String MakeSerial()
	{
		UUID guid = UUID.randomUUID(); 
		String text = guid.toString(); 
		text = text.replace("-", "".trim()); 
		
		mSerial = text;
		
		return mSerial;
	}
}
