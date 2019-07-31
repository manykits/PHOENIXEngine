// PX2AIAgentBase.cpp

#include "PX2AIAgentBase.hpp"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "PX2BtPhysicsWorld.hpp"
#include "PX2AIAgentUtilities.hpp"
#include "PX2BtPhysicsUtilities.hpp"
#include "PX2AIAgentWorld.hpp"
#include "PX2Robot.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, Controller, AIAgentBase);
PX2_IMPLEMENT_STREAM(AIAgentBase);
PX2_IMPLEMENT_FACTORY(AIAgentBase);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, AIAgentBase);
//----------------------------------------------------------------------------
const float AIAgentBase::DEFAULT_AGENT_MASS = 90.7f;  // kilograms (200 lbs)
const float AIAgentBase::DEFAULT_AGENT_RADIUS = 0.1f;  // meters (1.97 feet)
//----------------------------------------------------------------------------
AIAgentBase::AIAgentBase(Node *node, AgentType at) :
mNode(node),
mAgentType(at),
mAgentWorld(0),
mRigidBody(0),
mMass(DEFAULT_AGENT_MASS),
mIsMassZeroAvoid(false),
mRadius(DEFAULT_AGENT_RADIUS),
mRobot(0),
mIsUsePhysics(false),
mIsTag(false)
{

}
//----------------------------------------------------------------------------
AIAgentBase::~AIAgentBase()
{
	if (mRigidBody)
	{
		PhysicsUtilities::DeleteRigidBody(mRigidBody);
		mRigidBody = 0;
	}
}
//----------------------------------------------------------------------------
void AIAgentBase::Enable(bool enable)
{
	Controller::Enable(enable);

	if (mRigidBody)
	{
		mRigidBody->activate(enable);
	}
}
//----------------------------------------------------------------------------
void AIAgentBase::UsePhysics(bool usePhysics)
{
	mIsUsePhysics = usePhysics;
}
//----------------------------------------------------------------------------
bool AIAgentBase::IsUsePhysics() const
{
	return mIsUsePhysics;
}
//----------------------------------------------------------------------------
void AIAgentBase::SetMass(float mass)
{
	if (mRigidBody)
	{
		PhysicsUtilities::SetRigidBodyMass(
			mRigidBody,  Mathf::Max(0.0f, mass));
	}

	mMass = mass;
}
//----------------------------------------------------------------------------
float AIAgentBase::GetMass() const
{
	if (mRigidBody)
	{
		return PhysicsUtilities::GetRigidBodyMass(mRigidBody);
	}
	else
	{
		return mMass;
	}
}
//----------------------------------------------------------------------------
void AIAgentBase::SetMassZeroAvoid(bool avoid)
{
	mIsMassZeroAvoid = avoid;
}
//----------------------------------------------------------------------------
bool AIAgentBase::IsMassZeroAvoid() const
{
	return mIsMassZeroAvoid;
}
//----------------------------------------------------------------------------
void AIAgentBase::SetOrientation(const HQuaternion& quaternion)
{
	if (mRobot)
	{

	}
	else
	{
		PhysicsUtilities::SetRigidBodyOrientation(
			mRigidBody,
			PhysicsUtilities::QuaterionToBtQuaternion(quaternion));

		if (mRigidBody)
		{
			AIAgentUtilities::UpdateWorldTransTo(this);
		}
	}
}
//----------------------------------------------------------------------------
HQuaternion AIAgentBase::GetOrientation() const
{
	const btQuaternion& rotation =
		mRigidBody->getCenterOfMassTransform().getRotation();

	return HQuaternion(
		rotation.w(), rotation.x(), rotation.y(), rotation.z());
}
//----------------------------------------------------------------------------
void AIAgentBase::SetRotate(const HMatrix& mat)
{
	HQuaternion orie(mat);
	if (mRigidBody)
	{
		PhysicsUtilities::SetRigidBodyOrientation(
			mRigidBody, PhysicsUtilities::QuaterionToBtQuaternion(orie));
	}

	if (mNode)
	{
		AIAgentUtilities::SetWorldTansform(mNode, mat);
	}
}
//----------------------------------------------------------------------------
void AIAgentBase::SetPosition(const APoint& position)
{
	if (mRigidBody)
	{
		PhysicsUtilities::SetRigidBodyPosition(
			mRigidBody, PhysicsUtilities::Vector3ToBtVector3(position));
	}

	if (mNode)
	{
		AIAgentUtilities::SetWorldTansform(mNode, position);
	}
}
//----------------------------------------------------------------------------
APoint AIAgentBase::GetPosition() const
{
	if (mRobot)
	{
		APoint pos = mRobot->GetPosition();
		pos.Z() = 0.0f;
		return pos;
	}
	else
	{
		if (mRigidBody)
		{
			APoint bodyPos = PhysicsUtilities::BtVector3ToVector3(
				mRigidBody->getCenterOfMassPosition());
			bodyPos.Z() = 0.0f;
			return bodyPos;
		}
		else if (mNode)
		{
			APoint pos = mNode->WorldTransform.GetTranslate();
			pos.Z() = 0.0f;
			return pos;
		}
	}

	return APoint::ORIGIN;
}
//----------------------------------------------------------------------------
void AIAgentBase::SetRadius(float radius)
{
	mRadius = Mathf::Max(0.0f, radius);
}
//----------------------------------------------------------------------------
float AIAgentBase::GetRadius() const
{
	return mRadius;
}
//----------------------------------------------------------------------------
void AIAgentBase::SetRigidBody(btRigidBody* rigidBody)
{
	mRigidBody = rigidBody;

	if (mRigidBody)
	{
		mRigidBody->setUserPointer(this);
	}

	if (AT_AGENT == mAgentType)
		_RefreshRigidValue();
}
//----------------------------------------------------------------------------
void AIAgentBase::_RefreshRigidValue()
{
	SetMass(mMass);
}
//----------------------------------------------------------------------------
btRigidBody* AIAgentBase::GetRigidBody()
{
	return mRigidBody;
}
//----------------------------------------------------------------------------
const btRigidBody* AIAgentBase::GetRigidBody() const
{
	return mRigidBody;
}
//----------------------------------------------------------------------------
void AIAgentBase::SetNode(Node *node)
{
	mNode = node;
}
//----------------------------------------------------------------------------
void AIAgentBase::SetAgentWorld(AIAgentWorld *agentWorld)
{
	mAgentWorld = agentWorld;
}
//----------------------------------------------------------------------------
AIAgentWorld* AIAgentBase::GetAIAgentWorld()
{
	return mAgentWorld;
}
//----------------------------------------------------------------------------
const AIAgentWorld* AIAgentBase::GetAIAgentWorld() const
{
	return mAgentWorld;
}
//----------------------------------------------------------------------------
Node *AIAgentBase::GetNode()
{
	return mNode;
}
//----------------------------------------------------------------------------
const Node *AIAgentBase::GetNode() const
{
	return mNode;
}
//----------------------------------------------------------------------------
void AIAgentBase::_InitUpdate(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	Movable *mov = DynamicCast<Movable>(GetControlledable());
	if (mov)
	{
		APoint pos = mov->WorldTransform.GetTranslate();
		SetPosition(pos);

		HMatrix rotMat = mov->WorldTransform.GetRotate();
		HQuaternion quat;
		quat.FromRotationMatrix(rotMat);
		SetOrientation(quat);
	}
}
//----------------------------------------------------------------------------
void AIAgentBase::_Update(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);


	if (mRobot)
	{
		APoint pos = mRobot->GetPosition();
		AVector dir = mRobot->GetDirection();
		dir.Normalize();
		AVector right = dir.Cross(AVector::UNIT_Z);
		right.Normalize();
		AVector up = AVector::UNIT_Z;
		HMatrix mat = HMatrix(right, dir, up, APoint::ORIGIN, true);

		Node* node = GetNode();
		AIAgentUtilities::SetWorldTansform(node, pos, mat);
	}
	else
	{
		AIAgentUtilities::UpdateWorldTransTo(this);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void AIAgentBase::RegistProperties()
{
	Controller::RegistProperties();

	AddPropertyClass("AIAgentBase");

	std::vector<std::string> atTypes;
	atTypes.push_back("AT_AGENT");
	atTypes.push_back("AT_OBJCT");
	AddPropertyEnum("AgentType", (int)mAgentType, atTypes, false);

	AddProperty("Mass", Object::PT_FLOAT, mMass);
	AddProperty("Radius", Object::PT_FLOAT, mRadius);
}
//----------------------------------------------------------------------------
void AIAgentBase::OnPropertyChanged(const PropertyObject &obj)
{
	Controller::OnPropertyChanged(obj);

	if (obj.Name == "Mass")
	{
		SetMass(PX2_ANY_AS(obj.Data, float));
	}
	else if (obj.Name == "Radius")
	{
		SetRadius(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
AIAgentBase::AIAgentBase(LoadConstructor value) :
Controller(value),
mAgentWorld(0),
mRigidBody(0),
mNode(0),
mRobot(0),
mIsTag(false)
{
}
//----------------------------------------------------------------------------
void AIAgentBase::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mAgentType);
	source.Read(mMass);
	source.Read(mRadius);

	PX2_END_DEBUG_STREAM_LOAD(AIAgentBase, source);
}
//----------------------------------------------------------------------------
void AIAgentBase::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void AIAgentBase::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool AIAgentBase::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AIAgentBase::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mAgentType);
	target.Write(mMass);
	target.Write(mRadius);

	PX2_END_DEBUG_STREAM_SAVE(AIAgentBase, target);
}
//----------------------------------------------------------------------------
int AIAgentBase::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mAgentType);
	size += sizeof(mMass);
	size += sizeof(mRadius);

	return size;
}
//----------------------------------------------------------------------------
