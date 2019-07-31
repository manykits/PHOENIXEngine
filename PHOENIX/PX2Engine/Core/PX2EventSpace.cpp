// PX2EventSpace.cpp

#include "PX2EventSpace.hpp"
#include "PX2EventFactory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EventSpace::Space::Space () :
mBegin(0),
mEnd(0)
{
}
//----------------------------------------------------------------------------
EventSpace::Space::~Space ()
{
}
//----------------------------------------------------------------------------
EventSpace::EventSpace (const std::string &name, int eventQuantity)
:
mName(name),
mEventQuantity(eventQuantity)
{
}
//----------------------------------------------------------------------------
EventSpace::~EventSpace ()
{
}
//----------------------------------------------------------------------------
const std::string& EventSpace::GetName() const
{
	return mName;
}
//----------------------------------------------------------------------------
int EventSpace::GetEventQuantity()
{
	return mEventQuantity;
}
//----------------------------------------------------------------------------
bool EventSpace::IsIn (Event* event) const
{
	return mSpace.IsIn(event);
}
//----------------------------------------------------------------------------
Event* EventSpace::CreateEventX (Event::EventType eventType)
{
	Event* event = 0;
	event = EventFactory::GetInstance().CreateEventX();
	if (event) event->SetEventType(mSpace.UnCover(eventType));

	return event;
}
//----------------------------------------------------------------------------
Event::EventType EventSpace::UnCover (Event::EventType eventType)
{
	return mSpace.UnCover(eventType);
}
//----------------------------------------------------------------------------
bool EventSpace::IsEqual (Event* event, Event::EventType eventType) const
{
	return mSpace.IsEqual(event, eventType);
}
//----------------------------------------------------------------------------
Event::EventType EventSpace::Cover (Event* event) const
{
	assertion(event!=0, "event must exist.");
	return mSpace.Cover(event);
}
//----------------------------------------------------------------------------
void EventSpace::SetSpace (int shift)
{
	mSpace.Reset(shift, shift + GetEventQuantity());
}
//----------------------------------------------------------------------------