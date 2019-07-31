// PX2SkillItem.cpp

#include "PX2SkillItem.hpp"
#include "PX2SkillDefManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Item::Item()
{
	mTypeID = 0;
	mNum = 0;
	mDefItem = 0;
	mChara = 0;
	mIsEquipped = false;
}
//----------------------------------------------------------------------------
Item::~Item()
{
}
//----------------------------------------------------------------------------
void Item::SetTypeID(int typeID)
{
	mTypeID = typeID;

	auto it = SKILL_DEFM.DefItems.find(typeID);
	if (it != SKILL_DEFM.DefItems.end())
	{
		mDefItem = &(it->second);
	}
	else
	{
		mDefItem = 0;
	}
}
//----------------------------------------------------------------------------
void Item::SetNum(int num)
{
	mNum = num;
}
//----------------------------------------------------------------------------
void Item::OnAdded()
{
}
//----------------------------------------------------------------------------
void Item::OnRemove()
{
}
//----------------------------------------------------------------------------
void Item::SetEquipped(bool equipped)
{
	mIsEquipped = equipped;
}
//----------------------------------------------------------------------------
void Item::SetChara(SkillChara *chara)
{
	mChara = chara;
}
//----------------------------------------------------------------------------