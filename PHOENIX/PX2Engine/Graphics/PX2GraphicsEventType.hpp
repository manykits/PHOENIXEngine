// PX2GraphicsEventType.hpp

#ifndef PX2GRAPHICSEVENTTYPE_HPP
#define PX2GRAPHICSEVENTTYPE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM GraphicsES)
	PX2_EVENT(Info)
	PX2_EVENT(AddObject)
	PX2_EVENT(RemoveObject)
	PX2_EVENT(CanvasDestory)
	PX2_EVENT(WindowSizeChanged)
	PX2_EVENT(WindowMaxSize)
	PX2_EVENT(WindowShow)
	PX2_EVENT(WindowHide)
	PX2_EVENT(WindowSetPosition)
	PX2_EVENT(WindowSetCursorType)	
	PX2_EVENT(WindowAddMenu)
	PX2_EVENT(WindowOpenFileDlg)
	PX2_EVENT(WindowSaveFileDlg)
	PX2_EVENT(CameraNodeChanged)
	PX2_EVENT(GeneralString)
	PX2_DECLARE_EVENT_END(GraphicsES)

}

#endif