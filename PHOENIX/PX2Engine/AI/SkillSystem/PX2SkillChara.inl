// PX2SkillChara.inl

//----------------------------------------------------------------------------
inline int SkillChara::GetRoomID() const
{
	return mRoomID;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetMapID() const
{
	return mMapID;
}
//----------------------------------------------------------------------------
inline void SkillChara::SetBeMyS(bool isI)
{
	mIsMyS = isI;
}
//----------------------------------------------------------------------------
inline bool SkillChara::IsBeMyS() const
{
	return mIsMyS;
}
//----------------------------------------------------------------------------
inline float SkillChara::GetHeight() const
{
	return mHeight;
}
//----------------------------------------------------------------------------
inline float SkillChara::GetRadius() const
{
	return mRadius;
}
//----------------------------------------------------------------------------
inline const APoint &SkillChara::GetPosition() const
{
	return mPosition;
}
//----------------------------------------------------------------------------
inline AVector &SkillChara::GetDir()
{
	return mDir;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetTypeID() const
{
	return mTypeID;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetMonsterID() const
{
	return mMonsterID;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetGroupID() const
{
	return mGroupID;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetLevel() const
{
	return mLevel;
}
//----------------------------------------------------------------------------
inline DefMonster *SkillChara::GetDefMonster()
{
	return mDefMonster;
}
//----------------------------------------------------------------------------
inline DefChara *SkillChara::GetDefChara()
{
	return mDefChara;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetNumSkills() const
{
	return (int)mSkills.size();
}
//----------------------------------------------------------------------------
inline Skill *SkillChara::GetDefSkill()
{
	return mDefSkill;
}
//----------------------------------------------------------------------------
inline void SkillChara::SetWatchDistance(float dist)
{
	mWatchDistance = dist;
}
//----------------------------------------------------------------------------
inline float SkillChara::GetWatchDistance() const
{
	return mWatchDistance;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetAimTargetID() const
{
	return mAimTargetID;
}
//----------------------------------------------------------------------------
inline void SkillChara::SetCanBeAimed(bool can)
{
	mIsCanAimed = can;
}
//----------------------------------------------------------------------------
inline bool SkillChara::IsCanBeAimed() const
{
	return mIsCanAimed;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetNumAimingMe() const
{
	return mNumAimingMe;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetMaxHP() const
{
	return mMaxHP;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetCurHP() const
{
	return mCurHP;
}
//----------------------------------------------------------------------------
inline int SkillChara::GetCurAP() const
{
	return mCurAP;
}
//----------------------------------------------------------------------------;
inline int SkillChara::GetCurDP() const
{
	return mCurDP;
}
//----------------------------------------------------------------------------
inline bool SkillChara::IsDead() const
{
	return mIsDead;
}
//----------------------------------------------------------------------------