// PX2SkillBridge.cpp

#include "PX2SkillBridge.hpp"
#include "PX2SkillChara.hpp"
#include "PX2SkillFeild.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillBridge::SkillBridge()
{
}
//----------------------------------------------------------------------------
SkillBridge::~SkillBridge()
{
}
//----------------------------------------------------------------------------
void SkillBridge::C2S_CharacterActivateSkillInstance(int roomID, int mapID, 
	int charaID, int skillID, const APoint &fromPos, const AVector &fromDir,
	int aimTargetID, const APoint &targetPos)
{
	PX2_UNUSED(roomID);
	PX2_UNUSED(mapID);
	PX2_UNUSED(charaID);
	PX2_UNUSED(skillID);
	PX2_UNUSED(fromPos);
	PX2_UNUSED(fromDir);
	PX2_UNUSED(aimTargetID);
	PX2_UNUSED(targetPos);
}
//----------------------------------------------------------------------------
void SkillBridge::S2C_CharacterActivateSkillInstance(int roomID, int mapID, 
	int charaID, int skillID, const APoint & fromPos, const AVector &fromDir,
	int aimTargetID, const APoint &targetPos)
{
	PX2_UNUSED(roomID);
	PX2_UNUSED(mapID);
	PX2_UNUSED(charaID);
	PX2_UNUSED(skillID);
	PX2_UNUSED(fromPos);
	PX2_UNUSED(fromDir);
	PX2_UNUSED(aimTargetID);
	PX2_UNUSED(targetPos);
}
//----------------------------------------------------------------------------
void SkillBridge::C2S_CharacterAffect(int roomID, int mapID, 
	int fromID, int fromSkillID, int toTargetID, bool calValue)
{
	SkillChara *fromSkillChara = PX2_SKILLFEILD.GetSkillChara(fromID);
	if (!fromSkillChara) return;
	Skill *skill = fromSkillChara->GetSkillByID(fromSkillID);
	DefSkill *defSkill = skill->GetDefSkill();

	SkillChara *target = PX2_SKILLFEILD.GetSkillChara(toTargetID);

	// toTargetID
	SkillAffect sa;
	sa.TheAffectType = SkillAffect::AT_HP_MINUS;
	sa.Val0 = 10;
	sa.IsAffectValue = true;
	sa.IsAffectEffectBlood = true;
	for (int i = 0; i < (int)defSkill->DefSkillBufs.size(); i++)
	{
		DefSkillBuf defSkillBuf = defSkill->DefSkillBufs[i];
		if (defSkillBuf.TheGroupType == DefSkillBuf::GT_SKILL)
		{
			sa.BufIDs.push_back(defSkillBuf.TypeID);
		}
	}
	S2C_CharacterAffect(roomID, mapID, toTargetID, sa);

	// toMe
	SkillAffect saToMe;
	saToMe.IsAffectValue = true;
	saToMe.IsAffectEffectBlood = true;
	for (int i = 0; i < (int)defSkill->DefSkillBufs.size(); i++)
	{
		DefSkillBuf defSkillBuf = defSkill->DefSkillBufs[i];
		if (defSkillBuf.TheGroupType == DefSkillBuf::GT_SELF)
		{
			saToMe.BufIDs.push_back(defSkillBuf.TypeID);
		}
	}
	if (saToMe.BufIDs.size() > 0)
	{
		S2C_CharacterAffect(roomID, mapID, fromID, saToMe);
	}
}
//----------------------------------------------------------------------------
void SkillBridge::S2C_CharacterAffect(int roomID, int mapID, int toID,
	SkillAffect sa)
{
	SkillChara *skillChara = PX2_SKILLFEILD.GetSkillChara(toID);
	if (!skillChara) return;

	int curHP = skillChara->GetCurHP();

	if (SkillAffect::AT_HP_MINUS == sa.TheAffectType)
	{
		int minusVal = (int)sa.Val0;

		curHP -= minusVal;

		if (curHP < 0)
		{
			minusVal = skillChara->GetCurHP();
			curHP = 0;
		}

		if (sa.IsAffectEffectBlood)
		{

		}
	}
	else if (SkillAffect::AT_HP_ADD == sa.TheAffectType)
	{
		int addVal = (int)sa.Val0;

		curHP += addVal;

		if (curHP > skillChara->GetMaxHP())
		{
			addVal = (int)skillChara->GetMaxHP() - skillChara->GetCurHP();
			curHP = (int)skillChara->GetMaxHP();
		}

		if (sa.IsAffectEffectBlood)
		{
			if (0 != addVal)
			{

			}
		}
	}

	if (sa.IsAffectValue)
	{
		skillChara->SetCurHP(curHP);
		if (skillChara->GetCurHP() <= 0)
		{
			skillChara->SetDead(true);
		}

		for (int i = 0; i < (int)sa.BufIDs.size(); i++)
		{
			SkillBuf *buf = new0 SkillBuf();
			buf->SetID(sa.BufIDs[i]);
			buf->SetTypeID(sa.BufIDs[i]);
		}
	}
}
//----------------------------------------------------------------------------
void SkillBridge::C2S_GenMonsters(int roomID, int mapID, int fromID,
	std::vector<GenActorObj> &monster)
{
	PX2_UNUSED(roomID);
	PX2_UNUSED(mapID);
	PX2_UNUSED(fromID);
	PX2_UNUSED(monster);
}
//----------------------------------------------------------------------------
void SkillBridge::S2C_GenedMonsters(int roomID, int mapID, int fromID,
	std::vector<GenActorObj> &monsters)
{
	PX2_UNUSED(roomID);
	PX2_UNUSED(mapID);
	PX2_UNUSED(fromID);
	PX2_UNUSED(monsters);
}
//----------------------------------------------------------------------------