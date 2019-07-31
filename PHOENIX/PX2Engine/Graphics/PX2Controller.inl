// PX2Controller.inl

//----------------------------------------------------------------------------
inline int Controller::GetPriority () const
{
	return mPriority;
}
//----------------------------------------------------------------------------
inline bool Controller::IsPlaySelfCtrl() const
{
	return mIsPlaySelfCtrl;
}
//----------------------------------------------------------------------------
inline float Controller::GetMaxPlayTime () const
{
	return mMaxPlayTime;
}
//----------------------------------------------------------------------------
inline float Controller::GetDelayPlayTime () const
{
	return mDelayPlayTime;
}
//----------------------------------------------------------------------------
inline Controlledable* Controller::GetControlledable () const
{
    return mObject;
}
//----------------------------------------------------------------------------
inline float Controller::GetPlayedTime () const
{
	return mPlayedTime;
}
//----------------------------------------------------------------------------
inline void Controller::SetPlayedDoDetach(bool doDetach)
{
	mIsPlayedDoDetach = doDetach;
}
//----------------------------------------------------------------------------
inline bool Controller::IsPlayedDoDetach () const
{
	return mIsPlayedDoDetach;
}
//----------------------------------------------------------------------------
inline void Controller::SetPlayedCallback (CtrlPlayedCallback callback)
{
	mPlayedCallback = callback;
}
//----------------------------------------------------------------------------
inline CtrlPlayedCallback Controller::GetPlayedCallback ()
{
	return mPlayedCallback;
}
//----------------------------------------------------------------------------