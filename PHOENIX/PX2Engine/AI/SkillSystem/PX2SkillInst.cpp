// PX2SkillInst.cpp

#include "PX2SkillInst.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillInst::SkillInst(Skill *skill) :
mSkill(skill)
{
	IsFromNet = false;

	AimTargetID = 0;
	IsPursuitTarget = 0;

	AffectTargetID = 0;

	mCurSkillStepRunTime = 0;
	mIsInstanceOver = false;

	mFlyObj = new0 SkillFlyObj();
}
//----------------------------------------------------------------------------
SkillInst::~SkillInst()
{
}
//----------------------------------------------------------------------------
void SkillInst::Update(int elapsedMicroSeconds)
{
	mCurSkillStepRunTime += elapsedMicroSeconds;

	if (mFlyObj)
		mFlyObj->Update(elapsedMicroSeconds);

	bool isCurStepOver = false;
	if (mCurSkillStep)
	{
		if (SkillStep::M_TIME == mCurSkillStep->TheMode)
		{
			if (mCurSkillStepRunTime >= mCurSkillStep->TimeMicroSeconds)
			{
				isCurStepOver = true;
			}
		}
		else if (SkillStep::M_FLY == mCurSkillStep->TheMode)
		{
			if (mFlyObj && mFlyObj->IsOver())
			{
				mFlyObj = 0;

				isCurStepOver = true;
			}
		}
		else if (SkillStep::M_COLLIDE == mCurSkillStep->TheMode)
		{
			/*_*/
		}

		if (isCurStepOver)
		{
			GoToNextSkillStep();
		}
	}
}
//----------------------------------------------------------------------------
void SkillInst::GoToNextSkillStep()
{
	if (mCurSkillStep)
		Enter(mCurSkillStep->NextStep);
}
//----------------------------------------------------------------------------
SkillInst::SkillInst() :
mSkill(0)
{
}
//----------------------------------------------------------------------------
void SkillInst::Enter(SkillStep *sp)
{
	if (mCurSkillStep)
	{
		Leave(mCurSkillStep);
	}

	mCurSkillStepRunTime = 0;
	mCurSkillStep = sp;

	if (!mCurSkillStep)
	{
		mIsInstanceOver = true;
		return;
	}

	OnEnter(mCurSkillStep);
}
//----------------------------------------------------------------------------
void SkillInst::OnEnter(SkillStep *sp)
{
	sp->OnEnter(this);
}
//----------------------------------------------------------------------------
void SkillInst::Leave(SkillStep *sp)
{
	OnLeave(sp);
}
//----------------------------------------------------------------------------
void SkillInst::OnLeave(SkillStep *sp)
{
	sp->OnLeave(this);
}
//----------------------------------------------------------------------------