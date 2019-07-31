// PX2InterpCurveFloatController.inl

//----------------------------------------------------------------------------
inline void InterpCurveFloatController::SetInitValue (float val)
{
	mInitValue = val;
}
//----------------------------------------------------------------------------
inline const float &InterpCurveFloatController::GetInitValue () const
{
	return mInitValue;
}
//----------------------------------------------------------------------------
inline const float &InterpCurveFloatController::GetCurValueRaw () const
{
	return mCurValueRaw;
}
//----------------------------------------------------------------------------
inline const float &InterpCurveFloatController::GetCurValueRelatived () const
{
	return mCurValueRelatived;
}
//----------------------------------------------------------------------------