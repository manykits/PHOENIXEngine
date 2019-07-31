// PX2SkillEvent.hpp

#ifndef PX2SKILLEVENT_HPP
#define PX2SKILLEVENT_HPP

#include "PX2EventSystem.hpp"
#include "PX2EventSpace.hpp"
#include "PX2Event.hpp"
#include "PX2SkillSysPre.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(SKILLSYS_DLL_ITEM SkillES)
	PX2_EVENT(BufChanged)
	PX2_EVENT(SetMainChara)
	PX2_DECLARE_EVENT_END(SkillES)

}

#endif