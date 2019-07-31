// PX2EditEventType.hpp

#ifndef PX2_EDITEVENTTYPE_HPP
#define PX2_EDITEVENTTYPE_HPP

#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM EditES)
	PX2_EVENT(AddSelect)
	PX2_EVENT(RemoveSelect)
	PX2_EVENT(RemoveAllSelects)
	PX2_EVENT(PushUnDo)
	PX2_EVENT(UnDo)
	PX2_EVENT(ReDo)
	PX2_EVENT(NoUnDo)
	PX2_EVENT(NoReDo)
	PX2_EVENT(N_Window)
	PX2_EVENT(N_AddMenu)
	PX2_DECLARE_EVENT_END(EditES)

}

#endif