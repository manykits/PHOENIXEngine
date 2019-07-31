// PX2SkillBuf.inl

//----------------------------------------------------------------------------
inline int SkillBuf::GetTypeID() const
{
	return mTypeID;
}
//----------------------------------------------------------------------------
inline DefBuf *SkillBuf::GetDefBuf()
{
	return mDefBuf;
}
//----------------------------------------------------------------------------
inline SkillChara *SkillBuf::GetChara()
{
	return mChara;
}
//----------------------------------------------------------------------------
inline bool SkillBuf::IsOver() const
{
	return mIsOver;
}
//----------------------------------------------------------------------------