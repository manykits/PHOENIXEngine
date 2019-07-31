// PX2InputPushTransformController.inl

//----------------------------------------------------------------------------
inline const AVector &InputPushTransformController::GetLockDir() const
{
	return mLockDir;
}
//----------------------------------------------------------------------------
inline void InputPushTransformController::SetPushTriggerSpeed(float speed)
{
	mPushTriggerSpeed = speed;
}
//----------------------------------------------------------------------------
inline float InputPushTransformController::GetPusTriggerSpeed() const
{
	return mPushTriggerSpeed;
}
//----------------------------------------------------------------------------
inline void InputPushTransformController::SetTouchConvertCallback(
	TouchConvertCallback callback)
{
	mConvertCallback = callback;
}
//----------------------------------------------------------------------------
inline TouchConvertCallback InputPushTransformController
::GetTouchConvertCallback() const
{
	return mConvertCallback;
}
//----------------------------------------------------------------------------