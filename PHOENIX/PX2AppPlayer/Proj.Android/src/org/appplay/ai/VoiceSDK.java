package org.appplay.ai;

import org.appplay.lib.AppPlayBaseActivity;
import org.appplay.lib.AppPlayNatives;
import org.appplay.platformsdk.PlatformSDK;
import android.app.Activity;
import android.os.Bundle;

abstract public class VoiceSDK {

	public static Activity sTheActivtiy = null;
	public static VoiceSDK sTheVoiceSDK = null;
	public static String sPlatformStr = "empty";
	
	public VoiceSDK(Activity act)
	{
		sTheActivtiy = act;
		TheType = Type.T_NONE;
	}

	public enum Type
	{
		T_NONE,
		T_BAIDU,
		T_TULING,
		T_MAX_TYPE
	}
	
	public Type TheType;
	
	public void OnSpeechStart()
	{
		if (AppPlayBaseActivity.sIsInitlized)
			AppPlayNatives.nativeOnSpeechStart();		
	}
	
	public void OnSpeechText(String txt)
	{
		if (AppPlayBaseActivity.sIsInitlized)
			AppPlayNatives.nativeOnSpeakText(txt);	
	}
	
	public void OnSpeechPause()
	{
		if (AppPlayBaseActivity.sIsInitlized)
			AppPlayNatives.nativeOnSpeechPause();
	}
	
	public void OnSpeechFinish()
	{
		if (AppPlayBaseActivity.sIsInitlized)
			AppPlayNatives.nativeOnSpeechFinish();
	}
	
	public void OnSpeechCancel()
	{
		if (AppPlayBaseActivity.sIsInitlized)
			AppPlayNatives.nativeOnSpeechCancel();
	}
	
	public abstract void StartVoiceListening();
	public abstract void EndVoiceListening();
	
	public void OnVoiceRecordStart()
	{
		if (AppPlayBaseActivity.sIsInitlized)
			AppPlayNatives.nativeOnVoiceRecordStart();		
	}
	
	public void OnVoiceRecordEnd()
	{
		if (AppPlayBaseActivity.sIsInitlized)
			AppPlayNatives.nativeOnVoiceRecordEnd();		
	}
	
	public void OnVoiceRecognizeResults(String strRet, String strJSON)
	{
		if (AppPlayBaseActivity.sIsInitlized)
			AppPlayNatives.nativeOnVoiceRecognizeResults(strRet, strJSON);
	}
	
	public abstract void init();

	public abstract void speak(String text);
	public abstract void speakPause();
	public abstract void speakResume();
	public abstract void speakStop();
	public abstract void destory();
	
	public abstract void enableAutoSpeach(boolean enable);

	public void enableAutoSpeachTTS(boolean enable) {
		// TODO Auto-generated method stub
		
	}	
}
