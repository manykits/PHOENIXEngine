// PX2InterpCurveFloat3Controller.inl

//----------------------------------------------------------------------------
inline void InterpCurveFloat3Controller::SetInitValue (const Float3 &val)
{
	mInitValue = val;
}
//----------------------------------------------------------------------------
inline const Float3 &InterpCurveFloat3Controller::GetInitValue () const
{
	return mInitValue;
}
//----------------------------------------------------------------------------
inline const Float3 &InterpCurveFloat3Controller::GetCurValueRaw () const
{
	return mCurValueRaw;
}
//----------------------------------------------------------------------------
inline const Float3 &InterpCurveFloat3Controller::GetCurValueRelatived () const
{
	return mCurValueRelatived;
}
//----------------------------------------------------------------------------