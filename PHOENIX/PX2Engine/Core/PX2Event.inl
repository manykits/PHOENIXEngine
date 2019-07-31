// PX2Event.inl

//----------------------------------------------------------------------------
inline bool Event::IsDoDelay ()
{
	return mDelayTime>0.0f;
}
//----------------------------------------------------------------------------
inline float Event::GetTimeDelay ()
{
	return mDelayTime;
}
//----------------------------------------------------------------------------
template <class DType>
void Event::SetData (DType data)
{
	mEventData = EventDataTemplate<DType>::Set(data);
}
//----------------------------------------------------------------------------
template <class DType>
DType Event::GetData ()
{
	return EventDataTemplate<DType>::Get(mEventData);
}
//----------------------------------------------------------------------------