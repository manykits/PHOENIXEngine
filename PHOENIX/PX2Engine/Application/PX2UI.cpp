// PX2UI.cpp

#include "PX2UI.hpp"
#include "PX2Creater.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
#include "PX2Float2.hpp"
#include "PX2ProjectEvent.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI_V(PX2, UIFrame, UI, 1);
PX2_IMPLEMENT_STREAM(UI);
PX2_IMPLEMENT_FACTORY(UI);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UI);

//----------------------------------------------------------------------------
float UI::msUICameraY = -2000.0f;
//----------------------------------------------------------------------------
UI::UI()
{
	SetName("UI");

	ComeInEventWorld();

	_CreateAddEnvirController();

	mDefaultCamera = new0 Camera(false);
	mDefaultCamera->SetFrame(APoint(0.0f, msUICameraY, 0.0f), AVector::UNIT_Y,
		AVector::UNIT_Z, AVector::UNIT_X);
	mDefaultCamera->SetClearFlag(false, true, true);
	mDefaultCamera->SetClearColor(Float4::RED);
	mDefaultCamera->SetFrustum(0.1f, Mathf::FAbs(msUICameraY) + 1000.0f,
		-400.0f, 400.0f, -400.0f, 400.0f);

	mDefaultCameraNode = new0 CameraNode();
	mDefaultCameraNode->SetCamera(mDefaultCamera);
	AttachChild(mDefaultCameraNode);
	mDefaultCameraNode->SetName("DefaultCameraNode");
	mDefaultCameraNode->LocalTransform.SetTranslate(
		0.0f, msUICameraY, 0.0f);

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);
	EnableAnchorLayout(true);

	SetColorSelfCtrled(true);
	SetAlphaSelfCtrled(true);
	SetBrightnessSelfCtrled(true);
}
//----------------------------------------------------------------------------
UI::~UI()
{
	RemoveAllCameras();
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
bool UI::AddCamera(Camera *camera)
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
bool UI::RemoveCamera(Camera *camera)
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
void UI::RemoveAllCameras()
{
	mCameras.clear();

	Event *event = PX2_CREATEEVENTEX(ProjectES_Internal,
		SceneUICamerasChanged);
	event->SetData<Object*>(this);
	PX2_EW.BroadcastingLocalEvent(event);
}
//----------------------------------------------------------------------------
Camera *UI::GetFirstCamera()
{
	if ((int)mCameras.size() > 0)
	{
		return mCameras[0];
	}

	return 0;
}
//----------------------------------------------------------------------------
void UI::OnSizeChanged()
{
	_UpdateCamera();

	UIFrame::OnSizeChanged();

	Event *ent = ProjectES_Internal::CreateEventX(
		ProjectES_Internal::UISizeChanged);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void UI::UpdateLeftBottomCornerOffset(Movable *parent)
{
	UIFrame::UpdateLeftBottomCornerOffset(parent);

	_UpdateCamera();
}
//----------------------------------------------------------------------------
void UI::_UpdateCamera()
{
	float helfWidth = mSize.Width / 2.0f;
	float helfHeight = mSize.Height / 2.0f;

	if (mDefaultCamera)
	{
		mDefaultCamera->SetFrustum(0.1f, Mathf::FAbs(msUICameraY) + 1000.0f,
			-helfHeight, helfHeight, -helfWidth, helfWidth);
	}

	if (mDefaultCameraNode)
	{
		mDefaultCameraNode->LocalTransform.SetTranslate(
			helfWidth + LeftBottomCornerOffset.X(), msUICameraY,
			helfHeight + LeftBottomCornerOffset.Z());
	}
}
//----------------------------------------------------------------------------
void UI::OnEvent(Event *ent)
{
	if (GraphicsES::IsEqual(ent, GraphicsES::AddObject))
	{
		AddObjectData data = ent->GetData<AddObjectData>();
		CameraNode *camNode = DynamicCast<CameraNode>(data.Obj);
		if (camNode)
		{
			UI *ui = camNode->GetFirstParentDerivedFromType<UI>();
			if (ui)
			{
				ui->AddCamera(camNode->GetCamera());
			}
		}
	}
	else if (GraphicsES::IsEqual(ent, GraphicsES::RemoveObject))
	{
		Object *obj = ent->GetData<Object*>();
		CameraNode *camNode = DynamicCast<CameraNode>(obj);
		if (camNode)
		{
			UI *ui = camNode->GetFirstParentDerivedFromType<UI>();
			if (ui)
			{
				ui->RemoveCamera(camNode->GetCamera());
			}
		}
	}
}
//----------------------------------------------------------------------------
void UI::_CreateAddEnvirController()
{
	mEnvirParamController = new0 EnvirParamController();
	AttachController(mEnvirParamController);
	Float4 fogParam = Float4(-100000.0f, 0.0f, 0.0f, 100001.0f);
	mEnvirParamController->SetFogParam(fogParam);
	mEnvirParamController->SetFogColorDist(Float4::BLUE);
	mEnvirParamController->SetFogColorHeight(Float4::RED);
	mEnvirParamController->ResetPlay();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UI::RegistProperties()
{
	UIFrame::RegistProperties();

	AddPropertyClass("UI");
}
//----------------------------------------------------------------------------
void UI::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UI::UI(LoadConstructor value) :
UIFrame(value)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
void UI::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mDefaultCamera);
	source.ReadPointer(mDefaultCameraNode);

	int readedVersion = GetReadedVersion();
	if (1 <= readedVersion)
	{
		source.ReadPointer(mEnvirParamController);
	}

	PX2_END_DEBUG_STREAM_LOAD(UI, source);
}
//----------------------------------------------------------------------------
void UI::Link(InStream& source)
{
	UIFrame::Link(source);

	source.ResolveLink(mDefaultCamera);
	source.ResolveLink(mDefaultCameraNode);
	source.ResolveLink(mEnvirParamController);
}
//----------------------------------------------------------------------------
void UI::PostLink()
{
	UIFrame::PostLink();

	if (mDefaultCamera)
		mDefaultCamera->PostLink();

	if (mDefaultCameraNode)
		mDefaultCameraNode->PostLink();

	if (mEnvirParamController)
	{
		mEnvirParamController->PostLink();
	}
	else
	{
		_CreateAddEnvirController();
	}
}
//----------------------------------------------------------------------------
bool UI::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		if (mDefaultCamera)
		{
			target.Register(mDefaultCamera);
		}

		if (mDefaultCameraNode)
		{
			target.Register(mDefaultCameraNode);
		}

		if (mEnvirParamController)
		{
			target.Register(mEnvirParamController);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UI::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mDefaultCamera);
	target.WritePointer(mDefaultCameraNode);
	target.WritePointer(mEnvirParamController);

	PX2_END_DEBUG_STREAM_SAVE(UI, target);
}
//----------------------------------------------------------------------------
int UI::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mDefaultCamera);
	size += PX2_POINTERSIZE(mDefaultCameraNode);

	if (stream.IsIn())
	{
		int readedVersion = GetReadedVersion();
		if (1 <= readedVersion)
		{
			size += PX2_POINTERSIZE(mEnvirParamController);
		}
	}
	else
	{
		size += PX2_POINTERSIZE(mEnvirParamController);
	}

	return size;
}
//----------------------------------------------------------------------------