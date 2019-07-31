// PX2InterpCurveController.inl

//----------------------------------------------------------------------------
inline void InterpCurveController::SetRelatively (bool r)
{
	mIsRelatively = r;
}
//----------------------------------------------------------------------------
inline bool InterpCurveController::IsRelatively () const
{
	return mIsRelatively;
}
//----------------------------------------------------------------------------
inline void InterpCurveController::SetAttachUpdateInit (bool doUpdate)
{
	mIsDoAttachUpdate = doUpdate;
}
//----------------------------------------------------------------------------
inline bool InterpCurveController::IsAttachUpdateInit () const
{
	return mIsDoAttachUpdate;
}
//----------------------------------------------------------------------------
inline void InterpCurveController::SetDetachResetInit (bool doReset)
{
	mIsDoDetachReset = doReset;
}
//----------------------------------------------------------------------------
inline bool InterpCurveController::IsDetachResetInit () const
{
	return mIsDoDetachReset;
}
//----------------------------------------------------------------------------