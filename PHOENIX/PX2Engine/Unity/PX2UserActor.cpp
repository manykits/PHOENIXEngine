// PX2UserActor.cpp

#include "PX2UserActor.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
UserActor::UserActor() :
mUser(0),
mServerID(-1),
mLevel(0),
mMaxLevel(99),
mCurLevelExp(0),
mCurLevelMaxExp(0),
mCurLevelEnergy(0),
mCurLevelMaxEnergy(0),
mGold(0),
mMoney(0)
{
}
//----------------------------------------------------------------------------
UserActor::~UserActor()
{
}
//----------------------------------------------------------------------------
void UserActor::Update(float appTime, float elapsedTime)
{
	PX2_UNUSED(appTime);
	PX2_UNUSED(elapsedTime);
}
//----------------------------------------------------------------------------
void UserActor::SetServerID(int id)
{
	mServerID = id;
}
//----------------------------------------------------------------------------
void UserActor::SetNickName(const std::string &name)
{
	mNickName = name;
}
//----------------------------------------------------------------------------
void UserActor::SetLevel(int level)
{
	mLevel = level;
}
//----------------------------------------------------------------------------
void UserActor::SetMaxLevel(int maxLevel)
{
	mMaxLevel = maxLevel;
}
//----------------------------------------------------------------------------
void UserActor::SetCurLevelExp(int exp)
{
	mCurLevelExp = exp;
}
//----------------------------------------------------------------------------
void UserActor::SetCurLevelMaxExp(int maxExp)
{
	mCurLevelMaxExp = maxExp;
}
//----------------------------------------------------------------------------
void UserActor::SetCurLevelEnergy(int energy)
{
	mCurLevelEnergy = energy;
}
//----------------------------------------------------------------------------
void UserActor::SetCurLevelMaxEnergy(int maxEnergy)
{
	mCurLevelMaxEnergy = maxEnergy;
}
//----------------------------------------------------------------------------
void UserActor::SetGold(int gold)
{
	mGold = gold;
}
//----------------------------------------------------------------------------
void UserActor::SetMoney(int money)
{
	mMoney = money;
}
//----------------------------------------------------------------------------