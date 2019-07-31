// PX2Thread.inl

//----------------------------------------------------------------------------
inline int Thread::GetID ()
{
	return mID;
}
//----------------------------------------------------------------------------
inline std::string Thread::GetName ()
{
	return mName;
}
//----------------------------------------------------------------------------
inline Thread::Priority Thread::GetPriority() const
{
	return mPriority;
}
//----------------------------------------------------------------------------
inline int Thread::GetStackSize() const
{
	return mStackSize;
}
//----------------------------------------------------------------------------
inline ThreadType Thread::GetThread ()
{
	return mThread;
}
//----------------------------------------------------------------------------
inline unsigned int Thread::GetThreadID ()
{
	return mThreadID;
}
//----------------------------------------------------------------------------