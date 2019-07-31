// PX2AIAgentUtilities.hpp

#ifndef PX2AIAGENTUTILITIES_HPP
#define PX2AIAGENTUTILITIES_HPP

#include "PX2Vector3.hpp"
#include "PX2AVector.hpp"
#include "PX2Movable.hpp"

namespace PX2
{

	class AIAgent;
	class AIAgentObject;
	class AIAgentBase;

	class AIAgentUtilities
	{
	public:
		static void UpdateRigidBodyCapsule(AIAgent* agent);
		static void CreateRigidBodyCapsule(AIAgent* agent);

		static void CreateRigidBodyMesh(AIAgentObject* agentObject, 
			Movable *mov);

		static void UpdateRigidBodyInfinitePlane(AIAgentObject *agentObject, 
			const AVector &normal, float originOffset);
		static void CreateRigidBodyInfinitePlane(AIAgentObject *agentObject,
			const AVector &normal, float originOffset);

		static void UpdateWorldTransTo(AIAgentBase* agentBase);
		static void UpdateWorldTransform(AIAgentBase* agentBase);

		static void SetWorldTansform(Movable *movable, const APoint &pos);
		static void SetWorldTansform(Movable *movable, const HMatrix &rot);
		static void SetWorldTansform(Movable *movable, const APoint &pos,
			const HMatrix &rot);
	};

}

#endif