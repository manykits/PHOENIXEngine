// PX2EventHandler.inl

//----------------------------------------------------------------------------
inline bool EventHandler::IsUpdateEvent (Event *event)
{
	return (event->mEventType == EVENT_UPDATE_HANDLER);
}
//----------------------------------------------------------------------------