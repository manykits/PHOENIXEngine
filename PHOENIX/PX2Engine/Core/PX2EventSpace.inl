// PX2EventSpace.inl

//----------------------------------------------------------------------------
inline void EventSpace::Space::Reset (int begin, int end)
{
	mBegin = begin;
	mEnd = end;
}
//----------------------------------------------------------------------------
inline Event::EventType EventSpace::Space::Cover (Event* event) const
{
	assertion(event!=0, "event must exist.");

	return (event->GetEventType() - mBegin);
}
//----------------------------------------------------------------------------
inline Event::EventType EventSpace::Space::UnCover (Event::EventType eventType) const
{
	return (mBegin + eventType);
}
//----------------------------------------------------------------------------
inline bool EventSpace::Space::IsIn (Event* event) const
{
	assertion(event!=0, "event must exist.");

	Event::EventType eventType = event->GetEventType();
	
	return (eventType >= mBegin) && (eventType < mEnd);
}
//----------------------------------------------------------------------------
inline bool EventSpace::Space::IsEqual (Event* event, Event::EventType eventType) const
{
	assertion(event!=0, "event must exist.");

	return Cover(event) == eventType;
}
//----------------------------------------------------------------------------