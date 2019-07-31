// PX2UserActor.inl

//----------------------------------------------------------------------------
inline User *UserActor::GetUser()
{
	return mUser;
}
//----------------------------------------------------------------------------
inline void UserActor::SetUser(User *user)
{
	mUser = user;
}
//----------------------------------------------------------------------------
inline int UserActor::GetServerID() const
{
	return mServerID;
}
//----------------------------------------------------------------------------
inline const std::string &UserActor::GetNickName() const
{
	return mNickName;
}
//----------------------------------------------------------------------------
inline int UserActor::GetLevel() const
{
	return mLevel;
}
//----------------------------------------------------------------------------
inline int UserActor::GetMaxLevel() const
{
	return mMaxLevel;
}
//----------------------------------------------------------------------------
inline int UserActor::GetCurLevelExp() const
{
	return mCurLevelExp;
}
//----------------------------------------------------------------------------
inline int UserActor::GetCurLevelMaxExp() const
{
	return mCurLevelMaxExp;
}
//----------------------------------------------------------------------------
inline int UserActor::GetCurLevelEnergy() const
{
	return mCurLevelEnergy;
}
//----------------------------------------------------------------------------
inline int UserActor::GetCurLevelMaxEnergy() const
{
	return mCurLevelMaxEnergy;
}
//----------------------------------------------------------------------------
inline int UserActor::GetGold() const
{
	return mGold;
}
//----------------------------------------------------------------------------
inline int UserActor::GetMoney() const
{
	return mMoney;
}
//----------------------------------------------------------------------------