// PX2CanvasRenderBind.hpp

#ifndef PX2CANVASRENDERBIND_HPP
#define PX2CANVASRENDERBIND_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Renderable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CanvasRenderBind : public Renderable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CanvasRenderBind);

	public:
		CanvasRenderBind();
		virtual ~CanvasRenderBind();
	};

	PX2_REGISTER_STREAM(CanvasRenderBind);
	typedef Pointer0<CanvasRenderBind> CanvasRenderBindPtr;

}

#endif
