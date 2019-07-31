// PX2SkillInst.inl

//----------------------------------------------------------------------------
inline Skill *SkillInst::GetSkill()
{
	return mSkill;
}
//----------------------------------------------------------------------------
inline SkillStep *SkillInst::GetCurSkillStep()
{
	return mCurSkillStep;
}
//----------------------------------------------------------------------------
inline bool SkillInst::IsInstanceOver() const
{
	return mIsInstanceOver;
}
//----------------------------------------------------------------------------