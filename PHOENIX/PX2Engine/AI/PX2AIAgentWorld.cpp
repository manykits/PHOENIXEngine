// PX2AIAgentWorld.cpp

#include "PX2AIAgentWorld.hpp"
#include "PX2BtPhysicsWorld.hpp"
#include "PX2AIAgentObject.hpp"
#include "PX2AIAgent.hpp"
#include "PX2Node.hpp"
#include "PX2AISteeringWall2D.hpp"
#include "PX2AISteeringParamLoader.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, Controller, AIAgentWorld);
PX2_IMPLEMENT_STREAM(AIAgentWorld);
PX2_IMPLEMENT_FACTORY(AIAgentWorld);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, AIAgentWorld);
//----------------------------------------------------------------------------
AIAgentWorld::AIAgentWorld() :
mPhysicsWorld(0)
{
	mPhysicsWorld = new0 PhysicsWorld();
	mPhysicsWorld->Initialize();

	mCellSpace = new CellSpaceT<AIAgent*>((double)10.0, (double)10.0,
		Prm.NumCellsX, Prm.NumCellsY, Prm.NumAgents);

	mIsDrawPhysicsWorld = true;
}
//----------------------------------------------------------------------------
AIAgentWorld::~AIAgentWorld()
{
	if (mPhysicsWorld)
	{
		delete0(mPhysicsWorld);
		mPhysicsWorld = 0;
	}

	mAgents.clear();
	mObjects.clear();
	mNavMeshes.clear();
}
//----------------------------------------------------------------------------
void AIAgentWorld::Clear()
{
	RemoveAllAgents();
	RemoveAllAgentObjects();
}
//----------------------------------------------------------------------------
void AIAgentWorld::SetNode(Node *sceneNode)
{
	mNode = sceneNode;
}
//----------------------------------------------------------------------------
const AxisAlignedBox3f& AIAgentWorld::GetWorldAABB() const
{
	return mWorldAABB;
}
//----------------------------------------------------------------------------
Node *AIAgentWorld::GetRootNode()
{
	return mNode;
}
//----------------------------------------------------------------------------
const Node *AIAgentWorld::GetRootNode() const
{
	return mNode;
}
//----------------------------------------------------------------------------
PhysicsWorld* AIAgentWorld::GetPhysicsWorld()
{
	return mPhysicsWorld;
}
//----------------------------------------------------------------------------
const PhysicsWorld* AIAgentWorld::GetPhysicsWorld() const
{
	return mPhysicsWorld;
}
//----------------------------------------------------------------------------
bool AIAgentWorld::AddAgent(AIAgent * agent)
{
	GetPhysicsWorld()->RemoveRigidBody(agent->GetRigidBody());

	mAgents.push_back(agent);
	agent->SetAgentWorld(this);

	GetPhysicsWorld()->AddRigidBody(agent->GetRigidBody());

	return true;
}
//----------------------------------------------------------------------------
bool AIAgentWorld::IsHasAgent(AIAgent *agent)
{
	for (int i = 0; i < (int)mAgents.size(); i++)
	{
		if (agent == mAgents[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool AIAgentWorld::RemoveAgent(AIAgent *agent)
{
	auto it = mAgents.begin();
	for (; it != mAgents.end(); it++)
	{
		if (*it == agent)
		{
			// will be removed in SetAgentWorld
			//mPhysicsWorld->RemoveRigidBody(agent->GetRigidBody());
			(*it)->SetAgentWorld(0);
			mAgents.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void AIAgentWorld::RemoveAllAgents()
{
	for (auto it = mAgents.begin(); it != mAgents.end(); it++)
	{
		btRigidBody *body = (*it)->GetRigidBody();
		mPhysicsWorld->RemoveRigidBody(body);
	}

	mAgents.clear();
}
//----------------------------------------------------------------------------
AIAgent* AIAgentWorld::GetAgent(int agentID)
{
	std::vector<AIAgent*>::iterator it;

	for (it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		if ((*it)->GetID() == agentID)
		{
			return *it;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
const AIAgent* AIAgentWorld::GetAgent(int agentID) const
{
	std::vector<AIAgent*>::const_iterator it;

	for (it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		if ((*it)->GetID() == agentID)
		{
			return *it;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
std::vector<AIAgent*>& AIAgentWorld::GetAgents()
{
	return mAgents;
}
//----------------------------------------------------------------------------
const std::vector<AIAgent*>& AIAgentWorld::GetAgents() const
{
	return mAgents;
}
//----------------------------------------------------------------------------
void AIAgentWorld::AddAgentObject(AIAgentObject* agentObject)
{
	GetPhysicsWorld()->RemoveRigidBody(agentObject->GetRigidBody());

	mObjects.push_back(agentObject);
	agentObject->SetAgentWorld(this);

	GetPhysicsWorld()->AddRigidBody(agentObject->GetRigidBody());
}
//----------------------------------------------------------------------------
void AIAgentWorld::RemoveAgentObject(AIAgentObject* agentObject)
{
	for (auto it = mObjects.begin(); it != mObjects.end(); it++)
	{
		if (agentObject == (*it))
		{
			agentObject->SetAgentWorld(0);
			mObjects.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void AIAgentWorld::RemoveAllAgentObjects()
{
	for (auto it = mObjects.begin(); it != mObjects.end(); it++)
	{
		btRigidBody *body = (*it)->GetRigidBody();
		mPhysicsWorld->RemoveRigidBody(body);
	}

	mObjects.clear();
}
//----------------------------------------------------------------------------
std::vector<AIAgentObject*>& AIAgentWorld::GetObjects()
{
	return mObjects;
}
//----------------------------------------------------------------------------
const std::vector<AIAgentObject*>& AIAgentWorld::GetObjects() const
{
	return mObjects;
}
//----------------------------------------------------------------------------
void AIAgentWorld::AddNavigationMesh(const std::string& name,
	NavigationMesh *navMesh)
{
	auto it = mNavMeshes.find(name);
	if (it != mNavMeshes.end())
	{
		delete it->second;
		mNavMeshes.erase(it);
	}

	mNavMeshes[name] = navMesh;
}
//----------------------------------------------------------------------------
const std::vector<Wall2D>& AIAgentWorld::GetWalls()
{
	return mWalls;
}
//----------------------------------------------------------------------------
void AIAgentWorld::_Update(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);

	if (mPhysicsWorld)
	{
		mPhysicsWorld->StepWorld(elapsedTime);
	}

	if (mIsDrawPhysicsWorld)
	{
		mPhysicsWorld->DrawDebugWorld();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
AIAgentWorld::AIAgentWorld(LoadConstructor value) :
Controller(value)
{
	mPhysicsWorld = new0 PhysicsWorld();
	mPhysicsWorld->Initialize();
}
//----------------------------------------------------------------------------
void AIAgentWorld::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(AIAgentWorld, source);
}
//----------------------------------------------------------------------------
void AIAgentWorld::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void AIAgentWorld::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool AIAgentWorld::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AIAgentWorld::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(AIAgentWorld, target);
}
//----------------------------------------------------------------------------
int AIAgentWorld::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
