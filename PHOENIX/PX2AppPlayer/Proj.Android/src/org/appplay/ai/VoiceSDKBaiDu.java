package org.appplay.ai;
/*
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;

import org.appplay.lib.AppPlayBaseActivity;
import org.appplay.ai.VoiceSDK.Type;
import org.json.JSONArray;
import org.json.JSONObject;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.content.ContextWrapper;
import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.os.Build;
import android.preference.PreferenceManager;
import android.speech.RecognitionListener;
import android.speech.SpeechRecognizer;
import android.text.Layout;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.style.ForegroundColorSpan;
import android.util.Log;
import android.widget.RelativeLayout;
import android.widget.ScrollView;

import org.appplay.ap.R;
import com.baidu.speech.VoiceRecognitionService;
import com.baidu.tts.auth.AuthInfo;
import com.baidu.tts.client.SpeechError;
import com.baidu.tts.client.SpeechSynthesizer;
import com.baidu.tts.client.SpeechSynthesizerListener;
import com.baidu.tts.client.SynthesizerTool;
import com.baidu.tts.client.TtsMode;

public class VoiceSDKBaiDu extends VoiceSDK implements RecognitionListener, SpeechSynthesizerListener
{
    public VoiceSDKBaiDu(Activity act) {
		super(act);
		// TODO Auto-generated constructor stub
		
		TheType = Type.T_BAIDU;
		sPlatformStr = "baidu";
		
		Log.d("appplay.ap", "begin::VoiceSDKBaiDu");
		
		// speech recog
        speechRecognizer = SpeechRecognizer.createSpeechRecognizer(act, new ComponentName(act, VoiceRecognitionService.class));
        speechRecognizer.setRecognitionListener(this);   
        
        Log.d("appplay.ap", "end::VoiceSDKBaiDu");
        
        initialEnv();
        initalTts(act);
        
        Log.d("appplay.ap", "end::VoiceSDKBaiDu");
	}
    
	public void speakPause()
	{
        speechSynthesizer.pause();
	}
    
	public void speakResume()
	{
        speechSynthesizer.resume();
	}

	public void speakStop()
	{
        speechSynthesizer.stop();
	}

	public void destory()
	{
        speechSynthesizer.release();
	}

	private SpeechRecognizer speechRecognizer;
	
	public void StartVoiceListening()
	{
        speechRecognizer.cancel();
        Intent intent = new Intent();
        bindParams(intent);
        intent.putExtra("vad", "touch");
        speechRecognizer.startListening(intent);
	}
	
	public void EndVoiceListening()
	{
		speechRecognizer.stopListening();
	}
	
	public void bindParams(Intent intent) 
	{// 设置识别参数
        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(sTheActivtiy);
        if (sp.getBoolean("tips_sound", true))
        {
            intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_SOUND_START, R.raw.bdspeech_recognition_start);
            intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_SOUND_END, R.raw.bdspeech_speech_end);
            intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_SOUND_SUCCESS, R.raw.bdspeech_recognition_success);
            intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_SOUND_ERROR, R.raw.bdspeech_recognition_error);
            intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_SOUND_CANCEL, R.raw.bdspeech_recognition_cancel);
        }
        if (sp.contains(VoiceSDKConstantBaiDu.EXTRA_INFILE)) {
            String tmp = sp.getString(VoiceSDKConstantBaiDu.EXTRA_INFILE, "").replaceAll(",.*", "").trim();
            intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_INFILE, tmp);
        }
        if (sp.getBoolean(VoiceSDKConstantBaiDu.EXTRA_OUTFILE, false)) {
            intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_OUTFILE, "sdcard/outfile.pcm");
        }
        if (sp.contains(VoiceSDKConstantBaiDu.EXTRA_SAMPLE)) {
            String tmp = sp.getString(VoiceSDKConstantBaiDu.EXTRA_SAMPLE, "").replaceAll(",.*", "").trim();
            if (null != tmp && !"".equals(tmp)) {
                intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_SAMPLE, Integer.parseInt(tmp));
            }
        }
        if (sp.contains(VoiceSDKConstantBaiDu.EXTRA_LANGUAGE)) {
            String tmp = sp.getString(VoiceSDKConstantBaiDu.EXTRA_LANGUAGE, "").replaceAll(",.*", "").trim();
            if (null != tmp && !"".equals(tmp)) {
                intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_LANGUAGE, tmp);
            }
        }
        if (sp.contains(VoiceSDKConstantBaiDu.EXTRA_NLU)) {
            String tmp = sp.getString(VoiceSDKConstantBaiDu.EXTRA_NLU, "").replaceAll(",.*", "").trim();
            if (null != tmp && !"".equals(tmp)) {
                intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_NLU, tmp);
            }
        }

        if (sp.contains(VoiceSDKConstantBaiDu.EXTRA_VAD)) {
            String tmp = sp.getString(VoiceSDKConstantBaiDu.EXTRA_VAD, "").replaceAll(",.*", "").trim();
            if (null != tmp && !"".equals(tmp)) {
                intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_VAD, tmp);
            }
        }
        String prop = null;
        if (sp.contains(VoiceSDKConstantBaiDu.EXTRA_PROP)) {
            String tmp = sp.getString(VoiceSDKConstantBaiDu.EXTRA_PROP, "").replaceAll(",.*", "").trim();
            if (null != tmp && !"".equals(tmp)) {
                intent.putExtra(VoiceSDKConstantBaiDu.EXTRA_PROP, Integer.parseInt(tmp));
                prop = tmp;
            }
        }
	}
	
    private String buildTestSlotData() {
        JSONObject slotData = new JSONObject();
        JSONArray name = new JSONArray().put("李涌泉").put("郭下纶");
        JSONArray song = new JSONArray().put("七里香").put("发如雪");
        JSONArray artist = new JSONArray().put("周杰伦").put("李世龙");
        JSONArray app = new JSONArray().put("手机百度").put("百度地图");
        JSONArray usercommand = new JSONArray().put("关灯").put("开门");
        return slotData.toString();
    }
	
    @Override
    public void onReadyForSpeech(Bundle params) {
    	// 准备就绪
    }

    @Override
    public void onBeginningOfSpeech() {
    	// 开始说话处理
    }
    
    @Override
    public void onRmsChanged(float rmsdB) {
    	 // 音量变化处理
    }

    @Override
    public void onBufferReceived(byte[] buffer) {
    	// 录音数据传出处理
    }

    @Override
    public void onEndOfSpeech() {
    	// 说话结束处理
    }
    
    public void Synthesize(String text)
    {
        int result = speechSynthesizer.synthesize(text);
        if (result < 0) 
        {
            print("error,please look up error code in doc or URL:http://yuyin.baidu.com/docs/tts/122 ");
        }
    }
    
    @Override
	public void speak(String text)
    {         
    	int result = speechSynthesizer.speak(text);
    	print("speak result=" + result);    	
    }

    @Override
    public void onError(int error) {
    	// 出错处理
    	
        StringBuilder sb = new StringBuilder();
        switch (error) {
            case SpeechRecognizer.ERROR_AUDIO:
                sb.append("音频问题");
                break;
            case SpeechRecognizer.ERROR_SPEECH_TIMEOUT:
                sb.append("没有语音输入");
                break;
            case SpeechRecognizer.ERROR_CLIENT:
                sb.append("其它客户端错误");
                break;
            case SpeechRecognizer.ERROR_INSUFFICIENT_PERMISSIONS:
                sb.append("权限不足");
                break;
            case SpeechRecognizer.ERROR_NETWORK:
                sb.append("网络问题");
                break;
            case SpeechRecognizer.ERROR_NO_MATCH:
                sb.append("没有匹配的识别结果");
                break;
            case SpeechRecognizer.ERROR_RECOGNIZER_BUSY:
                sb.append("引擎忙");
                break;
            case SpeechRecognizer.ERROR_SERVER:
                sb.append("服务端错误");
                break;
            case SpeechRecognizer.ERROR_NETWORK_TIMEOUT:
                sb.append("连接超时");
                break;
        }
        sb.append(":" + error);
        print("识别失败：" + sb.toString());
    }
    
    @Override
    public void OnVoiceRecognizeResults(String strRet, String strJSON)
    {
    	super.OnVoiceRecognizeResults(strRet, strJSON);
    }

    @Override
    public void onResults(Bundle results) {
    	// 最终结果处理
    	
        ArrayList<String> nbest = results.getStringArrayList(SpeechRecognizer.RESULTS_RECOGNITION);
        String strRet = Arrays.toString(nbest.toArray(new String[nbest.size()]));
        print("识别成功：" + strRet);        
        
        String json_res = results.getString("origin_result");
        try 
        {
            print("origin_result=\n" + new JSONObject(json_res).toString(4));
        } 
        catch (Exception e) 
        {
            print("origin_result=[warning: bad json]\n" + json_res);
        }
        
        OnVoiceRecognizeResults(strRet, json_res);
    }

    @Override
    public void onPartialResults(Bundle partialResults) {
    	// 临时结果处理
    	
        ArrayList<String> nbest = partialResults.getStringArrayList(SpeechRecognizer.RESULTS_RECOGNITION);
        if (nbest.size() > 0)
        {
            print("~临时识别结果：" + Arrays.toString(nbest.toArray(new String[0])));
        }
    }

    private static final int EVENT_ERROR = 11;
    @Override
    public void onEvent(int eventType, Bundle params) {
    	// 处理事件回调
    	
        switch (eventType) {
            case EVENT_ERROR:
                String reason = params.get("reason") + "";
                print("EVENT_ERROR, " + reason);
                break;
            case VoiceRecognitionService.EVENT_ENGINE_SWITCH:
                break;
        }
    }
    
    private static final String TAG = "Sdk2Api";
    private void print(String msg) 
    {
        Log.d(TAG, "----" + msg);
    }
  
    // text to speech
    private String mSampleDirPath;
    private static final String AssetPath = "Data/Block/speech/baidu/";
    private static final String SAMPLE_DIR_NAME = "baiduTTS";
    private static final String SPEECH_FEMALE_MODEL_NAME = "bd_etts_speech_female.dat";
    private static final String SPEECH_MALE_MODEL_NAME = "bd_etts_speech_male.dat";
    private static final String TEXT_MODEL_NAME = "bd_etts_text.dat";
    private static final String LICENSE_FILE_NAME = "temp_license";
    private static final String ENGLISH_SPEECH_FEMALE_MODEL_NAME = "bd_etts_speech_female_en.dat";
    private static final String ENGLISH_SPEECH_MALE_MODEL_NAME = "bd_etts_speech_male_en.dat";
    private static final String ENGLISH_TEXT_MODEL_NAME = "bd_etts_text_en.dat";
    
    private SpeechSynthesizer speechSynthesizer;
    private void initialEnv()
    {
        Log.d(TAG, "begin initialEnv");
    	
        if (mSampleDirPath == null) {           
        	String sdcardPath = Environment.getExternalStorageDirectory().toString();
            print("sdcardPath:" + sdcardPath);
            mSampleDirPath = sdcardPath + "/" + SAMPLE_DIR_NAME;
        }
        
        makeDir(mSampleDirPath);
        
        copyFromAssetsToSdcard(false, AssetPath + SPEECH_FEMALE_MODEL_NAME, mSampleDirPath + "/" + SPEECH_FEMALE_MODEL_NAME);
        copyFromAssetsToSdcard(false, AssetPath + SPEECH_MALE_MODEL_NAME, mSampleDirPath + "/" + SPEECH_MALE_MODEL_NAME);
        copyFromAssetsToSdcard(false, AssetPath + TEXT_MODEL_NAME, mSampleDirPath + "/" + TEXT_MODEL_NAME);
        copyFromAssetsToSdcard(false, AssetPath + LICENSE_FILE_NAME, mSampleDirPath + "/" + LICENSE_FILE_NAME);
        copyFromAssetsToSdcard(false, AssetPath + "english/" + ENGLISH_SPEECH_FEMALE_MODEL_NAME, mSampleDirPath + "/" + ENGLISH_SPEECH_FEMALE_MODEL_NAME);
        copyFromAssetsToSdcard(false, AssetPath + "english/" + ENGLISH_SPEECH_MALE_MODEL_NAME, mSampleDirPath + "/" + ENGLISH_SPEECH_MALE_MODEL_NAME);
        copyFromAssetsToSdcard(false, AssetPath + "english/" + ENGLISH_TEXT_MODEL_NAME, mSampleDirPath + "/"
                + ENGLISH_TEXT_MODEL_NAME);
        
        Log.d(TAG, "end initialEnv");
    }
    
    private void makeDir(String dirPath) {
        File file = new File(dirPath);
        if (!file.exists()) {
            file.mkdirs();
        }
    }    
    
    private void copyFromAssetsToSdcard(boolean isCover, String source, String dest) {
        Log.d(TAG, "copy from " + source + " to " + dest);
    	
        File file = new File(dest);
        if (isCover || (!isCover && !file.exists())) {
            InputStream is = null;
            FileOutputStream fos = null;
            try {
                is = AppPlayBaseActivity.sTheActivity.getResources().getAssets().open(source);
                String path = dest;
                fos = new FileOutputStream(path);
                byte[] buffer = new byte[1024];
                int size = 0;
                while ((size = is.read(buffer, 0, 1024)) >= 0) {
                    fos.write(buffer, 0, size);
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (fos != null) {
                    try {
                        fos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                try {
                    if (is != null) {
                        is.close();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    
    private void initalTts(Activity act)
    {   
    	print("begin initalTts");
    	
        // text to speeck
        speechSynthesizer = SpeechSynthesizer.getInstance();
        speechSynthesizer.setContext(act);
        speechSynthesizer.setSpeechSynthesizerListener(this);
        
        speechSynthesizer.setParam(SpeechSynthesizer.PARAM_TTS_TEXT_MODEL_FILE, mSampleDirPath + "/"
                + TEXT_MODEL_NAME);
        
    	print("1");
        
        speechSynthesizer.setParam(SpeechSynthesizer.PARAM_TTS_SPEECH_MODEL_FILE, mSampleDirPath + "/"
                + SPEECH_FEMALE_MODEL_NAME);
        
    	print("2");
        
       speechSynthesizer.setParam(SpeechSynthesizer.PARAM_TTS_LICENCE_FILE, mSampleDirPath + "/"
                + LICENSE_FILE_NAME);
        
    	print("begin initalTts");
        
        speechSynthesizer.setAppId("8535996");
        speechSynthesizer.setApiKey("MxPpf3nF5QX0pndKKhS7IXcB", "7226e84664474aa098296da5eb2aa434");
        speechSynthesizer.setParam(SpeechSynthesizer.PARAM_SPEAKER, "0");
        speechSynthesizer.setParam(SpeechSynthesizer.PARAM_MIX_MODE, SpeechSynthesizer.MIX_MODE_DEFAULT);
        AuthInfo authInfo = speechSynthesizer.auth(TtsMode.MIX);

        if (authInfo.isSuccess()) 
        {
            print("auth success");
        }
        else 
        {
            String errorMsg = authInfo.getTtsError().getDetailMessage();
            print("auth failed errorMsg=" + errorMsg);
        }
        speechSynthesizer.initTts(TtsMode.MIX);
        
        int result = speechSynthesizer.loadEnglishModel(
        		mSampleDirPath + "/" + ENGLISH_TEXT_MODEL_NAME, mSampleDirPath
                        + "/" + ENGLISH_SPEECH_FEMALE_MODEL_NAME);
        print("loadEnglishModel result=" + result);

        printEngineInfo();
        
    	print("end initalTts");
    }
    
    private void printEngineInfo() {
        print("EngineVersioin=" + SynthesizerTool.getEngineVersion());
        print("EngineInfo=" + SynthesizerTool.getEngineInfo());
        String textModelInfo = SynthesizerTool.getModelInfo(mSampleDirPath + "/" + TEXT_MODEL_NAME);
        print("textModelInfo=" + textModelInfo);
        String speechModelInfo = SynthesizerTool.getModelInfo(mSampleDirPath + "/" + SPEECH_FEMALE_MODEL_NAME);
        print("speechModelInfo=" + speechModelInfo);
    }
    
    
    @Override
    public void onSynthesizeStart(String utteranceId) {
        print("onSynthesizeStart utteranceId=" + utteranceId);
    }

    @Override
    public void onSynthesizeDataArrived(String utteranceId, byte[] data, int progress) {

    }

    @Override
    public void onSynthesizeFinish(String utteranceId) {
        print("onSynthesizeFinish utteranceId=" + utteranceId);
    }

    @Override
    public void onSpeechStart(String utteranceId) {
        print("onSpeechStart utteranceId=" + utteranceId);
    }

    @Override
    public void onSpeechProgressChanged(String utteranceId, int progress) {

    }

    @Override
    public void onSpeechFinish(String utteranceId) {
        print("onSpeechFinish utteranceId=" + utteranceId);
    }

    @Override
    public void onError(String utteranceId, SpeechError error) {
        print("onError error=" + "(" + error.code + ")" + error.description + "--utteranceId=" + utteranceId);
    }
    
}

*/
