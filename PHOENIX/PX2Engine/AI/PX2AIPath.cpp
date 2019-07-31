// PX2AIPath.cpp

#include "PX2AIPath.hpp"
#include "PX2Assert.hpp"
#include "PX2Memory.hpp"
#include "PX2Log.hpp"
#include "PX2Math.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PathingNode::PathingNode(const Vector3f& pos, float tolerance): 
m_pos(pos),
ThePathingGraph(0)
{
	m_tolerance = tolerance; 
}
//----------------------------------------------------------------------------
PathingGraph *PathingNode::GetPathingGraph()
{
	return ThePathingGraph;
}
//----------------------------------------------------------------------------
void PathingNode::SetPathingGraph(PathingGraph *graph)
{
	ThePathingGraph = graph;
}
//----------------------------------------------------------------------------
void PathingNode::AddArc(PathingArc* pArc)
{
	assertion(pArc, "pointer must not be null");
	m_arcs.push_back(pArc);
}
//----------------------------------------------------------------------------
void PathingNode::GetNeighbors(PathingNodeList& outNeighbors)
{
	for (PathingArcList::iterator it = m_arcs.begin(); it != m_arcs.end(); ++it)
	{
		PathingArc* pArc = *it;
		outNeighbors.push_back(pArc->GetNeighbor(this));
	}
}
//----------------------------------------------------------------------------
PathingArcList &PathingNode::GetPathingArcList()
{
	return m_arcs;
}
//----------------------------------------------------------------------------
float PathingNode::GetCostFromNode(PathingNode* pFromNode)
{
	assertion(pFromNode, "pointer must not be null");
	PathingArc* pArc = FindArc(pFromNode);
	assertion(pArc, "pointer must not be null");
	Vector3f diff = pFromNode->GetPos() - m_pos;
	return pArc->GetWeight() * diff.Length();
}
//----------------------------------------------------------------------------
PathingArc* PathingNode::FindArc(PathingNode* pLinkedNode)
{
	assertion(pLinkedNode, "pointer must not be null");

	for (PathingArcList::iterator it = m_arcs.begin(); it != m_arcs.end(); ++it)
	{
		PathingArc* pArc = *it;
		if (pArc->GetNeighbor(this) == pLinkedNode)
			return pArc;
	}

	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// PathingArc
//----------------------------------------------------------------------------
PathingArc::PathingArc(float weight) 
{
	m_weight = weight; 
}
//----------------------------------------------------------------------------
void PathingArc::SetWeight(float weight)
{
	m_weight = weight;
}
//----------------------------------------------------------------------------
void PathingArc::LinkNodes(PathingNode* pNodeA, PathingNode* pNodeB)
{
	assertion(pNodeA, "pointer must not be null");
	assertion(pNodeB, "pointer must not be null");

	m_pNodes[0] = pNodeA;
	m_pNodes[1] = pNodeB;
}
//----------------------------------------------------------------------------
PathingNode* PathingArc::GetNeighbor(PathingNode* pMe)
{
	assertion(pMe, "pointer must not be null");

	if (m_pNodes[0] == pMe)
		return m_pNodes[1];
	else
		return m_pNodes[0];
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// PathPlan
//----------------------------------------------------------------------------
const Vector3f& PathPlan::GetCurrentNodePosition(void) const 
{
	assertion(m_index != m_path.end(), "");
	return (*m_index)->GetPos(); 
}
//----------------------------------------------------------------------------
bool PathPlan::CheckForNextNode(const Vector3f& pos)
{
	if (m_index == m_path.end())
		return false;

	Vector3f diff = pos - (*m_index)->GetPos();

	if (diff.Length() <= (*m_index)->GetTolerance())
	{
		++m_index;
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool PathPlan::CheckForEnd()
{
	if (m_index == m_path.end())
		return true;

	return false;
}
//----------------------------------------------------------------------------
bool PathPlan::IsCanGo(PathingNode *nodeFrom, PathingNode *nodeTo)
{
	PathingGraph *pathingGraph = nodeFrom->GetPathingGraph();
	assertion(pathingGraph, "pathingGraph must not be null");

	return pathingGraph->IsCanGo(nodeFrom->GetPos(), nodeTo->GetPos());
}
//----------------------------------------------------------------------------
void PathPlan::Optmize()
{
	m_pathOptmize.clear();
	PathingNodeList temp;

	Vector3f lastDir;
	Vector3f curDir;
	PathingNode *nodeCur = 0;
	auto it = m_path.begin();
	for (; it != m_path.end(); it++)
	{
		PathingNode *nodeIt = *it;
		if (!nodeCur)
		{
			temp.push_back(nodeIt);
		}
		nodeCur = nodeIt;

		const Vector3f &curPos = nodeCur->GetPos();

		auto itNext = std::next(it);
		if (itNext != m_path.end())
		{
			PathingNode *nodeNext = *itNext;
			const Vector3f &nextPos = nodeNext->GetPos();
			curDir = nextPos - curPos;
			curDir.Normalize();
			float val = curDir.Dot(lastDir);
			if (val > 0.99f)
			{
				// the same dir
			}
			else
			{
				temp.push_back(nodeCur);
			}

			lastDir = curDir;
		}
		else
		{
			// 加上最后一个点
			temp.push_back(nodeCur);
		}
	}

	PathingNodeList temp1;
	if (!temp.empty())
	{
		PathingNode *nodeCur = 0;
		PathingNode *nodeBefore = 0;
		PathingNode *nodeLastUse = 0;

		auto it = temp.begin();
		while (it != temp.end())
		{
			nodeCur = *it;

			if (!nodeLastUse)
			{
				// start
				nodeLastUse = nodeCur;
				temp1.push_back(nodeLastUse);
				it++;
			}
			else
			{
				if (IsCanGo(nodeLastUse, nodeCur))
				{
					// do nothing
				}
				else
				{
					temp1.push_back(nodeBefore);
					nodeLastUse = nodeBefore;
				}
			}

			nodeBefore = nodeCur;
			it++;
		}

		// add last
		temp1.push_back(nodeCur);
	}

	m_pathOptmize = temp1;
	m_path = m_pathOptmize;
}
//----------------------------------------------------------------------------
void PathPlan::AddNode(PathingNode* pNode)
{
	assertion(pNode, "pointer must not be null");
	m_path.push_front(pNode);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// PathPlanNode
//----------------------------------------------------------------------------
PathPlanNode::PathPlanNode(PathingNode* pNode, PathPlanNode* pPrevNode, 
	PathingNode* pGoalNode)
{
	assertion(0 != pNode, "pointer must not be null");

	m_pPathingNode = pNode;
	m_pPrev = pPrevNode;
	m_pGoalNode = pGoalNode;
	m_closed = false;
	UpdateHeuristics();
}
//----------------------------------------------------------------------------
void PathPlanNode::UpdatePrevNode(PathPlanNode* pPrev)
{
	assertion(0 != pPrev, "pointer must not be null");
	m_pPrev = pPrev;
	UpdateHeuristics();
}
//----------------------------------------------------------------------------
void PathPlanNode::UpdateHeuristics(void)
{
	if (m_pPrev)
		m_goal = m_pPrev->GetGoal() + 
		m_pPathingNode->GetCostFromNode(m_pPrev->GetPathingNode());
	else
		m_goal = 0;

	// heuristic (h)
	Vector3f diff = m_pPathingNode->GetPos() - m_pGoalNode->GetPos();
	m_heuristic = diff.Length();

	// cost to goal (f)
	m_fitness = m_goal + m_heuristic;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// AStar
//----------------------------------------------------------------------------
AStar::AStar()
{
	m_pStartNode = 0;
	m_pGoalNode = 0;
}
//----------------------------------------------------------------------------
AStar::~AStar()
{
	Destroy();
}
//----------------------------------------------------------------------------
void AStar::Destroy()
{
	for (PathingNodeToPathPlanNodeMap::iterator it = m_nodes.begin();
		it != m_nodes.end(); ++it)
		delete0(it->second);
	m_nodes.clear();

	m_openSet.clear();

	m_pStartNode = 0;
	m_pGoalNode = 0;
}
//----------------------------------------------------------------------------
PathPlan* AStar::operator()(PathingNode* pStartNode, PathingNode* pGoalNode)
{
	assertion(0 != pStartNode, "pStartNode must not be null");
	assertion(0 != pGoalNode, "pGoalNode must not be null");

	if (pStartNode == pGoalNode)
		return 0;

	m_pStartNode = pStartNode;
	m_pGoalNode = pGoalNode;

	AddToOpenSet(m_pStartNode, 0);

	while (!m_openSet.empty())
	{
		// grab the most likely candidate
		PathPlanNode* pNode = m_openSet.front();

		// If this node is our goal node, we've successfully found a path.
		if (pNode->GetPathingNode() == m_pGoalNode)
			return RebuildPath(pNode);

		// we're processing this node so remove it from the open set and add it to the closed set
		m_openSet.pop_front();
		AddToClosedSet(pNode);

		// get the neighboring nodes
		PathingNodeList neighbors;
		pNode->GetPathingNode()->GetNeighbors(neighbors);

		// loop though all the neighboring nodes and evaluate each one
		for (PathingNodeList::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
		{
			PathingNode* pNodeToEvaluate = *it;

			// Try and find a PathPlanNode object for this node.
			PathingNodeToPathPlanNodeMap::iterator findIt = m_nodes.find(pNodeToEvaluate);

			// If one exists and it's in the closed list, we've already evaluated the node.  We can
			// safely skip it.
			if (findIt != m_nodes.end() && findIt->second->IsClosed())
				continue;

			// figure out the cost for this route through the node
			float costForThisPath = pNode->GetGoal() + pNodeToEvaluate->GetCostFromNode(pNode->GetPathingNode());
			bool isPathBetter = false;

			// Grab the PathPlanNode if there is one.
			PathPlanNode* pPathPlanNodeToEvaluate = 0;
			if (findIt != m_nodes.end())
				pPathPlanNodeToEvaluate = findIt->second;

			// No PathPlanNode means we've never evaluated this pathing node so we need to add it to 
			// the open set, which has the side effect of setting all the heuristic data.  It also 
			// means that this is the best path through this node that we've found so the nodes are 
			// linked together (which is why we don't bother setting isPathBetter to true; it's done
			// for us in AddToOpenSet()).
			if (!pPathPlanNodeToEvaluate)
				pPathPlanNodeToEvaluate = AddToOpenSet(pNodeToEvaluate, pNode);

			// If this node is already in the open set, check to see if this route to it is better than
			// the last.
			else if (costForThisPath < pPathPlanNodeToEvaluate->GetGoal())
				isPathBetter = true;

			// If this path is better, relink the nodes appropriately, update the heuristics data, and
			// reinsert the node into the open list priority queue.
			if (isPathBetter)
			{
				pPathPlanNodeToEvaluate->UpdatePrevNode(pNode);
				ReinsertNode(pPathPlanNodeToEvaluate);
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
PathPlanNode* AStar::AddToOpenSet(PathingNode* pNode, PathPlanNode* pPrevNode)
{
	assertion(0 != pNode, "pNode must not be null.");

	PathingNodeToPathPlanNodeMap::iterator it = m_nodes.find(pNode);
	PathPlanNode* pThisNode = 0;
	if (it == m_nodes.end())
	{
		pThisNode = new0 PathPlanNode(pNode, pPrevNode, m_pGoalNode);
		m_nodes.insert(std::make_pair(pNode, pThisNode));
	}
	else
	{
		PX2_LOG_INFO("Adding existing PathPlanNode to open set");
		pThisNode = it->second;
		pThisNode->SetClosed(false);
	}

	InsertNode(pThisNode);

	return pThisNode;
}
//----------------------------------------------------------------------------
void AStar::AddToClosedSet(PathPlanNode* pNode)
{
	assertion(0 != pNode, "pNode must not be null");
	pNode->SetClosed();
}
//----------------------------------------------------------------------------
void AStar::InsertNode(PathPlanNode* pNode)
{
	assertion(0 != pNode, "pNode must not be null");

	// just add the node if the open set is empty
	if (m_openSet.empty())
	{
		m_openSet.push_back(pNode);
		return;
	}

	// otherwise, perform an insertion sort	
	PathPlanNodeList::iterator it = m_openSet.begin();
	PathPlanNode* pCompare = *it;
	while (pCompare->IsBetterChoiceThan(pNode))
	{
		++it;

		if (it != m_openSet.end())
			pCompare = *it;
		else
			break;
	}
	m_openSet.insert(it, pNode);
}
//----------------------------------------------------------------------------
void AStar::ReinsertNode(PathPlanNode* pNode)
{
	assertion(0 != pNode, "pNode must not be null.");

	for (PathPlanNodeList::iterator it = m_openSet.begin(); it != m_openSet.end(); ++it)
	{
		if (pNode == (*it))
		{
			m_openSet.erase(it);
			InsertNode(pNode);
			return;
		}
	}

	// if we get here, the node was never in the open set to begin with
	PX2_LOG_INFO("Attemping to reinsert node that was never in the open list");
	InsertNode(pNode);
}
//----------------------------------------------------------------------------
PathPlan* AStar::RebuildPath(PathPlanNode* pGoalNode)
{
	assertion(0 != pGoalNode, "pNode must not be null.");

	PathPlan* pPlan = new0 PathPlan;

	PathPlanNode* pNode = pGoalNode;
	while (pNode)
	{
		pPlan->AddNode(pNode->GetPathingNode());
		pNode = pNode->GetPrev();
	}

	pPlan->Optmize();

	return pPlan;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// PathingGraph
//----------------------------------------------------------------------------
PathingGraph::PathingGraph() :
mMapSize(256),
mResolution(1.0f),
mMapDataWidth(1024),
mMapDataResolution(1.0f)
{
}
//----------------------------------------------------------------------------
PathingGraph::~PathingGraph() 
{
	DestroyGraph(); 
}
//----------------------------------------------------------------------------
void PathingGraph::DestroyGraph()
{
	for (PathingNodeVec::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
		delete0 (*it);
	m_nodes.clear();

	for (PathingArcList::iterator it = m_arcs.begin(); it != m_arcs.end(); ++it)
		delete0 (*it);
	m_arcs.clear();
}
//----------------------------------------------------------------------------
PathingNode* PathingGraph::FindClosestNode(const Vector3f& pos)
{
	PathingNode* pClosestNode = m_nodes.front();
	float length = FLT_MAX;
	for (PathingNodeVec::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
	{
		PathingNode* pNode = *it;
		Vector3f diff = pos - pNode->GetPos();
		if (diff.Length() < length)
		{
			pClosestNode = pNode;
			length = diff.Length();
		}
	}

	return pClosestNode;
}
//----------------------------------------------------------------------------
PathingNode* PathingGraph::FindFurthestNode(const Vector3f& pos)
{
	PathingNode* pFurthestNode = m_nodes.front();
	float length = 0;
	for (PathingNodeVec::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
	{
		PathingNode* pNode = *it;
		Vector3f diff = pos - pNode->GetPos();
		if (diff.Length() > length)
		{
			pFurthestNode = pNode;
			length = diff.Length();
		}
	}

	return pFurthestNode;
}
//----------------------------------------------------------------------------
PathingNode* PathingGraph::FindRandomNode()
{
	unsigned int numNodes = (unsigned int)m_nodes.size();

	// choose a random node
	unsigned int node = Mathf::IntRandom(0, numNodes);

	// if we're in the lower half of the node list, start from the bottom
	if (node <= numNodes / 2)
	{
		PathingNodeVec::iterator it = m_nodes.begin();
		for (unsigned int i = 0; i < node; i++)
			++it;
		return (*it);
	}

	// otherwise, start from the top
	else
	{
		PathingNodeVec::iterator it = m_nodes.end();
		for (unsigned int i = numNodes; i >= node; i--)
			--it;
		return (*it);
	}
}
//----------------------------------------------------------------------------
PathPlan* PathingGraph::FindPath(const Vector3f& startPoint, 
	const Vector3f& endPoint)
{
	if (m_nodes.empty())
		return 0;

	PathingNode* pStart = FindClosestNode(startPoint);
	PathingNode* pGoal = FindClosestNode(endPoint);
	return FindPath(pStart, pGoal);
}
//----------------------------------------------------------------------------
PathPlan* PathingGraph::FindPath(const Vector3f& startPoint, 
	PathingNode* pGoalNode)
{
	PathingNode* pStart = FindClosestNode(startPoint);
	return FindPath(pStart, pGoalNode);
}
//----------------------------------------------------------------------------
PathPlan* PathingGraph::FindPath(PathingNode* pStartNode, 
	const Vector3f& endPoint)
{
	PathingNode* pGoal = FindClosestNode(endPoint);
	return FindPath(pStartNode, pGoal);
}
//----------------------------------------------------------------------------
PathPlan* PathingGraph::FindPath(PathingNode* pStartNode, 
	PathingNode* pGoalNode)
{
	AStar aStar;
	return aStar(pStartNode, pGoalNode);
}
//----------------------------------------------------------------------------
void PathingGraph::LinkNodes(PathingNode* pNodeA, PathingNode* pNodeB,
	float arcWeight)
{
	assert(pNodeA);
	assert(pNodeB);

	PathingArc* pArc = new0 PathingArc;
	pArc->LinkNodes(pNodeA, pNodeB);
	pNodeA->AddArc(pArc);
	pNodeB->AddArc(pArc);
	m_arcs.push_back(pArc);
	pArc->SetWeight(arcWeight);
}
//----------------------------------------------------------------------------
void PathingGraph::InitSlamMap(int mapSize, float resolution)
{
	DestroyGraph();

	m_nodes.clear();

	int x = 0;
	int y = 0;
	mMapSize = mapSize;
	int halfWidth = mMapSize * 0.5f;
	int sz = mapSize*mapSize;
	int index = 0;

	float adjustARC = PATHING_DEFAULT_ARC_WEIGHT * Mathf::Sqrt(2);

	for (int i = 0; i < sz; i++)
	{
		x = (i + mapSize) % mapSize;
		y = ((mapSize - 1) - i / mapSize);

		int iX = x - halfWidth;
		int iY = y - halfWidth;
		float posX = iX * resolution;
		float posY = iY * resolution;

		// add the new node
		PathingNode* pNode =
			new PathingNode(Vector3f(posX, posY, 0.0f));
		m_nodes.push_back(pNode);
		pNode->SetPathingGraph(this);

		// link it to the previous node
		int tempNodeLeft = index - 1;
		if (tempNodeLeft >= 0)
			LinkNodes(m_nodes[tempNodeLeft], pNode, PATHING_DEFAULT_ARC_WEIGHT);

		// link it to the node above it
		int tempNodeAbove = index - mapSize;  // reusing tempNode
		if (tempNodeAbove >= 0)
			LinkNodes(m_nodes[tempNodeAbove], pNode, PATHING_DEFAULT_ARC_WEIGHT);

		// link left above
		int tempLeftAbove = tempNodeLeft - mapSize;  // reusing left above
		if (tempLeftAbove >= 0)
			LinkNodes(m_nodes[tempLeftAbove], pNode, adjustARC);

		int tempRightAbove = index + 1 - mapSize;  // reusing right above
		if (tempRightAbove >= 0)
			LinkNodes(m_nodes[tempRightAbove], pNode, adjustARC);

		index++;
	}
}
//----------------------------------------------------------------------------
void PathingGraph::SetMapData(const std::vector<unsigned char> mapData,
	int mapDataSize, float mapDataResolution)
{
	mMapData = mapData;
	mMapDataWidth = mapDataSize;
	mMapDataResolution = mapDataResolution;
}
//----------------------------------------------------------------------------
int _Roundle1(float number)
{
	return (number > 0.0) ? floor(number + 0.5f) : ceil(number - 0.5f);
}
//----------------------------------------------------------------------------
bool PathingGraph::IsCanGo(const Vector3f &from1, const Vector3f &to1)
{
	if (mMapData.empty())
		return true;

	float width = mMapDataWidth * mMapDataResolution;

	Vector3f from;
	Vector3f to;

	if (from1.X() <= to1.X())
	{
		from = from1;
		to = to1;
	}
	else
	{
		from = to1;
		to = from1;
	}

	float xPerc = from.X() / width;
	float yPerc = from.Y() / width;
	xPerc += 0.5f;
	yPerc += 0.5f;
	float xPercTo = to.X() / width;
	float yPercTo = to.Y() / width;
	xPercTo += 0.5f;
	yPercTo += 0.5f;

	if (xPerc < 0.0f || xPerc > 1.0f)
		return false;

	if (yPerc < 0.0f || yPerc > 1.0f)
		return false;

	int x = xPerc * mMapDataWidth;
	int y = yPerc * mMapDataWidth;
	int xTo = xPercTo * mMapDataWidth;
	int yTo = yPercTo * mMapDataWidth;

	float xDist = (float)(xPercTo - xPerc);
	if (0.0f == xDist)
		return true;

	float k = (float)(yPercTo - yPerc) / (xPercTo - xPerc);

	for (int i = x; i < xTo; i++)
	{
		int y1 = _Roundle1(y + k*(i - x));
		int index = y1*mMapDataWidth + i;
		if (mMapData[index] != 0)
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------
int PathingGraph::GetMapSize() const
{
	return mMapSize;
}
//----------------------------------------------------------------------------
int PathingGraph::GetResolution() const
{
	return mResolution;
}
//----------------------------------------------------------------------------
void PathingGraph::SetArcValue(int nodeIndex, float val)
{
	if (m_nodes.empty())
		return;

	// obst
	PathingArcList &arcList = m_nodes[nodeIndex]->GetPathingArcList();
	auto it = arcList.begin();
	for (; it != arcList.end(); it++)
	{
		PathingArc *pathArc = *it;
		if (pathArc)
		{
			pathArc->SetWeight(val);
		}
	}
}
//----------------------------------------------------------------------------