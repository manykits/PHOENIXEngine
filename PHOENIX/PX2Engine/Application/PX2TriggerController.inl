// PX2TriggerController.inl

//----------------------------------------------------------------------------
inline TriggerController::AreaType TriggerController::GetAreaType() const
{
	return mAreaType;
}
//----------------------------------------------------------------------------
inline void TriggerController::SetAreaParam(const AVector &vec)
{
	mAreaParam = vec;
}
//----------------------------------------------------------------------------
inline const AVector &TriggerController::GetAreaParam() const
{
	return mAreaParam;
}
//----------------------------------------------------------------------------
inline bool TriggerController::IsTrigered() const
{
	return mIsTrigered;
}
//----------------------------------------------------------------------------
inline void TriggerController::SetTrigerType(TrigerType type)
{
	mTrigerType = type;
}
//----------------------------------------------------------------------------
inline TriggerController::TrigerType TriggerController::GetTrigerType() const
{
	return mTrigerType;
}
//----------------------------------------------------------------------------