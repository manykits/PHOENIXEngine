// PX2Event.cpp

#include "PX2Event.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

Event::Event ()
:
mEventType(0),
mSender(0),
mReceiver(0),
mDelayTime(0.0f),
mDelayTiming(0.0f),
mIsDoDelay(false),
mDataPointer0(0),
mDataPointer1(0)
{
	mEventChannel.FillUserChannel();
}
//----------------------------------------------------------------------------
Event::Event (const Event& event)
{
	Initlize(event);
}
//----------------------------------------------------------------------------
Event::~Event ()
{
	mSender = 0;
	mReceiver = 0;
}
//----------------------------------------------------------------------------
Event& Event::operator= (const Event& event)
{
	Initlize(event);
	return (*this);
}
//----------------------------------------------------------------------------
void Event::SetEventType (EventType eventType)
{
	mEventType = eventType;
}
//----------------------------------------------------------------------------
Event::EventType Event::GetEventType ()
{
	return mEventType;
}
//----------------------------------------------------------------------------
void Event::SetEventTypeStr(const std::string &typeStr)
{
	mEventTypeStr = typeStr;
}
//----------------------------------------------------------------------------
const std::string &Event::GetEventTypeStr() const
{
	return mEventTypeStr;
}
//----------------------------------------------------------------------------
void Event::SetChannel (const EventChannel& eventChannel)
{
	if (eventChannel.IsEmpty())
	{
		assertion(false, "input eventChannel must not be empty.");
	}

	mEventChannel = eventChannel;
}
//----------------------------------------------------------------------------
const EventChannel& Event::GetChannel ()
{
	return mEventChannel;
}
//----------------------------------------------------------------------------
void Event::SetTimeDelay (float timeDelay)
{
	mDelayTime = timeDelay;
	mDelayTiming = timeDelay;

	if (mDelayTiming > 0.0f)
		mIsDoDelay = true;
}
//----------------------------------------------------------------------------
void Event::SetSender (EventHandler *handler)
{
	mSender = handler;
}
//----------------------------------------------------------------------------
EventHandler* Event::GetSender ()
{
	return mSender;
}
//----------------------------------------------------------------------------
void Event::SetReceiver (EventHandler *handler)
{
	mReceiver = handler;
}
//----------------------------------------------------------------------------
EventHandler* Event::GetReceiver ()
{
	return mReceiver;
}
//----------------------------------------------------------------------------
void Event::SetDataStr0(const std::string &dataStr)
{
	mDataStr0 = dataStr;
}
//----------------------------------------------------------------------------
void Event::SetDataStr1(const std::string &dataStr)
{
	mDataStr1 = dataStr;
}
//----------------------------------------------------------------------------
void Event::SetDataStr2(const std::string &dataStr)
{
	mDataStr2 = dataStr;
}
//----------------------------------------------------------------------------
const std::string &Event::GetDataStr0() const
{
	return mDataStr0;
}
//----------------------------------------------------------------------------
const std::string &Event::GetDataStr1() const
{
	return mDataStr1;
}
//----------------------------------------------------------------------------
const std::string &Event::GetDataStr2() const
{
	return mDataStr2;
}
//----------------------------------------------------------------------------
void Event::SetDataPointer0(void *ptr)
{
	mDataPointer0 = ptr;
}
//----------------------------------------------------------------------------
void *Event::GetDataPointer0()
{
	return mDataPointer0;
}
//----------------------------------------------------------------------------
void Event::SetDataPointer1(void *ptr)
{
	mDataPointer1 = ptr;
}
//----------------------------------------------------------------------------
void *Event::GetDataPointer1()
{
	return mDataPointer1;
}
//----------------------------------------------------------------------------
bool Event::IsSystemEvent () const
{
	return mEventChannel.IsEmpty();
}
//----------------------------------------------------------------------------
void Event::SetBeSystemEvent ()
{
	mEventChannel.Clear();
}
//----------------------------------------------------------------------------
void Event::Initlize (const Event &event)
{
	mEventChannel = event.mEventChannel;
	mEventType = event.mEventType;
	mEventData = event.mEventData;
	mSender = event.mSender;
	mReceiver = event.mReceiver;
}
//----------------------------------------------------------------------------
void Event::Update (float timeDetail)
{
	if (mIsDoDelay)
	{
		mDelayTiming -= timeDetail;

		if (mDelayTiming < 0.0f)
			mDelayTiming = 0.0f;
	}
}
//----------------------------------------------------------------------------
bool Event::IsDelayActted ()
{
	if (mIsDoDelay && 0.0f==mDelayTiming)
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------