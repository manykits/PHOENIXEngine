package org.appplay.ap;

import java.util.List;

import org.appplay.lib.AppPlayBaseActivity;
import org.appplay.lib.AppPlayMetaData;
import org.appplay.platformsdk.PlatformSDK;
import org.appplay.platformsdk.PlatformSDKCreater;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

public class AppPlayActivity extends AppPlayBaseActivity
{	
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
		// super create     	
        super.onCreate(savedInstanceState);
    }
    
    @Override
	protected void onPause() 
    {  
    	Log.d("appplay.ap", "AppPlayActivity::onPause");
    	
	    super.onPause();
    }

	 @Override
	 protected void onResume() 
	 {	 
		 super.onResume();
	 }
}
