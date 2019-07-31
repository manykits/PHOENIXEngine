// PX2VoiceSDK.hpp

#ifndef PX2VOICESDK_HPP
#define PX2VOICESDK_HPP

#include "PX2CorePre.hpp"
#include "PX2Singleton.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2CURL.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM VoiceSDK : public Singleton<VoiceSDK>
	{
	public:
		VoiceSDK();
		virtual ~VoiceSDK();

		// you must call this before use tts
		void Initlize();

		void SetSDK_AutoSpeech(const std::string &name);
		const std::string &GetSDK_AutoSpeech() const;

		void SetSDK_TTS(const std::string &name);
		const std::string &GetSDK_TTS() const;

		void IngoreThisTimeAutoSpeech();

		// keys
		void SetAutoSpeech_ProductID(const std::string &strID);
		const std::string &GetAutoSpeech_ProductID() const;
		void SetAutoSpeech_APIKey(const std::string &apiKey);
		const std::string &GetAutoSpeech_APIKey() const;
		void SetAutoSpeech_SecretKey(const std::string &secretKey);
		const std::string &GetAutoSpeech_SecretKey() const;

		void SetTTS_ProdcutID(const std::string &strID);
		const std::string &GetTTS_ProductID() const;
		void SetTTS_APIKey(const std::string &apiKey);
		const std::string &GetTTS_APIKey() const;
		void SetTTS_SecretKey(const std::string &secretKey);
		const std::string &GetTTS_SecretKey() const;

		// TTSToken
		const std::string &GetTTSTokenStr() const;

		// functions
		void StartVoiceListening();
		void EndVoiceListening();
		const std::string &GetSTTStr() const;

		void OnSpeakStart();
		void OnSpeakPause();
		void OnSpeakCancel();
		void OnSpeakFinish();
		void OnSpeakText(std::string &txt);

		void OnVoiceRecordStart();
		void OnVoiceRecordEnd();
		void OnVoiceRecognizeResults(const std::string &strRet, 
			const std::string &strJSON);

		// 是否允许自由对话
		void EnableAutoSpeak(bool isAutoSpeakEnable);
		bool IsAutoSpeakEnabled() const;

		void EnableAutoSpeakTTS(bool isAutoSpeakTTSEnable);
		bool IsAutoSpeakTTSEnabled() const;
		std::string GetAnswer(const std::string &askStr, bool doSpeeck = false);

		void Speak(const std::string &text);

		void PlayHotMusic();
		void PlayMusic(const std::string &musicName);

	private:
		void _PlayFromHash(const std::string &hash);

		std::string _GetTokenJsonTTS();
		std::string _GetVoiceText(unsigned char *buf, unsigned int size);

		std::string mSDK_AutoSpeech;
		std::string mSDK_TTS;

		bool mIsAutoSpeakEnabled;
		bool mIsAutoTTSSpeakEnabled;
		bool mIsIngoreThisTimeAutoSpeech;

		std::string mAutoSpeech_ProductIDStr;
		std::string mAutoSpeech_ApiStr;
		std::string mAutoSpeech_SecStr;

		std::string mTTS_Token;
		std::string mTTS_ProductIDStr;
		std::string mTTS_ApiStr;
		std::string mTTS_SecStr;

		std::string mSTTStr;

		CurlObjPtr mCurl;
	};

#define PX2_VOICESDK VoiceSDK::GetSingleton()

}

#endif