// PX2Event.hpp

#ifndef PX2EVENT_HPP
#define PX2EVENT_HPP

#include "PX2CorePre.hpp"
#include "PX2EventChannel.hpp"
#include "PX2EventHandler.hpp"
#include "PX2EventData.hpp"

namespace PX2
{

	/// 系统事件
	enum SystemEvent
	{
		EVENT_NONE = -1,
	};

	/// 事件对象
	class PX2_ENGINE_ITEM Event
	{
	public:
		typedef int EventType;

	public:
		Event ();
		Event (const Event& event);
		~Event ();

		// 赋值
		Event& operator= (const Event& event);

		// 事件
		void SetEventType (EventType eventType);
		EventType GetEventType ();

		void SetEventTypeStr(const std::string &typeStr);
		const std::string &GetEventTypeStr() const;

		// 通道
		void SetChannel (const EventChannel& eventChannel);
		const EventChannel &GetChannel ();

		// 延时(秒)
		void SetTimeDelay (float timeDelay);
		float GetTimeDelay ();
		bool IsDoDelay ();

		// 数据
		template <class DType>
		void SetData (DType data);

		template <class DType>
		DType GetData ();

		void SetDataStr0(const std::string &dataStr);
		void SetDataStr1(const std::string &dataStr);
		void SetDataStr2(const std::string &dataStr);
		const std::string &GetDataStr0() const;
		const std::string &GetDataStr1() const;
		const std::string &GetDataStr2() const;

		void SetDataPointer0(void *ptr);
		void *GetDataPointer0();
		void SetDataPointer1(void *ptr);
		void *GetDataPointer1();

		// 发送/接收者
		void SetSender (EventHandler *handler);
		EventHandler* GetSender ();
		void SetReceiver (EventHandler *handler);
		EventHandler* GetReceiver ();

public_internal:
		void Update (float timeDetail);
		bool IsDelayActted (); //< 延时时间的时间倒计时到0，触发事件
		bool IsSystemEvent () const;
		void SetBeSystemEvent (); //< mEventChannel为0消息
		void Initlize (const Event &event);

		bool mIsDoDelay;
		float mDelayTime;
		float mDelayTiming;

		EventChannel mEventChannel;
		EventType mEventType;
		std::string mEventTypeStr;
		EventData mEventData;
		std::string mDataStr0;
		std::string mDataStr1;
		std::string mDataStr2;
		void *mDataPointer0;
		void *mDataPointer1;
		
		EventHandler* mSender;
		EventHandler* mReceiver;
	};

#include "PX2Event.inl"

}

#endif