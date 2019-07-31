// PX2SkillChara.cpp

#include "PX2SkillChara.hpp"
#include "PX2SkillEvent.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
bool SkillChara::AddSkill(Skill *skill)
{
	assertion(0 != skill, "skill should not be 0.");

	if (!skill)
		return false;

	if (HasSkill(skill))
		return false;

	mSkills.push_back(skill);
	skill->SetSkillChara(this);
	skill->OnAdded();

	if (skill->GetName() == "Def")
		mDefSkill = skill;

	return true;
}
//----------------------------------------------------------------------------
bool SkillChara::HasSkill(Skill *skill)
{
	assertion(0 != skill, "skill should not be 0.");

	if (!skill)
		return false;

	for (int i = 0; i < GetNumSkills(); i++)
	{
		if (skill == GetSkill(i))
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
Skill *SkillChara::GetSkillByID(int id)
{
	for (int i = 0; i < GetNumSkills(); i++)
	{
		Skill *skill = GetSkill(i);

		if (skill->GetID() == id)
		{
			return skill;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
Skill *SkillChara::GetSkillByTypeID(int typeID)
{
	for (int i = 0; i < GetNumSkills(); i++)
	{
		Skill *skill = GetSkill(i);

		if (skill->GetTypeID() == typeID)
		{
			return skill;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
Skill *SkillChara::GetSkillByName(const std::string &name)
{
	for (int i = 0; i < GetNumSkills(); i++)
	{
		Skill *skill = GetSkill(i);

		if (skill->GetName() == name)
		{
			return skill;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
Skill *SkillChara::GetSkill(int i)
{
	if (0 <= i && i < (int)mSkills.size())
		return mSkills[i];

	return 0;
}
//----------------------------------------------------------------------------
bool SkillChara::RemoveSkill(Skill *skill)
{
	assertion(0 != skill, "skill should not be 0.");

	if (!skill)
		return false;

	std::vector<SkillPtr>::iterator it = mSkills.begin();
	for (; it != mSkills.end(); it++)
	{
		if (skill == *it)
		{
			skill->OnRemove();
			(*it)->SetSkillChara(0);
			mSkills.erase(it);

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void SkillChara::RemoveAllSkills()
{
	for (int i = 0; i < (int)mSkills.size(); i++)
	{
		mSkills[i]->SetSkillChara(0);
	}

	mSkills.clear();
}
//----------------------------------------------------------------------------
bool SkillChara::IsAnySkillHasInst()
{
	for (int i = 0; i < (int)GetNumSkills(); i++)
	{
		Skill *skill = GetSkill(i);
		if (0 != skill->GetNumSkillInsts())
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void SkillChara::ResetAllSkillsCD()
{
	for (int i = 0; i < (int)mEquippedSkills.size(); i++)
	{
		if (mEquippedSkills[i])
		{
			mEquippedSkills[i]->SetCD(0);
			mEquippedSkills[i]->SetCDing(false);
		}
	}
}
//----------------------------------------------------------------------------
void SkillChara::EquipSkill(Skill *skill)
{
	if (IsSkillEquipped(skill))
		return;

	mEquippedSkills.push_back(skill);
	skill->SetEquipped(true);
}
//----------------------------------------------------------------------------
bool SkillChara::IsSkillEquipped(Skill *skill)
{
	for (int i = 0; i < (int)mEquippedSkills.size(); i++)
	{
		if (mEquippedSkills[i] == skill)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void SkillChara::UnEquipSkill(Skill *skill)
{
	auto it = mEquippedSkills.begin();
	for (; it != mEquippedSkills.end(); it++)
	{
		if (skill == *it)
		{
			mEquippedSkills.erase(it);
			skill->SetEquipped(false);
		}
	}
}
//----------------------------------------------------------------------------
void SkillChara::UnEquipAllSkill()
{
	for (int i = 0; i <(int)mEquippedSkills.size(); i++)
	{
		mEquippedSkills[i]->SetEquipped(false);
	}

	mEquippedSkills.clear();
}
//----------------------------------------------------------------------------
int SkillChara::GetNumEquippedSkill() const
{
	return (int)mEquippedSkills.size();
}
//----------------------------------------------------------------------------
Skill *SkillChara::GetEquippedSkill(int i)
{
	if (0 <= i && i < (int)mEquippedSkills.size())
	{
		return mEquippedSkills[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
const Skill *SkillChara::GetEquippedSkill(int i) const
{
	if (0 <= i && i < (int)mEquippedSkills.size())
	{
		return mEquippedSkills[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
void SkillChara::EquipAllSkills()
{
	for (int i = 0; i < GetNumSkills(); i++)
	{
		Skill *skill = GetSkill(i);
		if (skill)
		{
			EquipSkill(skill);
		}
	}
}
//----------------------------------------------------------------------------
bool SkillChara::IsAnySkillActing() const
{
	for (int i = 0; i < GetNumEquippedSkill(); i++)
	{
		const Skill *skill = GetEquippedSkill(i);
		if (skill->IsActing())
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool SkillChara::TryAddBuf(SkillBuf *buf)
{
	DefBuf *defBuf = buf->GetDefBuf();

	if (defBuf->Tag.empty() || DefBuf::TM_OVERLAP == defBuf->TheTagMode)
	{
		return AddBuf(buf);
	}
	else if (DefBuf::TM_REPLACE == defBuf->TheTagMode)
	{
		auto it = mBufs.begin();
		for (; it != mBufs.end();)
		{
			const std::string &skillBufTag = (*it)->GetTag();
			if (skillBufTag == defBuf->Tag)
			{
				it = mBufs.erase(it);
				OnBufChanged();
			}
			else
			{
				it++;
			}
		}

		return AddBuf(buf);
	}

	return false;
}
//----------------------------------------------------------------------------
bool SkillChara::AddBuf(SkillBuf *buf)
{
	assertion(0 != buf, "buf should not be 0.");

	if (!buf)
		return false;

	if (HasBuf(buf))
		return false;

	mBufs.push_back(buf);

	buf->SetChara(this);
	buf->OnAdded();

	OnBufChanged();

	return true;
}
//----------------------------------------------------------------------------
bool SkillChara::HasBuf(SkillBuf *buf)
{
	for (int i = 0; i < (int)mBufs.size(); i++)
	{
		if (buf == mBufs[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
SkillBuf *SkillChara::GetBuf(int i)
{
	assertion(0 <= i&&i < (int)mBufs.size(), "i should be in right range.\n");

	if (0 <= i && i < (int)mBufs.size())
	{
		return mBufs[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
int SkillChara::GetNumBufs() const
{
	return (int)mBufs.size();
}
//----------------------------------------------------------------------------
void SkillChara::RemoveAllBufs()
{
	std::vector<SkillBufPtr>::iterator it = mBufs.begin();
	if (it != mBufs.end())
	{
		(*it)->OnRemove();
		(*it)->SetChara(0);
	}

	mBufs.clear();

	OnBufChanged();
}
//----------------------------------------------------------------------------
bool SkillChara::RemoveBuf(SkillBuf *buf)
{
	assertion(0 != buf, "buf should not be 0.");

	if (!buf)
		return false;

	std::vector<SkillBufPtr>::iterator it = mBufs.begin();
	if (it != mBufs.end())
	{
		(*it)->OnRemove();
		(*it)->SetChara(0);

		mBufs.erase(it);

		OnBufChanged();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void SkillChara::OnBufChanged()
{
	Event *ent = SkillES::CreateEventX(SkillES::BufChanged);
	ent->SetData(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------