#ifndef PX2AIPATH_HPP
#define PX2AIPATH_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector3.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PathingArc;
	class PathingNode;
	class PathPlanNode;
	class AStar;

	typedef std::list<PathingArc*> PathingArcList;
	typedef std::list<PathingNode*> PathingNodeList;
	typedef std::vector<PathingNode*> PathingNodeVec;
	typedef std::map<PathingNode*, PathPlanNode*> PathingNodeToPathPlanNodeMap;
	typedef std::list<PathPlanNode*> PathPlanNodeList;

	const float PATHING_DEFAULT_NODE_TOLERANCE = 0.2f;
	const float PATHING_DEFAULT_ARC_WEIGHT = 1.0f;


	class PathingGraph;

	class PathingNode
	{
		float m_tolerance;
		Vector3f m_pos;
		PathingArcList m_arcs;
		PathingGraph *ThePathingGraph;

	public:
		explicit PathingNode(const Vector3f& pos, float tolerance = PATHING_DEFAULT_NODE_TOLERANCE);
		const Vector3f& GetPos() const { return m_pos; }
		float GetTolerance() const { return m_tolerance; }
		void AddArc(PathingArc* pArc);
		PathingArcList &GetPathingArcList();
		void GetNeighbors(PathingNodeList& outNeighbors);
		float GetCostFromNode(PathingNode* pFromNode);

		PathingGraph *GetPathingGraph();

	public_internal :
		void SetPathingGraph(PathingGraph *graph);

	private:
		PathingArc* FindArc(PathingNode* pLinkedNode);
	};

	class PathingArc
	{
		float m_weight;
		PathingNode* m_pNodes[2];

	public:
		explicit PathingArc(float weight = PATHING_DEFAULT_ARC_WEIGHT);
		void SetWeight(float weight);
		float GetWeight(void) const { return m_weight; }
		void LinkNodes(PathingNode* pNodeA, PathingNode* pNodeB);
		PathingNode* GetNeighbor(PathingNode* pMe);
	};

	class PathPlan
	{
		friend class AStar;

	public:
		PathingNodeList m_path;
		PathingNodeList m_pathOptmize;
		PathingNodeList::iterator m_index;

	public:
		PathPlan() { m_index = m_path.end(); }

		void ResetPath() { m_index = m_path.begin(); }
		const Vector3f& GetCurrentNodePosition() const;
		bool CheckForNextNode(const Vector3f& pos);
		bool CheckForEnd();

		bool IsCanGo(PathingNode *nodeFrom, PathingNode *nodeTo);
		void Optmize();
		
	private:
		void AddNode(PathingNode* pNode);
	};


	class PathPlanNode
	{
		PathPlanNode* m_pPrev;
		PathingNode* m_pPathingNode;
		PathingNode* m_pGoalNode;
		bool m_closed;
		float m_goal;
		float m_heuristic;
		float m_fitness;

	public:
		explicit PathPlanNode(PathingNode* pNode, PathPlanNode* pPrevNode, PathingNode* pGoalNode);
		PathPlanNode* GetPrev() const { return m_pPrev; }
		PathingNode* GetPathingNode() const { return m_pPathingNode; }
		bool IsClosed() const { return m_closed; }
		float GetGoal() const { return m_goal; }
		float GetHeuristic() const { return m_heuristic; }
		float GetFitness() const { return m_fitness; }

		void UpdatePrevNode(PathPlanNode* pPrev);
		void SetClosed(bool toClose = true) { m_closed = toClose; }

		bool IsBetterChoiceThan(PathPlanNode* pRight) { return (m_fitness < pRight->GetFitness()); }

	private:
		void UpdateHeuristics();
	};

	class AStar
	{
		PathingNodeToPathPlanNodeMap m_nodes;
		PathingNode* m_pStartNode;
		PathingNode* m_pGoalNode;
		PathPlanNodeList m_openSet;

	public:
		AStar();
		~AStar();
		void Destroy();

		PathPlan* operator()(PathingNode* pStartNode, PathingNode* pGoalNode);

	private:
		PathPlanNode* AddToOpenSet(PathingNode* pNode, PathPlanNode* pPrevNode);
		void AddToClosedSet(PathPlanNode* pNode);
		void InsertNode(PathPlanNode* pNode);
		void ReinsertNode(PathPlanNode* pNode);
		PathPlan* RebuildPath(PathPlanNode* pGoalNode);
	};
	typedef Pointer0<PathPlan> PathPlanPtr;

	class PathingGraph
	{
		PathingNodeVec m_nodes;  // master list of all nodes
		PathingArcList m_arcs;  // master list of all arcs

	public:
		PathingGraph();
		~PathingGraph();

		void InitSlamMap(int mapSize, float resolution);
		void SetMapData(const std::vector<unsigned char> mapData, int mapDataSize,
			float mapDataResolution);
		bool IsCanGo(const Vector3f &from, const Vector3f &to);

		int GetMapSize() const;
		int GetResolution() const;
		void DestroyGraph();

		PathingNode* FindClosestNode(const Vector3f& pos);
		PathingNode* FindFurthestNode(const Vector3f& pos);
		PathingNode* FindRandomNode();
		PathPlan* FindPath(const Vector3f& startPoint, const Vector3f& endPoint);
		PathPlan* FindPath(const Vector3f& startPoint, PathingNode* pGoalNode);
		PathPlan* FindPath(PathingNode* pStartNode, const Vector3f& endPoint);
		PathPlan* FindPath(PathingNode* pStartNode, PathingNode* pGoalNode);

		void SetArcValue(int nodeIndex, float val);

	private:
		void LinkNodes(PathingNode* pNodeA, PathingNode* pNodeB, float arcWeight);

		int mMapSize;
		float mResolution;
		std::vector<unsigned char> mMapData;
		int mMapDataWidth;
		float mMapDataResolution;
	};
	typedef Pointer0<PathingGraph> PathingGraphPtr;

}

#endif