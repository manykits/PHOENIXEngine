package org.appplay.lib;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;

public class AppPlayMetaData
{
	public static boolean sIsNettable;
	public static boolean sIsBluetoothable;
	public static boolean sIsVoiceable;
	public static boolean sIsDebug;
	public static boolean sIsTest;

	public static String sPFSDKName;
	public static String sVOICESDKName;
	public static String sAppName;

	public static String sURL_Version;
	public static String sURL_LibSO;

	private final static String _ISNETTABLE = "isnettable";
	private final static String _ISBLUETOOTHABLE = "isbluetoothable";
	private final static String _ISVOICEABLE = "isvoiceable";
	private final static String _ISDEBUG = "isdebug";
	private final static String _ISTEST = "istest";
	private final static String _PFSDKNAME = "pfsdkname";
	private final static String _VOICESDKNAME = "voicesdkname";
	private final static String _APPNAME = "appname";
	private final static String _URL_LIBSO = "url_libso";
	private final static String _URL_VERSION = "url_version";
	private final static String _URL_LIBSO_TEST = "url_libso_test";
	private final static String _URL_VERSION_TEST = "url_version_test";

	public static void Initlize(Context ctx)
	{
		sIsNettable = (GetMetaData(ctx, _ISNETTABLE) == "true");
		sIsBluetoothable = (GetMetaData(ctx, _ISBLUETOOTHABLE) == "true");
		sIsVoiceable = (GetMetaData(ctx, _ISVOICEABLE) == "true");
		sIsDebug = (GetMetaData(ctx, _ISDEBUG) == "true");
		sIsTest = (GetMetaData(ctx, _ISTEST) == "true");
		sPFSDKName = GetMetaData(ctx, _PFSDKNAME);
		sVOICESDKName = GetMetaData(ctx, _VOICESDKNAME);
		sAppName = GetMetaData(ctx, _APPNAME);

		if (!sIsTest)
		{
			sURL_LibSO = GetMetaData(ctx, _URL_LIBSO);
			sURL_Version = GetMetaData(ctx, _URL_VERSION);
		} else
		{
			sURL_LibSO = GetMetaData(ctx, _URL_LIBSO_TEST);
			sURL_Version = GetMetaData(ctx, _URL_VERSION_TEST);
		}
	}

	public static String GetMetaData(Context ctx, String key)
	{
		ApplicationInfo ai;
		try
		{
			ai = ctx.getPackageManager().getApplicationInfo(
					ctx.getPackageName(), PackageManager.GET_META_DATA);

			Bundle bundle = ai.metaData;
			Object obj = bundle.get(key);
			String source = String.valueOf(obj);

			return source;
		} catch (NameNotFoundException e)
		{
			e.printStackTrace();
		}

		return "";
	}
}
