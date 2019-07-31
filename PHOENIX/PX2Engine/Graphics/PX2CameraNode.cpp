// PX2CameraNode.cpp

#include "PX2CameraNode.hpp"
#include "PX2GraphicsEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, CameraNode);
PX2_IMPLEMENT_STREAM(CameraNode);
PX2_IMPLEMENT_FACTORY(CameraNode);

//----------------------------------------------------------------------------
CameraNode::CameraNode (Camera* camera)
:
mCamera(camera),
mEnableUpdate(true)
{
	SetName("CameraNode");

	if (mCamera)
	{
		LocalTransform.SetTranslate(mCamera->GetPosition());

		HMatrix rotate(mCamera->GetRVector(), mCamera->GetDVector(),
			mCamera->GetUVector(), APoint::ORIGIN, true);

		LocalTransform.SetRotate(rotate);
	}
}
//----------------------------------------------------------------------------
CameraNode::~CameraNode ()
{
}
//----------------------------------------------------------------------------
void CameraNode::SetCamera(Camera* camera, bool initFromCamera)
{
	if (mCamera)
	{
		mCamera->SetCameraNode(0);
	}

	mCamera = camera;

	if (mCamera && initFromCamera)
	{
		mCamera->SetCameraNode(this);

		LocalTransform.SetTranslate(mCamera->GetPosition());

		HMatrix rotate(mCamera->GetRVector(), mCamera->GetDVector(),
			mCamera->GetUVector(), APoint::ORIGIN, true);

		LocalTransform.SetRotate(rotate);

		Update();
	}
}
//----------------------------------------------------------------------------
void CameraNode::UpdateWorldData(double applicationTime, double elapsedTime)
{
	Node::UpdateWorldData(applicationTime, elapsedTime);

	if (mCamera && mEnableUpdate)
	{
		APoint camPosition = WorldTransform.GetTranslate();
		AVector camDVector, camUVector, camRVector;
		WorldTransform.GetRotate().GetColumn(0, camRVector); // x
		WorldTransform.GetRotate().GetColumn(1, camDVector); // y
		WorldTransform.GetRotate().GetColumn(2, camUVector); // z
		mCamera->SetFrame(camPosition, camDVector, camUVector, camRVector);
	}
}
//----------------------------------------------------------------------------
void CameraNode::Enable(bool enable)
{
	Node::Enable(enable);

	if (mCamera)
	{
		mCamera->Enable(enable);
	}
}
//----------------------------------------------------------------------------
void CameraNode::LookDir(const AVector &dir, const AVector &up0)
{
	AVector right = dir.UnitCross(up0);
	AVector up = right.UnitCross(dir);

	LocalTransform.SetRotate(HMatrix(right, dir, up, APoint::ORIGIN, true));
}
//----------------------------------------------------------------------------
void CameraNode::LookAt(const APoint &pos, const AVector &up)
{
	PX2_UNUSED(up);

	APoint localPos = LocalTransform.GetTranslate();
	AVector dir = pos - localPos;

	float length = dir.Normalize();
	if (length > 0.0f)
	{
		LookDir(dir);
	}
}
//----------------------------------------------------------------------------
void CameraNode::LookAt(const Movable *mov, const AVector &up)
{
	if (!mov) return;
	LookAt(mov->WorldTransform.GetTranslate(), up);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void CameraNode::RegistProperties ()
{
	Node::RegistProperties();

	AddPropertyClass("CameraNode");
	AddProperty("IsEnableUpdate", PT_BOOL, IsEnableUpdate());

	AddProperty("IsPerspective", PT_BOOL, mCamera->IsPerspective());

	float dMin = 0.0f;
	float dMax = 0.0f;
	float uMin = 0.0f;
	float uMax = 0.0f;
	float rMin = 0.0f;
	float rMax = 0.0f;
	mCamera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	AddProperty("DMin", PT_FLOAT, dMin);
	AddProperty("DMax", PT_FLOAT, dMax);
	AddProperty("UMin", PT_FLOAT, uMin);
	AddProperty("UMax", PT_FLOAT, uMax);
	AddProperty("RMin", PT_FLOAT, rMin);
	AddProperty("RMax", PT_FLOAT, rMax);

	float upFov = 0.0f;
	float aspRatio = 0.0f;
	mCamera->GetFrustum(upFov, aspRatio, dMin, dMax);
	AddProperty("UpFovDegress", PT_FLOAT, upFov);
	AddProperty("AspectRatio", PT_FLOAT, aspRatio);
}
//----------------------------------------------------------------------------
void CameraNode::OnPropertyChanged (const PropertyObject &obj)
{
	Node::OnPropertyChanged(obj);

	float dMin = 0.0f;
	float dMax = 0.0f;
	float uMin = 0.0f;
	float uMax = 0.0f;
	float rMin = 0.0f;
	float rMax = 0.0f;
	mCamera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);

	float upFov = 0.0f;
	float aspRatio = 0.0f;
	mCamera->GetFrustum(upFov, aspRatio, dMin, dMax);

	if ("IsEnableUpdate" == obj.Name)
	{
		EnableUpdate(PX2_ANY_AS(obj.Data, bool));
	}
	if ("IsPerspective" == obj.Name)
	{
		mCamera->SetPerspective(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("DMin" == obj.Name)
	{
		dMin = PX2_ANY_AS(obj.Data, float);
		mCamera->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	}
	else if ("DMax" == obj.Name)
	{
		dMax = PX2_ANY_AS(obj.Data, float);
		mCamera->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	}
	else if ("UMin" == obj.Name)
	{
		uMin = PX2_ANY_AS(obj.Data, float);
		mCamera->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	}
	else if ("UMax" == obj.Name)
	{
		uMax = PX2_ANY_AS(obj.Data, float);
		mCamera->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	}
	else if ("RMin" == obj.Name)
	{
		rMin = PX2_ANY_AS(obj.Data, float);
		mCamera->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	}
	else if ("RMax" == obj.Name)
	{
		rMax = PX2_ANY_AS(obj.Data, float);
		mCamera->SetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
	}
	else if ("UpFovDegress" == obj.Name)
	{
		float val = PX2_ANY_AS(obj.Data, float);
		mCamera->SetFrustum(val, aspRatio, dMin, dMax);
	}
	else if ("AspectRatio" == obj.Name)
	{
		float val = PX2_ANY_AS(obj.Data, float);
		mCamera->SetFrustum(upFov, val, dMin, dMax);
	}
	
	Event *ent = GraphicsES::CreateEventX(GraphicsES::CameraNodeChanged);
	ent->SetData<CameraNode*>(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Ãû³Æ
//----------------------------------------------------------------------------
Object* CameraNode::GetObjectByName (const std::string& name)
{
	Object* found = Node::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mCamera, name, found);
	return 0;
}
//----------------------------------------------------------------------------
void CameraNode::GetAllObjectsByName (const std::string& name,
									  std::vector<Object*>& objects)
{
	Node::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mCamera, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
CameraNode::CameraNode (LoadConstructor value)
:
Node(value),
mEnableUpdate(true)
{
}
//----------------------------------------------------------------------------
void CameraNode::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mCamera);
	source.ReadBool(mEnableUpdate);

	PX2_END_DEBUG_STREAM_LOAD(CameraNode, source);
}
//----------------------------------------------------------------------------
void CameraNode::Link (InStream& source)
{
	Node::Link(source);

	source.ResolveLink(mCamera);

	if (mCamera)
	{
		mCamera->SetCameraNode(this);
	}
}
//----------------------------------------------------------------------------
void CameraNode::PostLink ()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool CameraNode::Register (OutStream& target) const
{
	if (Node::Register(target))
	{
		target.Register(mCamera);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void CameraNode::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mCamera);
	target.WriteBool(mEnableUpdate);

	PX2_END_DEBUG_STREAM_SAVE(CameraNode, target);
}
//----------------------------------------------------------------------------
int CameraNode::GetStreamingSize (Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_POINTERSIZE(mCamera);
	size += PX2_BOOLSIZE(mEnableUpdate);
	return size;
}
//----------------------------------------------------------------------------