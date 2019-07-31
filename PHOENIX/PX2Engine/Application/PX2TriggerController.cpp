// PX2TriggerActor.cpp

#include "PX2TriggerController.hpp"
#include "PX2ContSphere3.hpp"
#include "PX2Node.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2Box3.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, TriggerController);
PX2_IMPLEMENT_STREAM(TriggerController);
PX2_IMPLEMENT_FACTORY(TriggerController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, TriggerController);

//----------------------------------------------------------------------------
TriggerController::TriggerController() :
mAreaType(AT_SPHERE),
mTrigerType(TT_ENTER_ONCE),
mIsTrigered(false)
{
	SetName("TriggerController");
}
//----------------------------------------------------------------------------
TriggerController::~TriggerController()
{
}
//----------------------------------------------------------------------------
void TriggerController::SetAreaType(AreaType type)
{
	mAreaType = type;

	Controlledable *controlledable = GetControlledable();
	if (!controlledable)
		return;

	Node *parentNode = DynamicCast<Node>(controlledable);
	parentNode->DetachChild(mAreaMovable);
	if (parentNode) return;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);
	if (AT_SPHERE == mAreaType)
	{
		StandardMesh stdMesh(vf);
		stdMesh.SetVertexColor(Float4(1.0f, 1.0f, 0.0f, 1.0f));
		TriMesh *mesh = stdMesh.Sphere(16, 16, 1.0f);
		VertexColor4MaterialPtr mtl = new0 VertexColor4Material();
		mtl->GetWireProperty(0, 0)->Enabled = true;
		mtl->GetCullProperty(0, 0)->Enabled = false;
		mesh->LocalTransform.SetUniformScale(0.5f);
		mesh->SetMaterialInstance(mtl->CreateInstance());
		mAreaMovable = mesh;
	}
	else if (AT_BOX == mAreaType)
	{
		StandardMesh stdMesh(vf);
		stdMesh.SetVertexColor(Float4(1.0f, 1.0f, 0.0f, 1.0f));
		TriMesh *mesh = stdMesh.Box(1, 1, 1);
		VertexColor4MaterialPtr mtl = new0 VertexColor4Material();
		mtl->GetWireProperty(0, 0)->Enabled = true;
		mtl->GetCullProperty(0, 0)->Enabled = false;
		mesh->LocalTransform.SetUniformScale(0.5f);
		mesh->SetMaterialInstance(mtl->CreateInstance());
		mAreaMovable = mesh;
	}

	parentNode->AttachChild(mAreaMovable);
}
//----------------------------------------------------------------------------
bool TriggerController::IsPointIn(const PX2::APoint &point) const
{
	Node *parentNode = DynamicCast<Node>(GetControlledable());
	if (parentNode) return false;

	Transform trans = parentNode->WorldTransform;
	APoint localPoint = trans.Inverse() * point;

	if (mAreaType == AT_SPHERE)
	{
		Sphere3f sphere;
		sphere.Radius = mAreaParam[0];

		if (InSphere<float>(localPoint, sphere))
			return true;
	}
	else if (mAreaType == AT_BOX)
	{
		Box3f box;
		box.Extent[0] = mAreaParam[0];
		box.Extent[1] = mAreaParam[1];
		box.Extent[2] = mAreaParam[2];

		if (localPoint.X() >= -box.Extent[0] &&
			localPoint.Y() >= -box.Extent[1] &&
			localPoint.Z() >= -box.Extent[2] &&
			localPoint.X() <= box.Extent[0] &&
			localPoint.Y() <= box.Extent[1] &&
			localPoint.Z() <= box.Extent[2])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TriggerController::SetControlledable(Controlledable* object)
{
	Controller::SetControlledable(object);

	SetAreaType(mAreaType);
}
//----------------------------------------------------------------------------
void TriggerController::Triger()
{
	mIsTrigered = true;
}
//----------------------------------------------------------------------------
void TriggerController::ResetTriger()
{
	mIsTrigered = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void TriggerController::RegistProperties()
{
	Controller::RegistProperties();

	AddPropertyClass("TriggerController");

	std::vector<std::string> areaTypes;
	areaTypes.push_back("AT_SPHERE");
	areaTypes.push_back("AT_BOX");
	AddPropertyEnum("AreaType", (int)mAreaType, areaTypes);

	std::vector<std::string> trigerTypes;
	trigerTypes.push_back("TT_ENTER_ONCE");
	trigerTypes.push_back("TT_ENTER_EVERYENTER");
	AddPropertyEnum("TrigerType", GetTrigerType(), trigerTypes);
}
//----------------------------------------------------------------------------
void TriggerController::OnPropertyChanged(const PropertyObject &obj)
{
	Controller::OnPropertyChanged(obj);

	if ("AreaType" == obj.Name)
	{
		SetAreaType((AreaType)PX2_ANY_AS(obj.Data, int));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
TriggerController::TriggerController(LoadConstructor value) :
Controller(value),
mTrigerType(TT_ENTER_ONCE),
mIsTrigered(false)
{
}
//----------------------------------------------------------------------------
void TriggerController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mAreaType);
	source.ReadAggregate(mAreaParam);
	source.ReadPointer(mAreaMovable);

	source.ReadEnum(mTrigerType);

	PX2_END_DEBUG_STREAM_LOAD(TriggerController, source);
}
//----------------------------------------------------------------------------
void TriggerController::Link(InStream& source)
{
	Controller::Link(source);

	if (mAreaMovable)
	{
		source.ResolveLink(mAreaMovable);
	}
}
//----------------------------------------------------------------------------
void TriggerController::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool TriggerController::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		if (mAreaMovable)
		{
			target.Register(mAreaMovable);
		}

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void TriggerController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mAreaType);
	target.WriteAggregate(mAreaParam);
	target.WritePointer(mAreaMovable);

	target.WriteEnum(mTrigerType);

	PX2_END_DEBUG_STREAM_SAVE(TriggerController, target);
}
//----------------------------------------------------------------------------
int TriggerController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mAreaType);
	size += sizeof(mAreaParam);
	size += PX2_POINTERSIZE(mAreaMovable);

	size += PX2_ENUMSIZE(mTrigerType);

	return size;
}
//----------------------------------------------------------------------------