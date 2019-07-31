// PX2UISizeExtendControl.inl

//----------------------------------------------------------------------------
inline void UISizeExtendControl::SetMode(UISizeExtendControl::Mode m)
{
	mMode = m;
}
//----------------------------------------------------------------------------
inline UISizeExtendControl::Mode UISizeExtendControl::GetMode() const
{
	return mMode;
}
//----------------------------------------------------------------------------
inline float UISizeExtendControl::GetWidthOverHeightPercent() const
{
	return mWidthOverHeightPercent;
}
//----------------------------------------------------------------------------
inline void UISizeExtendControl::SetOriginHeight(float height)
{
	mOriginHeight = height;
}
//----------------------------------------------------------------------------
inline float UISizeExtendControl::GetOriginHeight() const
{
	return mOriginHeight;
}
//----------------------------------------------------------------------------