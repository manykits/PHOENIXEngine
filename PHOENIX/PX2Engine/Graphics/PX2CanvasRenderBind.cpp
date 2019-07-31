// PX2CanvasRenderBind.cpp

#include "PX2CanvasRenderBind.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Renderable, CanvasRenderBind);
PX2_IMPLEMENT_STREAM(CanvasRenderBind);
PX2_IMPLEMENT_FACTORY(CanvasRenderBind);
PX2_IMPLEMENT_DEFAULT_NAMES(Renderable, CanvasRenderBind);
PX2_IMPLEMENT_DEFAULT_STREAM(Renderable, CanvasRenderBind);

//----------------------------------------------------------------------------
CanvasRenderBind::CanvasRenderBind()
{
	mModelBound.SetRadius(100.0f);
	SetRenderLayer(Renderable::RL_UI, 0);
	Culling = Movable::CULL_NEVER;
}
//----------------------------------------------------------------------------
CanvasRenderBind::~CanvasRenderBind()
{
}
//----------------------------------------------------------------------------