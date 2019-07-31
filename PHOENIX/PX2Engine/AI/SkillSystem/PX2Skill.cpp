// PX2Skill.cpp

#include "PX2Skill.hpp"
#include "PX2SkillDefManager.hpp"
#include "PX2SkillChara.hpp"
#include "PX2SkillBridge.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Skill::Skill()
{
	mTypeID = 0;

	mIsPrepareing = false;
	mPrepareTime = 0;
	mPrepareTimeing = 0;

	mCD = 0;
	mCDing = 0;
	mIsDoCDing = false;
	mIsPauseCDing = false;

	mAffectGroupType = AGT_NONE;
	mDefSkill = 0;

	mSkillChara = 0;
	mIsEquipped = false;

	mSkillRange = new0 SkillRange();

	mIsActing = false;
	mNextSkillActivatingID = 1;
}
//----------------------------------------------------------------------------
Skill::~Skill()
{
}
//----------------------------------------------------------------------------
void Skill::Clear()
{
}
//----------------------------------------------------------------------------
void Skill::Update(int elapsedMicroSeconds)
{
	// prepare
	int preTime = GetPrepareTime();
	if (IsPrepareing())
	{
		int preingTimeTemp = GetPrepareTimeing();
		preingTimeTemp += elapsedMicroSeconds;

		if (preingTimeTemp > preTime)
		{
			Prepare(false, true);
		}
		else
		{
			SetPrepareTimeing(preingTimeTemp);
		}
	}

	// cd
	int cdTime = GetCD();
	if (IsDoCDing())
	{
		int onCDTimeTemp = GetCDing();

		if (!IsPauseCDing())
		{
			onCDTimeTemp += elapsedMicroSeconds;
		}

		if (onCDTimeTemp > cdTime)
		{
			SetCDing(0);
			SetDoCDing(false);
		}
		else
		{
			SetCDing(onCDTimeTemp);
		}
	}

	// insts
	auto it = mSkillInsts.begin();
	for (; it!= mSkillInsts.end();)
	{
		(*it)->Update(elapsedMicroSeconds);

		if ((*it)->IsInstanceOver())
		{
			it = mSkillInsts.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
void Skill::SetTypeID(int typeID)
{
	SetID(typeID);

	mTypeID = typeID;

	auto it = SKILL_DEFM.DefSkills.find(typeID);
	if (it != SKILL_DEFM.DefSkills.end())
	{
		mDefSkill = &(it->second);

		SetName(mDefSkill->Name);

		SetCD(mDefSkill->CD);

		mSkillRange->TheRangeType = SkillRange::RT_CIRCLE;
		mSkillRange->RadiusLength = mDefSkill->RangeRadius;
		mSkillRange->Degree = (float)mDefSkill->RangeDegree;
	}
	else
	{
		SetName("NULL");
		mDefSkill = 0;
	}

	_GenSkillSteps();
}
//----------------------------------------------------------------------------
bool Skill::Prepare(bool pre, bool actSkill)
{
	if (!IsEnable()) return false;
	if (IsDoCDing()) return false;

	mIsPrepareing = pre;
	mPrepareTimeing = 0;

	if (actSkill)
	{
		MainActivateSkillInstance(true);
	}

	return true;
}
//----------------------------------------------------------------------------
void Skill::SetPrepareTime(int microSeconds)
{
	mPrepareTime = microSeconds;
}
//----------------------------------------------------------------------------
void Skill::SetPrepareTimeing(int microSeconds)
{
	mPrepareTimeing = microSeconds;
}
//----------------------------------------------------------------------------
void Skill::SetCD(int cd)
{
	mCD = cd;
}
//----------------------------------------------------------------------------
void Skill::SetCDing(int cding)
{
	mCDing = cding;
}
//----------------------------------------------------------------------------
void Skill::PauseCDing(bool pause)
{
	mIsPauseCDing = pause;
}
//----------------------------------------------------------------------------
float Skill::GetCDPercent() const
{
	int mcd = GetCD();
	if (0 == mcd)
	{
		return 0.0f;
	}

	float percent = (float)GetCDing() / (float)mcd;

	return percent;
}
//----------------------------------------------------------------------------
void Skill::SetDoCDing(bool cding)
{
	mIsDoCDing = cding;
}
//----------------------------------------------------------------------------
bool Skill::IsDoCDing() const
{
	return mIsDoCDing;
}
//----------------------------------------------------------------------------
void Skill::OnAdded()
{
}
//----------------------------------------------------------------------------
void Skill::OnRemove()
{
}
//----------------------------------------------------------------------------
void Skill::SetEquipped(bool equipped)
{
	mIsEquipped = equipped;
}
//----------------------------------------------------------------------------
void Skill::SetSkillChara(SkillChara *skillChara)
{
	mSkillChara = skillChara;
}
//----------------------------------------------------------------------------
void Skill::SetAffectGroupType(AffectGroupType agt)
{
	mAffectGroupType = agt;
}
//----------------------------------------------------------------------------
bool Skill::IsCanAddTarget(Skill *skill, SkillChara *sc)
{
	if (!skill) return false;
	if (!sc) return false;

	SkillChara *affectChara = skill->GetSkillChara();
	DefSkill *defSkill = skill->GetDefSkill();

	if (DefSkill::AGT_ENEMY == defSkill->TheAffectGroupType)
	{
		if (sc->GetGroupID() != affectChara->GetGroupID())
			return true;
	}
	else if (DefSkill::AGT_SELF == defSkill->TheAffectGroupType)
	{
		if (affectChara == skill->GetSkillChara())
			return true;
	}
	else if (DefSkill::AGT_SELFGROUP == defSkill->TheAffectGroupType)
	{
		if (sc->GetGroupID() == affectChara->GetGroupID())
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Skill::_SetActing(bool acting)
{
	mIsActing = acting;
}
//----------------------------------------------------------------------------
void Skill::Activate_SkillBut_ClickDown()
{
	if (!mDefSkill)
		return;

	DefSkill::TrigerType tt = mDefSkill->TheTrigerType;

	if (DefSkill::TT_BUT_DOWN == tt)
	{
		MainActivateSkillInstance(true);
	}
	else if (DefSkill::TT_BUT_HOLD == tt)
	{
		MainActivateSkillInstance(false);
	}
	else if (DefSkill::TT_BUT_DOWN_PREPARE_UP_ACT == tt ||
		DefSkill::TT_BUT_DOWN_PREPARE_TOTIME_ACT == tt ||
		DefSkill::TT_BUT_DOWN_PREPARE_TOTIME_HOLDACT == tt)
	{

	}
	else if (DefSkill::TT_BUT_DOWN_CHOOSEDIR == tt ||
		DefSkill::TT_BUT_DOWN_CHOOSERANGE == tt)
	{

	}
	else if (DefSkill::TT_BUT_DOWN_CHOOSEDIR_LAUCH == tt)
	{

	}
}
//----------------------------------------------------------------------------
void Skill::Activate_SkillBut_ClickUp()
{
	DefSkill::TrigerType tt = mDefSkill->TheTrigerType;

	if (DefSkill::TT_BUT_UP == tt)
	{

	}
	else if (DefSkill::TT_BUT_HOLD == tt)
	{

	}
	else if (DefSkill::TT_BUT_DOWN_PREPARE_UP_ACT == tt)
	{

	}
	else if (DefSkill::TT_BUT_DOWN_PREPARE_TOTIME_ACT == tt)
	{

	}
	else if (DefSkill::TT_BUT_DOWN_PREPARE_TOTIME_HOLDACT == tt)
	{

	}
}
//----------------------------------------------------------------------------
void Skill::Activate_SkillBut_Up()
{
	DefSkill::TrigerType tt = mDefSkill->TheTrigerType;

	if (DefSkill::TT_BUT_DOWN_CHOOSEDIR == tt ||
		DefSkill::TT_BUT_DOWN_CHOOSERANGE == tt)
	{

	}
	else if (DefSkill::TT_BUT_DOWN_CHOOSEDIR_LAUCH == tt)
	{

	}
}
//----------------------------------------------------------------------------
void Skill::Activate_SkillBut_Drag()
{

}
//----------------------------------------------------------------------------
bool Skill::_ActivateSkill(bool doCDing)
{
	if (!IsEnable()) return false;
	if (!mSkillChara) return false;

	if (mSkillChara->IsDead()) return false;

	if (mSkillChara->GetNumActingAnimingSkills() > 0) return false;

	if (IsDoCDing()) return false;

	if (doCDing)
	{
		_ActivateSkillCDing();
	}

	mSkillChara->OnSkillActivate(this);

	return true;
}
//----------------------------------------------------------------------------
void Skill::_ActivateSkillCDing()
{
	mIsDoCDing = true;
	mCDing = 0;
}
//----------------------------------------------------------------------------
void Skill::MainActivateSkillInstance(bool reDoCDing)
{
	PX2_UNUSED(reDoCDing);

	DefSkill::Mode mode = mDefSkill->TheMode;

	if (DefSkill::M_SIGNLE == mode)
	{
		if (_ActivateSkill())
		{
			APoint fromPos = mSkillChara->GetPosition();
			AVector fromDir = mSkillChara->GetDir();
			int aimTargetID = mSkillChara->GetAimTargetID();
			_ActivateSkillInstance(false, fromPos, fromDir, aimTargetID);
		}
	}
	else if (DefSkill::M_MULTI == mode)
	{
		if (_ActivateSkill())
		{
			//_ActivateMulti(reDoCDing);
		}
	}
	else if (DefSkill::M_MULTI_CONTINUE == mode)
	{
		//if (_ActivateSkill(reDoCDing))
			//_ActivateMulti(true);
	}
}
//----------------------------------------------------------------------------
void Skill::SetDirectPutPos(const APoint &putPos)
{
	mDirectPutPos = putPos;
}
//----------------------------------------------------------------------------
bool Skill::_ActivateSkillInstance(bool isFromNet, const APoint &fromPos,
	const AVector &fromDir, int aimTargetID,
	const APoint &targetPos)
{
	SkillInst *inst = new0 SkillInst(this);
	inst->IsFromNet = isFromNet;

	inst->FromPos = fromPos;
	inst->FromDir = fromDir;
	inst->AimTargetID = aimTargetID;
	inst->TargetPos = targetPos;

	inst->IsPursuitTarget = false;

	if (!isFromNet && mDefSkill->TheType == DefSkill::T_FAR_FLY)
	{
		// 从锚点获取位置
	}

	mSkillInsts.push_back(inst);
	inst->Enter(mFirstSkillStep);
	mSkillChara->OnSkillInstActivate(inst);

	if (!isFromNet)
	{
		SkillChara *sc = inst->GetSkill()->GetSkillChara();
		int roomID = sc->GetRoomID();
		int mapID = sc->GetMapID();
		int charaID = sc->GetID();

		PX2_SKILLB.C2S_CharacterActivateSkillInstance(roomID, mapID, 
			charaID, GetTypeID(), fromPos,
			fromDir, aimTargetID, targetPos);
	}

	return true;
}
//----------------------------------------------------------------------------
int Skill::_GenSkillActivatingID()
{
	return mNextSkillActivatingID++;
}
//----------------------------------------------------------------------------
void Skill::_GenSkillSteps()
{
	if (!mDefSkill)
		return;

	SkillStep *firstStep = new0 SkillStep(this);
	mFirstSkillStep = firstStep;

	SkillStep *setNextStep = firstStep;

	SkillStep *stepActivate = 0;
	SkillStep *stepFlyChain = 0;
	SkillStep *stepHit = 0;

	// act
	stepActivate = new0 SkillStep(this);
	stepActivate->TimeMicroSeconds = mDefSkill->AffectTime;
	stepActivate->AffectKey = mDefSkill->AffectKey;
	stepActivate->AnimEffects = mDefSkill->AnimEffects;
	stepActivate->IsStartStep = true;
	stepActivate->IsActivateStep = true;
	stepActivate->ProcessStep();

	// fly
	if (mDefSkill->TheType == DefSkill::T_FAR_FLY)
	{
		stepFlyChain = new SkillStep(this);
		stepFlyChain->TheMode = SkillStep::Mode::M_FLY;
		stepFlyChain->TimeMicroSeconds = mDefSkill->FlyTime;

		DefSkillAnimEffect defSAE;
		defSAE.AffectEffect = mDefSkill->FlyEffect;
		defSAE.AffectEffectAnchor = mDefSkill->FlyAnchor;
		stepFlyChain->AnimEffects.push_back(defSAE);

		stepFlyChain->FlySpeed = mDefSkill->FlySpeed;
		stepFlyChain->ProcessStep();
	}

	// hit
	stepHit = new SkillStep(this);
	stepHit->IsCalAffectInThisStep = true;
	stepHit->TimeMicroSeconds = 5000;

	DefSkillAnimEffect defSAE;
	defSAE.AffectEffect = mDefSkill->HitEffect;
	defSAE.AffectEffectAnchor = mDefSkill->HitEffectAnchor;
	defSAE.AffectPosEffect = mDefSkill->HitPosEffect;
	stepHit->AnimEffects.push_back(defSAE);
	
	stepHit->ProcessStep();

	// link the steps
	if (stepActivate)
	{
		setNextStep->NextStep = stepActivate;
		setNextStep = stepActivate;
	}

	if (stepFlyChain)
	{
		setNextStep->NextStep = stepFlyChain;
		setNextStep = stepFlyChain;
	}

	if (stepHit)
	{
		setNextStep->NextStep = stepHit;
	}
}
//----------------------------------------------------------------------------