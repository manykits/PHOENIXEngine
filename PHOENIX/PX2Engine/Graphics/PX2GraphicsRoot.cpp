// PX2GraphicsRoot.cpp

#include "PX2GraphicsRoot.hpp"
#include "PX2Environment.hpp"
#include "PX2StringHelp.hpp"
#include "PX2FString.hpp"
#include "PX2Renderer.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2LightAmbientConstant.hpp"
#include "PX2LightDiffuseConstant.hpp"
#include "PX2LightSpecularConstant.hpp"
#include "PX2LightAttenuationConstant.hpp"
#include "PX2LightModelDVectorConstant.hpp"
#include "PX2LightWorldDVectorConstant.hpp"
#include "PX2MaterialManager.hpp"
#include "PX2TriMesh.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2Time.hpp"
#include "PX2VBIBManager.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2InterpCurveUniformScaleCtrl.hpp"
using namespace PX2;

const std::string GraphicsRoot::sEmptyResPath = "EmptyResPath";
const std::string GraphicsRoot::sTerResPath = "TerResPath";
//----------------------------------------------------------------------------
GraphicsRoot::GraphicsRoot ()
{
	mIsInEditor = false;
	mIsUIShareDraw = false;
	mPlayType = PT_NONE;

	MaterialManager *mi = new0 MaterialManager();
	PX2_UNUSED(mi);

	mCurEnvirParamController = new0 EnvirParamController();
}
//----------------------------------------------------------------------------
GraphicsRoot::~GraphicsRoot ()
{
	MaterialManager *mi = MaterialManager::GetSingletonPtr();
	if (mi)
	{
		mi->Terminate();

		delete0(mi);
		MaterialManager::Set(0);
	}
}
//----------------------------------------------------------------------------
bool GraphicsRoot::Initlize ()
{
	InitTerm::ExecuteInitializers();

	// Camera
	Camera::DepthType dt = Camera::PM_DEPTH_ZERO_TO_ONE;
#if defined (PX2_USE_DX9)
	// DirectX使用深度在区间[0,1]内的矩阵。
	dt = Camera::PM_DEPTH_ZERO_TO_ONE;
#endif

#if defined(PX2_USE_OPENGL) || defined(PX2_USE_OPENGLES)
	// OpenGL使用深度在区间[-1,1]内的矩阵。
	dt = Camera::PM_DEPTH_MINUS_ONE_TO_ONE;
#endif
	Camera::SetDefaultDepthType(dt);

	PX2_MATERIALMAN.Initlize();

	// create help meshs
	VertexFormat *vf = GetVertexFormat(GraphicsRoot::VFT_PC);
	StandardMesh stdMesh(vf);

	mTriMeshXY = stdMesh.Rectangle(4, 4, 20000.0f, 20000.0f);
	mTriMeshXY->WorldBoundIsCurrent = true;
	mTriMeshXY->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);
	mTriMeshXY->SetMaterialInstance(VertexColor4Material
		::CreateUniqueInstance());
	mTriMeshXY->GetMaterialInstance()->GetMaterial()->GetWireProperty(0, 0)
		->Enabled = true;
	mTriMeshXY->Update(Time::GetTimeInSeconds(), false);

	Transform trans;
	trans.SetRotate(Matrix3f().MakeEulerZYX(0.0f, 0.0f, Mathf::HALF_PI));
	stdMesh.SetTransform(trans);
	mTriMeshXZ = stdMesh.Rectangle(4, 4, 20000.0f, 20000.0f);
	mTriMeshXZ->WorldBoundIsCurrent = true;

	mTriMeshXZ->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);
	mTriMeshXZ->SetMaterialInstance(VertexColor4Material
		::CreateUniqueInstance());
	mTriMeshXZ->Update(Time::GetTimeInSeconds(), false);

	Transform trans1;
	trans1.SetRotate(Matrix3f().MakeEulerZYX(0.0f, Mathf::HALF_PI, 0.0f));
	stdMesh.SetTransform(trans1);
	mTriMeshYZ = stdMesh.Rectangle(4, 4, 20000.0f, 20000.0f);
	mTriMeshYZ->WorldBoundIsCurrent = true;
	mTriMeshYZ->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);
	mTriMeshYZ->SetMaterialInstance(VertexColor4Material
		::CreateUniqueInstance());
	mTriMeshYZ->Update(Time::GetTimeInSeconds(), false);

	mMainRenderWindow = new0 RenderWindow();
	mMainRenderWindow->SetMain(true);
	mMainRenderWindow->SetName("Main");
	mMainRenderWindow->SetFloat(false);
	mMainRenderWindow->SetScreenSize(Sizef(800.0f, 600.0f));
	AddRenderWindow("Main", mMainRenderWindow);

	VBIBManager *man = new0 VBIBManager();
	PX2_UNUSED(man);

	return true;
}
//-----------------------------------------------------------------------------
bool GraphicsRoot::Terminate ()
{
	VBIBManager *vbibMan = VBIBManager::GetSingletonPtr();
	if (vbibMan)
	{
		delete0(vbibMan);
		VBIBManager::Set(0);
	}

	mRenderWindows.clear();
	mMainRenderWindow = 0;

	mCurEnvirParamController = 0;

	mTriMeshXY = 0;
	mTriMeshXZ = 0;
	mTriMeshYZ = 0;

	PX2_MATERIALMAN.Terminate();

	mCreatedVFs.clear();

	Environment::RemoveAllDirectories();

	InitTerm::ExecuteTerminators();

	return true;
}
//----------------------------------------------------------------------------
void GraphicsRoot::Update(double appSeconds, double elapsedSeconds)
{
	std::map<std::string, RenderWindowPtr>::iterator it = 
		mRenderWindows.begin();
	for (; it != mRenderWindows.end(); it++)
	{
		RenderWindow *rw = it->second;
		if (rw)
		{
			rw->Update(appSeconds, elapsedSeconds);
		}
	}
}
//----------------------------------------------------------------------------
void GraphicsRoot::Draw()
{
	std::map<std::string, RenderWindowPtr>::iterator it =
		mRenderWindows.begin();
	for (; it != mRenderWindows.end(); it++)
	{
		RenderWindow *rw = it->second;
		if (rw)
		{
			rw->ComputeVisibleSet();
			rw->Draw();
		}
	}
}
//----------------------------------------------------------------------------
void GraphicsRoot::SetInEditor(bool isInEditor)
{
	mIsInEditor = isInEditor;
	mIsUIShareDraw = !mIsInEditor;
}
//----------------------------------------------------------------------------
void GraphicsRoot::SetUIShareDraw(bool shareDraw)
{
	mIsUIShareDraw = shareDraw;
}
//----------------------------------------------------------------------------
void GraphicsRoot::SendGeneralEvent(const std::string &eventDataStr0,
	float timeDelay)
{
	Event *ent = PX2_CREATEEVENTEX(GraphicsES, GeneralString);
	ent->SetData<std::string>(eventDataStr0);
	ent->SetDataStr0(eventDataStr0);
	ent->SetTimeDelay(timeDelay);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void GraphicsRoot::SendGeneralEvent(const std::string &eventDataStr0,
	const std::string &eventDataStr1, float timeDelay)
{
	Event *ent = PX2_CREATEEVENTEX(GraphicsES, GeneralString);
	ent->SetData<std::string>(eventDataStr0);
	ent->SetDataStr0(eventDataStr0);
	ent->SetDataStr1(eventDataStr1);
	ent->SetTimeDelay(timeDelay);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
bool GraphicsRoot::AddRenderWindow(const std::string &name, RenderWindow *rw)
{
	if (IsHasRenderWindow(name))
	{
		assertion(false, "RenderWindow %s already exist.", name.c_str());
		return false;
	}

	mRenderWindows[name] = rw;

	if ("Main" == name)
		mMainRenderWindow = rw;

	return true;
}
//----------------------------------------------------------------------------
bool GraphicsRoot::IsHasRenderWindow(const std::string &name)
{
	return mRenderWindows.find(name) != mRenderWindows.end();
}
//----------------------------------------------------------------------------
bool GraphicsRoot::RemoveRenderWindow(const std::string &name)
{
	if (!IsHasRenderWindow(name))
		return false;

	mRenderWindows.erase(name);

	if ("Main" == name)
		mMainRenderWindow = 0;

	return true;
}
//----------------------------------------------------------------------------
RenderWindow *GraphicsRoot::GetRenderWindow(const std::string &name)
{
	if (!IsHasRenderWindow(name))
		return 0;

	return mRenderWindows[name];
}
//----------------------------------------------------------------------------
VertexFormat *GraphicsRoot::GetVertexFormat(VertexFormatType type)
{
	std::map<VertexFormatType, VertexFormatPtr >::iterator it =
		mCreatedVFs.find(type);

	if (it != mCreatedVFs.end())
	{
		return it->second;
	}
	else
	{
		VertexFormat *vf = 0;
		if (VFT_P == type)
		{
			vf = VertexFormat::Create(1,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0);
		}
		else if (VFT_PC == type)
		{
			vf = VertexFormat::Create(2,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0);
		}
		else if (VFT_PT1 == type)
		{
			vf = VertexFormat::Create(2,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
		}
		else if (VFT_PT2 == type)
		{
			vf = VertexFormat::Create(3,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
		}
		else if (VFT_PCT1 == type)
		{
			vf = VertexFormat::Create(3,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
		}
		else if (VFT_PCT2 == type)
		{
			vf = VertexFormat::Create(4,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
		}
		else if (VFT_PN == type)
		{
			vf = VertexFormat::Create(2,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0);
		}
		else if (VFT_PNT1 == type)
		{
			vf = VertexFormat::Create(3,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
		}
		else if (VFT_PNT2 == type)
		{
			vf = VertexFormat::Create(4,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
		}
		else if (VFT_PCN == type)
		{
			vf = VertexFormat::Create(3,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0,
				VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0);
		}
		else if (VFT_PCNT1 == type)
		{
			vf = VertexFormat::Create(4,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0,
				VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
		}
		else if (VFT_PCNT2 == type)
		{
			vf = VertexFormat::Create(5,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0,
				VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
		}
		else if (VFT_PNT1TB == type)
		{
			vf = VertexFormat::Create(5,
				VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0,
				VertexFormat::AU_TANGENT, VertexFormat::AT_FLOAT3, 0,
				VertexFormat::AU_BINORMAL, VertexFormat::AT_FLOAT3, 0);
		}

		mCreatedVFs[type] = vf;

		return vf;
	}
}
//----------------------------------------------------------------------------
ObjectLoadFun GraphicsRoot::msUserLoadFun = 0;
//----------------------------------------------------------------------------
void GraphicsRoot::SetUserLoadFun(ObjectLoadFun userLoadFun)
{
	msUserLoadFun = userLoadFun;
}
//----------------------------------------------------------------------------
ObjectLoadFun1 GraphicsRoot::msUserLoadFun1 = 0;
void GraphicsRoot::SetUserLoadFun1(ObjectLoadFun1 userLoadFun)
{
	msUserLoadFun1 = userLoadFun;
}
//----------------------------------------------------------------------------
BufferLoadFun GraphicsRoot::msBufferLoadFun = 0;
//----------------------------------------------------------------------------
void GraphicsRoot::SetBufferLoadFun(BufferLoadFun bufferLoadFun)
{
	msBufferLoadFun = bufferLoadFun;
}
//----------------------------------------------------------------------------
void GraphicsRoot::SetAddTexPackFun(AddTexPackFun fun)
{
	msAddTexPackFun = fun;
}
//----------------------------------------------------------------------------
AddTexPackFun GraphicsRoot::msAddTexPackFun = 0;
//----------------------------------------------------------------------------
void GraphicsRoot::SetGetTexPackFun(GetTexPackFun fun)
{
	msGetTexPackFun = fun;
}
//----------------------------------------------------------------------------
GetTexPackFun GraphicsRoot::msGetTexPackFun = 0;
//----------------------------------------------------------------------------
void GraphicsRoot::SetGetTexPackElementFun(GetTexPackElementFun fun)
{
	msGetTexPackElementFun = fun;
}
//----------------------------------------------------------------------------
GetTexPackElementFun GraphicsRoot::msGetTexPackElementFun = 0;
//----------------------------------------------------------------------------
const std::string *GraphicsRoot::GetShaderStr(const char *filename)
{
	std::map<FString, std::string>::iterator it = mShadersMap.find(filename);
	if (it != mShadersMap.end())
	{
		return &(it->second);
	}
	else
	{
		int bufferSize = 0;
		char *buffer = 0;
		PX2_GR.msBufferLoadFun(filename, bufferSize, buffer);
		if (0 != bufferSize && buffer)
		{
			std::string str(buffer, bufferSize);
			mShadersMap[filename] = str;
			return &(mShadersMap[filename]);
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
void GraphicsRoot::LogInfo(const std::string &infoStr)
{
	Event *ent = PX2_CREATEEVENTEX(GraphicsES, Info);
	ent->SetDataStr0(infoStr);
	ent->SetData(infoStr);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void GraphicsRoot::PlayScale(Controlledable *contrable)
{
	auto ctrlScalePlay = DynamicCast<InterpCurveUniformScaleController>(
		contrable->GetControllerByName("CtrlScalePlay"));
	if (0 == ctrlScalePlay)
	{
		ctrlScalePlay = InterpCurveUniformScaleController::New("CtrlScalePlay");
		contrable->AttachController(ctrlScalePlay);
		ctrlScalePlay->Clear();
		ctrlScalePlay->AddPoint(0.0f, 1.0f, ICM_CURVE_AUTO);
		ctrlScalePlay->AddPoint(0.1f, 0.8f, ICM_CURVE_AUTO);
		contrable->AttachController(ctrlScalePlay);
	}
	ctrlScalePlay->ResetPlay();

	auto ctrlScaleStop = DynamicCast<InterpCurveUniformScaleController>(
		contrable->GetControllerByName("CtrlScaleStop"));
	if (0 == ctrlScaleStop)
	{
		ctrlScaleStop = InterpCurveUniformScaleController::New("CtrlScaleStop");
		contrable->AttachController(ctrlScaleStop);
		ctrlScaleStop->Clear();
		ctrlScaleStop->AddPoint(0.0f, 0.8f, ICM_CURVE_AUTO);
		ctrlScaleStop->AddPoint(0.1f, 1.0f, ICM_CURVE_AUTO);
		contrable->AttachController(ctrlScaleStop);
	}
	ctrlScaleStop->Pause();
}
//----------------------------------------------------------------------------
void GraphicsRoot::PlayNormal(Controlledable *contrable)
{
	auto ctrlScalePlay = DynamicCast<InterpCurveUniformScaleController>(
		contrable->GetControllerByName("CtrlScalePlay"));
	if (0 != ctrlScalePlay)
	{
		ctrlScalePlay->Pause();
	}

	auto ctrlScaleStop = DynamicCast<InterpCurveUniformScaleController>(
		contrable->GetControllerByName("CtrlScaleStop"));
	if (0 != ctrlScaleStop)
	{
		ctrlScaleStop->ResetPlay();
	}
}
//----------------------------------------------------------------------------