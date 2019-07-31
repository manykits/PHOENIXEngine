package org.appplay.ap;

import java.util.HashMap;

//import com.jieli.lib.stream.beans.DeviceVersionInfo;
//import com.jieli.stream.dvrunning_ec.util.PreferencesHelper;
//import com.jieli.stream.dvrunning_ec.util.IConstant;

import android.app.Application;
import android.content.pm.PackageManager;
import android.text.TextUtils;
import android.util.Log;

public class AppPlayApplication extends Application {
    private final String tag = getClass().getSimpleName();
    private static AppPlayApplication sMyApplication = null;
    private String mDeviceUUID = null;
    private boolean isModifySSID = false;
    private String lastModifySSID = null;
    private boolean isModifyPWD = false;
    private String appName = null;
    private String appLocalVersion = null;
    private int captureSize;
    private String currentProductType = null;
    //private DeviceVersionInfo serverVersionInfo = null;
    
    public AppPlayApplication GetInstance()
    {
    	return sMyApplication;
    }
	
    @Override
    public void onCreate() {
        super.onCreate();
        
        sMyApplication = this;
        
        /*
        appName = PreferencesHelper.getSharedPreferences(getApplicationContext()).getString(IConstant.KEY_ROOT_PATH_NAME, null);
        PackageManager pm = this.getPackageManager();
        if(TextUtils.isEmpty(appName)){
            appName = getApplicationInfo().loadLabel(pm).toString();
        }
        try {
            appLocalVersion  = pm.getPackageInfo(getPackageName(), 0).versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        */
        
        /*
        PreferencesHelper.remove(getApplicationContext(), IConstant.CURRENT_SSID);
        PreferencesHelper.remove(getApplicationContext(), IConstant.CURRENT_PWD);

        PreferencesHelper.remove(getApplicationContext(), IConstant.DEVICE_VERSION_MSG);
        Log.e(tag, " appName : " +appName + ", appLocalVersion = "+appLocalVersion);
        */
    }
    
    public static synchronized AppPlayApplication getApplication() {
        return sMyApplication;
    }
    
    public String getDeviceUUID() {
        return mDeviceUUID;
    }

    public void setDeviceUUID(String uuid) {
        this.mDeviceUUID = uuid;
    }
    
    public void setAppLocalVersion(String vs){
        this.appLocalVersion = vs;
    }

    public String getAppLocalVersion(){
        return appLocalVersion;
    }
    
    public boolean isModifySSID() {
        return isModifySSID;
    }

    public void setModifySSID(boolean isModifySSID) {
        this.isModifySSID = isModifySSID;
    }
    
    public String getLastModifySSID() {
        return lastModifySSID;
    }

    public void setLastModifySSID(String lastModifySSID) {
        this.lastModifySSID = lastModifySSID;
    }
    
    public boolean isModifyPWD() {
        return isModifyPWD;
    }

    public void setModifyPWD(boolean isModifyPWD) {
        this.isModifyPWD = isModifyPWD;
    }
    
    public void setCaptureSize(int size){
        this.captureSize = size;
    }

    public int getCaptureSize(){
        return captureSize;
    }
    
    public String getAppName(){
        return appName;
    }

    public void setAppName(String appName) {
        this.appName = appName;
    }
    
    public void setCurrentProductType(String type){
        this.currentProductType = type;
    }

    public String getCurrentProductType(){
        return currentProductType;
    }
    
    //public void setServerVersionInfo(DeviceVersionInfo versionInfo){
        //this.serverVersionInfo = versionInfo;
    //}

    //public DeviceVersionInfo getServerVersionInfo(){
        //return serverVersionInfo;
    //}
    
}
