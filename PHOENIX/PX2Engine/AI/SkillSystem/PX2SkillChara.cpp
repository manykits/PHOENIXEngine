// PX2SkillChara.cpp

#include "PX2SkillChara.hpp"
#include "PX2SkillDefManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillChara::SkillChara()
{
	mPosition = APoint::ORIGIN;
	mDir = AVector::ZERO;

	mRoomID = 0;
	mMapID = 0;
	mOwner = 0;
	mHeight = 0.0f;
	mRadius = 0.0f;
	mIsMyS = false;

	mTypeID = 0;
	mMonsterID = 0;
	mGroupID = 0;
	mLevel = 0;

	mDefMonster = 0;
	mDefChara = 0;

	mWatchDistance = 0.0f;
	mIsCanAimed = true;
	mAimTargetID = 0;
	mNumAimingMe = 0;

	mMaxHP = 0;
	mCurHP = 0;
	mCurAP = 0;
	mCurDP = 0;

	mIsDead = false;

	mIsNeedRecalInfo = true;
}
//----------------------------------------------------------------------------
SkillChara::~SkillChara()
{
}
//----------------------------------------------------------------------------
void SkillChara::SetRoomID(int roomID)
{
	mRoomID = roomID;
}
//----------------------------------------------------------------------------
void SkillChara::SetMapID(int mapID)
{
	mMapID = mapID;
}
//----------------------------------------------------------------------------
void SkillChara::SetHeight(float height)
{
	mHeight = height;
}
//----------------------------------------------------------------------------
void SkillChara::SetRadius(float radius)
{
	mRadius = radius;
}
//----------------------------------------------------------------------------
void SkillChara::SetPosition(const APoint &pos)
{
	mPosition = pos;
}
//----------------------------------------------------------------------------
void SkillChara::SetDir(const AVector &dir)
{
	mDir = dir;
}
//----------------------------------------------------------------------------
void SkillChara::AddBloodTextObj(SkillBloodTextObj *bloodTextObj)
{
	mBloodObjs.push_back(bloodTextObj);
}
//----------------------------------------------------------------------------
std::vector<BloodTextObjPtr> &SkillChara::GetBloodTextObjs()
{
	return mBloodObjs;
}
//----------------------------------------------------------------------------
void SkillChara::SetOwner(Movable *mov)
{
	mOwner = mov;
}
//----------------------------------------------------------------------------
Movable *SkillChara::GetOwner()
{
	return mOwner;
}
//----------------------------------------------------------------------------
void SkillChara::Update(int elapsedMicroSeconds)
{
	for (int i = 0; i < (int)mSkills.size(); i++)
	{
		Skill *skill = mSkills[i];
		if (skill)
		{
			skill->Update(elapsedMicroSeconds);
		}
	}

	if (mIsNeedRecalInfo)
	{
		_RecalInfo();
	}

	auto it = mBufs.begin();
	for (; it != mBufs.end();)
	{
		SkillBuf *buf = *it;
		buf->Update(elapsedMicroSeconds);
		if (buf->IsOver())
		{ 
			buf->OnRemove();
			buf->SetChara(0);
			it = mBufs.erase(it);
			OnBufChanged();
		}
		else
		{
			it++;
		}
	}

	auto itBlood = mBloodObjs.begin();
	for (; itBlood != mBloodObjs.end();)
	{
		SkillBloodTextObj *sbt = *itBlood;
		sbt->Update(elapsedMicroSeconds);
		if (sbt->IsOver())
		{
			itBlood = mBloodObjs.erase(itBlood);
		}
		else
		{
			itBlood++;
		}
	}
}
//----------------------------------------------------------------------------
void SkillChara::SetTypeID(int typeID)
{
	mTypeID = typeID;

	RemoveAllSkills();
	RemoveAllBufs();

	auto it = SKILL_DEFM.DefCharacters.find(typeID);
	if (it != SKILL_DEFM.DefCharacters.end())
	{
		mDefChara = &(it->second);

		_RecalInfo();
		SetCurHP(mMaxHP);

		for (int i = 0; i < (int)mDefChara->Skills.size(); i++)
		{
			int typeID = mDefChara->Skills[i];

			Skill *skill = new0 Skill();
			AddSkill(skill);
			skill->SetTypeID(typeID);
		}
	}
	else
	{
		mDefChara = 0;
	}
}
//----------------------------------------------------------------------------
void SkillChara::SetMonsterID(int monsID)
{
	mMonsterID = monsID;
}
//----------------------------------------------------------------------------
void SkillChara::SetGroupID(int groupID)
{
	mGroupID = groupID;
}
//----------------------------------------------------------------------------
void SkillChara::SetLevel(int level)
{
	mLevel = level;
}
//----------------------------------------------------------------------------
void SkillChara::SetAimTargetID(int targetID)
{
	mAimTargetID = targetID;
}
//----------------------------------------------------------------------------
SkillChara *SkillChara::GetAimTargetCharacter() const
{
	return 0;
}
//----------------------------------------------------------------------------
SkillChara *SkillChara::GetAimTargetCharacterAlive() const
{
	return 0;
}
//----------------------------------------------------------------------------
Animation *SkillChara::GetAnim(const std::string &animName)
{
	PX2_UNUSED(animName);
	return 0;
}
//----------------------------------------------------------------------------
Animation *SkillChara::PlayAnim(const std::string &animName)
{
	PX2_UNUSED(animName);
	return 0;
}
//----------------------------------------------------------------------------
Node *SkillChara::GetAnchor(const std::string &anchorName)
{
	PX2_UNUSED(anchorName);
	return 0;
}
//----------------------------------------------------------------------------}