// PX2AIAgentObject.hpp

#ifndef PX2AIAGENTOBJECT_HPP
#define PX2AIAGENTOBJECT_HPP

#include "PX2CorePre.hpp"
#include "PX2Object.hpp"
#include "PX2HQuaternion.hpp"
#include "PX2Controller.hpp"
#include "PX2Movable.hpp"
#include "PX2AIAgentBase.hpp"

class btRigidBody;

namespace PX2
{
	class AIAgentWorld;
	class Node;

	class PX2_ENGINE_ITEM AIAgentObject : public AIAgentBase
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(AIAgentObject);
		PX2_DECLARE_PROPERTY;

	public:
		AIAgentObject(Node *node=0);
		virtual ~AIAgentObject();

		enum PhysicsShapeType
		{
			PST_INFINITEPLANE,
			PST_MESH,
			PST_MAX_TYPE
		};
		PhysicsShapeType GetPhysicsShapeType() const;

		void InitializeInfinitePlane(const AVector &normal, float originOffset);
		void InitializeMesh(Movable *mov);

		void SetOrientation(const HQuaternion& quaternion);
		HQuaternion GetOrientation() const;

		float GetRigidBodyRadius() const;

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);

	private:
		float mLastWorldPosZ;
		PhysicsShapeType mPhysicsShapeType;
	};

	PX2_REGISTER_STREAM(AIAgentObject);
	typedef Pointer0<AIAgentObject> AIAgentObjectPtr;

}

#endif