// PX2LModule.inl

//----------------------------------------------------------------------------
inline LBlock::BlockType LBlock::GetBlockType() const
{
	return mBlockType;
}
//----------------------------------------------------------------------------
inline LBlock::CtrlType LBlock::GetCtrlType() const
{
	return mCtrlType;
}
//----------------------------------------------------------------------------
inline LBlock::ParamType LBlock::GetParamType() const
{
	return mParamType;
}
//----------------------------------------------------------------------------
inline std::vector<LParamPtr> &LBlock::GetInputParamsVec()
{
	return mInputParamsVec;
}
//----------------------------------------------------------------------------
inline std::vector<LParamPtr> &LBlock::GetOutParamsVec()
{
	return mOutputParamsVec;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, LParam*> &LBlock::
GetInputParams() const
{
	return mInputParams;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, LParam*> &LBlock::
GetOutputParams() const
{
	return mOutputParams;
}
//----------------------------------------------------------------------------
inline int LBlock::GetNumInputs() const
{
	return (int)mInputParams.size();
}
//----------------------------------------------------------------------------
inline int LBlock::GetNumOutputs() const
{
	return (int)mOutputParams.size();
}
//----------------------------------------------------------------------------