// PX2AIAgentObject.cpp

#include "PX2AIAgentObject.hpp"
#include "PX2AIAgentWorld.hpp"
#include "PX2BtPhysicsUtilities.hpp"
#include "PX2AIAgentWorldUtilities.hpp"
#include "PX2AIAgentUtilities.hpp"
#include "PX2Node.hpp"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "PX2BtPhysicsWorld.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, AIAgentBase, AIAgentObject);
PX2_IMPLEMENT_STREAM(AIAgentObject);
PX2_IMPLEMENT_FACTORY(AIAgentObject);
PX2_IMPLEMENT_DEFAULT_NAMES(AIAgentBase, AIAgentObject);
//----------------------------------------------------------------------------
AIAgentObject::AIAgentObject(Node *node) :
AIAgentBase(node, AT_OBJCT),
mLastWorldPosZ(0.0f),
mPhysicsShapeType(PST_MESH)
{
	if (!IsRegistedToScriptSystem())
		RegistToScriptSystem();

	SetName("AIAgentObject");
}
//----------------------------------------------------------------------------
AIAgentObject::~AIAgentObject()
{
}
//----------------------------------------------------------------------------
AIAgentObject::PhysicsShapeType AIAgentObject::GetPhysicsShapeType() const
{
	return mPhysicsShapeType;
}
//----------------------------------------------------------------------------
void AIAgentObject::InitializeInfinitePlane(const AVector &normal, 
	float originOffset)
{
	AIAgentUtilities::CreateRigidBodyInfinitePlane(this, normal, originOffset);

	mPhysicsShapeType = AIAgentObject::PST_INFINITEPLANE;
}
//----------------------------------------------------------------------------
void AIAgentObject::InitializeMesh(Movable *mov)
{
	AIAgentUtilities::CreateRigidBodyMesh(this, mov);

	mPhysicsShapeType = AIAgentObject::PST_MESH;
}
//----------------------------------------------------------------------------
void AIAgentObject::SetOrientation(const HQuaternion& quaternion)
{
	PhysicsUtilities::SetRigidBodyOrientation(
		mRigidBody,
		btQuaternion(quaternion.X(), quaternion.Y(), quaternion.Z(), 
		quaternion.W()));

	AIAgentUtilities::UpdateWorldTransTo(this);
}
//----------------------------------------------------------------------------
HQuaternion AIAgentObject::GetOrientation() const
{
	const btQuaternion& rotation =
		mRigidBody->getCenterOfMassTransform().getRotation();

	return HQuaternion(
		rotation.w(), rotation.x(), rotation.y(), rotation.z());
}
//----------------------------------------------------------------------------
float AIAgentObject::GetRigidBodyRadius() const
{
	return PhysicsUtilities::GetRigidBodyRadius(mRigidBody);
}
//----------------------------------------------------------------------------
void AIAgentObject::_Update(double applicationTime, double elapsedTime)
{
	AIAgentBase::_Update(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void AIAgentObject::RegistProperties()
{
	AIAgentBase::RegistProperties();

	AddPropertyClass("AIAgentObject");

	std::vector<std::string> pstTypes;
	pstTypes.push_back("PST_INFINITEPLANE");
	pstTypes.push_back("PST_MESH");
	AddPropertyEnum("PhysicsShapeType", (int)mPhysicsShapeType, pstTypes,
		false);
}
//----------------------------------------------------------------------------
void AIAgentObject::OnPropertyChanged(const PropertyObject &obj)
{
	AIAgentBase::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
AIAgentObject::AIAgentObject(LoadConstructor value) :
AIAgentBase(value),
mLastWorldPosZ(0.0f)
{
	if (!IsRegistedToScriptSystem())
		RegistToScriptSystem();
}
//----------------------------------------------------------------------------
void AIAgentObject::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	AIAgentBase::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mPhysicsShapeType);

	PX2_END_DEBUG_STREAM_LOAD(AIAgentObject, source);
}
//----------------------------------------------------------------------------
void AIAgentObject::Link(InStream& source)
{
	AIAgentBase::Link(source);
}
//----------------------------------------------------------------------------
void AIAgentObject::PostLink()
{
	AIAgentBase::PostLink();
}
//----------------------------------------------------------------------------
bool AIAgentObject::Register(OutStream& target) const
{
	if (AIAgentBase::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AIAgentObject::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	AIAgentBase::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mPhysicsShapeType);

	PX2_END_DEBUG_STREAM_SAVE(AIAgentObject, target);
}
//----------------------------------------------------------------------------
int AIAgentObject::GetStreamingSize(Stream &stream) const
{
	int size = AIAgentBase::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mPhysicsShapeType);

	return size;
}
//----------------------------------------------------------------------------
