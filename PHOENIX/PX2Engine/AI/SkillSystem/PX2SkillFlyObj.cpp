// PX2SkillFlyObj.cpp

#include "PX2SkillFlyObj.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillFlyObj::SkillFlyObj()
{
	mFlyType = FT_DIR;
	mIsOver = false;
}
//----------------------------------------------------------------------------
SkillFlyObj::~SkillFlyObj()
{

}
//----------------------------------------------------------------------------
void SkillFlyObj::Update(int elapsedMicroSeconds)
{
	PX2_UNUSED(elapsedMicroSeconds);
}
//----------------------------------------------------------------------------
void SkillFlyObj::SetFlyType(FlyType ft)
{
	mFlyType = ft;
}
//----------------------------------------------------------------------------
void SkillFlyObj::SetOver(bool over)
{
	mIsOver = over;
}
//----------------------------------------------------------------------------