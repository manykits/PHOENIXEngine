// PX2SkillTarget.hpp

#ifndef PX2SKILLTARGET_HPP
#define PX2SKILLTARGET_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM SkillTarget
	{
	public:
		SkillTarget();
		~SkillTarget();

		int CharaID;
		APoint AffectPos;
	};

}

#endif