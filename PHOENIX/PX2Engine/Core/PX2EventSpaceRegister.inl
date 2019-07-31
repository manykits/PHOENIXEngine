// PX2EventSpaceRegister.inl

//----------------------------------------------------------------------------
template <class ES>
EventSpaceRegister<ES>::EventSpaceRegister ()
:
EventSpace(ES::mName, ES::E_QUANTITY)
{
	EventFactory::GetInstance().RegisterEventSpace(this);
}
//----------------------------------------------------------------------------
template <class ES>
EventSpaceRegister<ES>::~EventSpaceRegister ()
{
	EventFactory::GetInstance().UnRegisterEventSpace(this->GetName());
}
//----------------------------------------------------------------------------
template <class ES>
Event *EventSpaceRegister<ES>::CreateEventX (Event::EventType coveredType)
{
	return ES::GetPtr()->CreateEventX(coveredType);
}
//----------------------------------------------------------------------------
template <class ES>
Event *EventSpaceRegister<ES>::CreateEventX(Event::EventType coveredType, 
	std::string typeStr)
{
	Event *ent = ES::GetPtr()->CreateEventX(coveredType);
	ent->SetEventTypeStr(typeStr);

	return ent;
}
//----------------------------------------------------------------------------
template <class ES>
bool EventSpaceRegister<ES>::IsEqual (Event *event, Event::EventType coveredType)
{
	return ES::GetPtr()->IsEqual(event, coveredType);
}
//----------------------------------------------------------------------------
template <class ES>
Event::EventType EventSpaceRegister<ES>::UnCover (Event::EventType coveredType)
{
	return ES::GetPtr()->UnCover(coveredType);
}
//----------------------------------------------------------------------------
template <class ES>
Event::EventType EventSpaceRegister<ES>::Cover (Event *event)
{
	return ES::GetPtr()->Cover(event);
}
//----------------------------------------------------------------------------
template <class ES>
bool EventSpaceRegister<ES>::IsIn (Event *event)
{
	return ES::GetPtr()->IsIn(event);
}
//----------------------------------------------------------------------------