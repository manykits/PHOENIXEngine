// PX2VoiceEventType.hpp

#ifndef PX2VOICEEVENTTYPE_HPP
#define PX2VOICEEVENTTYPE_HPP

#include "PX2UnityPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM VoiceSDKSpace)
	PX2_EVENT(SpeakStart)
	PX2_EVENT(SpeakPause)
	PX2_EVENT(SpeakCancel)
	PX2_EVENT(SpeakFinish)
	PX2_EVENT(SpeakText)
	PX2_EVENT(RecordStart)
	PX2_EVENT(RecordEnd)
	PX2_EVENT(RecognizeResults)
	PX2_DECLARE_EVENT_END(VoiceSDKSpace)

	class PX2_ENGINE_ITEM VoiceData
	{
	public:
		VoiceData();
		~VoiceData();

		std::string StrRet;
		std::string StrJSON;
	};

}

#endif