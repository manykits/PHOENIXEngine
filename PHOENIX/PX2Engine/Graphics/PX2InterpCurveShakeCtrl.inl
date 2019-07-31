// PX2InterpCurveShakeCtrl.inl

//----------------------------------------------------------------------------
inline void InterpCurveShakeController::SetShakeRangeType (ShakeRangeType type)
{
	mShakeRangeType = type;
}
//----------------------------------------------------------------------------
inline InterpCurveShakeController::ShakeRangeType 
	InterpCurveShakeController::GetShakeRangeType ()
{
	return mShakeRangeType;
}
//----------------------------------------------------------------------------