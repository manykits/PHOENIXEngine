package org.appplay.lib;

import org.appplay.ai.VoiceSDK;

public class AppPlayNatives
{
	public static native void nativeOdle ();
	public static native void nativeInit (int width, int height);
	public static native void nativeOnPause ();
	public static native void nativeOnResume ();
	public static native void nativeOnTerm ();
	public static native void nativeSetApkDataPath(String path);
	public static native void nativeSetDataUpdateServerType (String type);
	public static native void nativeSetDeviceIDStr(String deviceIDStr);
	public static native void nativeIPMac(String ip, String mac);
	
	public static native void nativeTouchPressed(int id, float x, float y);
	public static native void nativeTouchReleased(int id, float x, float y);
    public static native void nativeTouchMoved(int[] id, float[] x, float[] y);
    public static native void nativeTouchCancelled(int[] id, float[] x, float[] y);
    public static native boolean nativeKeyDown(int keyCode);
    
    public static native void nativeInsertText(String text);
    public static native void nativeDeleteBackward();
    public static native String nativeGetContentText();
    
    public static native void nativeBluetoothOnConnected();
    public static native void nativeBluetoothOnDisConnected();
    public static native void nativeBluetoothOnConnectFailed();
    public static native void nativeBluetoothDiscoveryNewDevice(String strDevice);
    public static native void nativeBluetoothDiscoveryFinished();
    public static native void nativeBluetoothReceived(byte[] strData);
    
    public static native void nativeWifiDiscoveryNewDevice(String strDevice);
    public static native void nativeWifiDiscoveryFinished();
    public static native void nativeWifiOnConnected();
    public static native void nativeWifiOnConnectFailed();
    public static native void nativeWifiOnDisconnected();
	
	public static native void nativeVoiceSetSDK (String sdkName);
	
	public static native void nativeOnSpeechStart();
	public static native void nativeOnSpeechPause();
	public static native void nativeOnSpeechFinish();
	public static native void nativeOnSpeechCancel();
	public static native void nativeOnSpeakText(String txt);
	
	public static native void nativeOnVoiceRecordStart();
	public static native void nativeOnVoiceRecordEnd();
	
    public static native void nativeOnVoiceRecognizeResults(String strRet, String strJSON);
    
	public static native void nativeCameraSendFrame (int width, int height, byte[] strData);
	public static native void nativeHardCameraSendFrame (int width, int height, byte[] strData);
	
    public static native void nativeUSBSendData(byte[] strData);
    
    public static native void nativeAcceleratorChanged(float x, float y, float z);
}
