// PX2VoiceSDK.cpp

#include "PX2VoiceSDK.hpp"
#include "PX2Log.hpp"
#include "PX2VoiceEventType.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2JSONData.hpp"
#include "PX2StringHelp.hpp"
#if defined (__ANDROID__)
#include "AppPlayJNI.hpp"
#endif
using namespace PX2;
using namespace std;

//----------------------------------------------------------------------------
unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}
unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}
std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}
//----------------------------------------------------------------------------
void _ProgressCallback(CurlObj *obj, double dltotal,
	double dlnow, double ultotal, double ulnow)
{
	PX2_UNUSED(obj);
	PX2_UNUSED(dltotal);
	PX2_UNUSED(dlnow);
	PX2_UNUSED(ultotal);
	PX2_UNUSED(ulnow);
}
//----------------------------------------------------------------------------
VoiceSDK::VoiceSDK() :
mIsAutoSpeakEnabled(true),
mIsAutoTTSSpeakEnabled(true),
mIsIngoreThisTimeAutoSpeech(false)
{
	// manykit tuling
	mAutoSpeech_ProductIDStr = "";
	mAutoSpeech_ApiStr = "783798bd15124bcb841685fc32a437bb";
	mAutoSpeech_SecStr = "";

	// manykit baidu
	mTTS_ProductIDStr = "11221194";
	mTTS_ApiStr = "i2sA4vGZMwio9QpN3HKr6SG7";
	mTTS_SecStr = "dXS0qdMv3qhC2P41mUcbTKimOtImqYV3";

	SetSDK_AutoSpeech("tuling");
	SetSDK_TTS("baidu");
}
//----------------------------------------------------------------------------
VoiceSDK::~VoiceSDK()
{
}
//----------------------------------------------------------------------------
void VoiceSDK::Initlize()
{
	mCurl = new0 CurlObj();
	mCurl->SetGetType(CurlObj::GT_MEMORY);
	mCurl->Initlize();
	mCurl->SetCurlProgressCallback(_ProgressCallback);

	std::string str = _GetTokenJsonTTS();
	JSONData jsData;
	if (jsData.LoadBuffer(str))
	{
		mTTS_Token = jsData.GetMember("access_token").ToString();
	}
}
//----------------------------------------------------------------------------
void VoiceSDK::SetSDK_AutoSpeech(const std::string &name)
{
	mSDK_AutoSpeech = name;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetSDK_AutoSpeech() const
{
	return mSDK_AutoSpeech;
}
//----------------------------------------------------------------------------
void VoiceSDK::SetSDK_TTS(const std::string &thirdname)
{
	mSDK_TTS = thirdname;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetSDK_TTS() const
{
	return mSDK_TTS;
}
//----------------------------------------------------------------------------
void VoiceSDK::IngoreThisTimeAutoSpeech()
{
	mIsIngoreThisTimeAutoSpeech = true;
}
//----------------------------------------------------------------------------
void VoiceSDK::SetAutoSpeech_ProductID(const std::string &strID)
{
	mAutoSpeech_ProductIDStr = strID;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetAutoSpeech_ProductID() const
{
	return mAutoSpeech_ProductIDStr;
}
//----------------------------------------------------------------------------
void VoiceSDK::SetAutoSpeech_APIKey(const std::string &apiKey)
{
	mAutoSpeech_ApiStr = apiKey;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetAutoSpeech_APIKey() const
{
	return mAutoSpeech_ApiStr;
}
//----------------------------------------------------------------------------
void VoiceSDK::SetAutoSpeech_SecretKey(const std::string &secretKey)
{
	mAutoSpeech_SecStr = secretKey;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetAutoSpeech_SecretKey() const
{
	return mAutoSpeech_SecStr;
}
//----------------------------------------------------------------------------
void VoiceSDK::SetTTS_ProdcutID(const std::string &strID)
{
	mTTS_ProductIDStr = strID;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetTTS_ProductID() const
{
	return mTTS_ProductIDStr;
}
//----------------------------------------------------------------------------
void VoiceSDK::SetTTS_APIKey(const std::string &apiKey)
{
	mTTS_ApiStr = apiKey;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetTTS_APIKey() const
{
	return mTTS_ApiStr;
}
//----------------------------------------------------------------------------
void VoiceSDK::SetTTS_SecretKey(const std::string &secretKey)
{
	mTTS_SecStr = secretKey;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetTTS_SecretKey() const
{
	return mTTS_SecStr;
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetSTTStr() const
{
	return mSTTStr;
}
//----------------------------------------------------------------------------
void VoiceSDK::StartVoiceListening()
{
//#ifdef __ANDROID__
//	VoiceStartVoiceListening();
//#else

	PX2_SS.StartRecording(7);

	// 开始录音
	OnVoiceRecordStart();

//#endif
}
//----------------------------------------------------------------------------
void VoiceSDK::EndVoiceListening()
{
//#ifdef __ANDROID__
//	VoiceEndVoiceListening();
//#else
	PX2_SS.StopRecording();

	// 结束录音，请求对话
	OnVoiceRecordEnd();

	unsigned char *buf = 0;
	unsigned int bufSize = 0;
	PX2_SS.GetRecordingBuf(buf, bufSize);
	std::string voiceJson = _GetVoiceText(buf, bufSize);
	delete [] buf;
	buf = 0;

	JSONData jsData;
	if (jsData.LoadBuffer(voiceJson))
	{
		if (jsData.IsHasMember("result"))
		{
			JSONValue jsVal = jsData.GetMember("result");
			std::string retStr = jsVal.GetArrayElement(0).ToString();
			retStr = UrlDecode(retStr);

			OnVoiceRecognizeResults(retStr, voiceJson);

			if (mIsIngoreThisTimeAutoSpeech)
			{
				mIsIngoreThisTimeAutoSpeech = false;
			}
			else
			{
				if (mIsAutoSpeakEnabled)
				{
					GetAnswer(retStr, mIsAutoTTSSpeakEnabled);
				}
			}
		}
		else
		{
			PX2_LOG_INFO("%s", voiceJson.c_str());
		}
	}
//#endif
}
//----------------------------------------------------------------------------
void VoiceSDK::OnSpeakStart()
{
	Event *ent = PX2_CREATEEVENTEX(VoiceSDKSpace, SpeakStart);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void VoiceSDK::OnSpeakPause()
{
	Event *ent = PX2_CREATEEVENTEX(VoiceSDKSpace, SpeakPause);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void VoiceSDK::OnSpeakCancel()
{
	Event *ent = PX2_CREATEEVENTEX(VoiceSDKSpace, SpeakCancel);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void VoiceSDK::OnSpeakFinish()
{
	Event *ent = PX2_CREATEEVENTEX(VoiceSDKSpace, SpeakFinish);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void VoiceSDK::OnSpeakText(std::string &txt)
{
	Event *ent = PX2_CREATEEVENTEX(VoiceSDKSpace, SpeakText);
	ent->SetDataStr0(txt);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void VoiceSDK::OnVoiceRecordStart()
{
	Event *ent = PX2_CREATEEVENTEX(VoiceSDKSpace, RecordStart);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void VoiceSDK::OnVoiceRecordEnd()
{
	Event *ent = PX2_CREATEEVENTEX(VoiceSDKSpace, RecordEnd);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void VoiceSDK::OnVoiceRecognizeResults(const std::string &strRet,
	const std::string &strJSON)
{
	VoiceData vData;
	vData.StrRet = strRet;
	vData.StrJSON = strJSON;
	Event *ent = PX2_CREATEEVENTEX(VoiceSDKSpace, RecognizeResults);
	ent->SetDataStr0(strRet);
	ent->SetDataStr1(strJSON);
	ent->SetData<VoiceData>(vData);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void VoiceSDK::EnableAutoSpeak(bool isAutoSpeakEnable)
{
	mIsAutoSpeakEnabled = isAutoSpeakEnable;
	
//#ifdef __ANDROID__
//	VoiceEnableAutoSpeak(isAutoSpeakEnable);
//#endif
}
//----------------------------------------------------------------------------
bool VoiceSDK::IsAutoSpeakEnabled() const
{
	return mIsAutoSpeakEnabled;
}
//----------------------------------------------------------------------------
void VoiceSDK::EnableAutoSpeakTTS(bool isAutoSpeakTTSEnable)
{
	mIsAutoTTSSpeakEnabled = isAutoSpeakTTSEnable;

//#ifdef __ANDROID__
//	VoiceEnableAutoSpeakTTS(isAutoSpeakTTSEnable);
//#endif
}
//----------------------------------------------------------------------------
bool VoiceSDK::IsAutoSpeakTTSEnabled() const
{
	return mIsAutoTTSSpeakEnabled;
}
//----------------------------------------------------------------------------
void VoiceSDK::Speak(const std::string &text)
{
	PX2_UNUSED(text);

//#ifdef __ANDROID__
//	VoiceSpeak(text.c_str());
//#else
	//UrlEncode(text)
	// 请求百度
	if (std::string("baidu") == GetSDK_TTS())
	{
		std::string url = "http://tsn.baidu.com/text2audio?tex='"
			+ UrlEncode(text) 
			+ "'&lan=zh&per=2&pit=4&spd=7&cuid=11221194&ctp=1&tok=" 
			+ mTTS_Token;
		PX2_SS.PlayASound(url.c_str(), 1.0f, -1.0f);
		OnSpeakStart();
	}
//#endif
}
//----------------------------------------------------------------------------
void VoiceSDK::PlayHotMusic()
{
	std::string url = "http://www.manykit.com/api/music/list";
	
	mCurl->Get(url);

	char *chMem = mCurl->GetGettedMemory();
	int size = mCurl->GetGettedMemorySize();
	std::string ret = std::string(chMem, size);

	JSONData data;
	if (data.LoadBuffer(ret))
	{
		JSONValue dataVal = data.GetMember("data");
		if (dataVal.IsArray())
		{
			JSONValue jv = dataVal.GetArrayElement(0);
			std::string hash = jv.GetMember("hash").ToString();

			_PlayFromHash(hash);
		}
	}
}
//----------------------------------------------------------------------------
void VoiceSDK::PlayMusic(const std::string &musicName)
{
	std::string url = "http://www.manykit.com/api/music/search?word="
		+ UrlEncode(musicName) + "&page=1&pagesize=20";

	mCurl->Get(url);

	char *chMem = mCurl->GetGettedMemory();
	int size = mCurl->GetGettedMemorySize();
	std::string ret = std::string(chMem, size);

	JSONData data;
	if (data.LoadBuffer(ret))
	{
		JSONValue dataVal = data.GetMember("data");
		if (dataVal.IsArray())
		{
			JSONValue jv = dataVal.GetArrayElement(0);
			std::string hash = jv.GetMember("hash").ToString();

			_PlayFromHash(hash);
		}
	}
}
//----------------------------------------------------------------------------
void VoiceSDK::_PlayFromHash(const std::string &hash)
{
	std::string musicpath = "http://www.manykit.com/api/music/song?hash=" 
		+ hash;
	mCurl->Get(musicpath);

	char *chMem = mCurl->GetGettedMemory();
	int size = mCurl->GetGettedMemorySize();
	std::string musicRet = std::string(chMem, size);

	JSONData data;
	if (data.LoadBuffer(musicRet))
	{
		JSONValue dataMusicVal = data.GetMember("data");

		JSONValue dataMusic = dataMusicVal.GetMember("url");
		std::string musicUrl = dataMusic.ToString();
		if (!musicUrl.empty())
		{
			PX2_SS.ClearAllSounds();
			PX2_SS.PlayASound(musicUrl.c_str(), 1.0f, 400.0f);
		}
	}
}
//----------------------------------------------------------------------------
const std::string &VoiceSDK::GetTTSTokenStr() const
{
	return mTTS_Token;
}
//----------------------------------------------------------------------------
std::string VoiceSDK::_GetTokenJsonTTS()
{
	mCurl->ClearOptionList();
	std::string allURL;

	if (std::string("baidu") == GetSDK_TTS())
	{
		std::string autoStr =
			"http://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&";
		std::string dataStr = std::string("client_id=") + mTTS_ApiStr +
			"&client_secret=" + mTTS_SecStr;
		allURL = autoStr + dataStr;
	}

	mCurl->Get(allURL);
	char *chMem = mCurl->GetGettedMemory();
	int size = mCurl->GetGettedMemorySize();
	return std::string(chMem, size);
}
//----------------------------------------------------------------------------
std::string VoiceSDK::_GetVoiceText(unsigned char *buf, unsigned int size)
{
	if ("baidu" == GetSDK_TTS())
	{
		mCurl->ClearOptionList();
		mCurl->AddOptionListStr("Content-Type: audio/pcm; rate=16000");
		std::string strLength = StringHelp::IntToString(size);
		std::string str = "Content-Length: " + strLength;
		mCurl->AddOptionListStr(str);

		std::string dataStr((char*)buf, size);
		std::string url = std::string("http://vop.baidu.com/server_api")
			+ "?cuid=" + mTTS_ProductIDStr + "&token=" + mTTS_Token;
		mCurl->Post(url, dataStr);
		mSTTStr = std::string(mCurl->GetGettedMemory(),
			mCurl->GetGettedMemorySize());
	}

	return mSTTStr;
}
//----------------------------------------------------------------------------
std::string VoiceSDK::GetAnswer(const std::string &askStr, bool doSpeeck)
{
	if ("tuling" == GetSDK_AutoSpeech())
	{
		mCurl->ClearOptionList();

		std::string request =
			std::string("http://www.tuling123.com/openapi/api?key=") + mAutoSpeech_ApiStr
			+ "&info=" + askStr;
		mCurl->Get(request);
		char *chMem = mCurl->GetGettedMemory();
		int size = mCurl->GetGettedMemorySize();

		std::string retStr = std::string(chMem, size);
		if (doSpeeck)
		{
			JSONData jsData;
			if (jsData.LoadBuffer(retStr))
			{
				std::string txtUtf8 = jsData.GetMember("text").ToString();
				Speak(UrlDecode(txtUtf8));
				return txtUtf8;
			}
		}
	}

	return "";
}
//----------------------------------------------------------------------------