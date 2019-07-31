// PX2AINavigationMesh.hpp

#ifndef PX2AINAVIGATIONMESH_HPP
#define PX2AINAVIGATIONMESH_HPP

#include "PX2CorePre.hpp"
#include "PX2APoint.hpp"

struct rcConfig;
class dtNavMesh;
class dtNavMeshQuery;

namespace PX2
{

	class AIAgentObject;
	class Node;

	class PX2_ENGINE_ITEM NavigationMesh
	{
	public:
		NavigationMesh(
			rcConfig* config,
			const std::vector<AIAgentObject*>& objects);

		~NavigationMesh();

		APoint FindClosestPoint(const APoint& point);

		void FindPath(
			const APoint& start,
			const APoint& end,
			std::vector<APoint>& outPath);

		Node* GetDebugNode() const;

		APoint RandomPoint();

	private:
		dtNavMesh* navMesh_;
		dtNavMeshQuery* query_;
		Node* debugNode_;
	};

}

#endif