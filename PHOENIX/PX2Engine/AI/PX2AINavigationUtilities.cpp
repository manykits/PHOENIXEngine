// PX2NavigationUtilities.cpp

#include "PX2AINavigationUtilities.hpp"
#include "Recast.h"
#include "RecastAlloc.h"
#include "PX2Vector3.hpp"
#include "PX2AxisAlignedBox3.hpp"
#include "PX2AIMeshUtilities.hpp"
#include "PX2TriMesh.hpp"
#include "DetourNavMeshQuery.h"
#include "PX2AIAgentWorld.hpp"
#include "DetourNavMeshBuilder.h"
using namespace PX2;

//----------------------------------------------------------------------------
MeshMetadata::MeshMetadata(
	unsigned char* const dataBuffer,
	const size_t dataBufferSize)
	: dataBuffer(dataBuffer),
	dataBufferSize(dataBufferSize)
{
	memset(dataBuffer, 0, sizeof(dataBuffer[0] * dataBufferSize));
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
dtNavMesh* NavigationUtilities::CreateNavMesh(
	const rcConfig *config,
	rcPolyMesh& polyMesh,
	rcPolyMeshDetail& polyMeshDetail)
{
	dtNavMesh* const navMesh = dtAllocNavMesh();

	dtNavMeshCreateParams params;
	memset(&params, 0, sizeof(params));
	params.verts = polyMesh.verts;
	params.vertCount = polyMesh.nverts;
	params.polys = polyMesh.polys;
	params.polyAreas = polyMesh.areas;
	params.polyFlags = polyMesh.flags;
	params.polyCount = polyMesh.npolys;
	params.nvp = polyMesh.nvp;
	params.detailMeshes = polyMeshDetail.meshes;
	params.detailVerts = polyMeshDetail.verts;
	params.detailVertsCount = polyMeshDetail.nverts;
	params.detailTris = polyMeshDetail.tris;
	params.detailTriCount = polyMeshDetail.ntris;
	params.offMeshConVerts = 0;
	params.offMeshConRad = 0;
	params.offMeshConDir = 0;
	params.offMeshConAreas = 0;
	params.offMeshConFlags = 0;
	params.offMeshConUserID = 0;
	params.offMeshConCount = 0;
	params.walkableHeight = config->walkableHeight * config->ch;
	params.walkableRadius = config->walkableRadius * config->cs;
	params.walkableClimb = config->walkableClimb * config->ch;
	params.bmin[0] = polyMesh.bmin[0];
	params.bmin[1] = polyMesh.bmin[1];
	params.bmin[2] = polyMesh.bmin[2];
	params.bmax[0] = polyMesh.bmax[0];
	params.bmax[1] = polyMesh.bmax[1];
	params.bmax[2] = polyMesh.bmax[2];
	params.cs = config->cs;
	params.ch = config->ch;
	params.buildBvTree = true;

	unsigned char* navData = 0;
	int navDataSize = 0;

	if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
	{
		dtFreeNavMesh(navMesh);
		return NULL;
	}

	dtStatus status;

	status = navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);

	if (dtStatusFailed(status))
	{
		dtFreeNavMesh(navMesh);
		dtFree(navData);
		return NULL;
	}

	return navMesh;
}
//----------------------------------------------------------------------------
void NavigationUtilities::CreatePolyMeshes(
	rcConfig *config,
	const std::vector<AIAgentObject*>& objects,
	rcPolyMesh*& polyMesh,
	rcPolyMeshDetail*& polyMeshDetail)
{
	rcContext context;

	SetBounds(config, objects);

	rcHeightfield* const heightfield = CreateHeightfield(&context, config);

	RasterizeSandboxObjects(&context, config, objects, heightfield);
	FilterWalkableSurfaces(&context, config, heightfield);

	rcCompactHeightfield* const compactHeightfield =
		CreateCompactHeightfield(&context, config, heightfield);

	ErodeWalkableArea(&context, config, compactHeightfield);

	BuildDistanceField(&context, config, compactHeightfield);

	BuildRegions(&context, config, compactHeightfield);

	rcContourSet* const contourSet = CreateContourSet(
		&context, config, compactHeightfield);

	polyMesh = CreatePolyMesh(&context, config, contourSet);

	polyMeshDetail = CreatePolyMeshDetail(
		&context, config, compactHeightfield, polyMesh);

	DestroyHeightfield(heightfield);
	DestroyCompactHeightfield(compactHeightfield);
	DestroyContourSet(contourSet);
}
//----------------------------------------------------------------------------
void NavigationUtilities::ErodeWalkableArea(
	rcContext *context,
	const rcConfig *config,
	rcCompactHeightfield *heightfield)
{
	rcErodeWalkableArea(context, config->walkableRadius, *heightfield);
}
//----------------------------------------------------------------------------
void NavigationUtilities::BuildDistanceField(
	rcContext *context,
	const rcConfig *config,
	rcCompactHeightfield *heightfield)
{
	rcBuildDistanceField(context, *heightfield);
}
//----------------------------------------------------------------------------
void NavigationUtilities::BuildRegions(
	rcContext *context,
	const rcConfig *config,
	rcCompactHeightfield *heightfield)
{
	rcBuildRegions(
		context,
		*heightfield,
		0,
		config->minRegionArea,
		config->mergeRegionArea);
}
//----------------------------------------------------------------------------
rcContourSet* NavigationUtilities::CreateContourSet(
	rcContext *context,
	const rcConfig *config,
	rcCompactHeightfield *heightfield)
{
	rcContourSet* contourSet = rcAllocContourSet();

	rcBuildContours(
		context,
		*heightfield,
		config->maxSimplificationError,
		config->maxEdgeLen,
		*contourSet);

	return contourSet;
}
//----------------------------------------------------------------------------
rcPolyMesh* NavigationUtilities::CreatePolyMesh(
	rcContext *context,
	const rcConfig *config,
	rcContourSet *contourSet)
{
	rcPolyMesh* polyMesh = rcAllocPolyMesh();

	rcBuildPolyMesh(context, *contourSet, config->maxVertsPerPoly, *polyMesh);

	for (int index = 0; index < polyMesh->npolys; ++index)
	{
		polyMesh->flags[index] = 1;
	}

	return polyMesh;
}
//----------------------------------------------------------------------------
rcPolyMeshDetail* NavigationUtilities::CreatePolyMeshDetail(
	rcContext *context,
	const rcConfig *config,
	rcCompactHeightfield *compactHeightfield,
	rcPolyMesh *polyMesh)
{
	rcPolyMeshDetail* const polyMeshDetail = rcAllocPolyMeshDetail();

	rcBuildPolyMeshDetail(
		context,
		*polyMesh,
		*compactHeightfield,
		config->detailSampleDist,
		config->detailSampleMaxError,
		*polyMeshDetail);

	return polyMeshDetail;
}
//----------------------------------------------------------------------------
void NavigationUtilities::FilterWalkableSurfaces(
	rcContext *context,
	const rcConfig *config,
	rcHeightfield *heightfield)
{
	rcFilterLowHangingWalkableObstacles(
		context, config->walkableClimb, *heightfield);
	rcFilterLedgeSpans(
		context, config->walkableHeight, config->walkableClimb, *heightfield);
	rcFilterWalkableLowHeightSpans(
		context, config->walkableHeight, *heightfield);
}
//----------------------------------------------------------------------------
rcCompactHeightfield* NavigationUtilities::CreateCompactHeightfield(
	rcContext *context, const rcConfig *config, rcHeightfield *heightfield)
{
	rcCompactHeightfield* const compactHeightfield =
		rcAllocCompactHeightfield();

	rcBuildCompactHeightfield(
		context,
		config->walkableHeight,
		config->walkableClimb,
		*heightfield,
		*compactHeightfield);

	return compactHeightfield;
}
//----------------------------------------------------------------------------
void NavigationUtilities::SetBounds(
	rcConfig *config,
	const std::vector<AIAgentObject*>& objects)
{
	Vector3f minimum;
	Vector3f maximum;

	for (std::vector<AIAgentObject*>::const_iterator it = objects.begin();
		it != objects.end(); ++it)
	{
		AxisAlignedBox3f box = (*it)->GetAIAgentWorld()->GetWorldAABB();

		const Vector3f currentMin = box.GetMin();

		if (currentMin.X() < minimum.X())
			minimum.X() = currentMin.X();

		if (currentMin.Y() < minimum.Y())
			minimum.Y() = currentMin.Y();

		if (currentMin.Z() < minimum.Z())
			minimum.Z() = currentMin.Z();

		const Vector3f& currentMax = box.GetMax();

		if (currentMax.X() > maximum.X())
			maximum.X() = currentMax.X();

		if (currentMax.Y() > maximum.Y())
			maximum.Y() = currentMax.Y();

		if (currentMax.Z() > maximum.Z())
			maximum.Z() = currentMax.Z();
	}
}
//----------------------------------------------------------------------------
void NavigationUtilities::RasterizeSandboxObjects(
	rcContext *context, const rcConfig *config,
	const std::vector<AIAgentObject*>& objects,
	rcHeightfield *heightfield)
{
	size_t maxVertexCount = 0;
	size_t maxIndexCount = 0;

	for (std::vector<AIAgentObject*>::const_iterator it = objects.begin();
		it != objects.end(); ++it)
	{
		AIAgentObject* object = *it;

		AIAgentWorld *agentWorld = object->GetAIAgentWorld();
		if (agentWorld)
		{
			// Currently only process the first attached mesh.
			TriMesh* mesh = 0;

			if (mesh)
			{
				size_t vertexCount;
				size_t indexCount;

				MeshUtilities::GetMeshInformation(
					*mesh, vertexCount, indexCount);

				if (vertexCount > maxVertexCount)
				{
					maxVertexCount = vertexCount;
					maxIndexCount = indexCount;
				}
			}
		}
	}

	if (maxVertexCount == 0 || maxIndexCount == 0)
	{
		return;
	}

	float* const vertexBuffer = new float[maxVertexCount * 3];
	int* const indexBuffer = new int[maxIndexCount];
	assert(!(maxIndexCount % 3));
	unsigned char* const dataBuffer = new unsigned char[maxIndexCount / 3];

	for (std::vector<AIAgentObject*>::const_iterator it = objects.begin();
		it != objects.end(); ++it)
	{
		AIAgentObject* object = *it;

		AIAgentWorld *agentWorld = object->GetAIAgentWorld();

		if (agentWorld)
		{
			TriMesh * mesh = 0;
			if (mesh)
			{
				RawMesh rawMesh(
					vertexBuffer,
					maxVertexCount * 3,
					indexBuffer,
					maxIndexCount,
					object->GetPosition(),
					object->GetOrientation());

				MeshUtilities::ConvertToRawMesh(*mesh, rawMesh);

				MeshMetadata metadata(dataBuffer, maxIndexCount / 3);

				RasterizeMesh(context, config, rawMesh, metadata, heightfield);
			}
		}
	}

	delete vertexBuffer;
	delete indexBuffer;
	delete dataBuffer;
}
//----------------------------------------------------------------------------
rcHeightfield* NavigationUtilities::CreateHeightfield(
	rcContext* context, const rcConfig* config)
{
	rcHeightfield* const heightField = rcAllocHeightfield();

	rcCreateHeightfield(
		context,
		*heightField,
		config->width,
		config->height,
		config->bmin,
		config->bmax,
		config->cs,
		config->ch);

	return heightField;
}
//----------------------------------------------------------------------------
void NavigationUtilities::RasterizeMesh(
	rcContext *context,
	const rcConfig *config,
	const RawMesh& mesh,
	const MeshMetadata& metadata,
	rcHeightfield *heightfield)
{
	memset(metadata.dataBuffer, 0,
		metadata.dataBufferSize * sizeof(metadata.dataBuffer[0]));

	assert(mesh.indexBufferSize / 3 >= metadata.dataBufferSize);

	rcMarkWalkableTriangles(
		context,
		config->walkableSlopeAngle,
		mesh.vertexBuffer,
		static_cast<int>(mesh.vertexBufferSize),
		mesh.indexBuffer,
		static_cast<int>(mesh.indexBufferSize / 3),
		metadata.dataBuffer);

	rcRasterizeTriangles(
		context,
		mesh.vertexBuffer,
		static_cast<int>(mesh.vertexBufferSize),
		mesh.indexBuffer,
		metadata.dataBuffer,
		static_cast<int>(mesh.indexBufferSize / 3),
		*heightfield,
		config->walkableClimb);
}
//----------------------------------------------------------------------------
dtNavMeshQuery* NavigationUtilities::CreateNavMeshQuery(
	const dtNavMesh *navMesh)
{
	dtNavMeshQuery* query = dtAllocNavMeshQuery();
	query->init(navMesh, 2048);
	return query;
}
//----------------------------------------------------------------------------
void NavigationUtilities::DestroyNavMesh(dtNavMesh& navMesh)
{
	dtFreeNavMesh(&navMesh);
}
//----------------------------------------------------------------------------
void NavigationUtilities::DestroyNavMeshQuery(dtNavMeshQuery& navMeshQuery)
{
	dtFreeNavMeshQuery(&navMeshQuery);
}
//----------------------------------------------------------------------------
void NavigationUtilities::DestroyPolyMesh(rcPolyMesh *polyMesh)
{
	rcFreePolyMesh(polyMesh);
}
//----------------------------------------------------------------------------
void NavigationUtilities::DestroyPolyMeshDetail(
	rcPolyMeshDetail *polyMeshDetail)
{
	rcFreePolyMeshDetail(polyMeshDetail);
}
//----------------------------------------------------------------------------
void NavigationUtilities::DestroyCompactHeightfield(
	rcCompactHeightfield *heightfield)
{
	rcFreeCompactHeightfield(heightfield);
}
//----------------------------------------------------------------------------
void NavigationUtilities::DestroyContourSet(rcContourSet *countourSet)
{
	rcFreeContourSet(countourSet);
}
//----------------------------------------------------------------------------
void NavigationUtilities::DestroyHeightfield(rcHeightfield *heightfield)
{
	rcFreeHeightField(heightfield);
}
//----------------------------------------------------------------------------
APoint NavigationUtilities::FindClosestPoint(
	const APoint& point, const dtNavMeshQuery& query)
{
	const float center[3] = { point.X(), point.Y(), point.Z() };
	const float extents[3] = { 0.5, 2, 0.5 };

	dtQueryFilter filter;

	dtPolyRef poly;
	float nearestPoint[3];

	dtStatus status = query.findNearestPoly(
		center, extents, &filter, &poly, nearestPoint);

	if (dtStatusSucceed(status))
	{
		return Vector3f(point[0], point[1], point[2]);
	}
	return point;
}
//----------------------------------------------------------------------------
float navRand()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
//----------------------------------------------------------------------------
APoint NavigationUtilities::FindRandomPoint(const dtNavMeshQuery& query)
{
	dtQueryFilter filter;

	dtPolyRef poly;
	float point[3];

	query.findRandomPoint(&filter, navRand, &poly, point);

	return Vector3f(point[0], point[1], point[2]);
}
//----------------------------------------------------------------------------
void NavigationUtilities::FindStraightPath(
	const APoint& start,
	const APoint& end,
	const dtNavMeshQuery& query,
	std::vector<APoint>& outPath)
{
	dtStatus status;

	const float startPoint[] = { start.X(), start.Y(), start.Z()};
	const float endPoint[] = { end.X(), end.Y(), end.Z() };

	dtPolyRef startPoly;
	dtPolyRef endPoly;
	float startPolyPoint[3];
	float endPolyPoint[3];

	const float extents[] = { 2, 5, 2 };

	dtQueryFilter filter;

	outPath.clear();

	status = query.findNearestPoly(
		startPoint, extents, &filter, &startPoly, startPolyPoint);

	if (dtStatusFailed(status))
	{
		return;
	}

	status = query.findNearestPoly(
		endPoint, extents, &filter, &endPoly, endPolyPoint);

	if (dtStatusFailed(status))
	{
		return;
	}

	dtPolyRef path[128];
	int pathCount = 0;

	status = query.findPath(
		startPoly,
		endPoly,
		startPoint,
		endPoint,
		&filter,
		path,
		&pathCount,
		sizeof(path));

	if (dtStatusFailed(status))
	{
		return;
	}

	// No path found.
	if (!pathCount || path[pathCount - 1] != endPoly)
	{
		return;
	}

	const size_t pathLength = 256;
	float straightPath[pathLength * 3];
	int straightPathCount;

	status = query.findStraightPath(
		startPolyPoint,
		endPolyPoint,
		path,
		pathCount,
		straightPath,
		0,
		0,
		&straightPathCount,
		pathLength,
		DT_STRAIGHTPATH_ALL_CROSSINGS);

	if (dtStatusFailed(status))
	{
		return;
	}

	outPath.resize(straightPathCount);

	for (int index = 0; index < straightPathCount; ++index)
	{
		APoint& point = outPath[index];

		point.X() = straightPath[index * 3];
		point.Y() = straightPath[index * 3 + 1];
		point.Z() = straightPath[index * 3 + 2];
	}
}
//----------------------------------------------------------------------------