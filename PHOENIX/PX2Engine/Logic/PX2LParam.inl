// PX2LParam.inl

//----------------------------------------------------------------------------
inline LParam::LParamType LParam::GetLParamType() const
{
	return mLParamType;
}
//----------------------------------------------------------------------------
inline FunParamType LParam::GetDataType() const
{
	return mDataType;
}
//----------------------------------------------------------------------------
inline const Any &LParam::GetValue() const
{
	return mData;
}
//----------------------------------------------------------------------------
inline const std::string &LParam::GetOutputScriptVarString() const
{
	return mOutputScriptVarString;
}
//----------------------------------------------------------------------------
inline LBlock *LParam::GetModule()
{
	return mModule;
}
//----------------------------------------------------------------------------
inline const LBlock *LParam::GetModule() const
{
	return mModule;
}
//----------------------------------------------------------------------------
inline bool LParam::IsOwnObjectParam() const
{
	return FPT_POINTER_THIS == mDataType ||
		FPT_POINTER_THIS_STATIC == mDataType;
}
//----------------------------------------------------------------------------