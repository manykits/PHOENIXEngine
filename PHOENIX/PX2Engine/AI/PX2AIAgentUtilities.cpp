// PX2AIAgentUtilities.cpp

#include "PX2AIAgentUtilities.hpp"
#include "PX2AIAgent.hpp"
#include "PX2AIAgentWorld.hpp"
#include "PX2BtPhysicsWorld.hpp"
#include "PX2BtPhysicsUtilities.hpp"
#include "PX2Node.hpp"
#include "PX2AIAgentObject.hpp"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "Bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "Bullet/LinearMath/btDefaultMotionState.h"
using namespace PX2;

//----------------------------------------------------------------------------
void AIAgentUtilities::UpdateRigidBodyCapsule(AIAgent* agent)
{
	agent->GetAIAgentWorld()->GetPhysicsWorld()->RemoveRigidBody(
		agent->GetRigidBody());
	PhysicsUtilities::DeleteRigidBody(agent->GetRigidBody());
	agent->SetRigidBody(0);

	CreateRigidBodyCapsule(agent);
}
//----------------------------------------------------------------------------
void AIAgentUtilities::CreateRigidBodyCapsule(AIAgent* agent)
{
	assert(!agent->GetRigidBody());

	btRigidBody* rigidBody =
		PhysicsUtilities::CreateCapsule(agent->GetHeight(),
			agent->GetPhysicsRadius());

	rigidBody->setAngularFactor(btVector3(0, 0.0f, 0));
	agent->SetRigidBody(rigidBody);
	rigidBody->activate(agent->IsEnable());

	AIAgentWorld *aiAgentWorld = agent->GetAIAgentWorld();
	if (aiAgentWorld && aiAgentWorld->GetPhysicsWorld())
	{
		aiAgentWorld->GetPhysicsWorld()->AddRigidBody(rigidBody);
	}
}
//----------------------------------------------------------------------------
void AIAgentUtilities::CreateRigidBodyMesh(AIAgentObject* agentObject,
	Movable *mov)
{
	APoint pos = mov->WorldTransform.GetTranslate();
	btRigidBody* const rigidBody = PhysicsUtilities::CreateRigidBodyFromNode(
		mov, btVector3(pos.X(), pos.Y(), pos.Z()), agentObject->GetMass());

	rigidBody->setUserPointer(agentObject);
	agentObject->SetRigidBody(rigidBody);

	rigidBody->activate(agentObject->IsEnable());

	AIAgentWorld *aiAgentWorld = agentObject->GetAIAgentWorld();
	if (aiAgentWorld && aiAgentWorld->GetPhysicsWorld())
	{
		aiAgentWorld->GetPhysicsWorld()->AddRigidBody(rigidBody);
	}
}
//----------------------------------------------------------------------------
void AIAgentUtilities::UpdateRigidBodyInfinitePlane(AIAgentObject *agentObject,
	const AVector &normal, float originOffset)
{
	agentObject->GetAIAgentWorld()->GetPhysicsWorld()->RemoveRigidBody(
		agentObject->GetRigidBody());
	PhysicsUtilities::DeleteRigidBody(agentObject->GetRigidBody());
	agentObject->SetRigidBody(0);

	CreateRigidBodyInfinitePlane(agentObject, normal, originOffset);
}
//----------------------------------------------------------------------------
void AIAgentUtilities::CreateRigidBodyInfinitePlane(AIAgentObject *agentObject,
	const AVector &normal, float originOffset)
{
	btVector3 btNormal = btVector3(normal.X(), normal.Y(), normal.Z());
	btCollisionShape* const groundShape = new btStaticPlaneShape(btNormal,
		0);
	btDefaultMotionState* const groundMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(
		0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBodyCI.m_rollingFriction = 0.1f;

	btRigidBody *rigidBody = new btRigidBody(groundRigidBodyCI);
	btTransform trans = rigidBody->getWorldTransform();
	trans.setOrigin(btVector3(0.0f, 0.0f, originOffset));
	rigidBody->setWorldTransform(trans);
	rigidBody->setUserPointer(agentObject);
	agentObject->SetRigidBody(rigidBody);

	bool isEnable = agentObject->IsEnable();
	rigidBody->activate(isEnable);

	AIAgentWorld *aiAgentWorld = agentObject->GetAIAgentWorld();
	if (aiAgentWorld && aiAgentWorld->GetPhysicsWorld())
	{
		aiAgentWorld->GetPhysicsWorld()->AddRigidBody(rigidBody);
	}
}
//----------------------------------------------------------------------------
void AIAgentUtilities::UpdateWorldTransTo(AIAgentBase* agentBase)
{
	btRigidBody* rigidBody = agentBase->GetRigidBody();
	if (rigidBody)
	{
		Node* node = agentBase->GetNode();

		const btVector3& rigidBodyPosition =
			rigidBody->getWorldTransform().getOrigin();
		APoint pos = PhysicsUtilities::BtVector3ToVector3(rigidBodyPosition);

		const btQuaternion rigidBodyOrientation =
			rigidBody->getWorldTransform().getRotation();
		HQuaternion quat = PhysicsUtilities::BtQuaterionToQuaternion(
			rigidBodyOrientation);
		HMatrix rotMat;
		quat.ToRotationMatrix(rotMat);

		SetWorldTansform(node, pos, rotMat);
	}
}
//----------------------------------------------------------------------------
void AIAgentUtilities::UpdateWorldTransform(AIAgentBase* agentBase)
{
}
//----------------------------------------------------------------------------
void AIAgentUtilities::SetWorldTansform(Movable *movable, const APoint &pos)
{
	Movable *parent = movable->GetParent();
	if (parent)
	{
		HMatrix inverTrans = parent->WorldTransform.Inverse();
		APoint posLocal = inverTrans * pos;
		movable->LocalTransform.SetTranslate(posLocal);
		movable->Update();
	}
	else
	{
		movable->LocalTransform.SetTranslate(pos);
	}
}
//----------------------------------------------------------------------------
void AIAgentUtilities::SetWorldTansform(Movable *movable, const HMatrix &rot)
{
	Movable *parent = movable->GetParent();
	if (parent)
	{
		HMatrix inversRot = parent->WorldTransform.GetRotate().Inverse();
		HMatrix rotLocal = inversRot * rot;
		movable->LocalTransform.SetRotate(rotLocal);
	}
	else
	{
		movable->LocalTransform.SetRotate(rot);
	}
}
//----------------------------------------------------------------------------
void AIAgentUtilities::SetWorldTansform(Movable *movable, const APoint &pos,
	const HMatrix &rot)
{
	Movable *parent = movable->GetParent();
	if (parent)
	{
		HMatrix inverTrans = parent->WorldTransform.Inverse();
		HMatrix inversRot = parent->WorldTransform.GetRotate().Inverse();
		APoint posLocal = inverTrans * pos;
		HMatrix rotLocal = inversRot * rot;
		movable->LocalTransform.SetTranslate(posLocal);
		movable->LocalTransform.SetRotate(rotLocal);
		movable->LocalTransform.SetUniformScale(1.0f);
	}
	else
	{
		movable->LocalTransform.SetTranslate(pos);
		movable->LocalTransform.SetRotate(rot);
	}

	//movable->LocalTransform.SetTranslate(pos);
	//movable->LocalTransform.SetRotate(rot);
}
//----------------------------------------------------------------------------