// PX2BtPhysicsUtilities.hpp

#ifndef PX2BTPHYSICSUTILITIES_HPP
#define PX2BTPHYSICSUTILITIES_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector3.hpp"
#include "PX2TriMesh.hpp"
#include "PX2Node.hpp"
#include "PX2HQuaternion.hpp"
#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"

class btConvexHullShape;
class btQuaternion;
class btRigidBody;

namespace PX2
{

	class PhysicsUtilities
	{
	public:
		static void ApplyForce(
			btRigidBody* rigidBody, const btVector3& force);

		//static void ApplyImpulse(
		//	const btRigidBody* rigidBody, const btVector3& impulse);

		//static void ApplyTorque(
		//	const btRigidBody* rigidBody, const btVector3& torque);

		//static void ApplyTorqueImpulse(
		//	const btRigidBody* rigidBody, const btVector3& impulse);

		static Vector3f BtVector3ToVector3(const btVector3& vector);
		static btVector3 Vector3ToBtVector3(const Vector3f& vector);

		static HQuaternion BtQuaterionToQuaternion(const btQuaternion& quaternion);
		static btQuaternion QuaterionToBtQuaternion(const HQuaternion &quat);

		//static btRigidBody* CreateBox(
		//	const btScalar width, const btScalar height, const btScalar length);

		static btRigidBody* CreateCapsule(
			const btScalar height, btScalar radius);

		static btRigidBody* CreateInfinitePlane(
			const btVector3& normal, btScalar originOffset);

		static btRigidBody* CreateRigidBodyFromNode(
			Movable *mov, const btVector3& position, const btScalar mass);

		static btConvexHullShape* CreateSimplifiedConvexHull(
			TriMesh *mesh);

		//static btRigidBody* CreateSphere(const btScalar radius);

		static void DeleteRigidBody(btRigidBody* rigidBody);

		static btScalar GetRigidBodyRadius(const btRigidBody* rigidBody);

		static btScalar GetRigidBodyMass(const btRigidBody* rigidBody);

		//static bool IsPlane(const btRigidBody& rigidBody);

		//static void SetRigidBodyGravity(
		//	const btRigidBody* rigidBody, const btVector3& gravity);

		static void SetRigidBodyMass(
			btRigidBody* rigidBody, const btScalar mass);
		static void SetRigidBodyOrientation(
			btRigidBody* rigidBody, const btQuaternion& orientation);
		static void SetRigidBodyPosition(
			btRigidBody* rigidBody, const btVector3& position);
		static void SetRigidBodyVelocity(
			btRigidBody* rigidBody, const btVector3& velocity);

		//static Object* ToObject(
		//	const const btRigidBody* rigidBody);

		//static btVector3 Vector3ToBtVector3(const Vector3f& vector);

	private:
		PhysicsUtilities();
		~PhysicsUtilities();
		PhysicsUtilities(const PhysicsUtilities&);
		PhysicsUtilities& operator=(const PhysicsUtilities&);
	};


}

#endif