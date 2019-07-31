// PX2SkillStep.hpp

#ifndef PX2SKILLSTEP_HPP
#define PX2SKILLSTEP_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Object.hpp"
#include "PX2Animation.hpp"
#include "PX2SkillDef.hpp"

namespace PX2
{

	class Skill;
	class SkillInst;

	class SKILLSYS_DLL_ITEM SkillStep : public Object
	{
	public:
		SkillStep(Skill *skill);
		virtual ~SkillStep();

		Skill *GetSkill();
		Pointer0<SkillStep> NextStep;

	public_internal:
		void SetSkill(Skill *skill);
		void ProcessStep();

	protected:
		Skill *mSkill;

	public:
		enum Mode
		{
			M_TIME,
			M_FLY,
			M_COLLIDE,
			M_MAX_MODE
		};
		Mode TheMode;
		std::string AffectKey;
		int TimeMicroSeconds;
		
		std::vector<DefSkillAnimEffect> AnimEffects;

		float FlySpeed;

		bool IsStartStep;
		bool IsActivateStep;
		bool IsCalAffectInThisStep;

		virtual void OnEnter(SkillInst *si);
		virtual void OnLeave(SkillInst *si);

	public_internal:
		void PlayEffect(int index);

	protected:
		void _CalResult(SkillInst *inst);

		SkillInst *mCurSkillInst;
		std::vector<AnimationPtr> mAnims;
		bool mUseAnimCall;
	};

#include "PX2SkillStep.inl"
	typedef Pointer0<SkillStep> SkillStepPtr;

}

#endif