// PX2Skill.inl

//----------------------------------------------------------------------------
inline SkillChara *Skill::GetSkillChara()
{
	return mSkillChara;
}
//----------------------------------------------------------------------------
inline bool Skill::IsEquipped() const
{
	return mIsEquipped;
}
//----------------------------------------------------------------------------
inline int Skill::GetTypeID() const
{
	return mTypeID;
}
//----------------------------------------------------------------------------
inline DefSkill *Skill::GetDefSkill()
{
	return mDefSkill;
}
//----------------------------------------------------------------------------
inline bool Skill::IsPrepareing() const
{
	return mIsPrepareing;
}
//----------------------------------------------------------------------------
inline int Skill::GetPrepareTime() const
{
	return mPrepareTime;
}
//----------------------------------------------------------------------------
inline int Skill::GetPrepareTimeing() const
{
	return mPrepareTimeing;
}
//----------------------------------------------------------------------------
inline int Skill::GetCD() const
{
	return mCD;
}
//----------------------------------------------------------------------------
inline int Skill::GetCDing() const
{
	return mCDing;
}
//----------------------------------------------------------------------------
inline bool Skill::IsPauseCDing() const
{
	return mIsPauseCDing;
}
//----------------------------------------------------------------------------
inline Skill::AffectGroupType Skill::GetAffectGroupType() const
{
	return mAffectGroupType;
}
//----------------------------------------------------------------------------
inline SkillRange *Skill::GetSkillRange()
{
	return mSkillRange;
}
//----------------------------------------------------------------------------
inline bool Skill::IsActing() const
{
	return mIsActing;
}
//----------------------------------------------------------------------------
inline int Skill::GetNumSkillInsts() const
{
	return (int)mSkillInsts.size();
}
//----------------------------------------------------------------------------
inline const APoint &Skill::GetDirectPutPos() const
{
	return mDirectPutPos;
}
//----------------------------------------------------------------------------