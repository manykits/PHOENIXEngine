// PX2SkillFeild.cpp

#include "PX2SkillFeild.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillFeild::SkillFeild()
{
}
//----------------------------------------------------------------------------
SkillFeild::~SkillFeild()
{
}
//----------------------------------------------------------------------------
void SkillFeild::AddSkillChara(SkillChara *chara)
{
	int id = chara->GetID();
	if (0 == id) return;

	mSkillCharas[id] = chara;
}
//----------------------------------------------------------------------------
void SkillFeild::RemoveSkillChara(SkillChara *chara)
{
	int id = chara->GetID();
	if (0 == id) return;

	mSkillCharas.erase(id);
}
//----------------------------------------------------------------------------
bool SkillFeild::IsHasSkillChara(SkillChara *chara)
{
	int id = chara->GetID();
	if (0 == id) return false;

	auto it = mSkillCharas.find(id);
	if (it != mSkillCharas.end())
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
SkillChara *SkillFeild::GetSkillChara(int id)
{
	auto it = mSkillCharas.find(id);
	if (it != mSkillCharas.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
int SkillFeild::GetNumSkillCharas()
{
	return (int)mSkillCharas.size();
}
//----------------------------------------------------------------------------