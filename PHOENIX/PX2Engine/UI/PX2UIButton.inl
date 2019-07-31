// PX2UIButton.inl

//----------------------------------------------------------------------------
inline UIButton::AfterReleasedType UIButton::GetAfterReleasedType() const
{
	return mAfterReleasedType;
}
//----------------------------------------------------------------------------
inline void UIButton::SetAfterReleasedRecoverTime(float time)
{
	mAfterReleasedRecoverTime = time;
}
//----------------------------------------------------------------------------
inline float UIButton::GetAfterReleasedRecoverTime() const
{
	return mAfterReleasedRecoverTime;
}
//----------------------------------------------------------------------------