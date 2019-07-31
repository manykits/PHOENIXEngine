// PX2UISlider.inl

//----------------------------------------------------------------------------
inline UISlider::DirectionType UISlider::GetDirectionType()
{
	return mDirectionType;
}
//----------------------------------------------------------------------------
inline UIFPicBox *UISlider::GetFPicBoxBack()
{
	return mFPicBoxBack;
}
//----------------------------------------------------------------------------
inline UIButton *UISlider::GetButSlider()
{
	return mButSlider;
}
//----------------------------------------------------------------------------
inline UIFrame *UISlider::GetContentFrame()
{
	return mContentFrame;
}
//----------------------------------------------------------------------------
inline float UISlider::GetSliderLength() const
{
	return mButSliderLength;
}
//----------------------------------------------------------------------------
inline float UISlider::GetPercent() const
{
	return mPercent;
}
//----------------------------------------------------------------------------
inline bool UISlider::IsDraging() const
{
	return mIsDraging;
}
//----------------------------------------------------------------------------