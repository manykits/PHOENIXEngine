// PX2LogicManager.inl

//----------------------------------------------------------------------------
inline const FunObject *LogicManager::GetStartGF() const
{
	return mFunStartObject;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, std::vector<std::string> > &LogicManager::GetGFs()
const
{
	return mGeneralFunctions;
}
//----------------------------------------------------------------------------
inline std::map<std::string, FunObjectPtr> &LogicManager::GetEventMap()
{
	return mEventObjects;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, std::vector<std::string> > &LogicManager
::GetEvents() const
{
	return mEvents;
}
//----------------------------------------------------------------------------
inline std::map<std::string, FunObjectPtr> &LogicManager::GetGFMap()
{
	return mGenFunObjects;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, std::vector<std::string> >&LogicManager::GetCtrls() const
{
	return mCtrls;
}
//----------------------------------------------------------------------------
inline std::map<std::string, FunObjectPtr> &LogicManager::GetCtrlMap()
{
	return mCtrlObjects;
}
//----------------------------------------------------------------------------
inline std::map<std::string, FunObjectPtr> &LogicManager::GetParamMap()
{
	return mParamObjects;
}
//----------------------------------------------------------------------------
inline const std::map<std::string, std::vector<std::string> >&LogicManager::GetOperators() const
{
	return mOperators;
}
//----------------------------------------------------------------------------
inline std::map<std::string, FunObjectPtr> &LogicManager::GetOperatorMap()
{
	return mOperatorsObjects;
}
//----------------------------------------------------------------------------