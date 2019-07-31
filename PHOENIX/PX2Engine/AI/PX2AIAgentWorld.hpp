// PX2AIAgentWorld.hpp

#ifndef PX2AIAGENTWORLD_HPP
#define PX2AIAGENTWORLD_HPP

#include "PX2AxisAlignedBox3.hpp"
#include "PX2Controller.hpp"
#include "PX2AISteeringWall2D.hpp"
#include "PX2AIAgent.hpp"
#include "PX2AIAgentObject.hpp"
#include "PX2CellSpaceT.hpp"

namespace PX2
{

	class PhysicsWorld;
	class NavigationMesh;
	class Node;

	//------------------------------------------------------------------------
	template <class T, class conT>
	void TagNeighbors(const T& entity, conT& containerOfEntities, double radius)
	{
		for (typename conT::iterator curEntity = containerOfEntities.begin();
			curEntity != containerOfEntities.end();
			++curEntity)
		{
			//first clear any current tag
			(*curEntity)->UnTag();

			Vector2f to = (*curEntity)->GetPosition().To2() - entity->GetPosition().To2();

			//the bounding radius of the other is taken into account by adding it 
			//to the range
			double range = radius + (*curEntity)->GetRadius();

			//if entity within range, tag for further consideration. (working in
			//distance-squared space to avoid sqrts)
			if (((*curEntity) != entity) && (to.SquaredLength() < range*range) &&
				((*curEntity)->GetMass()>0.0f || (*curEntity)->IsMassZeroAvoid()))
			{
				(*curEntity)->Tag();
			}

		}//next entity
	}

	class PX2_ENGINE_ITEM AIAgentWorld : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(AIAgentWorld);

	public:
		AIAgentWorld();
		virtual ~AIAgentWorld();

		void Clear();

		const AxisAlignedBox3f& GetWorldAABB() const;

		Node *GetRootNode();
		const Node *GetRootNode() const;

		PhysicsWorld* GetPhysicsWorld();
		const PhysicsWorld* GetPhysicsWorld() const;

		bool AddAgent(AIAgent * agent);
		bool IsHasAgent(AIAgent *agent);
		bool RemoveAgent(AIAgent *agent);
		void RemoveAllAgents();
		AIAgent* GetAgent(int agentID);
		const AIAgent* GetAgent(int agentID) const;
		std::vector<AIAgent*>& GetAgents();
		const std::vector<AIAgent*>& GetAgents() const;
		CellSpaceT<AIAgent*>* CellSpace() { return mCellSpace; }

		void AddAgentObject(AIAgentObject* agentObject);
		void RemoveAgentObject(AIAgentObject* agentObject);
		void RemoveAllAgentObjects();
		std::vector<AIAgentObject*>& GetObjects();
		const std::vector<AIAgentObject*>& GetObjects() const;

		void AddNavigationMesh(const std::string& name, NavigationMesh *navMesh);

		const std::vector<Wall2D>& GetWalls();

		void TagVehiclesWithinViewRange(AIAgentBase* pVehicle, double range)
		{
			TagNeighbors(pVehicle, mAgents, range);
		}

		void TagObstaclesWithinViewRange(AIAgentBase* pVehicle, double range)
		{
			TagNeighbors(pVehicle, mObjects, range);
		}

	public_internal:
		void SetNode(Node *sceneNode);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);

		AxisAlignedBox3f mWorldAABB;
		Node* mNode;

		PhysicsWorld* mPhysicsWorld;
		bool mIsDrawPhysicsWorld;

		std::vector<AIAgent*> mAgents;
		CellSpaceT<AIAgent*>* mCellSpace;

		std::vector<AIAgentObject*> mObjects;
		std::map<std::string, NavigationMesh*> mNavMeshes;

		std::vector<Wall2D> mWalls;
	};

	PX2_REGISTER_STREAM(AIAgentWorld);
	typedef Pointer0<AIAgentWorld> AIAgentWorldPtr;

}

#endif