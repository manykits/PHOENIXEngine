// PX2Skill.hpp

#ifndef PX2SKILL_HPP
#define PX2SKILL_HPP

#include "PX2CorePre.hpp"
#include "PX2Object.hpp"
#include "PX2SkillSysPre.hpp"
#include "PX2SkillInst.hpp"
#include "PX2APoint.hpp"
#include "PX2SkillStep.hpp"
#include "PX2SkillRange.hpp"
#include "PX2SkillDef.hpp"

namespace PX2
{

	class SkillChara;

	class SKILLSYS_DLL_ITEM Skill : public Object
	{
		PX2_NEW(Skill);

	public:
		Skill();
		virtual ~Skill();

		void Clear();
		void Update(int elapsedMicroSeconds);

		virtual void OnAdded();
		virtual void OnRemove();

		bool IsEquipped() const;

		SkillChara *GetSkillChara();
		
	public_internal:
		void SetEquipped(bool equipped);
		void SetSkillChara(SkillChara *skillChara);

	private:
		void _GenSkillSteps();

		SkillChara *mSkillChara;
		bool mIsEquipped;

	public:
		virtual void SetTypeID(int typeID);
		int GetTypeID() const;

		DefSkill *GetDefSkill();

	public:
		bool Prepare(bool pre, bool actSkill);
		bool IsPrepareing() const;
		void SetPrepareTime(int microSeconds);
		int GetPrepareTime() const;
		void SetPrepareTimeing(int microSeconds);
		int GetPrepareTimeing() const;

	protected:
		bool mIsPrepareing;
		int mPrepareTime;
		int mPrepareTimeing;

	public:
		void SetCD(int cd);
		int GetCD() const;
		void SetCDing(int cding);
		int GetCDing() const;
		float GetCDPercent() const;
		void PauseCDing(bool pause);
		bool IsPauseCDing() const;

		void SetDoCDing(bool cding);
		bool IsDoCDing() const;

		enum AffectGroupType
		{
			AGT_NONE,
			AGT_ENEMY,
			AGT_I,
			AGT_IGROUP,
			AGT_MAX_TYPE
		};
		void SetAffectGroupType(AffectGroupType agt);
		AffectGroupType GetAffectGroupType() const;

		SkillRange *GetSkillRange();

		static bool IsCanAddTarget(Skill *skill, SkillChara *sc);

	private:
		int mTypeID;
		int mCD;
		int mCDing;
		bool mIsDoCDing;
		bool mIsPauseCDing;
		AffectGroupType mAffectGroupType;
		DefSkill *mDefSkill;
		SkillStepPtr mFirstSkillStep;
		SkillRangePtr mSkillRange;

	public:
		bool IsActing() const;// 是否正在发动过程中

		virtual void Activate_SkillBut_ClickDown();
		virtual void Activate_SkillBut_ClickUp();
		virtual void Activate_SkillBut_Up();
		virtual void Activate_SkillBut_Drag();

		void MainActivateSkillInstance(bool reDoCDing);

		int GetNumSkillInsts() const;

		void SetDirectPutPos(const APoint &putPos);
		const APoint &GetDirectPutPos() const;

	public_internal:
		void _SetActing(bool acting);

	public_internal:
		bool _ActivateSkill(bool doCDing = true);
		void _ActivateSkillCDing();
		bool _ActivateSkillInstance(bool isFromNet = false,
			const APoint &fromPos = APoint::ORIGIN,
			const AVector &fromDir = AVector::ZERO,
			int aimTargetID = 0,
			const APoint &targetPos = APoint::ORIGIN);
		int _GenSkillActivatingID();

		bool mIsActing;
		std::vector<SkillInstPtr> mSkillInsts;
		APoint mDirectPutPos;

		int mNextSkillActivatingID;
		std::vector<int> mSkillActivatingIDs;
	};

#include "PX2Skill.inl"
	typedef Pointer0<Skill> SkillPtr;

}

#endif