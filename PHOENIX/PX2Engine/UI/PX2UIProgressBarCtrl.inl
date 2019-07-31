// PX2UIProgressBarCtrl.inl

//----------------------------------------------------------------------------
inline void UIProgressBarCtrl::SetProgressSpeed(float speed)
{
	mSpeed = speed;
}
//----------------------------------------------------------------------------
inline float UIProgressBarCtrl::GetProgressSpeed() const
{
	return mSpeed;
}
//----------------------------------------------------------------------------
inline float UIProgressBarCtrl::GetProgress() const
{
	return mProgress;
}
//----------------------------------------------------------------------------