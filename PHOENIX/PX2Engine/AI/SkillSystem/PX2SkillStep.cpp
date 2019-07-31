// PX2SkillStep.cpp

#include "PX2SkillStep.hpp"
#include "PX2SkillInst.hpp"
#include "PX2SkillChara.hpp"
#include "PX2SkillEffectMan.hpp"
#include "PX2SkillAffect.hpp"
#include "PX2AddDeleteManager.hpp"
#include "PX2SkillTarget.hpp"
#include "PX2SkillFeild.hpp"
#include "PX2SkillBridge.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillStep::SkillStep(Skill *skill) :
mSkill(skill)
{
	TheMode = M_TIME;
	TimeMicroSeconds = 0;
	FlySpeed = 0.0f;
	IsStartStep = false;
	IsActivateStep = false;
	IsCalAffectInThisStep = false;
	mUseAnimCall = false;

	mCurSkillInst = 0;
}
//----------------------------------------------------------------------------
SkillStep::~SkillStep()
{
}
//----------------------------------------------------------------------------
void SkillStep::OnEnter(SkillInst *si)
{
	mCurSkillInst = si;

	SkillChara *skillChara = mSkill->GetSkillChara();
	Movable *owner = skillChara->GetOwner();
	PX2_UNUSED(owner);

	if (M_TIME == TheMode)
	{
		int animSize = (int)mAnims.size();
		if (animSize > 0)
		{
			int random = Mathf::IntRandom(0, animSize);
			
			const std::string animName = mAnims[random]->GetName();
			Animation *anim = skillChara->PlayAnim(animName);

			if (!AffectKey.empty())
			{
				AnimationCall *animCall = anim->GetAnimationCall(AffectKey);
				TimeMicroSeconds = (int)(animCall->CallTime * 1000.0f);
			}
			else
			{
				float animTime = anim->GetTime();
				TimeMicroSeconds = (int)(animTime * 1000.0f);
			}
		}
		else
		{
			TimeMicroSeconds = 0;
		}
	}

	if (!mUseAnimCall)
	{
		PlayEffect(0);
	}

	if (IsStartStep)
	{
		si->GetSkill()->_SetActing(true);
	}

	if (IsCalAffectInThisStep)
	{
		_CalResult(si);
	}
}
//----------------------------------------------------------------------------
void SkillStep::OnLeave(SkillInst *si)
{
	if (IsActivateStep)
	{
		si->GetSkill()->_SetActing(false);

		Skill *skill = si->GetSkill();
		SkillChara *skillChara = skill->GetSkillChara();
		if (skillChara)
		{
			skillChara->OnSkillInstActivateOver(si);
		}
	}
}
//----------------------------------------------------------------------------
void _SkillStepCallbackBegin(AnimationCall *call)
{
	Animation *anim = call->GetAnimation();

	SkillStep *step = call->GetUserData<SkillStep*>("step");
	if (step->IsActivateStep)
	{
		Skill *skill = step->GetSkill();
		SkillChara *skillChara = skill->GetSkillChara();
		skillChara->OnSkillActivateAnimBegin(skill, anim);
	}
}
//----------------------------------------------------------------------------
void _SkillStepCallbackEnd(AnimationCall *call)
{
	Animation *anim = call->GetAnimation();

	SkillStep *step = call->GetUserData<SkillStep*>("step");
	if (step->IsActivateStep)
	{
		Skill *skill = step->GetSkill();
		SkillChara *skillChara = skill->GetSkillChara();
		skillChara->OnSkillActivateAnimOver(skill, anim);
	}
}
//----------------------------------------------------------------------------
void _SkillStepCallback(AnimationCall *call)
{
	SkillStep *step = call->GetUserData<SkillStep*>("step");
	int index = call->GetUserData<int>("index");
	step->PlayEffect(index);
}
//----------------------------------------------------------------------------
void SkillStep::ProcessStep()
{
	SkillChara *skillChara = mSkill->GetSkillChara();

	for (int i = 0; i < (int)AnimEffects.size(); i++)
	{
		DefSkillAnimEffect &def = AnimEffects[i];
		
		if (!def.AffectAnim.empty())
		{
			Animation *anim = skillChara->GetAnim(def.AffectAnim);
			if (anim)
			{
				mAnims.push_back(anim);

				AnimationCall *callBegin = anim->GetAnimationCall("begin");
				callBegin->SetUserData("step", this);
				callBegin->CallBack = _SkillStepCallbackBegin;

				AnimationCall *callEnd = anim->GetAnimationCall("end");
				callEnd->SetUserData("step", this);
				callEnd->CallBack = _SkillStepCallbackEnd;

				if (!def.AffectEffectKey.empty() && anim)
				{
					AnimationCall *call = anim->GetAnimationCall(def.AffectEffectKey);
					if (call)
					{
						call->SetUserData("step", this);
						call->SetUserData("index", i);
						call->CallBack = _SkillStepCallback;
						mUseAnimCall = true;
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void SkillStep::PlayEffect(int index)
{
	if (index<0 || index>=(int)AnimEffects.size())
		return;

	SkillChara *skillChara = mSkill->GetSkillChara();
	Movable *owner = skillChara->GetOwner();
	DefSkillAnimEffect &def = AnimEffects[index];

	if (!def.AffectEffect.empty())
	{
		MovablePtr mov = PX2_SEMAN.GetEffect(def.AffectEffect.c_str());
		if (mov)
		{
			APoint pos = owner->WorldTransform.GetTranslate();
			HMatrix rot = owner->WorldTransform.GetRotate();
			pos.Z() += 0.5f;

			Node *scene = 0;
			if (scene)
			{
				scene->AttachChild(mov);
				mov->WorldTransformIsCurrent = true;
				mov->WorldTransform.SetTranslate(pos);
				mov->WorldTransform.SetRotate(rot);

				PX2_ADM.AddDeletingObj(mov, 5.0f, 0.0f);
			}

			mov->ResetPlay();
		}
	}

	if (!def.AffectPosEffect.empty())
	{
		Movable *anchorMov = owner;

		MovablePtr mov = PX2_SEMAN.GetEffect(def.AffectPosEffect.c_str());
		if (mov)
		{
			if (!def.AffectPosEffectAnchor.empty())
			{
				Movable *anchor = skillChara->GetAnchor(def.AffectPosEffectAnchor);
				if (anchor)
					anchorMov = anchor;
			}

			APoint pos = anchorMov->WorldTransform.GetTranslate();
			HMatrix rot = anchorMov->WorldTransform.GetRotate();

			Node *scene = 0;
			if (scene)
			{
				scene->AttachChild(mov);
				mov->WorldTransformIsCurrent = true;
				mov->WorldTransform.SetTranslate(pos);
				mov->WorldTransform.SetRotate(rot);

				PX2_ADM.AddDeletingObj(mov, 5.0f, 0.0f);
			}

			mov->ResetPlay();
		}
	}
}
//----------------------------------------------------------------------------
void SkillStep::_CalResult(SkillInst *si)
{
	Skill *skill = si->GetSkill();
	DefSkill *defSkill = skill->GetDefSkill();
	SkillStep *skillStep = si->GetCurSkillStep();
	SkillChara *skillChara = skill->GetSkillChara();

	PX2_UNUSED(skillStep);
	
	bool isRangeSkill = defSkill->RangeDegree > 0;

	if (DefSkill::T_NEAR == defSkill->TheType)
	{
		si->AffectPos = si->FromPos;
	}

	std::vector<SkillTarget> targets;
	if (DefSkill::T_NEAR == defSkill->TheType ||
		DefSkill::T_FAR_FLY == defSkill->TheType)
	{
		APoint skillAffectPos = si->AffectPos;

		if (!isRangeSkill)
		{
			int affectTargetID = si->AffectTargetID;

			if (DefSkill::AGT_ENEMY == defSkill->TheAffectGroupType)
			{
				SkillChara *targetSkillChara = PX2_SKILLFEILD.GetSkillChara(affectTargetID);
				if (targetSkillChara)
				{
					SkillTarget st;
					if (targetSkillChara->GetGroupID() != skillChara->GetGroupID())
					{
						st.CharaID = si->AffectTargetID;
						st.AffectPos = si->AffectPos;
						targets.push_back(st);
					}
				}
			}
			else if (DefSkill::AGT_SELF == defSkill->TheAffectGroupType)
			{
				SkillTarget st;
				st.CharaID = skillChara->GetID();
				st.AffectPos = si->AffectPos;
				targets.push_back(st);
			}
			else if (DefSkill::AGT_SELFGROUP == defSkill->TheAffectGroupType)
			{
				std::map<int, SkillCharaPtr> &skillCharas = PX2_SKILLFEILD.GetSkillCharas();
				auto it = skillCharas.begin();
				for (; it != skillCharas.end(); it++)
				{
					int charaID = it->first;
					SkillChara *sc = it->second;

					if (sc->GetGroupID() == skillChara->GetGroupID())
					{
						SkillTarget st;
						st.CharaID = charaID;
						st.AffectPos = si->AffectPos;
						targets.push_back(st);
					}
				}
			}
		}
		else
		{
			if (DefSkill::AGT_NONE == defSkill->TheAffectGroupType)
			{
				/*_*/
			}
			else
			{
				std::map<int, SkillCharaPtr> &skillCharas = PX2_SKILLFEILD.GetSkillCharas();
				auto it = skillCharas.begin();
				for (; it != skillCharas.end(); it++)
				{
					int charaID = it->first;
					PX2_UNUSED(charaID);

					SkillChara *chara = it->second;
					const APoint &charaPos = chara->GetPosition();

					AVector dir = charaPos - skillAffectPos;
					float length = dir.Normalize();

					bool isInRange = (length < skill->GetSkillRange()->RadiusLength);
					if (isInRange && Skill::IsCanAddTarget(skill, chara))
					{
						bool isAddTarget = false;
						if (360.0f == skill->GetSkillRange()->Degree)
						{
							isAddTarget = true;
						}
						else
						{
							AVector forward = si->FromDir;
							float dot = forward.Dot(dir);
							float degree = Mathf::ACos(dot) * Mathf::RAD_TO_DEG;
							if (degree < skill->GetSkillRange()->Degree)
							{
								isAddTarget = true;
							}
						}

						if (isAddTarget)
						{
							SkillTarget st;
							st.CharaID = chara->GetID();
							st.AffectPos = charaPos;
							targets.push_back(st);
						}
					}
				}
			}
		}
	}

	if (skillChara->IsBeMyS())
	{
		// affect
		for (int i = 0; i < (int)targets.size(); i++)
		{
			SkillTarget st = targets[i];
			SkillChara *targetSkillChara = PX2_SKILLFEILD.GetSkillChara(st.CharaID);
			if (targetSkillChara)
			{
				PX2_SKILLB.C2S_CharacterAffect(skillChara->GetRoomID(),
					skillChara->GetMapID(), skillChara->GetID(), skill->GetID(),
					targetSkillChara->GetID(), true);
			}
		}

		// genmonster
		if (defSkill->MonsterID > 0 && !si->IsFromNet)
		{
			std::vector<GenActorObj> monsObjs;
			GenActorObj monObj;
			monObj.MonsID = defSkill->MonsterID;
			monObj.GroupID = skillChara->GetGroupID();

			if (DefSkill::MPT_ANCHOR == defSkill->TheMonsterPosType)
			{
				Movable *anchorMov = 0;
				const std::string &anchor = defSkill->MonsterPosData;
				if (!anchor.empty())
				{
					anchorMov = skillChara->GetAnchor(anchor);
				}

				if (!anchorMov)
					anchorMov = skillChara->GetOwner();

				monObj.Pos = anchorMov->WorldTransform.GetTranslate();
				float rotX = 0.0f;
				float rotY = 0.0f;
				float rotZ = 0.0f;
				anchorMov->WorldTransform.GetRotate(rotX, rotY, rotZ);
				monObj.Rot = APoint(rotX, rotY, rotZ);
			}
			else
			{
				monObj.Pos = si->AffectPos;
			}
			monsObjs.push_back(monObj);

			PX2_SKILLB.C2S_GenMonsters(skillChara->GetRoomID(), skillChara->GetMapID(),
				skillChara->GetID(), monsObjs);
		}
	}
}
//----------------------------------------------------------------------------
void SkillStep::SetSkill(Skill *skill)
{
	mSkill = skill;
}
//----------------------------------------------------------------------------