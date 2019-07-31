// PX2LogicES.hpp

#ifndef PX2LOGICES_HPP
#define PX2LOGICES_HPP

#include "PX2LogicPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{


	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM LogicES)
	PX2_EVENT(SetSelectLogicObject)
	PX2_EVENT(RefreshGeneratedScript)
	PX2_DECLARE_EVENT_END(LogicES)

}

#endif