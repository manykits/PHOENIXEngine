// PX2EngineSceneCanvas.cpp

#include "PX2EngineSceneCanvas.hpp"
#include "PX2EngineUICanvas.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2ProjectEventData.hpp"
#include "PX2Project.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2VertexColor4Material.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Canvas, EngineSceneCanvas);
PX2_IMPLEMENT_STREAM(EngineSceneCanvas);
PX2_IMPLEMENT_FACTORY(EngineSceneCanvas);
PX2_IMPLEMENT_DEFAULT_NAMES(Canvas, EngineSceneCanvas);

//----------------------------------------------------------------------------
EngineSceneCanvas::EngineSceneCanvas() :
mIsShadowMapConfigChanged(true),
mIsBloomConfigChanged(true)
{
	mScreenCamera = new0 Camera(false);
	mScreenCamera->SetName("ScreenCamera");
	mScreenCamera->SetAxes(AVector::UNIT_Y, AVector::UNIT_Z, AVector::UNIT_X);

	mPicBox_Shadow = new0 UIPicBox();
	mPicBox_Shadow->SetSize(200.0f, 200.0f);
	mPicBox_Shadow->SetPivot(Float2::ZERO);
	std::string renderTag = Renderer::GetRenderTag();
	if ("OpenGLES" == renderTag || "OpenGL" == renderTag)
		mPicBox_Shadow->SetPicBoxType(UIPicBox::PBT_NORAML_UVREVERSE);

	SetPickPosRecal(true);
	SetName("EngineSceneCanvas");

	PX2_SC_LUA->SetUserTypePointer("PX2_ENGINESCENECANVAS", 
		"EngineSceneCanvas", this);

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
EngineSceneCanvas::~EngineSceneCanvas()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::OnEvent(Event *ent)
{
	Canvas::OnEvent(ent);

	if (ProjectES::IsEqual(ent, ProjectES::NewScene))
	{
		mIsShadowMapConfigChanged = true;
	}
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::OnSizeChanged()
{
	Canvas::OnSizeChanged();

	mIsBloomConfigChanged = true;

	Event *ent = ProjectES::CreateEventX(ProjectES::SceneCanvasSizeChanged);
	ent->SetData<Sizef>(mSize);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	ClearDebugLine();

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		Scene *scene = proj->GetScene();
		if (scene)
		{
			PX2_GR.SetCurEnvirParamController(
				scene->GetEnvirParamController());
			CameraNode *mainCameraNode = scene->GetMainCameraNode();
			PX2_GR.SetCurUpdateCamera(mainCameraNode->GetCamera());
		}
	}

	Canvas::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsShadowMapConfigChanged)
	{
		_UpdateShadowChanged();
	}

	if (mIsBloomConfigChanged)
	{
		mIsBloomConfigChanged = false;
		_UpdateBloomChanged();
	}
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::_UpdateShadowChanged()
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	Scene *scene = proj->GetScene();
	if (!scene) return;

	EnvirParamController *epc = scene->GetEnvirParamController();
	epc->SetLight_Dir_DepthTexture(0);

	if (epc->IsUseShadowMap())
	{
		Sizef rtSize = epc->GetShadowRenderTargetSize();
		if (epc->IsShadowRenderTargetSizeSameWithCanvas())
			rtSize = Sizef(mScreenRect.Width(), mScreenRect.Height());

		if (rtSize.Width > 0 && rtSize.Height > 0)
		{
			Texture::Format tformat = Texture::TF_A8R8G8B8;

			std::string renderTag = Renderer::GetRenderTag();
			if ("OpenGLES" == renderTag)
			{
				mRenderTarget_Shadow = new0 RenderTarget(1, tformat,
					(int)rtSize.Width, (int)rtSize.Height, false, true, true);
			}
			else
			{
				mRenderTarget_Shadow = new0 RenderTarget(1, tformat,
					(int)rtSize.Width, (int)rtSize.Height, false, true, false);
			}

			mMaterial_Shadow = new0 ShadowMap_Material();

			epc->SetLight_Dir_DepthTexture(mRenderTarget_Shadow->GetDepthStencilTexture());

			mPicBox_Shadow->SetTexture(mRenderTarget_Shadow->GetDepthStencilTexture());
			mPicBox_Shadow->Update(0.0f);
			mPicBox_Shadow->GetMaterialInstance()->GetMaterial()->GetAlphaProperty(0, 0)->BlendEnabled = false;

			mIsShadowMapConfigChanged = false;
		}
	}
	else
	{
		mRenderTarget_Shadow = 0;
		mPicBox_Shadow->SetTexture(0);

		mIsShadowMapConfigChanged = false;
	}
}
//----------------------------------------------------------------------------
float gaussianDistribution(float x, float y, float rho)
{
	float g = 1.0f / sqrtf(2.0f * Mathf::PI * rho * rho);
	g *= expf(-(x*x + y*y) / (2 * rho*rho));

	return g;
}
//----------------------------------------------------------------------------
void _SetSampleOffsetWeight(const Float2 &surfsize, Float4 *offsets, float fDeviation,
	float multiplier, bool isHor)
{
	int i = 0;

	offsets[0][2] = multiplier * gaussianDistribution(0, 0, fDeviation); // weight
	offsets[0][0] = 0.0f; // offset
	offsets[0][1] = 0.0f;

	for (i = 1; i < 8; i++)
	{
		offsets[i][2] = multiplier * gaussianDistribution((float)i, 0, fDeviation);

		if (isHor)
		{
			float tu = 1.0f / (float)surfsize[0];

			offsets[i][0] = i * tu;
			offsets[i][1] = 0.0f;
		}
		else
		{
			float tu = 1.0f / (float)surfsize[1];

			offsets[i][0] = 0.0f;
			offsets[i][1] = i * tu;
		}
	}

	// Mirror to the second half
	for (i = 8; i < 15; i++)
	{
		offsets[i][2] = offsets[i - 7][2];

		offsets[i][0] = -offsets[i - 7][0];
		offsets[i][1] = -offsets[i - 7][1];
	}
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::_UpdateBloomChanged()
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	Scene *scene = proj->GetScene();
	if (!scene) return;

	std::string renderTag = Renderer::GetRenderTag();
	EnvirParamController *epc = scene->GetEnvirParamController();

	if (epc->IsUseBloom())
	{
		float minSize = Mathf::Min(mSize.Width, mSize.Height);
		Sizef norMormalSize = mSize;
		Sizef otherSize = Sizef(minSize, minSize);

		Texture::Format tformat = Texture::TF_A8R8G8B8;

		mBloomRenderTarget_Normal = new0 RenderTarget(1, tformat,
			(int)norMormalSize.Width, (int)norMormalSize.Height, false, true, false);
		mBloomPicBox_Normal = new0 UIPicBox();
		mBloomPicBox_Normal->SetPivot(Float2::ZERO);
		if ("OpenGLES" == renderTag || "OpenGL" == renderTag) mBloomPicBox_Normal->SetPicBoxType(UIPicBox::PBT_NORAML_UVREVERSE);
		mBloomPicBox_Normal->SetSize(mSize);
		mBloomPicBox_Normal->Update(0.0f);
		mBloomPicBox_Normal->GetMaterialInstance()->GetMaterial()->GetAlphaProperty(0, 0)->BlendEnabled = false;

		MaterialInstancePtr blurMtlInstanceBloomBright = new0 MaterialInstance("Data/engine_mtls/bloom/bloom.px2obj", "bloom_bright", false);
		mBloomRenderTarget_Bright = new0 RenderTarget(1, tformat,
			(int)otherSize.Width, (int)otherSize.Height, false, false, false);
		mBloomPicBox_Bright = new0 UIPicBox();
		mBloomPicBox_Bright->SetPivot(Float2::ZERO);
		if ("OpenGLES" == renderTag || "OpenGL" == renderTag) mBloomPicBox_Bright->SetPicBoxType(UIPicBox::PBT_NORAML_UVREVERSE);
		mBloomPicBox_Bright->SetSize(mSize);
		mBloomPicBox_Bright->SetMaterialInstance(blurMtlInstanceBloomBright);
		mBloom_BrightParam = blurMtlInstanceBloomBright->GetPixelConstant(0, "BrightParam");
		mBloomPicBox_Bright->Update(0.0f);

		MaterialInstancePtr blurMtlInstanceH = new0 MaterialInstance("Data/engine_mtls/blur/blur.px2obj", "blur", false);
		mBloomRenderTarget_BlurH = new0 RenderTarget(1, tformat,
			(int)otherSize.Width, (int)otherSize.Height, false, false, false);
		mBloomPicBox_BlurH = new0 UIPicBox();
		mBloomPicBox_BlurH->SetPivot(Float2::ZERO);
		mBloomPicBox_BlurH->SetMaterialInstance(blurMtlInstanceH);
		if ("OpenGLES" == renderTag || "OpenGL" == renderTag) mBloomPicBox_BlurH->SetPicBoxType(UIPicBox::PBT_NORAML_UVREVERSE);
		mBloomPicBox_BlurH->SetSize(mSize);
		mBloomPicBox_BlurH->SetUVRepeat(Float2::UNIT);
		mBloomPicBox_BlurH->SetTexture("Data/engine/default.png");
		mBloomPicBox_BlurH->Update(0.0f);

		_SetSampleOffsetWeight(Float2(otherSize.Width, otherSize.Height), mBloomBlur_UVOffsets_H, epc->GetBloomBlurDeviation(), epc->GetBloomBlurWeight(), true);
		ShaderFloat *shaderFloatUVOffsets_H = mBloomPicBox_BlurH->GetMaterialInstance()->GetPixelConstant(0, "UVOffsets");
		shaderFloatUVOffsets_H->SetRegisters((const float*)mBloomBlur_UVOffsets_H);

		MaterialInstancePtr blurMtlInstanceV = new0 MaterialInstance("Data/engine_mtls/blur/blur.px2obj", "blur", false);
		mBloomRenderTarget_BlurV = new0 RenderTarget(1, tformat,
			(int)otherSize.Width, (int)otherSize.Height, false, false, false);
		mBloomPicBox_BlurV = new0 UIPicBox();
		mBloomPicBox_BlurV->SetPivot(Float2::ZERO);
		mBloomPicBox_BlurV->SetMaterialInstance(blurMtlInstanceV);
		if ("OpenGLES" == renderTag || "OpenGL" == renderTag) mBloomPicBox_BlurV->SetPicBoxType(UIPicBox::PBT_NORAML_UVREVERSE);
		mBloomPicBox_BlurV->SetSize(mSize);
		mBloomPicBox_BlurV->SetUVRepeat(Float2::UNIT);
		mBloomPicBox_BlurV->SetTexture("Data/engine/default.png");
		mBloomPicBox_BlurV->Update(0.0f);

		_SetSampleOffsetWeight(Float2(otherSize.Width, otherSize.Height), mBloomBlur_UVOffsets_V, epc->GetBloomBlurDeviation(), epc->GetBloomBlurWeight(), false);
		ShaderFloat *shaderFloatUVOffsets_V = mBloomPicBox_BlurV->GetMaterialInstance()->GetPixelConstant(0, "UVOffsets");
		shaderFloatUVOffsets_V->SetRegisters((const float*)mBloomBlur_UVOffsets_V);

		mBoom_MtlInstance = new0 MaterialInstance("Data/engine_mtls/bloom/bloom.px2obj", "bloom", false);
		mBloomPicBox_Final = new0 UIPicBox();
		mBloomPicBox_Final->SetPivot(Float2::ZERO);
		mBloomPicBox_Final->SetSize(mSize);
		if ("OpenGLES" == renderTag || "OpenGL" == renderTag) mBloomPicBox_Final->SetPicBoxType(UIPicBox::PBT_NORAML_UVREVERSE);
		mBloomPicBox_Final->SetMaterialInstance(mBoom_MtlInstance);
		mBloom_Param = mBoom_MtlInstance->GetPixelConstant(0, "BloomParam");
		mBloomPicBox_Final->Update(0.0f);

		_UpdateBloomParams();
	}
	else
	{
		mBloomRenderTarget_Normal = 0;
		mBloomPicBox_Normal = 0;

		mBloomRenderTarget_Bright = 0;
		mBloomPicBox_Bright = 0;
		mBloom_BrightParam = 0;

		mBloomRenderTarget_BlurH = 0;
		mBloomPicBox_BlurH = 0;
		mBloomRenderTarget_BlurV = 0;
		mBloomPicBox_BlurV = 0;

		mBloomPicBox_Final = 0;
		mBoom_MtlInstance = 0;
		mBloom_Param = 0;
	}
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::_UpdateBloomParams()
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	Scene *scene = proj->GetScene();
	if (!scene) return;

	EnvirParamController *epc = scene->GetEnvirParamController();

	if (mBloom_BrightParam)
	{
		mBloom_BrightParam->SetRegister(0, epc->GetBloomBrightParam());
	}

	float minSize = Mathf::Min(mSize.Width, mSize.Height);
	Sizef otherSize = Sizef(minSize, minSize);

	_SetSampleOffsetWeight(Float2(otherSize.Width, otherSize.Height), mBloomBlur_UVOffsets_H,
		epc->GetBloomBlurDeviation(),
		epc->GetBloomBlurWeight(), true);

	_SetSampleOffsetWeight(Float2(otherSize.Width, otherSize.Height), mBloomBlur_UVOffsets_V,
		epc->GetBloomBlurDeviation(),
		epc->GetBloomBlurWeight(), false);

	if (mBloomPicBox_BlurH)
	{
		ShaderFloat *shaderFloatUVOffsets_H = mBloomPicBox_BlurH->GetMaterialInstance()->GetPixelConstant(0, "UVOffsets");
		shaderFloatUVOffsets_H->SetRegisters((const float*)mBloomBlur_UVOffsets_H);
	}

	if (mBloomPicBox_BlurV)
	{
		ShaderFloat *shaderFloatUVOffsets_V = mBloomPicBox_BlurV->GetMaterialInstance()->GetPixelConstant(0, "UVOffsets");
		shaderFloatUVOffsets_V->SetRegisters((const float*)mBloomBlur_UVOffsets_V);
	}

	if (mBloom_Param)
		mBloom_Param->SetRegister(0, epc->GetBloomParam());
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::Draw(Renderer *renderer)
{
	CameraPtr senseMainCamera;

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		Scene *scene = proj->GetScene();
		if (scene)
		{
			PX2_GR.SetCurEnvirParamController(scene->GetEnvirParamController());
			senseMainCamera = scene->GetMainCameraNode()->GetCamera();
		}
	}

	Canvas::Draw(renderer);
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::SetShadowMapConfigChanged(bool changed)
{
	mIsShadowMapConfigChanged = changed;
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::SetBloomConfigChanged(bool changed)
{
	mIsBloomConfigChanged = changed;
}
//----------------------------------------------------------------------------
UIPicBox *EngineSceneCanvas::GetPicBox_Shadow()
{
	return mPicBox_Shadow;
}
//----------------------------------------------------------------------------
void _SetCameraF(Camera *camera, UIPicBox *uiPicBox)
{
	const APoint &screenPos = uiPicBox->WorldTransform.GetTranslate();
	const Float2 &anchor = uiPicBox->GetPivot();
	const Sizef &size = uiPicBox->GetSize();

	APoint cameraPos = screenPos;
	cameraPos.Y() -= 10.0f;

	float uMin = 0.0f;
	float uMax = 0.0f;
	float rMin = 0.0f;
	float rMax = 0.0f;

	rMin = anchor[0] * size.Width;
	rMax = (1.0f - anchor[0]) * size.Width;

	uMin = anchor[1] * size.Height;
	uMax = (1.0f - anchor[1]) * size.Height;

	camera->SetPosition(cameraPos);
	camera->SetFrustum(0.1f, 1000.0f, uMin, uMax, rMin, rMax);
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::UpdateLayout(Movable *parent)
{
	Canvas::UpdateLayout(parent);
	mIsShadowMapConfigChanged = true;
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::_DoClear(Renderer *renderer, Camera *camera)
{
	bool clearColor = false; bool clearDepth = false; bool clearStencil = false;
	camera->GetClearFlag(clearColor, clearDepth, clearStencil);
	_Clear(renderer,
		clearColor, camera->GetClearColor(),
		clearDepth, camera->GetClearDepth(),
		clearStencil, camera->GetClearStencil());
}
//----------------------------------------------------------------------------
void _DrawRenderTarget(Renderer *renderer, Camera *cam, RenderTarget *rtPicFrom, 
	RenderTarget *rt, UIPicBox *renderPicBox)
{
	renderPicBox->SetTexture(rtPicFrom->GetColorTexture(0));

	renderer->Enable(rt);
	renderer->InitRenderStates();

	renderer->ClearColorBuffer();
	renderer->ClearDepthBuffer();

	_SetCameraF(cam, renderPicBox);
	renderer->SetCamera(cam);
	renderer->Draw(renderPicBox);

	renderer->Disable(rt);
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::_Draw(Camera *camera, Renderer *renderer,
	Culler *culler)
{
	VisibleSet &visibleSet = culler->GetVisibleSet();

	CameraPtr beforeCamera = renderer->GetCamera();

	renderer->SetCamera(camera);
	renderer->InitRenderStates();

	// shadow
	EnvirParamController *evrParam = PX2_GR.GetCurEnvirParamController();
	
	bool isShadowPass = evrParam &&
		camera == evrParam->GetLight_Dir_Projector() &&
		mRenderTarget_Shadow;

	if (isShadowPass)
	{
		if (evrParam) evrParam->ComputeEnvironment(visibleSet);

		renderer->Enable(mRenderTarget_Shadow);
		float scale = 2.0f;
		float bias = 4.0f;
		evrParam->SetShadowMap_OffsetPropertyScale(scale);
		evrParam->SetShadowMap_OffsetPropertyBias(bias);
	}
	else
	{
		if (evrParam) evrParam->ComputeEnvironment(visibleSet);
	}

	if (isShadowPass)
	{
		// shadow pass

		_DoClear(renderer, camera);
		renderer->Draw(visibleSet, mMaterial_Shadow);
		evrParam->SetLight_Dir_DepthTexture(mRenderTarget_Shadow->GetDepthStencilTexture());
	}
	else
	{
		// bloom
		bool isBloom = evrParam && camera != evrParam->GetLight_Dir_Projector() && mBloomRenderTarget_Normal;
		if (isBloom)
		{
			// normal
			if (mBloomRenderTarget_Normal)
			{
				renderer->Enable(mBloomRenderTarget_Normal);
				renderer->InitRenderStates();

				renderer->SetCamera(camera);
				_DoClear(renderer, camera);

				renderer->Draw(visibleSet);

				renderer->Disable(mBloomRenderTarget_Normal);

				mBloomPicBox_Normal->SetTexture(mBloomRenderTarget_Normal->GetColorTexture(0));
			}

			_DrawRenderTarget(renderer, mScreenCamera, mBloomRenderTarget_Normal, mBloomRenderTarget_Bright, mBloomPicBox_Bright);
			_DrawRenderTarget(renderer, mScreenCamera, mBloomRenderTarget_Bright, mBloomRenderTarget_BlurH, mBloomPicBox_BlurH);
			_DrawRenderTarget(renderer, mScreenCamera, mBloomRenderTarget_BlurH, mBloomRenderTarget_BlurV, mBloomPicBox_BlurV);

			renderer->SetCamera(camera);
			renderer->InitRenderStates();
			_DoClear(renderer, camera);
			renderer->Draw(visibleSet);

			_SetCameraF(mScreenCamera, mBloomPicBox_BlurV);
			mBloomPicBox_BlurV->SetAlpha(0.5f);
			mBloomPicBox_BlurV->GetMaterialInstance()->GetMaterial()->GetAlphaProperty(0, 0)->BlendEnabled = true;
			mBloomPicBox_BlurV->GetMaterialInstance()->GetMaterial()->GetAlphaProperty(0, 0)->SrcBlend = AlphaProperty::SBM_SRC_ALPHA;
			mBloomPicBox_BlurV->GetMaterialInstance()->GetMaterial()->GetAlphaProperty(0, 0)->DstBlend = AlphaProperty::DBM_ONE;
			renderer->SetCamera(mScreenCamera);
			renderer->Draw(mBloomPicBox_BlurV);

			//mBoom_MtlInstance->SetPixelTexture(0, "SampleBase", mBloomRenderTarget_Normal->GetColorTexture(0));
			//mBoom_MtlInstance->SetPixelTexture(0, "SampleBloom", mBloomRenderTarget_BlurV->GetColorTexture(0));
			//_SetCameraF(mScreenCamera, mBloomPicBox_Final);
			//renderer->SetCamera(mScreenCamera);
			//renderer->Draw(mBloomPicBox_Final);
		}
		else
		{
			_DoClear(renderer, camera);
			renderer->Draw(visibleSet);

			int numSeg = mDebugPoly->GetNumSegments();
			if (numSeg > 0)
			{
				renderer->UpdateAll(mDebugPolyVB);
				renderer->Draw(mDebugPoly);
			}

			// cameradrawcallback
			CameraDrawCallback callback = camera->GetCameraDrawCallback();
			if (callback)
			{
				callback(renderer);
			}
		}
	}

	if (isShadowPass)
		renderer->Disable(mRenderTarget_Shadow);

	// draw depthtexture
	//_SetCameraF(mScreenCamera, mPicBox_Shadow);
	//renderer->SetCamera(mScreenCamera);
	//renderer->Draw(mPicBox_Shadow);

	renderer->SetCamera(beforeCamera);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EngineSceneCanvas::EngineSceneCanvas(LoadConstructor value) :
Canvas(value),
mIsShadowMapConfigChanged(true),
mIsBloomConfigChanged(true)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Canvas::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EngineSceneCanvas, source);
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::Link(InStream& source)
{
	Canvas::Link(source);
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::PostLink()
{
	Canvas::PostLink();
}
//----------------------------------------------------------------------------
bool EngineSceneCanvas::Register(OutStream& target) const
{
	if (Canvas::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EngineSceneCanvas::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Canvas::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EngineSceneCanvas, target);
}
//----------------------------------------------------------------------------
int EngineSceneCanvas::GetStreamingSize(Stream &stream) const
{
	int size = Canvas::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------