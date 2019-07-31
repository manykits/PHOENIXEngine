// PX2UISkills.hpp

#ifndef PX2UISKILLS_HPP
#define PX2UISKILLS_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UISkill.hpp"

namespace PX2
{

	class SkillChara;
	
	class SKILLSYS_DLL_ITEM UISkills : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_NEW(UISkills);
		PX2_DECLARE_STREAM(UISkills);

	public:
		UISkills();
		virtual ~UISkills();

		void RefreshSkills(SkillChara *skillChara);
		void ClearSkills();

		void CreateSampleSkills();

		virtual void OnEvent(Event *ent);

	protected:
		void _UICallback(UIFrame *frame, UICallType type);

		std::vector<V_UISkillPtr> mUISkills;
	};

	PX2_REGISTER_STREAM(UISkills);
	typedef Pointer0<UISkills> V_UISkillsPtr;

}

#endif