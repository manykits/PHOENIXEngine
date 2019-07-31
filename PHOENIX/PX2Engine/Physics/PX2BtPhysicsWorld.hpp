// PX2BtPhysicsWorld.hpp

#ifndef PX2BTPHYSICSWORLD_HPP
#define PX2BTPHYSICSWORLD_HPP

#include "PX2CorePre.hpp"

class btRigidBody;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace PX2
{

	class PhysicsDebugDraw;

	class PX2_ENGINE_ITEM PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		btDiscreteDynamicsWorld* GetDynamicsWorld();

		void Initialize();

		void AddRigidBody(btRigidBody* rigidBody);
		void RemoveRigidBody(btRigidBody* rigidBody);
		void Cleanup();

		void StepWorld(float elapsedSeconds);
		void DrawDebugWorld();

	private:
		btBroadphaseInterface* mBroadphase;
		btDefaultCollisionConfiguration* mCollisionConfiguration;
		btCollisionDispatcher* mDispatcher;
		btSequentialImpulseConstraintSolver* mSolver;
		btDiscreteDynamicsWorld* mDynamicsWorld;
		PhysicsDebugDraw* mDebugDraw;
	};

}

#endif