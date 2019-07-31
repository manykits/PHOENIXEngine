package org.appplay.ai;

/*

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.speech.RecognitionListener;
import android.util.Log;


import com.turing.androidsdk.HttpRequestListener;
import com.turing.androidsdk.RecognizeListener;
import com.turing.androidsdk.RecognizeManager;
import com.turing.androidsdk.TTSListener;
import com.turing.androidsdk.TTSManager;
import com.turing.androidsdk.TuringManager;

public class VoiceSDKTuLing extends VoiceSDK
{
    private final String TURING_APIKEY = "783798bd15124bcb841685fc32a437bb";
    private final String TURING_SECRET = "62b43aabb171b01d";
    private final String BD_APIKEY = "i2sA4vGZMwio9QpN3HKr6SG7";
    private final String BD_SECRET = "dXS0qdMv3qhC2P41mUcbTKimOtImqYV3";	
	
    private final String TAG = "PhoenixCreate.px2";
    private TTSManager mTtsManager;
    private RecognizeManager mRecognizerManager;
    private TuringManager mTuringManager;
    
    public static final int MSG_SPEECH_START = 0;
    public static final int MSG_RECOGNIZE_RESULT = 1;
    public static final int MSG_RECOGNIZE_START = 2;

	public VoiceSDKTuLing(Activity act) {
		super(act);
		
        mRecognizerManager = new RecognizeManager(sTheActivtiy, BD_APIKEY, BD_SECRET);
        mRecognizerManager.setVoiceRecognizeListener(myVoiceRecognizeListener);
        
        mTtsManager = new TTSManager(sTheActivtiy, BD_APIKEY, BD_SECRET);
        mTtsManager.setTTSListener(myTTSListener);
        
        mTuringManager = new TuringManager(sTheActivtiy, TURING_APIKEY, TURING_SECRET);        
        mTuringManager.setHttpRequestListener(myHttpConnectionListener);
	}
	
    private Handler mHandler = new Handler() {
        public void handleMessage(android.os.Message msg) {
            switch (msg.what) {
            case MSG_SPEECH_START:
            	String txt = (String) msg.obj;
            	VoiceSDK.sTheVoiceSDK.OnSpeechText(txt);
            	if (mIsAutoSpeachTTS)
            		mTtsManager.startTTS(txt);
            	else
            		mTtsManager.startTTS("");
                break;
            case MSG_RECOGNIZE_RESULT:
            	if (mIsEanableAutoSpeach)
            	{
            		mTuringManager.requestTuring((String) msg.obj);
            	}  
                break;
            case MSG_RECOGNIZE_START:
                mRecognizerManager.startRecognize();
                break;
            }
        };
    };
    
    @Override
	public void init()
    {
    	mTtsManager.startTTS("");
    }

	// Start VoiceSDK
	
	@Override
	public void StartVoiceListening() {
		// TODO Auto-generated method stub
        mRecognizerManager.startRecognize();
	}

	@Override
	public void EndVoiceListening() {
		// TODO Auto-generated method stub
		mRecognizerManager.stopRecognize();		
	}

	@Override
	public void speak(String text) {
		// TODO Auto-generated method stub
		mTtsManager.startTTS(text);
	}

	@Override
	public void speakPause() {
		// TODO Auto-generated method stub	
		mTtsManager.pauseTTS();
	}

	@Override
	public void speakResume() {
		// TODO Auto-generated method stub
		mTtsManager.resumeTTS();		
	}

	@Override
	public void speakStop() {
		// TODO Auto-generated method stub
		mTtsManager.stopTTS();
	}

	@Override
	public void destory() {
		// TODO Auto-generated method stub
		
	}	
	// End VoiceSDK
	
    RecognizeListener myVoiceRecognizeListener = new RecognizeListener() {

        @Override
        public void onVolumeChange(int volume) {        	
        }

        @Override
        public void onStartRecognize() {

        }

        @Override
        public void onRecordStart() {        	
        	VoiceSDK.sTheVoiceSDK.OnVoiceRecordStart();
        }

        @Override
        public void onRecordEnd() {
        	VoiceSDK.sTheVoiceSDK.OnVoiceRecordEnd();
        }

        @Override
        public void onRecognizeResult(String result) {
            Log.d(TAG, "onRecognizeResult:" + result);
            
            if (result == null) 
            {
                mHandler.sendEmptyMessage(MSG_RECOGNIZE_START);
                return;         
            }
            
        	VoiceSDK.sTheVoiceSDK.OnVoiceRecognizeResults(result, "");
            Log.e(TAG, "OnVoiceRecognizeResults:" + result);
        	
            mHandler.obtainMessage(MSG_RECOGNIZE_RESULT, result).sendToTarget();
        }

        @Override
        public void onRecognizeError(String error) {
            Log.e(TAG, "onRecognizeError:" + error);
            mHandler.sendEmptyMessage(MSG_RECOGNIZE_START);
        }
    };
	
	
    TTSListener myTTSListener = new TTSListener() {

        @Override
        public void onSpeechStart() {
            Log.d(TAG, "onSpeechStart");   
            
        	VoiceSDK.sTheVoiceSDK.OnSpeechStart();           
        }

        @Override
        public void onSpeechProgressChanged() {

        }

        @Override
        public void onSpeechPause() {

          	VoiceSDK.sTheVoiceSDK.OnSpeechPause();    
        }

        @Override
        public void onSpeechFinish() {        	
            Log.d(TAG, "onSpeechFinish");
            
        	VoiceSDK.sTheVoiceSDK.OnSpeechFinish(); 
        	
            mHandler.sendEmptyMessage(MSG_RECOGNIZE_START);
        }

        @Override
        public void onSpeechError(int errorCode) {
            Log.d(TAG, "onSpeechError" + errorCode);
            mHandler.sendEmptyMessage(MSG_RECOGNIZE_START);
        }

        @Override
        public void onSpeechCancel() {
        	VoiceSDK.sTheVoiceSDK.OnSpeechCancel(); 
        	
            Log.d(TAG, "TTS Cancle!");
        }
    };

    HttpRequestListener myHttpConnectionListener = new HttpRequestListener() {

        @Override
        public void onSuccess(String result) {
        	if (mIsEanableAutoSpeach)
        	{
                if (result != null) {
                    try {
                        Log.d(TAG, "result" + result);
                        JSONObject result_obj = new JSONObject(result);
                        if (result_obj.has("text")) 
                        {
                        	String txt = result_obj.get("text").toString(); 
                            
                            mHandler.obtainMessage(MSG_SPEECH_START,
                                    result_obj.get("text")).sendToTarget();
                        }
                    } catch (JSONException e) {
                        Log.d(TAG, "JSONException:" + e.getMessage());
                    }
                }	
        	}
        }

        @Override
        public void onFail(int code, String error) {
        	if (mIsEanableAutoSpeach)
        	{
                Log.d(TAG, "onFail code:" + code + "|error:" + error);
                mHandler.obtainMessage(MSG_SPEECH_START, "").sendToTarget();	
        	}
        }
    };

    private boolean mIsEanableAutoSpeach = false;
	@Override
	public void enableAutoSpeach(boolean enable) {
		// TODO Auto-generated method stub
		mIsEanableAutoSpeach = enable;		
	}
	
    private boolean mIsAutoSpeachTTS = true;
	@Override
	public void enableAutoSpeachTTS(boolean enable) {
		// TODO Auto-generated method stub
		mIsAutoSpeachTTS = enable;		
	}
}

*/
