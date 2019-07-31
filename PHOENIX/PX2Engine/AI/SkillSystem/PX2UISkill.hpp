// PX2UISkill.hpp

#ifndef PX2UISKILL_HPP
#define PX2UISKILL_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIRound.hpp"
#include "PX2Skill.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM UISkill : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_NEW(UISkill);
		PX2_DECLARE_STREAM(UISkill);

	public:
		UISkill();
		virtual ~UISkill();

		UIRound *GetRoundBut();
		UIFText *GetText();

		void SetSkill(Skill *skill);
		Skill *GetSkill();

	protected:
		void _UICallback(UIFrame *frame, UICallType type);
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		UIRoundPtr mUIRound;
		UIFTextPtr mTextSkillName;
		UIFTextPtr mTextSkillTime;

		SkillPtr mSkill;
	};

	PX2_REGISTER_STREAM(UISkill);
	typedef Pointer0<UISkill> V_UISkillPtr;

}

#endif