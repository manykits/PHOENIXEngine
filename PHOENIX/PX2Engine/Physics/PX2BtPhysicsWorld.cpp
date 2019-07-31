// PX2BtPhysicsWorld.cpp

#include "PX2BtPhysicsWorld.hpp"
#include "PX2BtPhysicsDebugDraw.hpp"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "Bullet/BulletCollision/CollisionShapes/btCollisionShape.h"
#include "Bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "Bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "Bullet/BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "Bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "Bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
using namespace PX2;

//----------------------------------------------------------------------------
PhysicsWorld::PhysicsWorld(): 
mDynamicsWorld(0),
mSolver(0),
mDispatcher(0),
mCollisionConfiguration(0),
mBroadphase(0),
mDebugDraw(0)
{
}
//----------------------------------------------------------------------------
PhysicsWorld::~PhysicsWorld()
{
	Cleanup();
}
//----------------------------------------------------------------------------
btDiscreteDynamicsWorld* PhysicsWorld::GetDynamicsWorld()
{
	return mDynamicsWorld;
}
//----------------------------------------------------------------------------
void PhysicsWorld::Initialize()
{
	static const float gravity = -9.8f;

	mBroadphase = new btDbvtBroadphase();

	mCollisionConfiguration = new btDefaultCollisionConfiguration();

	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	mSolver = new btSequentialImpulseConstraintSolver();

	mDynamicsWorld = new btDiscreteDynamicsWorld(
		mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

	mDynamicsWorld->setGravity(btVector3(0, 0.0f, gravity));

	mDebugDraw = new PhysicsDebugDraw();
	mDynamicsWorld->setDebugDrawer(mDebugDraw);
	// btIDebugDraw::DBG_DrawAabb
	mDebugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}
//----------------------------------------------------------------------------
void PhysicsWorld::AddRigidBody(btRigidBody* rigidBody)
{
	if (!rigidBody)
		return;

	mDynamicsWorld->addRigidBody(rigidBody);
}
//----------------------------------------------------------------------------
void PhysicsWorld::RemoveRigidBody(btRigidBody* rigidBody)
{
	if (!rigidBody)
		return;

	mDynamicsWorld->removeRigidBody(rigidBody);
}
//----------------------------------------------------------------------------
void PhysicsWorld::Cleanup()
{
	if (mDynamicsWorld)
	{
		delete mDynamicsWorld;
		mDynamicsWorld = 0;
	}

	if (mSolver)
	{
		delete mSolver;
		mSolver = 0;
	}

	if (mDispatcher)
	{
		delete mDispatcher;
		mDispatcher = 0;
	}

	if (mCollisionConfiguration)
	{
		delete mCollisionConfiguration;
		mCollisionConfiguration = 0;
	}

	if (mBroadphase)
	{
		delete mBroadphase;
		mBroadphase = 0;
	}
}
//----------------------------------------------------------------------------
void PhysicsWorld::StepWorld(float elapsedSeconds)
{
	mDynamicsWorld->stepSimulation(elapsedSeconds, 1, 1.0f / 30.0f);
}
//----------------------------------------------------------------------------
void PhysicsWorld::DrawDebugWorld()
{
	mDynamicsWorld->debugDrawWorld();
}
//----------------------------------------------------------------------------