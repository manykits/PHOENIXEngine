// PX2ProjectEvent.hpp

#ifndef PX2PROJECTEVENTTYPE_HPP
#define PX2PROJECTEVENTTYPE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM ProjectES)
	PX2_EVENT(NewProject)
	PX2_EVENT(LoadedProject)
	PX2_EVENT(SavedProject)
	PX2_EVENT(CloseProject)
	PX2_EVENT(NewScene)
	PX2_EVENT(SavedScene)
	PX2_EVENT(CloseScene)
	PX2_EVENT(NewUI)
	PX2_EVENT(CloseUI)
	PX2_EVENT(NewLogic)
	PX2_EVENT(CloseLogic)
	PX2_EVENT(Play)
	PX2_EVENT(Stop)
	PX2_EVENT(SceneCanvasSizeChanged)
	PX2_EVENT(UICanvasSizeChanged)
	PX2_EVENT(WillEnterForeground)
	PX2_EVENT(DidEnterBackground)
	PX2_DECLARE_EVENT_END(ProjectES)

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM ProjectES_Internal)
	PX2_EVENT(ProjectSizeSameWithScreenChanged)
	PX2_EVENT(SceneShadowChanged)
	PX2_EVENT(SceneUICamerasChanged)
	PX2_EVENT(UISizeChanged)
	PX2_EVENT(AddUpdateProject)
	PX2_DECLARE_EVENT_END(ProjectES_Internal)

}

#endif