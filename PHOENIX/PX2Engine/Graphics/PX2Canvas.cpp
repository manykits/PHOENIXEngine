// PX2UIView.cpp

#include "PX2Canvas.hpp"
#include "PX2Renderer.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2MaterialInstance.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2EventWorld.hpp"
#include "PX2Log.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, SizeNode, Canvas);
PX2_IMPLEMENT_STREAM(Canvas);
PX2_IMPLEMENT_FACTORY(Canvas);
PX2_IMPLEMENT_DEFAULT_NAMES(SizeNode, Canvas);

//----------------------------------------------------------------------------
Canvas::Canvas() :
mRenderNode(0),
mIsRenderNodeUpdate(false),
mRenderWindow(0),
mIsMain(false),
mOverCamera(0),
mIsUICameraAutoAdjust(true),
mClearStencil(0),
mClearFlagColor(false),
mClearFlagDepth(false),
mClearFlagStencil(false),
mIsOverWireframe(false),
mIsPickPosRecal(false),
mIsPressed(false),
mIsEntered(false),
mIsLeftPressed(false),
mIsRightPressed(false),
mIsMiddlePressed(false),
mIsMoved(false),
mClearDepth(1.0f),
mPickedWidget(0)
{
	SetName("Canvas");

	mCanvasRenderBind = new0 CanvasRenderBind();
	AttachChild(mCanvasRenderBind);

	mOverrideWireProperty = new0 WireProperty();
	mOverrideWireProperty->Enabled = true;

	mPixelToWorld.first = 0.0f;
	mPixelToWorld.second = 0.0f;

	VertexColor4MaterialPtr mtl = new0 VertexColor4Material();
	AlphaProperty *alphaProperty = mtl->GetAlphaProperty(0, 0);
	alphaProperty->BlendEnabled = true;
	MaterialInstance *mtlInst = mtl->CreateUniqueInstance();
	mtlInst->GetPass(0)->GetAlphaProperty()->BlendEnabled = true;
	mCanvasRenderBind->SetMaterialInstance(mtlInst);

	mOverWireProperty = new0 WireProperty();
	mOverWireProperty->Enabled = true;

	SetColor(Float3::WHITE);
	SetAlpha(1.0f);
	SetBrightness(1.0f);
	SetColorSelfCtrled(true);
	SetAlphaSelfCtrled(true);
	SetBrightnessSelfCtrled(true);

	VertexFormat *vfPoly = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT1);
	mDebugPolyVB = new0 VertexBuffer(20000, vfPoly->GetStride(), Buffer::BU_DYNAMIC);
	mDebugPoly = new0 Polysegment(vfPoly, mDebugPolyVB, false);
	mDebugPoly->SetNumSegments(0);
	MaterialInstance *mi = VertexColor4Material::CreateUniqueInstance();
	mDebugPoly->SetMaterialInstance(mi);

	VertexFormat *vFormat = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT1);
	VertexBufferPtr vb = new0 VertexBuffer(5120, vFormat->GetStride(), Buffer::BU_DYNAMIC);
	IndexBufferPtr ib = new0 IndexBuffer(10240, 2, Buffer::BU_DYNAMIC);
	mUIShareMesh = new0 TriMesh(vFormat, vb, ib);
	mUIShareMesh->SetColor(Float3::WHITE);
	mUIShareMesh->Culling = Movable::CULL_NEVER;
	mUIShareMesh->SetRenderLayer(Renderable::RL_UI);
	mUIShareMesh->GetShine()->Emissive = Float4::WHITE;
	mUIShareMesh->SetName("ShareMesh");

	// ui
#if defined(_WIN32) || defined(WIN32)
	mMoveAdjugeParam = 5.0f;
#else
	mMoveAdjugeParam = 64.0f;
