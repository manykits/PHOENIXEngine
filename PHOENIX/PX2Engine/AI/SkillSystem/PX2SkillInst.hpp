// PX2SkillInst.hpp

#ifndef PX2SKILLINST_HPP
#define PX2SKILLINST_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Object.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2SkillStep.hpp"
#include "PX2SkillFlyObj.hpp"

namespace PX2
{

	class Skill;

	class SKILLSYS_DLL_ITEM SkillInst : public Object
	{
		PX2_NEW(SkillInst);

	public:
		SkillInst(Skill *skill);
		virtual ~SkillInst();

		Skill *GetSkill();
		SkillStep *GetCurSkillStep();

		virtual void Update(int elapsedMicroSeconds);

		void Enter(SkillStep *sp);
		void Leave(SkillStep *sp);
		virtual void OnEnter(SkillStep *sp);
		virtual void OnLeave(SkillStep *sp);

		bool IsInstanceOver() const;

		bool IsFromNet;

		APoint FromPos;
		AVector FromDir;

		int AimTargetID;
		bool IsPursuitTarget;
		APoint TargetPos;

		int AffectTargetID;
		APoint AffectPos;

	protected:
		SkillInst();
		void GoToNextSkillStep();

		Skill *mSkill;
		SkillFlyObjPtr mFlyObj;

		SkillStepPtr mCurSkillStep;
		int mCurSkillStepRunTime;

		bool mIsInstanceOver;
	};

#include "PX2SkillInst.inl"
	typedef Pointer0<SkillInst> SkillInstPtr;

}

#endif