// PX2EngineNetEvent.hpp

#ifndef PX2ENGINENETEVENT_HPP
#define PX2ENGINENETEVENT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM EngineNetES)
	PX2_EVENT(OnEngineClientConnected)
	PX2_EVENT(OnEngineClientDisConnected)
	PX2_EVENT(OnEngineServerBeConnected)
	PX2_EVENT(OnEngineServerBeDisConnected)
	PX2_EVENT(EngineClientUDPInfoChanged)
	PX2_DECLARE_EVENT_END(EngineNetES)

}

#endif