#endif
	mMoveAdjugeParamSquare = mMoveAdjugeParam * mMoveAdjugeParam;

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);
	EnableAnchorLayout(true);

	mCanvasRenderBind->SetRenderLayer(Renderable::RL_UI);

	SetWidget(true);

	SetClearFlag(true, true, true);
	SetClearColor(Float4::BLACK);

	SetChildNotPickRecursion(false);
	SetActivateSelfCtrled(false);
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
Canvas::~Canvas()
{
	GoOutEventWorld();

	Event *ent = GraphicsES::CreateEventX(GraphicsES::CanvasDestory);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Canvas::SetMain(bool main)
{
	mIsMain = main;
}
//----------------------------------------------------------------------------
void Canvas::SetRenderNode(Node *movable)
{
	mRenderNode = movable;

	if (mRenderNode)
	{
		SizeNode *sn = DynamicCast<SizeNode>(mRenderNode);
		if (sn)
		{
			const Rectf &screenRect = GetScreenRect();
			sn->SetScreenRect(screenRect);
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::SetScreenRect(const Rectf &rect)
{
	SizeNode::SetScreenRect(rect);

	if (mRenderNode)
	{
		SizeNode *sn = DynamicCast<SizeNode>(mRenderNode);
		if (sn)
		{
			sn->SetScreenRect(rect);
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::SetRenderWindow(RenderWindow *rw)
{
	mRenderWindow = rw;
	mIsScreenRectChanged = true;
}
//----------------------------------------------------------------------------
void Canvas::OnSizeChanged()
{
	SizeNode::OnSizeChanged();

	CalPixelToWorld(mOverCamera);
}
//----------------------------------------------------------------------------
void Canvas::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	ClearDebugLine();

	SizeNode::UpdateWorldData(applicationTime, elapsedTime);

	if (mRenderNode && mIsRenderNodeUpdate)
	{
		mRenderNode->Update(applicationTime, elapsedTime);
	}
}
//----------------------------------------------------------------------------
void Canvas::UpdateLayout(Movable *parent)
{
	SizeNode::UpdateLayout(parent);

	if (mIsUICameraAutoAdjust)
		_UpdateCamera();
}
//----------------------------------------------------------------------------
void Canvas::UpdateLeftBottomCornerOffset(Movable *parent)
{
	SizeNode::UpdateLeftBottomCornerOffset(parent);

	if (mIsUICameraAutoAdjust)
		_UpdateCamera();
}
//----------------------------------------------------------------------------
void Canvas::_UpdateCamera()
{
	if (mUICameraNode)
	{
		float helfWidth = mSize.Width / 2.0f;
		float helfHeight = mSize.Height / 2.0f;
		mUICameraNode->GetCamera()->SetFrustum(0.1f, Mathf::FAbs(msUICameraY) + 100.0f,
			-helfHeight, helfHeight, -helfWidth, helfWidth);

		mUICameraNode->LocalTransform.SetTranslate(
			helfWidth + LeftBottomCornerOffset.X(), msUICameraY,
			helfHeight + LeftBottomCornerOffset.Z());
		mUICameraNode->Update(0.0, 0.0);
	}
}
//----------------------------------------------------------------------------
void Canvas::UpdateWorldBound()
{
	// 如果bound为0，我们需要重新计算一个bound

	if (!WorldBoundIsCurrent)
	{
		bool foundFirstBound = false;

		WorldBound.SetCenter(APoint::ORIGIN);
		WorldBound.SetRadius(0.0f);

		for (int i = 0; i < (int)mChild.size(); i++)
		{
			Movable *child = mChild[i];

			if (child)
			{
				if (foundFirstBound)
				{
					// 将所有孩子的节点包围体融合到一起，作为当前节点的包围体
					WorldBound.GrowToContain(child->WorldBound);
				}
				else
				{
					// 首先将第一个孩子的包围盒作为节点的包围盒
					foundFirstBound = true;
					WorldBound = child->WorldBound;
				}
			}
		}

		if (0.0f == WorldBound.GetRadius())
		{
			float radius = Mathf::Sqrt(mSize.Width*mSize.Width +
				mSize.Height + mSize.Height) / 2.0f;
			WorldBound.SetCenter(WorldTransform.GetTranslate());
			WorldBound.SetRadius(radius);
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::SetOverCamera(Camera *overCamera)
{
	if (mOverCamera)
	{
		mOverCameraCuller = 0;
	}

	mOverCamera = overCamera;

	if (mOverCamera)
	{
		mOverCameraCuller = new0 Culler();
		mOverCameraCuller->SetCamera(mOverCamera);
	}
}
//----------------------------------------------------------------------------
void Canvas::AddCamera(Camera *camera)
{
	std::map<Camera*, CullerPtr>::iterator it = mCullers.find(camera);
	if (it == mCullers.end())
	{
		mCameras.push_back(camera);
		Culler *culler = new0 Culler();
		culler->SetCamera(camera);

		mCullers[camera] = culler;
	}
}
//----------------------------------------------------------------------------
void Canvas::RemoveCamera(Camera *camera)
{
	std::vector<Camera *>::iterator itVect = std::find(mCameras.begin(),
		mCameras.end(), camera);
	if (itVect != mCameras.end())
	{
		mCameras.erase(itVect);
	}

	std::map<Camera*, CullerPtr>::iterator it = mCullers.find(camera);
	if (it != mCullers.end())
	{
		mCullers.erase(it);
	}
}
//----------------------------------------------------------------------------
void Canvas::RemoveAllCameras()
{
	mCameras.clear();
	mCullers.clear();
}
//----------------------------------------------------------------------------
float Canvas::msUICameraY = -2000.0f;
//----------------------------------------------------------------------------
CameraNode *Canvas::CreateUICameraNode()
{
	Camera *camera = new0 Camera(false);
	camera->SetFrame(APoint(0.0f, msUICameraY, 0.0f), AVector::UNIT_Y,
		AVector::UNIT_Z, AVector::UNIT_X);
	camera->SetFrustum(0.1f, Mathf::FAbs(msUICameraY) + 1000.0f, -100.0, 100.0f,
		-100.0f, 100.0f);
	AddCamera(camera);
	camera->SetClearFlag(false, false, false);
	camera->SetClearColor(Float4::RED);

	mUICameraNode = new0 CameraNode();
	AttachChild(mUICameraNode);
	mUICameraNode->SetName("UICameraNode");
	mUICameraNode->SetCamera(camera);

	mCanvasRenderBind->SetRenderLayer(Renderable::RL_UI);

	return mUICameraNode;
}
//----------------------------------------------------------------------------
CameraNode *Canvas::GetUICameraNode()
{
	return mUICameraNode;
}
//----------------------------------------------------------------------------
void Canvas::EnableUICameraAutoAdjust(bool autoAdjust)
{
	mIsUICameraAutoAdjust = autoAdjust;

	if (mIsUICameraAutoAdjust)
		_UpdateCamera();
}
//----------------------------------------------------------------------------
bool Canvas::IsUICameraAutoAdjust() const
{
	return mIsUICameraAutoAdjust;
}
//----------------------------------------------------------------------------
void Canvas::OnGetVisibleSet(Culler& culler, bool noCull)
{
	if (!IsShow())
		return;

	Movable *cullingScene = culler.GetCurCullingScene();
	if (cullingScene == this)
	{ 
		// 从当前canvas开始，根

		std::vector<MovablePtr>::iterator iter = mChild.begin();
		std::vector<MovablePtr>::iterator end = mChild.end();
		for (/**/; iter != end; ++iter)
		{
			Movable* child = *iter;
			if (child && child != mCanvasRenderBind)
			{
				child->GetVisibleSet(culler, noCull);
			}
		}

		if (mRenderNode)
		{
			mRenderNode->GetVisibleSet(culler, noCull);
		}
	}
	else
	{
		// 内部canvas, 只把canvas的的CanvasRenderBind加入
		mCanvasRenderBind->GetVisibleSet(culler, noCull);

		ClearVisibleSet();
		ComputeVisibleSet();
	}
}
//----------------------------------------------------------------------------
void Canvas::OnCulled(Culler& culler)
{
	SizeNode::OnCulled(culler);

	if (mRenderNode)
	{
		mRenderNode->OnCulled(culler);
	}
}
//----------------------------------------------------------------------------
void Canvas::SetClearColor(const Float4 &color)
{
	mClearColor = color;
}
//----------------------------------------------------------------------------
void Canvas::SetClearDepth(float depth)
{
	mClearDepth = depth;
}
//----------------------------------------------------------------------------
void Canvas::SetClearStencil(unsigned int stencil)
{
	mClearStencil = stencil;
}
//----------------------------------------------------------------------------
void Canvas::SetClearFlag(bool color, bool depth, bool stencil)
{
	mClearFlagColor = color;
	mClearFlagDepth = depth;
	mClearFlagStencil = stencil;
}
//----------------------------------------------------------------------------
void Canvas::SetOverWireframe(bool wireframe)
{
	mIsOverWireframe = wireframe;
}
//----------------------------------------------------------------------------
void Canvas::ClearVisibleSet()
{
	if (mOverCamera)
	{
		mOverCameraCuller->Clear();
	}
	else
	{
		std::map<Camera*, CullerPtr>::iterator it = mCullers.begin();
		for (; it != mCullers.end(); it++)
		{
			it->second->Clear();
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::ComputeVisibleSet()
{
	if (!IsShow()) return;

	if (mOverCamera)
	{
		Movable *cullMov = this;
		if (mRenderNode)
			cullMov = mRenderNode;
		mOverCameraCuller->Clear();
		mOverCameraCuller->ComputeVisibleSet(cullMov);
		mOverCameraCuller->GetVisibleSet().Sort();
	}
	else
	{
		Movable *cullMov = this;
		if (mRenderNode)
			cullMov = mRenderNode;

		std::map<Camera*, CullerPtr>::iterator it = mCullers.begin();
		for (; it != mCullers.end(); it++)
		{
			it->second->Clear();
			it->second->ComputeVisibleSet(cullMov);
			it->second->GetVisibleSet().Sort();
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::Draw(Renderer *renderer)
{
	if (!IsShow())
		return;

	// before
	Rectf beforeViewPort = renderer->GetViewPort();

	if (mScreenRect.Height() > 0)
	{
		renderer->SetViewport(mScreenRect);
		renderer->InitRenderStates();

		bool bClearColor = false;
		bool bClearDepth = false;
		bool bClearStencil = false;
		GetClearFlag(bClearColor, bClearDepth, bClearStencil);
		_Clear(renderer, bClearColor, GetClearColor(),
			bClearDepth, GetClearDepth(), bClearStencil, GetClearStencil());

		if (mIsOverWireframe)
		{
			renderer->SetOverrideWireProperty(mOverWireProperty);
		}

		if (mOverCamera && mOverCamera->IsEnable())
		{
			renderer->SetViewport(mScreenRect);
			_Draw(mOverCamera, renderer, mOverCameraCuller);
		}
		else
		{
 			for (int i = 0; i < (int)mCameras.size(); i++)
			{
				Camera *camera = mCameras[i];
				Culler *culler = mCullers[camera];

				renderer->SetViewport(mScreenRect);
				if (camera->IsEnable())
				{
					_Draw(camera, renderer, culler);
				}
			}
		}
	}

	renderer->SetViewport(beforeViewPort);

	if (mIsOverWireframe)
	{
		renderer->SetOverrideWireProperty(0);
	}
}
//----------------------------------------------------------------------------
void Canvas::_Clear(Renderer *renderer, bool bColor, const Float4 &color,
	bool bDepth, float depth, bool bStencil, int stencil)
{
	const Rectf &viewPort = renderer->GetViewPort();
	PX2_UNUSED(viewPort);

	if (bColor && bDepth && bStencil)
	{
		renderer->SetClearColor(color);
		renderer->SetClearDepth(depth);
		renderer->SetClearStencil(stencil);

#if defined PX2_USE_DX9
		renderer->ClearBuffers();
#elif defined PX2_USE_OPENGL || defined PX2_USE_OPENGLES
		renderer->ClearBuffers((int)viewPort.Left, (int)viewPort.Bottom,
			(int)viewPort.Width(), (int)viewPort.Height());
#endif
	}
	else
	{
		if (bColor)
		{
			renderer->SetClearColor(color);

#if defined PX2_USE_DX9
			renderer->ClearColorBuffer();
#elif defined PX2_USE_OPENGL || defined PX2_USE_OPENGLES
			renderer->ClearColorBuffer((int)viewPort.Left, (int)viewPort.Bottom,
				(int)viewPort.Width(), (int)viewPort.Height());
#endif
		}
		if (bDepth)
		{
			renderer->SetClearDepth(depth);

#if defined PX2_USE_DX9
			renderer->ClearDepthBuffer();
#elif defined PX2_USE_OPENGL || defined PX2_USE_OPENGLES
			renderer->ClearDepthBuffer((int)viewPort.Left, (int)viewPort.Bottom,
				(int)viewPort.Width(), (int)viewPort.Height());
#endif
		}
		if (bStencil)
		{
			renderer->SetClearStencil(stencil);

#if defined PX2_USE_DX9
			renderer->ClearStencilBuffer();
#elif defined PX2_USE_OPENGL || defined PX2_USE_OPENGLES
			renderer->ClearStencilBuffer((int)viewPort.Left, (int)viewPort.Bottom,
				(int)viewPort.Width(), (int)viewPort.Height());
#endif
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::_Draw(Camera *camera, Renderer *renderer, Culler *culler)
{
	bool isDoDrawShare = PX2_GR.IsUIShareDraw();
	if (isDoDrawShare)
	{
		VisibleSet &visibleSet = culler->GetVisibleSet();

		CameraPtr beforeCamera = renderer->GetCamera();

		renderer->SetCamera(camera);
		renderer->InitRenderStates();
		EnvirParamController *evrParam = PX2_GR.GetCurEnvirParamController();
		if (evrParam)
			evrParam->ComputeEnvironment(visibleSet);

		bool clearColor = false;
		bool clearDepth = false;
		bool clearStencil = false;
		camera->GetClearFlag(clearColor, clearDepth, clearStencil);

		_Clear(renderer, clearColor, camera->GetClearColor(),
			clearDepth, camera->GetClearDepth(),
			clearStencil, camera->GetClearStencil());

		mUIShareMesh->GetVertexBuffer()->SetNumElements(0);
		mUIShareMesh->GetIndexBuffer()->SetNumElements(0);

		for (int i = 0; i < (int)visibleSet.GetNumVisible(); i++)
		{
			Renderable *renderable = visibleSet.GetVisible(i);
			CanvasRenderBind *crb = DynamicCast<CanvasRenderBind>(renderable);
			if (crb)
			{
				_FlushShareDraw(renderer);
				renderer->Draw(crb);
			}
			else if (renderable)
			{
				if (renderable->IsEnableShareDraw())
				{
					Renderable::PreDrawRetType ret = renderable->OnPreShareDraw(mUIShareMesh);
					if (Renderable::PBRT_NONE == ret)
					{
						renderable->OnShareDraw(mUIShareMesh);
					}
					else
					{
						_FlushShareDraw(renderer);

						renderable->OnShareDraw(mUIShareMesh);
					}
				}
				else
				{
					_FlushShareDraw(renderer);
					renderer->Draw(renderable);
				}
			}
		}

		_FlushShareDraw(renderer);

		renderer->SetCamera(beforeCamera);
	}
	else
	{
		VisibleSet &visibleSet = culler->GetVisibleSet();

		CameraPtr beforeCamera = renderer->GetCamera();

		renderer->SetCamera(camera);
		renderer->InitRenderStates();
		EnvirParamController *evrParam = PX2_GR.GetCurEnvirParamController();
		if (evrParam)
			evrParam->ComputeEnvironment(visibleSet);

		bool clearColor = false;
		bool clearDepth = false;
		bool clearStencil = false;
		camera->GetClearFlag(clearColor, clearDepth, clearStencil);

		_Clear(renderer, clearColor, camera->GetClearColor(),
			clearDepth, camera->GetClearDepth(),
			clearStencil, camera->GetClearStencil());

		renderer->Draw(visibleSet);

		// cameradrawcallback
		CameraDrawCallback callback = camera->GetCameraDrawCallback();
		if (callback)
		{
			callback(renderer);
		}

		renderer->SetCamera(beforeCamera);
	}
}
//----------------------------------------------------------------------------
void Canvas::_FlushShareDraw(Renderer *renderer)
{
	if (0 == mUIShareMesh->GetVertexBuffer()->GetNumElements())
		return;

	renderer->Update(mUIShareMesh->GetVertexBuffer());
	renderer->Update(mUIShareMesh->GetIndexBuffer());
	renderer->Draw(mUIShareMesh);
	mUIShareMesh->SetMaterialInstance(0);
	mUIShareMesh->GetVertexBuffer()->SetNumElements(0);
	mUIShareMesh->GetIndexBuffer()->SetNumElements(0);
}
//----------------------------------------------------------------------------
CanvasInputData Canvas::ConvertInputEventDataToCanvasInputData(
	const APoint &screenPos,
	const APoint &logicPos, int entType, int butID,
	float wheel)
{
	CanvasInputData cid;
	cid.ScreenPos = screenPos;
	cid.LogicPos = logicPos;
	cid.CameraLogicPos = logicPos;
	cid.Wheel = wheel;

	cid.TheMouseTag = CanvasInputData::MT_LEFT;
	if (MBID_LEFT == butID)
	{
		cid.TheMouseTag = CanvasInputData::MT_LEFT;
	}
	else if (MBID_RIGHT == butID)
	{
		cid.TheMouseTag = CanvasInputData::MT_RIGHT;
	}
	else if (MBID_MIDDLE == butID)
	{
		cid.TheMouseTag = CanvasInputData::MT_MIDDLE;
	}

	if (InputEventSpace::MousePressed == entType)
	{
		cid.PickType = UIPT_PRESSED;
	}
	else if (InputEventSpace::MouseReleased == entType)
	{
		cid.PickType = UIPT_RELEASED;
	}
	else if (InputEventSpace::MouseDoublePressed == entType)
	{
		cid.PickType = UIPT_DOUBLE_PRESSED;
	}
	else if (InputEventSpace::MouseMoved == entType)
	{
		cid.PickType = UIPT_MOVED;
	}
	else if (InputEventSpace::MouseWheeled == entType)
	{
		cid.PickType = UIPT_WHELLED;
	}
	else if (InputEventSpace::TouchPressed == entType)
	{
		cid.PickType = UIPT_PRESSED;
	}
	else if (InputEventSpace::TouchReleased == entType)
	{
		cid.PickType = UIPT_RELEASED;
	}
	else if (InputEventSpace::TouchMoved == entType)
	{
		cid.PickType = UIPT_MOVED;
	}

	return cid;
}
//----------------------------------------------------------------------------
void Canvas::SetPickPosRecal(bool recal)
{
	mIsPickPosRecal = recal;
}
//----------------------------------------------------------------------------
bool Canvas::IsPickPosRecal() const
{
	return mIsPickPosRecal;
}
//----------------------------------------------------------------------------
void Canvas::OnFirstInput(const InputEventData &data)
{
	for (int i = 0; i < (int)data.TPoses.size(); i++)
	{
		const TouchState &ts = data.TPoses[i];
		const APoint &screenPos = ts.Pos;
		const Rectf &worldRect = GetWorldRect();
		if (worldRect.IsInsize(screenPos.X(), screenPos.Z()) || data.IsKeyEvent())
		{
			CanvasInputData cid = ConvertInputEventDataToCanvasInputData(
				screenPos, screenPos, data.TheEventType, data.MButtonID, data.MWheel);
			cid.TouchID = ts.ID;
			OnWidgetPicked(cid);
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::_DoPick(const CanvasInputData &inputData)
{
	_ClearInRangePickWidget();

	//if (RenderWindow::IsScreenDrag() && UIPT_MOVED==inputData.PickType)
	//	return;

	if (mRenderNode)
	{
		SizeNode *sizeNode = DynamicCast<SizeNode>(mRenderNode);
		if (sizeNode)
			sizeNode->PreCanvasPick(inputData, this);
	}

	for (int i = 0; i < (int)mUpdateChild.size(); i++)
	{
		SizeNode *sizeNode = DynamicCast<SizeNode>(mUpdateChild[i]);
		if (sizeNode)
		{
			sizeNode->PreCanvasPick(inputData, this);
		}
	}

	SizeNode *inRangeWidget = 0;
	if ((int)mInRangeWidgets.size() > 0)
	{
		_SortInRangePickWidget();
		inRangeWidget = *(mInRangeWidgets.begin());
	}

	if (inRangeWidget && inRangeWidget->IsEnable())
	{
		inRangeWidget->OnWidgetPicked(inputData);
	}

	if (inRangeWidget && inRangeWidget->IsEnable())
	{
		mPickedWidget = inRangeWidget;
	}
	else
	{
		mPickedWidget = 0;
	}
}
//----------------------------------------------------------------------------
SizeNode *Canvas::GetPickedWidget()
{
	return mPickedWidget;
}
//----------------------------------------------------------------------------
APoint Canvas::_PreScreenPosToLogicPos(const APoint &inputPos)
{
	APoint logicPos = inputPos;
	const Rectf &screenRect = GetScreenRect();
	const Rectf &worldRect = GetWorldRect();

	float leftPercent = (logicPos.X() - screenRect.Left) / screenRect.Width();
	float upPercent = (logicPos.Z() - screenRect.Bottom) / screenRect.Height();

	logicPos.X() = worldRect.Left + leftPercent * worldRect.Width();
	logicPos.Z() = worldRect.Bottom + upPercent * worldRect.Height();

	return logicPos;
}
//----------------------------------------------------------------------------
APoint Canvas::_PreLogicPosToCameraPos(const APoint &logicPos, 
	float worldRectLeft, float worldRectBottom, float worldRectWidth,
	float worldRectHeight)
{
	if (!mUICameraNode)
		return logicPos;

	APoint pos = logicPos;

	const APoint &camWorldPos =
		mUICameraNode->WorldTransform.GetTranslate();
	float camRMax = mUICameraNode->GetCamera()->GetRMax();
	float camUMax = mUICameraNode->GetCamera()->GetUMax();

	float left = camWorldPos.X() - camRMax;
	float bottom = camWorldPos.Z() - camUMax;

	pos.X() = left + ((logicPos.X() - worldRectLeft) / worldRectWidth)
		* camRMax * 2.0f;
	pos.Z() = bottom + ((logicPos.Z() - worldRectBottom) / worldRectHeight)
		* camUMax * 2.0f;
	
	return pos;
}
//----------------------------------------------------------------------------
void Canvas::PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas)
{
	if (!IsShow())
		return;

	if (!IsDoPick())
		return;

	if (!IsActivated())
		return;

	CanvasInputData data = inputData;

	APoint screenPos = data.ScreenPos;
	APoint viewPortPos = _PreScreenPosToLogicPos(screenPos);

	float worldRectLeft = GetWorldRect().Left;
	float worldRectBottom = GetWorldRect().Bottom;
	float worldRectWidth = GetWorldRect().Width();
	float worldRectHeight = GetWorldRect().Height();

	APoint logicPos = viewPortPos;
	if (mIsPickPosRecal)
	{
		logicPos = WorldPosToViewPortPos(viewPortPos);
	}

	APoint camLogicPos = _PreLogicPosToCameraPos(logicPos, worldRectLeft,
		worldRectBottom, worldRectWidth, worldRectHeight);
	camLogicPos.X() = (float)((int)camLogicPos.X());
	camLogicPos.Y() = (float)((int)camLogicPos.Y());
	camLogicPos.Z() = (float)((int)camLogicPos.Z());

	CanvasInputData data1;
	data1.ScreenPos = screenPos;
	data1.LogicPos = logicPos;
	data1.CameraLogicPos = camLogicPos;
	data1.Wheel = data.Wheel;

	bool isPosInSizeRange = false;
	if (mIsPickPosRecal)
	{
		isPosInSizeRange = data1.LogicPos.X() >= 0.0f && data1.LogicPos.X() <= GetSize().Width&&
			data1.LogicPos.Z() >= 0.0f && data1.LogicPos.Z() <= GetSize().Height;
	}
	else
	{
		isPosInSizeRange = _IsInRect(data1.LogicPos);
	}

	if (isPosInSizeRange)
	{
		OnSizeNodePicked(data);

		if (mIsWidget && this!=canvas)
			canvas->_AddInRangePickWidget(this, data1);
	}
	else
	{
		OnSizeNodeNotPicked(data1);
	}
}
//----------------------------------------------------------------------------
void Canvas::OnWidgetPicked(const CanvasInputData &inputData)
{
	if (!IsActivated())
		return;

	if (!IsShow())
		return;

	if (!IsEnable())
		return;

	if (!IsDoPick())
		return;

	CanvasInputData data = inputData;

	APoint screenPos = data.ScreenPos;
	APoint viewPortPos = _PreScreenPosToLogicPos(screenPos);

	float worldRectLeft = GetWorldRect().Left;
	float worldRectBottom = GetWorldRect().Bottom;
	float worldRectWidth = GetWorldRect().Width();
	float worldRectHeight = GetWorldRect().Height();

	APoint logicPos = viewPortPos;
	if (mIsPickPosRecal)
	{
		logicPos = WorldPosToViewPortPos(viewPortPos);
	}

	APoint camLogicPos = _PreLogicPosToCameraPos(logicPos, worldRectLeft,
		worldRectBottom, worldRectWidth, worldRectHeight);
	camLogicPos.X() = (float)((int)camLogicPos.X());
	camLogicPos.Y() = (float)((int)camLogicPos.Y());
	camLogicPos.Z() = (float)((int)camLogicPos.Z());
	
	PickInputData data1;
	data1.TouchID = inputData.TouchID;
	data1.ScreenPos = screenPos;
	data1.LogicPos = logicPos;
	data1.CameraLogicPos = camLogicPos;
	data1.MoveDelta = data.MoveDelta;
	data1.Wheel = data.Wheel;

	if (UIPT_PRESSED == data.PickType)
	{
		if (CanvasInputData::MT_LEFT == data.TheMouseTag)
		{
			OnLeftDown(data1);
		}
		else if (CanvasInputData::MT_RIGHT == data.TheMouseTag)
		{
			OnRightDown(data1);
		}
		else if (CanvasInputData::MT_MIDDLE == data.TheMouseTag)
		{
			OnMiddleDown(data1);
		}
	}
	else if (UIPT_RELEASED == data.PickType)
	{
		if (CanvasInputData::MT_LEFT == data.TheMouseTag)
		{
			OnLeftUp(data1);
		}
		else if (CanvasInputData::MT_RIGHT == data.TheMouseTag)
		{
			OnRightUp(data1);
		}
		else if (CanvasInputData::MT_MIDDLE == data.TheMouseTag)
		{
			OnMiddleUp(data1);
		}
	}
	else if (UIPT_DOUBLE_PRESSED == data.PickType)
	{
		if (CanvasInputData::MT_LEFT == data.TheMouseTag)
		{
			OnLeftDClick(data1);
		}
	}
	else if (UIPT_WHELLED == data.PickType)
	{
		OnMouseWheel(data1);
	}
	else if (UIPT_MOVED == data.PickType)
	{
		OnMotion(data1);
	}
}
//----------------------------------------------------------------------------
void Canvas::OnSizeNodePicked(const CanvasInputData &inputData)
{
	if (!IsEntered())
	{
		OnEnter();
	}

	SizeNode::OnSizeNodePicked(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnSizeNodeNotPicked(const CanvasInputData &inputData)
{
	if (IsEntered())
	{
		OnLeave();
	}

	SizeNode::OnSizeNodeNotPicked(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnLeftDown(const PickInputData &data)
{
	mIsPressed = true;
	mIsLeftPressed = true;
	mIsMoved = false;
	mMoveDelta = AVector::ZERO;

	mPressedPos = data.LogicPos;
	mLeftPressedPos = data.LogicPos;

	mCurPickPos = mPressedPos;
	mLastPickPos = mCurPickPos;

	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.TheMouseTag = CanvasInputData::MT_LEFT;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.PickType = UIPT_PRESSED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnLeftUp(const PickInputData &data)
{
	mIsPressed = false;
	mIsLeftPressed = false;
	mIsMoved = false;

	mReleasedPos = data.LogicPos;
	mLeftReleasedPos = data.LogicPos;

	mCurPickPos = mReleasedPos;
	mLastPickPos = mCurPickPos;

	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.TheMouseTag = CanvasInputData::MT_LEFT;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.PickType = UIPT_RELEASED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnLeftDClick(const PickInputData &data)
{
	mIsPressed = false;
	mIsLeftPressed = false;
	mIsMoved = false;

	mReleasedPos = data.LogicPos;
	mLeftReleasedPos = data.LogicPos;

	mCurPickPos = mReleasedPos;
	mLastPickPos = mCurPickPos;

	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.TheMouseTag = CanvasInputData::MT_LEFT;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.PickType = UIPT_DOUBLE_PRESSED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnMiddleDown(const PickInputData &data)
{
	mIsPressed = true;
	mIsMiddlePressed = true;
	mIsMoved = false;
	mMoveDelta = AVector::ZERO;

	mPressedPos = data.LogicPos;
	mMiddlePressedPos = data.LogicPos;

	mCurPickPos = data.LogicPos;
	mLastPickPos = mCurPickPos;

	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.TheMouseTag = CanvasInputData::MT_MIDDLE;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.PickType = UIPT_PRESSED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnMiddleUp(const PickInputData &data)
{
	mIsPressed = false;
	mIsMiddlePressed = false;
	mIsMoved = false;

	mReleasedPos = data.LogicPos;
	mMiddleReleasedPos = data.LogicPos;

	mCurPickPos = data.LogicPos;
	mLastPickPos = mCurPickPos;

	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.TheMouseTag = CanvasInputData::MT_MIDDLE;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.PickType = UIPT_RELEASED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnMouseWheel(const PickInputData &data)
{
	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.TheMouseTag = CanvasInputData::MT_MIDDLE;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.Wheel = data.Wheel;
	inputData.PickType = UIPT_WHELLED;

	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnRightDown(const PickInputData &data)
{
	mIsPressed = true;
	mIsRightPressed = true;
	mIsMoved = false;
	mMoveDelta = AVector::ZERO;

	mPressedPos = data.LogicPos;
	mRightPressedPos = data.LogicPos;

	mCurPickPos = data.LogicPos;
	mLastPickPos = mCurPickPos;

	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.TheMouseTag = CanvasInputData::MT_RIGHT;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.PickType = UIPT_PRESSED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnRightUp(const PickInputData &data)
{
	mIsPressed = false;
	mIsRightPressed = false;
	mIsMoved = false;

	mReleasedPos = data.LogicPos;
	mRightReleasedPos = data.LogicPos;

	mCurPickPos = data.LogicPos;
	mLastPickPos = mCurPickPos;

	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.TheMouseTag = CanvasInputData::MT_RIGHT;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.PickType = UIPT_RELEASED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnMotion(const PickInputData &data)
{
	mCurPickPos = data.LogicPos;
	mMoveDelta = mCurPickPos - mLastPickPos;
	mLastPickPos = mCurPickPos;

	mIsMoved = true;

	CanvasInputData inputData;
	inputData.TouchID = data.TouchID;
	inputData.ScreenPos = data.ScreenPos;
	inputData.LogicPos = data.LogicPos;
	inputData.CameraLogicPos = data.CameraLogicPos;
	inputData.MoveDelta = mMoveDelta;
	inputData.PickType = UIPT_MOVED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void Canvas::OnEnter()
{
	mIsEntered = true;
}
//----------------------------------------------------------------------------
void Canvas::OnLeave()
{
	mIsEntered = false;
}
//----------------------------------------------------------------------------
std::pair<float, float> Canvas::CalPixelToWorld(Camera *camera)
{
	Rectf viewPort = GetScreenRect();

	std::pair<float, float> pixelToWorld;

	if (camera)
	{
		if (camera->IsPerspective())
		{
			float rMin = camera->GetRMin();
			float uMin = camera->GetUMin();
			float viewPortWidth = viewPort.Width();
			float viewPortHeight = viewPort.Height();

			float worldW = 2.0f * -rMin;
			float worldH = 2.0f * -uMin;

			worldW *= 10.0f;
			worldH *= 10.0f;

			pixelToWorld.first = worldW / (float)viewPortWidth;
			pixelToWorld.second = worldH / (float)viewPortHeight;
		}
		else
		{
			float rMin = camera->GetRMin();
			float uMin = camera->GetUMin();
			float viewPortWidth = viewPort.Width();
			float viewPortHeight = viewPort.Height();

			float worldW = 2.0f * -rMin;
			float worldH = 2.0f * -uMin;

			worldW *= 1.0f;
			worldH *= 1.0f;

			pixelToWorld.first = worldW / (float)viewPortWidth;
			pixelToWorld.second = worldH / (float)viewPortHeight;
		}

	}

	mPixelToWorld = pixelToWorld;

	return mPixelToWorld;
}
//----------------------------------------------------------------------------
void Canvas::_AddInRangePickWidget(SizeNode *widget,
	const CanvasInputData &inputData)
{
	PX2_UNUSED(inputData);

	mInRangeWidgets.push_back(widget);
}
//----------------------------------------------------------------------------
void Canvas::_ClearInRangePickWidget()
{
	mPickedWidget = 0;
	mInRangeWidgets.clear();
}
//----------------------------------------------------------------------------
bool _UIFrameFrontThan(const Node *frame0,
	const Node *frame1)
{
	if (frame0->IsDoPick() && !frame1->IsDoPick())
	{
		return true;
	}
	else if (!frame0->IsDoPick() && frame1->IsDoPick())
	{
		return false;
	}

	float posY0 = frame0->WorldTransform.GetTranslate().Y();
	float posY1 = frame1->WorldTransform.GetTranslate().Y();

	if (posY0 == posY1)
	{
		return frame0 < frame1;
	}
	else
	{
		return posY0 < posY1;
	}
}
//----------------------------------------------------------------------------
void Canvas::_SortInRangePickWidget()
{
	mInRangeWidgets.sort(_UIFrameFrontThan);
}
//----------------------------------------------------------------------------
void Canvas::ClearDebugLine()
{
	mDebugPoly->SetNumSegments(0);
}
//----------------------------------------------------------------------------
void Canvas::AddDebugLine(const APoint &fromPos, const APoint &toPos,
	const Float4 &color)
{
	int maxSeg = mDebugPoly->GetMaxNumSegments();
	int numPoly = mDebugPoly->GetNumSegments();

	if (numPoly < maxSeg)
	{
		VertexBufferAccessor vba(mDebugPoly);

		int index = numPoly * 2;
		vba.Position<Float3>(index) = fromPos;
		vba.Color<Float4>(0, index) = color;
		vba.Position<Float3>(index + 1) = toPos;
		vba.Color<Float4>(0, index + 1) = color;

		mDebugPoly->SetNumSegments(numPoly + 1);
	}
	else {
		assertion(false, "not enoughSeg!");
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Canvas::Canvas(LoadConstructor value) :
SizeNode(value),
mRenderNode(0),
mIsRenderNodeUpdate(false),
mRenderWindow(0),
mIsMain(false),
mOverCamera(0),
mIsUICameraAutoAdjust(true),
mClearStencil(0),
mClearFlagColor(false),
mClearFlagDepth(false),
mClearFlagStencil(false),
mIsOverWireframe(false),
mIsPickPosRecal(false),
mIsPressed(false),
mIsEntered(false),
mIsLeftPressed(false),
mIsRightPressed(false),
mIsMiddlePressed(false),
mIsMoved(false),
mClearDepth(1.0f),
mPickedWidget(0)
{
	SetColor(Float3::WHITE);
	SetAlpha(1.0f);
	SetBrightness(1.0f);
	SetColorSelfCtrled(true);
	SetAlphaSelfCtrled(true);
	SetBrightnessSelfCtrled(true);

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);
	EnableAnchorLayout(true);

	// ui
#if defined(_WIN32) || defined(WIN32)
	mMoveAdjugeParam = 5.0f;
#else
	mMoveAdjugeParam = 64.0f;
#endif
	mMoveAdjugeParamSquare = mMoveAdjugeParam * mMoveAdjugeParam;
}
//----------------------------------------------------------------------------
void Canvas::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	SizeNode::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mCanvasRenderBind);

	source.ReadPointer(mUICameraNode);

	source.ReadBool(mClearFlagColor);
	source.ReadBool(mClearFlagDepth);
	source.ReadBool(mClearFlagStencil);
	source.ReadAggregate(mClearColor);
	source.Read(mClearDepth);
	source.Read(mClearStencil);

	source.ReadBool(mIsOverWireframe);
	source.ReadPointer(mOverrideWireProperty);

	PX2_END_DEBUG_STREAM_LOAD(Canvas, source);
}
//----------------------------------------------------------------------------
void Canvas::Link(InStream& source)
{
	SizeNode::Link(source);

	source.ResolveLink(mCanvasRenderBind);

	source.ResolveLink(mUICameraNode);

	source.ResolveLink(mOverrideWireProperty);
}
//----------------------------------------------------------------------------
void Canvas::PostLink()
{
	SizeNode::PostLink();

	VertexFormat *vfPoly = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);
	mDebugPolyVB = new0 VertexBuffer(20000, vfPoly->GetStride(), Buffer::BU_DYNAMIC);
	mDebugPoly = new0 Polysegment(vfPoly, mDebugPolyVB, false);
	mDebugPoly->SetNumSegments(0);
	MaterialInstance *mi = VertexColor4Material::CreateUniqueInstance();
	mDebugPoly->SetMaterialInstance(mi);

	VertexFormat *vFormat = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT1);
	VertexBufferPtr vb = new0 VertexBuffer(5120, vFormat->GetStride(), Buffer::BU_DYNAMIC);
	IndexBufferPtr ib = new0 IndexBuffer(10240, 2, Buffer::BU_DYNAMIC);
	mUIShareMesh = new0 TriMesh(vFormat, vb, ib);
	mUIShareMesh->SetColor(Float3::WHITE);
	mUIShareMesh->Culling = Movable::CULL_NEVER;
	mUIShareMesh->SetRenderLayer(Renderable::RL_UI);
	mUIShareMesh->GetShine()->Emissive = Float4::WHITE;
	mUIShareMesh->SetName("ShareMesh");

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
bool Canvas::Register(OutStream& target) const
{
	if (SizeNode::Register(target))
	{
		target.Register(mCanvasRenderBind);

		target.Register(mUICameraNode);

		target.Register(mOverrideWireProperty);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Canvas::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	SizeNode::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mCanvasRenderBind);

	target.WritePointer(mUICameraNode);

	target.WriteBool(mClearFlagColor);
	target.WriteBool(mClearFlagDepth);
	target.WriteBool(mClearFlagStencil);
	target.WriteAggregate(mClearColor);
	target.Write(mClearDepth);
	target.Write(mClearStencil);

	target.WriteBool(mIsOverWireframe);
	target.WritePointer(mOverrideWireProperty);

	PX2_END_DEBUG_STREAM_SAVE(Canvas, target);
}
//----------------------------------------------------------------------------
int Canvas::GetStreamingSize(Stream &stream) const
{
	int size = SizeNode::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mCanvasRenderBind);

	size += PX2_POINTERSIZE(mUICameraNode);

	size += PX2_BOOLSIZE(mClearFlagColor);
	size += PX2_BOOLSIZE(mClearFlagDepth);
	size += PX2_BOOLSIZE(mClearFlagStencil);
	size += sizeof(mClearColor);
	size += sizeof(mClearDepth);
	size += sizeof(mClearStencil);

	size += PX2_BOOLSIZE(mIsOverWireframe);
	size += PX2_POINTERSIZE(mOverrideWireProperty);

	return size;
}
//----------------------------------------------------------------------------