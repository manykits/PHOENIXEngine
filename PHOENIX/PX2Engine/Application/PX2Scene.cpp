// PX2Scene.cpp

#include "PX2Scene.hpp"
#include "PX2Creater.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
#include "PX2Float2.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2AIAgentWorld.hpp"
#include "PX2Application.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, Scene);
PX2_IMPLEMENT_STREAM(Scene);
PX2_IMPLEMENT_FACTORY(Scene);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, Scene);

//----------------------------------------------------------------------------
int Scene::sNextIDs[] = {0,0,0,0,0,0,0,0,0,0};
//----------------------------------------------------------------------------
Scene::Scene()
{
	ComeInEventWorld();

	SetName("Scene");

	mEnvirParam = new0 EnvirParamController();
	AttachController(mEnvirParam);
	mEnvirParam->SetPlaySelfCtrl(true);
	mEnvirParam->ResetPlay();

	mAmbientRegionController = new0 AmbientRegionController();
	AttachController(mAmbientRegionController);
	mAmbientRegionController->SetPlaySelfCtrl(true);
	mAmbientRegionController->ResetPlay();

	CameraNode *camNode = PX2_CREATER.CreateNode_Camera();
	AttachChild(camNode);
	camNode->SetName("MainCameraNode");
	camNode->GetCamera()->SetClearColor(Float4::WHITE);
	camNode->LocalTransform.SetTranslate(-10.0f, -10.0f, 10.0f);
	camNode->LookAt(APoint(0.0f, 0.0f, 0.0f));
	SetMainCameraNode(camNode);

	mPathRoot = new0 Node();
	AttachChild(mPathRoot);
	mPathRoot->SetName("Path");

	mAreaRoot = new0 Node();
	AttachChild(mAreaRoot);
	mAreaRoot->SetName("Area");

	mIsNeedReCalCameras = true;

	int nextID = NextID(0);
	SetID(nextID);

	mAIAgentWorld = new0 AIAgentWorld();
	AttachController(mAIAgentWorld);
	mAIAgentWorld->SetNode(this);
}
//----------------------------------------------------------------------------
Scene::~Scene()
{
	mAIAgentWorld->Clear();
	
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
int Scene::NextID(int solt)
{
	if (0 <= solt && solt < sNumIDGroup)
	{
		return sNextIDs[solt]++;
	}

	return 0;
}
//----------------------------------------------------------------------------
void Scene::SetTerrain(Terrain *terrain)
{
	mTerrain = terrain;
}
//----------------------------------------------------------------------------
Terrain *Scene::GetTerrain()
{
	return mTerrain;
}
//----------------------------------------------------------------------------
void Scene::SetMainCameraNode(CameraNode *camNode)
{
	mMainCameraNode = camNode;
}
//----------------------------------------------------------------------------
CameraNode *Scene::GetMainCameraNode()
{
	return mMainCameraNode;
}
//----------------------------------------------------------------------------
bool Scene::AddCamera(Camera *camera)
{
	if (!camera)
		return false;

	for (int i = 0; i < (int)mCameras.size(); i++)
	{
		if (mCameras[i] == camera)
			return false;
	}

	mCameras.push_back(camera);

	Event *event = PX2_CREATEEVENTEX(ProjectES_Internal,
		SceneUICamerasChanged);
	event->SetData<Object*>(this);
	PX2_EW.BroadcastingLocalEvent(event);

	return true;
}
//----------------------------------------------------------------------------
void Scene::ClearCameras()
{
	mCameras.clear();

	Event *event = PX2_CREATEEVENTEX(ProjectES_Internal, 
		SceneUICamerasChanged);
	event->SetData<Object*>(this);
	PX2_EW.BroadcastingLocalEvent(event);
}
//----------------------------------------------------------------------------
bool Scene::RemoveCamera(Camera *camera)
{
	if (!camera)
		return false;

	auto it = mCameras.begin();
	for (; it != mCameras.end(); it++)
	{
		if (camera == *it)
		{
			it = mCameras.erase(it);

			Event *event = PX2_CREATEEVENTEX(ProjectES_Internal, 
				SceneUICamerasChanged);
			event->SetData<Object*>(this);
			PX2_EW.BroadcastingLocalEvent(event);

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void Scene::_CollectCameras(Scene *scene, Node *node)
{
	Scene *sc = DynamicCast<Scene>(node);
	CameraNode *camNode = DynamicCast<CameraNode>(node);
	if (sc)
	{
		EnvirParamController *epc = sc->GetEnvirParamController();
		Projector *projector = epc->GetLight_Dir_Projector();
		if (projector)
		{
			scene->AddCamera(projector);
		}
	}
	else if (camNode)
	{
		Camera *camera = camNode->GetCamera();
		if (camera)
		{
			scene->AddCamera(camera);
		}
	}

	for (int i = 0; i < node->GetNumChildren(); i++)
	{
		Movable *mov = node->GetChild(i);
		Node *childNode = DynamicCast<Node>(mov);
		if (childNode)
		{
			_CollectCameras(scene, childNode);
		}
	}
}
//----------------------------------------------------------------------------
void Scene::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	Node::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsNeedReCalCameras)
	{
		ClearCameras();

		_CollectCameras(this, this);

		mIsNeedReCalCameras = false;
	}
}
//----------------------------------------------------------------------------
void Scene::OnEvent(Event *ent)
{
	if (GraphicsES::IsEqual(ent, GraphicsES::AddObject))
	{
		AddObjectData data = ent->GetData<AddObjectData>();
		CameraNode *camNode = DynamicCast<CameraNode>(data.Obj);
		Terrain *terrain = DynamicCast<Terrain>(data.Obj);
		if (camNode)
		{
			mIsNeedReCalCameras = true;
		}
		else if (terrain)
		{
			SetTerrain(terrain);
		}
	}
	else if (GraphicsES::IsEqual(ent, GraphicsES::RemoveObject))
	{
		Object *obj = ent->GetData<Object*>();
		CameraNode *camNode = DynamicCast<CameraNode>(obj);
		if (camNode)
		{
			mIsNeedReCalCameras = true;
		}
	}
	else if (ProjectES::IsEqual(ent, ProjectES::SceneCanvasSizeChanged))
	{
		Sizef size = ent->GetData<Sizef>();
		AdjustCameraFrustum(size);
	}
	else if (ProjectES::IsEqual(ent, ProjectES::Play))
	{
		ResetPlay();
	}
	else if (ProjectES::IsEqual(ent, ProjectES::Stop))
	{
		if (mAIAgentWorld)
		{
			Reset();
		}
	}
}
//----------------------------------------------------------------------------
void Scene::AdjustCameraFrustum(const Sizef &canvasSize)
{
	for (int i = 0; i < (int)mCameras.size(); i++)
	{
		Camera *cam = mCameras[i];
		if (cam->IsPerspective())
		{
			Projector *projector = DynamicCast<Projector>(cam);

			float upFovDegrees = 50.0f;
			float aspectRation = 1.0f;
			float dMin = 0.1f;
			float dMax = 1000.0f;
			if (cam->GetFrustum(upFovDegrees, aspectRation, dMin, dMax))
			{
				aspectRation = canvasSize.Width / canvasSize.Height;
				if (projector)
				{
					if (!mEnvirParam->IsShadowRenderTargetSizeSameWithCanvas())
						aspectRation = 1.0f;

					upFovDegrees = 50.0f;
				}

				cam->SetFrustum(upFovDegrees, aspectRation, dMin, dMax);
			}
			else
			{
				cam->SetFrustum(upFovDegrees, aspectRation, dMin, dMax);
			}
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Scene::RegistProperties()
{
	Node::RegistProperties();

	AddPropertyClass("Scene");
}
//----------------------------------------------------------------------------
void Scene::OnPropertyChanged(const PropertyObject &obj)
{
	Node::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Scene::Scene(LoadConstructor value) :
Node(value)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void Scene::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mEnvirParam);
	source.ReadPointer(mAmbientRegionController);
	source.ReadPointer(mMainCameraNode);

	source.ReadPointer(mPathRoot);
	source.ReadPointer(mAreaRoot);

	source.ReadPointer(mAIAgentWorld);

	PX2_END_DEBUG_STREAM_LOAD(Scene, source);
}
//----------------------------------------------------------------------------
void Scene::Link(InStream& source)
{
	Node::Link(source);

	if (mEnvirParam)
		source.ResolveLink(mEnvirParam);

	if (mAmbientRegionController)
		source.ResolveLink(mAmbientRegionController);

	if (mMainCameraNode)
		source.ResolveLink(mMainCameraNode);

	if (mPathRoot)
		source.ResolveLink(mPathRoot);

	if (mAreaRoot)
		source.ResolveLink(mAreaRoot);

	if (mAIAgentWorld)
		source.ResolveLink(mAIAgentWorld);
}
//----------------------------------------------------------------------------
void Scene::PostLink()
{
	Node::PostLink();

	if (mEnvirParam)
	{
		mEnvirParam->PostLink();
		mEnvirParam->ResetPlay();
	}

	if (mAmbientRegionController)
	{
		mAmbientRegionController->PostLink();
		mAmbientRegionController->ResetPlay();
	}

	if (mMainCameraNode)
		mMainCameraNode->PostLink();

	if (mPathRoot)
		mPathRoot->PostLink();

	if (mAreaRoot)
		mAreaRoot->PostLink();

	if (mAIAgentWorld)
	{
		mAIAgentWorld->PostLink();
		mAIAgentWorld->SetNode(this);
	}
}
//----------------------------------------------------------------------------
bool Scene::Register(OutStream& target) const
{
	if (Node::Register(target))
	{
		if (mEnvirParam)
		{
			mEnvirParam->Register(target);
		}

		if (mAmbientRegionController)
		{
			mAmbientRegionController->Register(target);
		}

		if (mMainCameraNode)
		{
			mMainCameraNode->Register(target);
		}

		if (mMainCameraNode)
			mMainCameraNode->Register(target);

		if (mPathRoot)
			mPathRoot->Register(target);

		if (mAreaRoot)
			mAreaRoot->Register(target);

		if (mAIAgentWorld)
			mAIAgentWorld->Register(target);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Scene::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mEnvirParam);
	target.WritePointer(mAmbientRegionController);
	target.WritePointer(mMainCameraNode);

	target.WritePointer(mPathRoot);
	target.WritePointer(mAreaRoot);

	target.WritePointer(mAIAgentWorld);

	PX2_END_DEBUG_STREAM_SAVE(Scene, target);
}
//----------------------------------------------------------------------------
int Scene::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mEnvirParam);
	size += PX2_POINTERSIZE(mAmbientRegionController);
	size += PX2_BOOLSIZE(mMainCameraNode);

	size += PX2_POINTERSIZE(mPathRoot);
	size += PX2_POINTERSIZE(mAreaRoot);

	size += PX2_POINTERSIZE(mAIAgentWorld);
	
	return size;
}
//----------------------------------------------------------------------------