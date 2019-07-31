// PX2AIES.hpp

#ifndef PX2AIES_HPP
#define PX2AIES_HPP

#include "PX2CorePre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM AIES)
	PX2_EVENT(LiDarOpen)
	PX2_EVENT(LiDarClose)
	PX2_EVENT(AxisOpen)
	PX2_EVENT(AxisClose)
	PX2_DECLARE_EVENT_END(AIES)

}

#endif