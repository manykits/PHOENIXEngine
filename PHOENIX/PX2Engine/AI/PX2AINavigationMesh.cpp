// PX2AINavigationMesh.cpp

#include "PX2AINavigationMesh.hpp"
#include "PX2AINavigationUtilities.hpp"
#include "Recast.h"
#include "DetourNavMeshQuery.h"
using namespace PX2;

//----------------------------------------------------------------------------
NavigationMesh::NavigationMesh(
	rcConfig* config,
	const std::vector<AIAgentObject*>& objects)
{
	rcPolyMesh* polyMesh = NULL;
	rcPolyMeshDetail* polyMeshDetail = NULL;

	NavigationUtilities::CreatePolyMeshes(
		config, objects, polyMesh, polyMeshDetail);

	navMesh_ = NavigationUtilities::CreateNavMesh(
		config, *polyMesh, *polyMeshDetail);

	query_ = NavigationUtilities::CreateNavMeshQuery(navMesh_);

	NavigationUtilities::DestroyPolyMesh(polyMesh);
	NavigationUtilities::DestroyPolyMeshDetail(polyMeshDetail);
}
//----------------------------------------------------------------------------
NavigationMesh::~NavigationMesh()
{
	NavigationUtilities::DestroyNavMesh(*navMesh_);
	navMesh_ = 0;

	NavigationUtilities::DestroyNavMeshQuery(*query_);
}
//----------------------------------------------------------------------------
APoint NavigationMesh::FindClosestPoint(const APoint& point)
{
	return NavigationUtilities::FindClosestPoint(point, *query_);
}
//----------------------------------------------------------------------------
void NavigationMesh::FindPath(
	const APoint& start,
	const APoint& end,
	std::vector<APoint>& outPath)
{
	NavigationUtilities::FindStraightPath(start, end, *query_, outPath);
}
//----------------------------------------------------------------------------
Node* NavigationMesh::GetDebugNode() const
{
	return debugNode_;
}
//----------------------------------------------------------------------------
APoint NavigationMesh::RandomPoint()
{
	return NavigationUtilities::FindRandomPoint(*query_);
}
//----------------------------------------------------------------------------