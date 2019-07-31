// PX2TCPServerParams.inl

//----------------------------------------------------------------------------
inline const Timespan& TCPServerParams::GetThreadIdleTime() const
{
	return mThreadIdleTime;
}
//----------------------------------------------------------------------------
inline int TCPServerParams::GetNumMaxThreads() const
{
	return mMaxThreads;
}
//----------------------------------------------------------------------------
inline int TCPServerParams::GetMaxQueued() const
{
	return mMaxQueued;
}
//----------------------------------------------------------------------------
inline Thread::Priority TCPServerParams::GetThreadPriority() const
{
	return mThreadPriority;
}
//----------------------------------------------------------------------------