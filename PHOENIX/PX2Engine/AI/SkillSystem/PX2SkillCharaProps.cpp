// PX2SkillChara.cpp

#include "PX2SkillChara.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void SkillChara::OnSkillActivate(Skill *skill)
{
	mLastActivateSkill = skill;
}
//----------------------------------------------------------------------------
void SkillChara::OnSkillActivateAnimBegin(Skill *skill, Animation *anim)
{
	mActingAnimingSkills.insert(skill);
	PX2_UNUSED(anim);
}
//----------------------------------------------------------------------------
void SkillChara::OnSkillActivateAnimOver(Skill *skill, Animation *anim)
{
	mActingAnimingSkills.erase(skill);
	PX2_UNUSED(anim);
}
//----------------------------------------------------------------------------
int SkillChara::GetNumActingAnimingSkills() const
{
	return (int)mActingAnimingSkills.size();
}
//----------------------------------------------------------------------------
void SkillChara::OnSkillInstActivate(SkillInst *si)
{
	mLastActivateSkillInst = si;
}
//----------------------------------------------------------------------------
void SkillChara::OnSkillInstActivateOver(SkillInst *inst)
{
	PX2_UNUSED(inst);
}
//----------------------------------------------------------------------------
void SkillChara::MarkNeedRecalInfo()
{
	mIsNeedRecalInfo = true;
}
//----------------------------------------------------------------------------
void SkillChara::OnInfoChanged()
{
}
//----------------------------------------------------------------------------
float SkillChara::GetHPPercent() const
{
	return (float)mCurHP / (float)mMaxHP;
}
//----------------------------------------------------------------------------
void SkillChara::SetMaxHP(int maxHP)
{
	mMaxHP = maxHP;

	MarkNeedRecalInfo();
}
//----------------------------------------------------------------------------
void SkillChara::SetCurHP(int curHP)
{
	mCurHP = curHP;

	MarkNeedRecalInfo();
}
//----------------------------------------------------------------------------
void SkillChara::SetCurAP(int curAP)
{
	mCurAP = curAP;

	MarkNeedRecalInfo();
}
//----------------------------------------------------------------------------
void SkillChara::SetCurDP(int curDP)
{
	mCurDP = curDP;

	MarkNeedRecalInfo();
}
//----------------------------------------------------------------------------
void SkillChara::SetDead(bool dead)
{
	mIsDead = dead;
}
//----------------------------------------------------------------------------
void SkillChara::_RecalInfo()
{
	mIsNeedRecalInfo = false;

	if (!mDefChara) return;

	mMaxHP = mDefChara->BaseHP;
	mCurAP = mDefChara->BaseAP;
	mCurDP = mDefChara->BaseDP;

	for (int i = 0; i < (int)mBufs.size(); i++)
	{
		SkillBuf *buf = mBufs[i];

		mMaxHP += buf->GetDefBuf()->MaxHP;
		mCurAP += buf->GetDefBuf()->AP;
		mCurDP += buf->GetDefBuf()->DP;
	}

	if (mCurHP > mMaxHP) mCurHP = mMaxHP;

	OnInfoChanged();
}
//----------------------------------------------------------------------------