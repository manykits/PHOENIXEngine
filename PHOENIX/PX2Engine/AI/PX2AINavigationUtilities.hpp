// PX2NavigationUtilities.hpp

#ifndef PX2NAVIGATIONUTILITIES_HPP
#define PX2NAVIGATIONUTILITIES_HPP

#include "PX2CorePre.hpp"
#include "PX2AIAgentObject.hpp"

class dtNavMesh;
struct rcConfig;
struct rcPolyMesh;
class rcPolyMeshDetail;
class rcContext;
class dtNavMeshQuery;
struct rcHeightfield;
struct rcCompactHeightfield;
struct rcContourSet;

namespace PX2
{

	class RawMesh;

	struct MeshMetadata
	{
		unsigned char* const dataBuffer;
		const size_t dataBufferSize;

		MeshMetadata(
			unsigned char* const dataBuffer,
			const size_t dataBufferSize);
		MeshMetadata(const MeshMetadata& data);

	private:
		MeshMetadata& operator=(const MeshMetadata&);
	};

	class NavigationUtilities
	{
	public:
		static dtNavMesh* CreateNavMesh(
			const rcConfig *config,
			rcPolyMesh& polyMesh,
			rcPolyMeshDetail& polyMeshDetail);

		static void CreatePolyMeshes(
			rcConfig *config,
			const std::vector<AIAgentObject*>& objects,
			rcPolyMesh*& polyMesh,
			rcPolyMeshDetail*& polyMeshDetail);

		static rcContourSet* CreateContourSet(
			rcContext *context,
			const rcConfig *config,
			rcCompactHeightfield *heightfield);

		static rcPolyMesh* CreatePolyMesh(
			rcContext *context,
			const rcConfig *config,
			rcContourSet *contourSet);

		static rcPolyMeshDetail* CreatePolyMeshDetail(
			rcContext *context,
			const rcConfig *config,
			rcCompactHeightfield *compactHeightfield,
			rcPolyMesh *polyMesh);

		static void FilterWalkableSurfaces(
			rcContext *context,
			const rcConfig *config,
			rcHeightfield *heightfield);

		static void ErodeWalkableArea(
			rcContext *context,
			const rcConfig *config,
			rcCompactHeightfield *heightfield);

		static void BuildDistanceField(
			rcContext *context,
			const rcConfig *config,
			rcCompactHeightfield *heightfield);

		static void BuildRegions(
			rcContext *context,
			const rcConfig *config,
			rcCompactHeightfield *heightfield);

		static rcCompactHeightfield* CreateCompactHeightfield(
			rcContext *context, const rcConfig *config, rcHeightfield *heightfield);

		static void RasterizeSandboxObjects(
			rcContext *context,
			const rcConfig *config,
			const std::vector<AIAgentObject*>& objects,
			rcHeightfield *heightfield);

		static void SetBounds(
			rcConfig *config,
			const std::vector<AIAgentObject*>& objects);

		static rcHeightfield* CreateHeightfield(
			rcContext* context, const rcConfig* config);

		static void RasterizeMesh(
			rcContext *context,
			const rcConfig *config,
			const RawMesh& mesh,
			const MeshMetadata& metadata,
			rcHeightfield *heightfield);

		static dtNavMeshQuery* CreateNavMeshQuery(const dtNavMesh *navMesh);

		static void DestroyNavMesh(dtNavMesh& navMesh);
		static void DestroyNavMeshQuery(dtNavMeshQuery& navMeshQuery);
		static void DestroyPolyMesh(rcPolyMesh *polyMesh);
		static void DestroyPolyMeshDetail(rcPolyMeshDetail *polyMeshDetail);

		static void DestroyCompactHeightfield(rcCompactHeightfield *heightfield);
		static void DestroyContourSet(rcContourSet *countourSet);
		static void DestroyHeightfield(rcHeightfield *heightfield);

		static APoint FindClosestPoint(
			const APoint& point, const dtNavMeshQuery& query);

		static APoint FindRandomPoint(const dtNavMeshQuery& query);

		static void FindStraightPath(
			const APoint& start,
			const APoint& end,
			const dtNavMeshQuery& query,
			std::vector<APoint>& outPath);
	};

}

#endif