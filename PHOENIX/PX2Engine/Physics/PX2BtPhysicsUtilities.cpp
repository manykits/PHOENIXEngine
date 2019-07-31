// PX2BtPhysicsUtilities.cpp

#include "PX2BtPhysicsUtilities.hpp"
#include "PX2Any.hpp"
#include "Bullet/BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "Bullet/LinearMath/btMotionState.h"
#include "Bullet/LinearMath/btDefaultMotionState.h"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "Bullet/BulletCollision/CollisionShapes/btCollisionShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btCompoundShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"
#include "Bullet/BulletCollision/CollisionShapes/btBoxShape.h"
using namespace PX2;

//----------------------------------------------------------------------------
void PhysicsUtilities::ApplyForce(
	btRigidBody* rigidBody, const btVector3& force)
{
	rigidBody->applyCentralForce(force);
	rigidBody->activate(true);
}
//----------------------------------------------------------------------------
Vector3f PhysicsUtilities::BtVector3ToVector3(const btVector3& vector)
{
	return Vector3f(
		vector.m_floats[0], vector.m_floats[1], vector.m_floats[2]);
}
//----------------------------------------------------------------------------
btVector3 PhysicsUtilities::Vector3ToBtVector3(const Vector3f& vector)
{
	return btVector3(
		vector.X(), vector.Y(), vector.Z());
}
//----------------------------------------------------------------------------
HQuaternion PhysicsUtilities::BtQuaterionToQuaternion(const btQuaternion& quaternion)
{
	return HQuaternion(
		quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}
//----------------------------------------------------------------------------
btQuaternion PhysicsUtilities::QuaterionToBtQuaternion(const HQuaternion &quat)
{
	return btQuaternion(quat.X(), quat.Y(), quat.Z(), quat.W());
}
//----------------------------------------------------------------------------
btRigidBody* PhysicsUtilities::CreateCapsule(
	const btScalar height, btScalar radius)
{
	// Since the height of
	btCollisionShape* const capsuleShape = new btCapsuleShapeZ(radius, height);

	btVector3 localInertia(0, 0, 0);
	capsuleShape->calculateLocalInertia(1.0f, localInertia);

	btDefaultMotionState* const capsuleMotionState =
		new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo capsuleRigidBodyCI(
		1.0f, capsuleMotionState, capsuleShape, localInertia);

	// Prevent rolling forever.
	capsuleRigidBodyCI.m_rollingFriction = 0.2f;

	btRigidBody* const rigidBody = new btRigidBody(capsuleRigidBodyCI);
	rigidBody->setCcdMotionThreshold(0.5f);
	rigidBody->setCcdSweptSphereRadius(radius);

	return rigidBody;
}
//----------------------------------------------------------------------------
btRigidBody* PhysicsUtilities::CreateInfinitePlane(
	const btVector3& normal, btScalar originOffset)
{
	btCollisionShape* const groundShape =
		new btStaticPlaneShape(normal, originOffset);

	btDefaultMotionState* const groundMotionState = new btDefaultMotionState();

	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(
		0, groundMotionState, groundShape, btVector3(0, 0, 0));

	groundRigidBodyCI.m_rollingFriction = 0.1f;

	return new btRigidBody(groundRigidBodyCI);
}
//----------------------------------------------------------------------------
static btVector3 aabbMin;
static btVector3 aabbMax;
void _TravelExecuteFun(Movable *mov, Any *data, bool &goOn)
{
	TriMesh *mesh = DynamicCast<TriMesh>(mov);
	if (mesh)
	{
		btCompoundShape *compoundShape = PX2_ANY_AS(*data, btCompoundShape*);
		btConvexHullShape* hullShape = PhysicsUtilities::
			CreateSimplifiedConvexHull(mesh);

		btVector3 aabbMin;
		btVector3 aabbMax;
		hullShape->getAabb(
			btTransform(btQuaternion::getIdentity()), aabbMin, aabbMax);

		compoundShape->addChildShape(
			btTransform(btQuaternion::getIdentity()), hullShape);
	}
}
//----------------------------------------------------------------------------
btRigidBody* PhysicsUtilities::CreateRigidBodyFromNode(
	Movable *mov, const btVector3& position, const btScalar mass)
{
	btCompoundShape* compoundShape = new btCompoundShape();
	compoundShape->setLocalScaling(btVector3(0.2f, 0.2f, 0.2f));
	Any data(compoundShape);

	Node::TravelExecute(mov, _TravelExecuteFun, &data);

	btDefaultMotionState* const motionState = new btDefaultMotionState(
		btTransform(btQuaternion::getIdentity(), position));

	btVector3 localInertia(0, 0, 0);
	compoundShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(mass, motionState, compoundShape, localInertia);

	rigidBodyCI.m_linearSleepingThreshold = 0.3f;

	btRigidBody* const rigidBody = new btRigidBody(rigidBodyCI);

	rigidBody->setCcdMotionThreshold(0.5f);
	rigidBody->setCcdSweptSphereRadius(aabbMax.length() / 2.0f);

	return rigidBody;
}
//----------------------------------------------------------------------------
btConvexHullShape* PhysicsUtilities::CreateSimplifiedConvexHull(
	TriMesh *mesh)
{
	btConvexHullShape* const shape = new btConvexHullShape();
	shape->setMargin(0.01f);
	shape->setSafeMargin(0.01f);
	float scale = mesh->WorldTransform.GetUniformScale();
	shape->setLocalScaling(btVector3(scale, scale, scale));

	VertexBufferAccessor vba(mesh);
	for (int i = 0; i < mesh->GetNumTriangles(); i++)
	{
		int v0 = 0;
		int v1 = 0;
		int v2 = 0;
		if (mesh->GetTriangle(i, v0, v1, v2))
		{
			Float3 p0 = vba.Position<Float3>(v0);
			Float3 p1 = vba.Position<Float3>(v1);
			Float3 p2 = vba.Position<Float3>(v2);
			shape->addPoint(btVector3(p0.X(), p0.Y(), p0.Z()));
			shape->addPoint(btVector3(p1.X(), p1.Y(), p1.Z()));
			shape->addPoint(btVector3(p2.X(), p2.Y(), p2.Z()));
		}
	}

	btShapeHull* const hull = new btShapeHull(shape);

	hull->buildHull(shape->getMargin());

	btConvexHullShape* const simplifiedConvexShape = new btConvexHullShape();

	const btVector3* const btVertices = hull->getVertexPointer();
	const int numVertices = hull->numVertices();

	for (int index = 0; index < numVertices; ++index)
	{
		simplifiedConvexShape->addPoint(btVertices[index]);
	}

	simplifiedConvexShape->setMargin(0.01f);

	delete hull;
	delete shape;

	return simplifiedConvexShape;
}
//----------------------------------------------------------------------------
void PhysicsUtilities::DeleteRigidBody(btRigidBody* rigidBody)
{
	delete rigidBody->getMotionState();
	delete rigidBody->getCollisionShape();
	delete rigidBody;
}
//----------------------------------------------------------------------------
btScalar PhysicsUtilities::GetRigidBodyRadius(const btRigidBody* rigidBody)
{
	btVector3 aabbMin;
	btVector3 aabbMax;
	rigidBody->getAabb(aabbMin, aabbMax);

	return aabbMax.distance(aabbMin) / 2.0f;
}
//----------------------------------------------------------------------------
btScalar PhysicsUtilities::GetRigidBodyMass(const btRigidBody* rigidBody)
{
	btScalar inverseMass = rigidBody->getInvMass();

	if (inverseMass == 0)
		return 0;

	return 1.0f / inverseMass;
}
//----------------------------------------------------------------------------
void PhysicsUtilities::SetRigidBodyMass(
	btRigidBody* rigidBody, const btScalar mass)
{
	btVector3 localInertia(0, 0, 0);
	rigidBody->getCollisionShape()->calculateLocalInertia(mass, localInertia);
	rigidBody->setMassProps(mass, localInertia);
	rigidBody->updateInertiaTensor();
	rigidBody->activate(true);
}
//----------------------------------------------------------------------------
void PhysicsUtilities::SetRigidBodyOrientation(
	btRigidBody* rigidBody, const btQuaternion& orientation)
{
	btTransform transform = rigidBody->getWorldTransform();
	transform.setRotation(orientation);

	rigidBody->setWorldTransform(transform);
	rigidBody->activate(true);
}
//----------------------------------------------------------------------------
void PhysicsUtilities::SetRigidBodyPosition(
	btRigidBody* rigidBody, const btVector3& position)
{
	btTransform transform = rigidBody->getWorldTransform();
	transform.setOrigin(position);

	rigidBody->setWorldTransform(transform);
	rigidBody->activate(true);
}
//----------------------------------------------------------------------------
void PhysicsUtilities::SetRigidBodyVelocity(
	btRigidBody* rigidBody, const btVector3& velocity)
{
	rigidBody->setLinearVelocity(velocity);
	rigidBody->activate(true);
}
//----------------------------------------------------------------------------