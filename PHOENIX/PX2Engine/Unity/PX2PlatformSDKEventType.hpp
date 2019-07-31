// PX2PlatformSDKEventType.hpp

#ifndef PX2PLATFORMSDKEVENTTYPE_HPP
#define PX2PLATFORMSDKEVENTTYPE_HPP

#include "PX2UnityPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM PlatformSDKSpace)
	PX2_EVENT(Login)
	PX2_EVENT(Buy)
	PX2_EVENT(TVGameOpen)
	PX2_EVENT(TVGameClose)
	PX2_EVENT(TVShowCorsor)
	PX2_EVENT(TVSetCorsorPos)
	PX2_EVENT(TVSetCorsorFirePressed)
	PX2_EVENT(TVSetCorsorFireReleased)
	PX2_DECLARE_EVENT_END(PlatformSDKSpace)

}

#endif