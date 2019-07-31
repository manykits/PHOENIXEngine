// PX2EventChannel.cpp

#include "PX2EventChannel.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EventChannel::EventChannel ()
{
	mEventChannelType = 0;
}
//----------------------------------------------------------------------------
EventChannel::EventChannel (const EventChannel &channel)
{
	Initlize(channel);
}
//----------------------------------------------------------------------------
EventChannel::~EventChannel ()
{
}
//----------------------------------------------------------------------------
EventChannel &EventChannel::operator= (const EventChannel &channel)
{
	Initlize(channel);

	return (*this);
}
//----------------------------------------------------------------------------
EventChannel &EventChannel::Fill ()
{
	mEventChannelType = ~(static_cast<EventChannelType>(0));

	return (*this);
}
//----------------------------------------------------------------------------
EventChannel &EventChannel::Clear ()
{
	mEventChannelType = 0;

	return (*this);
}
//----------------------------------------------------------------------------
EventChannel &EventChannel::FillUserChannel ()
{
	Fill();
	return (*this);
}
//----------------------------------------------------------------------------
EventChannel &EventChannel::ClearUserChannel ()
{
	Clear();
	return (*this);
}
//----------------------------------------------------------------------------
EventChannel &EventChannel::FillSystemChannel ()
{
	Fill();
	return (*this);
}
//----------------------------------------------------------------------------
EventChannel &EventChannel::ClearSystemChannel ()
{
	Clear();
	return (*this);
}
//----------------------------------------------------------------------------
void EventChannel::Merge (const EventChannel &channel)
{
	AddChannel(channel.mEventChannelType);
}
//----------------------------------------------------------------------------
void EventChannel::DisMerge (const EventChannel &channel)
{
	RemoveChannel(channel.mEventChannelType);
}
//----------------------------------------------------------------------------
bool EventChannel::IsEmpty () const
{
	return (mEventChannelType == 0);
}
//----------------------------------------------------------------------------
bool EventChannel::IsListening (const EventChannel &channel) const
{
	return ((mEventChannelType&channel.mEventChannelType) != 0);
}
//----------------------------------------------------------------------------
EventChannel EventChannel::Create ()
{
	return EventChannel();
}
//----------------------------------------------------------------------------
void EventChannel::Initlize (const EventChannel &channel)
{
	mEventChannelType = channel.mEventChannelType;
}
//----------------------------------------------------------------------------
void EventChannel::AddChannel (const EventChannelType type)
{
	mEventChannelType |= type;
}
//----------------------------------------------------------------------------
void EventChannel::RemoveChannel (const EventChannelType type)
{
	mEventChannelType &= ~type;
}
//----------------------------------------------------------------------------