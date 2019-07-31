// PX2EffectModule.inl

//----------------------------------------------------------------------------
inline void EffectModule::SetTypeName (const std::string &typeName)
{
	mTypeName = typeName;
}
//----------------------------------------------------------------------------
inline const std::string &EffectModule::GetTypeName () const
{
	return mTypeName;
}
//----------------------------------------------------------------------------
inline void EffectModule::SetRepeatType (RepeatType type)
{
	mRepeatType = type;
}
//----------------------------------------------------------------------------
inline EffectModule::RepeatType EffectModule::GetRepeatType () const
{
	return mRepeatType;
}
//----------------------------------------------------------------------------
inline void EffectModule::SetRepeatTime (float time)
{
	mRepeatTime = time;
}
//----------------------------------------------------------------------------
inline float EffectModule::GetRepeatTime () const
{
	return mRepeatTime;
}
//----------------------------------------------------------------------------
inline InterpCurveFloat &EffectModule::GetCurveFloatMin()
{
	return mCurveFloatMin;
}
//----------------------------------------------------------------------------
inline InterpCurveFloat &EffectModule::GetCurveFloatMax()
{
	return mCurveFloatMax;
}
//----------------------------------------------------------------------------
inline InterpCurveFloat3 &EffectModule::GetCurveFloat3Min()
{
	return mCurveFloat3Min;
}
//----------------------------------------------------------------------------
inline InterpCurveFloat3 &EffectModule::GetCurveFloat3Max()
{
	return mCurveFloat3Max;
}
//----------------------------------------------------------------------------
inline bool EffectModule::IsForEffectable () const
{
	return mIsForEffectable;
}
//----------------------------------------------------------------------------
inline bool EffectModule::IsRange () const
{
	return mIsRange;
}
//----------------------------------------------------------------------------
inline EffectModule::ModuleType EffectModule::GetModuleType ()
{
	return mModuleType;
}
//----------------------------------------------------------------------------
inline EffectableController *EffectModule::GetEffectableController () const
{
	return mEffectableCtrl;
}
//----------------------------------------------------------------------------
inline UpdateEffectableCallback EffectModule::GetUpdateEffectableCallback () const
{
	return mEffectableCallback;
}
//----------------------------------------------------------------------------
inline UpdateEffectObjectCallback EffectModule::GetUpdateEffectObjectCallback () const
{
	return mEffectObjectCallback;
}
//----------------------------------------------------------------------------