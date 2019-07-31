package org.appplay.platformsdk;

import org.appplay.lib.AppPlayMetaData;

import android.app.Activity;

public class PlatformSDKCreater
{
	public static String sSDK_NAME_UNKNOWN = "unknown";
	public static String sSDK_NAME_91 = "91";
	public static String sSDK_NAME_360 = "360";
	
	public static String sSDK_CurrentName = "";
	
	public static PlatformSDK Create (Activity activity)
	{
		sSDK_CurrentName = AppPlayMetaData.sPFSDKName;
		
		return _Create(activity, sSDK_CurrentName);
	}
	
	private static PlatformSDK _Create (Activity activity, String source)
	{
		if (sSDK_NAME_91.equals(source)) 
		{
			return null;
		}
		else if (sSDK_NAME_360.equals(source)) 
		{
			return null;
		}
		else 
		{
			return null;
		}
	}
}
