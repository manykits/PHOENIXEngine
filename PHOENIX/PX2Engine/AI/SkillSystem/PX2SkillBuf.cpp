// PX2SkillBuf.cpp

#include "PX2SkillBuf.hpp"
#include "PX2StringHelp.hpp"
#include "PX2SkillDefManager.hpp"
#include "PX2SkillChara.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillBuf::SkillBuf()
{
	mTypeID = 0;

	mDefBuf = 0;

	mChara = 0;

	mTimeMicroSeconds = 0;
	mIsOver = false;
}
//----------------------------------------------------------------------------
SkillBuf::~SkillBuf()
{
}
//----------------------------------------------------------------------------
void SkillBuf::SetTypeID(int typeID)
{
	mTypeID = typeID;

	auto it = SKILL_DEFM.DefBufs.find(typeID);
	if (it != SKILL_DEFM.DefBufs.end())
	{
		mDefBuf = &(it->second);
		mTimeMicroSeconds = mDefBuf->Time;
	}
}
//----------------------------------------------------------------------------
const std::string &SkillBuf::GetTag() const
{
	return mDefBuf->Tag;
}
//----------------------------------------------------------------------------
void SkillBuf::SetChara(SkillChara *chara)
{
	mChara = chara;
}
//----------------------------------------------------------------------------
void SkillBuf::OnAdded()
{
	mChara->MarkNeedRecalInfo();
}
//----------------------------------------------------------------------------
void SkillBuf::OnRemove()
{
	mChara->MarkNeedRecalInfo();
}
//----------------------------------------------------------------------------
float SkillBuf::GetPercent() const
{
	if (mDefBuf)
	{
		return 1.0f - ((float)mTimeMicroSeconds / (float)mDefBuf->Time);
	}
	else
	{
		return 0.0f;
	}
}
//----------------------------------------------------------------------------
void SkillBuf::Update(int elapsedMicroSeconds)
{
	if (mTimeMicroSeconds > 0)
	{
		mTimeMicroSeconds -= elapsedMicroSeconds;

		if (mTimeMicroSeconds <= 0)
		{
			mIsOver = true;
		}
	}
}
//----------------------------------------------------------------------------