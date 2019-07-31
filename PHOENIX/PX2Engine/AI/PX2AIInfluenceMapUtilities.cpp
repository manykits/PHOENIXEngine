// PX2AIInfluenceMapUtilities.cpp

#include "PX2AIInfluenceMapUtilities.hpp"
#include "PX2AxisAlignedBox3.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
InfluenceMapGrid* InfluenceMapUtilities::BuildInfluenceMapGrid(
	const InfluenceMapConfig& config, const Node& sceneNode)
{
	InfluenceMapGrid* const mapGrid = new InfluenceMapGrid();
	
	AxisAlignedBox3f aabb;// = sceneNode._getWorldAABB();

	const Vector3f min = Vector3f(aabb.Min[0], aabb.Min[1], aabb.Min[2]);
	const Vector3f max = Vector3f(aabb.Max[0], aabb.Max[1], aabb.Max[2]);

	mapGrid->boundaryMin[0] = min.X() + config.boundaryMin[0];
	mapGrid->boundaryMin[1] = min.Y() + config.boundaryMin[1];
	mapGrid->boundaryMin[2] = min.Z() + config.boundaryMin[2];

	mapGrid->boundaryMax[0] += max.X() + config.boundaryMax[0] + config.cellWidth;
	mapGrid->boundaryMax[1] += max.Y() + config.boundaryMax[1] + config.cellHeight;
	mapGrid->boundaryMax[2] += max.Z() + config.boundaryMax[2] + config.cellWidth;

	mapGrid->cellHeight = config.cellHeight;
	mapGrid->cellWidth = config.cellWidth;

	const float xDiff = abs(mapGrid->boundaryMax[0] - mapGrid->boundaryMin[0]);
	const float yDiff = abs(mapGrid->boundaryMax[1] - mapGrid->boundaryMin[1]);
	const float zDiff = abs(mapGrid->boundaryMax[2] - mapGrid->boundaryMin[2]);

	// Calculate number of cells in each dimension, rounding up.
	mapGrid->xCellCount = static_cast<size_t>(xDiff / mapGrid->cellWidth);
	mapGrid->xCellCount +=
		static_cast<size_t>(ceil(xDiff - mapGrid->xCellCount * mapGrid->cellWidth));

	mapGrid->yCellCount = static_cast<size_t>(yDiff / mapGrid->cellHeight);
	mapGrid->yCellCount +=
		static_cast<size_t>(ceil(yDiff - mapGrid->yCellCount * mapGrid->cellHeight));

	mapGrid->zCellCount = static_cast<size_t>(zDiff / mapGrid->cellWidth);
	mapGrid->zCellCount +=
		static_cast<size_t>(ceil(zDiff - mapGrid->zCellCount * mapGrid->cellWidth));

	mapGrid->cellCount =
		mapGrid->xCellCount * mapGrid->yCellCount * mapGrid->zCellCount;
	assert(mapGrid->cellCount);

	mapGrid->cells = new InfluenceMapCell[mapGrid->cellCount];
	assert(mapGrid->cells);

	memset(mapGrid->cells, 0, sizeof(InfluenceMapCell) * mapGrid->cellCount);

	//// Currently only process the first attached mesh.
	//const Ogre::Mesh* const mesh = MeshUtilities::GetMesh(sceneNode);

	//if (mesh)
	//{
	//	size_t vertexCount;
	//	size_t indexCount;

	//	MeshUtilities::GetMeshInformation(
	//		*mesh, vertexCount, indexCount);

	//	float* const vertexBuffer = new float[vertexCount * 3];
	//	int* const indexBuffer = new int[indexCount];

	//	RawMesh rawMesh(
	//		vertexBuffer,
	//		vertexCount * 3,
	//		indexBuffer,
	//		indexCount,
	//		sceneNode._getDerivedPosition(),
	//		sceneNode._getDerivedOrientation());

	//	MeshUtilities::ConvertToRawMesh(*mesh, rawMesh);

	//	const float boxHalfSize[3] = {
	//		mapGrid->cellWidth / 2,
	//		mapGrid->cellHeight / 2,
	//		mapGrid->cellWidth / 2
	//	};

	//	float boxCenter[3];
	//	float triVerts[3][3];

	//	size_t vertIndex1;
	//	size_t vertIndex2;
	//	size_t vertIndex3;

	//	for (size_t zIndex = 0; zIndex < mapGrid->zCellCount; ++zIndex)
	//	{
	//		for (size_t yIndex = 0; yIndex < mapGrid->yCellCount; ++yIndex)
	//		{
	//			for (size_t xIndex = 0; xIndex < mapGrid->xCellCount; ++xIndex)
	//			{
	//				InfluenceMapCell& cell = mapGrid->cells[
	//					xIndex + yIndex * mapGrid->xCellCount +
	//						zIndex * mapGrid->xCellCount * mapGrid->yCellCount];

	//				if (cell.used)
	//				{
	//					continue;
	//				}

	//				boxCenter[0] =
	//					mapGrid->boundaryMin[0] +
	//					xIndex * mapGrid->cellWidth +
	//					boxHalfSize[0];
	//				boxCenter[1] =
	//					mapGrid->boundaryMin[1] +
	//					yIndex * mapGrid->cellHeight +
	//					boxHalfSize[1];
	//				boxCenter[2] =
	//					mapGrid->boundaryMin[2] +
	//					zIndex * mapGrid->cellWidth +
	//					boxHalfSize[2];

	//				// Check to see if any triangles intersect with the
	//				// current grid cell.
	//				for (size_t index = 0; index < indexCount; index += 3)
	//				{
	//					vertIndex1 = indexBuffer[index] * 3;
	//					vertIndex2 = indexBuffer[index + 1] * 3;
	//					vertIndex3 = indexBuffer[index + 2] * 3;

	//					triVerts[0][0] = vertexBuffer[vertIndex1];
	//					triVerts[0][1] = vertexBuffer[vertIndex1 + 1];
	//					triVerts[0][2] = vertexBuffer[vertIndex1 + 2];

	//					triVerts[1][0] = vertexBuffer[vertIndex2];
	//					triVerts[1][1] = vertexBuffer[vertIndex2 + 1];
	//					triVerts[1][2] = vertexBuffer[vertIndex2 + 2];

	//					triVerts[2][0] = vertexBuffer[vertIndex3];
	//					triVerts[2][1] = vertexBuffer[vertIndex3 + 1];
	//					triVerts[2][2] = vertexBuffer[vertIndex3 + 2];

	//					if (triBoxOverlap(
	//						boxCenter,
	//						const_cast<float*>(boxHalfSize),
	//						triVerts))
	//					{
	//						// Found an intersection, skip all other tests.
	//						cell.used = true;
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}

	//	delete vertexBuffer;
	//	delete indexBuffer;
	//}

	return mapGrid;
}
//----------------------------------------------------------------------------
float InfluenceMapUtilities::ClampFalloff(const float falloff)
{
	return std::max(std::min(falloff, 1.0f), 0.0f);
}
//----------------------------------------------------------------------------
float InfluenceMapUtilities::ClampInertia(const float inertia)
{
	return std::max(std::min(inertia, 1.0f), 0.0f);
}
//----------------------------------------------------------------------------
void InfluenceMapUtilities::ClearInfluences(InfluenceMap& influenceMap)
{
	for (size_t index = 0; index < MAX_INFLUENCE_LAYERS; ++index)
	{
		influenceMap.ClearInfluence(index);
	}
}
//----------------------------------------------------------------------------
InfluenceMapCell* InfluenceMapUtilities::GetCell(
	const InfluenceMapGrid& grid, const Vector3f& position)
{
	if (InGridBounds(grid, position))
	{
		size_t cellIndex;

		if (GetCellIndex(grid, position, &cellIndex))
		{
			return &grid.cells[cellIndex];
		}
	}

	return NULL;
}
//----------------------------------------------------------------------------
bool InfluenceMapUtilities::InGridBounds(
	const InfluenceMapGrid& grid, const Vector3f& position)
{
	return grid.boundaryMin[0] <= position.X() &&
		grid.boundaryMax[0] >= position.X() &&
		grid.boundaryMin[1] <= position.Y() &&
		grid.boundaryMax[1] >= position.Y() &&
		grid.boundaryMin[2] <= position.Z() &&
		grid.boundaryMax[2] >= position.Z();
}
//----------------------------------------------------------------------------
bool InfluenceMapUtilities::GetCellIndex(const InfluenceMapGrid& grid,
	const Vector3f& position,
	size_t* const cellIndex)
{
	assert(cellIndex);

	if (InGridBounds(grid, position))
	{
		size_t xCellIndex;
		size_t yCellIndex;
		size_t zCellIndex;

		GetCellIndexes(grid, position, &xCellIndex, &yCellIndex, &zCellIndex);

		*cellIndex = xCellIndex +
			yCellIndex * grid.xCellCount +
			zCellIndex * grid.xCellCount * grid.yCellCount;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool InfluenceMapUtilities::GetCellIndexes(
	const InfluenceMapGrid& grid,
	const Vector3f& position,
	size_t* xCellIndex,
	size_t* yCellIndex,
	size_t* zCellIndex)
{
	assert(xCellIndex && yCellIndex && zCellIndex);

	if (InGridBounds(grid, position))
	{
		*xCellIndex = static_cast<size_t>((position.X() - grid.boundaryMin[0]) /
			grid.cellWidth);
		*yCellIndex = static_cast<size_t>((position.Y() - grid.boundaryMin[1]) /
			grid.cellHeight);
		*zCellIndex = static_cast<size_t>((position.Z() - grid.boundaryMin[2]) /
			grid.cellWidth);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
inline static float GetInfluenceAtCell(const InfluenceMapGrid& grid,
	const size_t layer,	const int xCellIndex, const int yCellIndex,
	const int zCellIndex)
{
	if (xCellIndex >= 0 && xCellIndex < static_cast<int>(grid.xCellCount) &&
		yCellIndex >= 0 && yCellIndex < static_cast<int>(grid.yCellCount) &&
		zCellIndex >= 0 && zCellIndex < static_cast<int>(grid.zCellCount))
	{
		const InfluenceMapCell* const cell = &grid.cells[
			xCellIndex +
				yCellIndex * grid.xCellCount +
				zCellIndex * grid.xCellCount * grid.yCellCount];

		if (cell->used)
		{
			return cell->values[layer];
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
void InfluenceMapUtilities::UpdateInfluenceGrid(InfluenceMapGrid& grid,
	const size_t layer, const float inertia, const float falloff)
{
	// 3D offsets from each cell used for calculating influence to propagate.
	static const int filterCells[3][9][3] =
	{
		{
			{ -1, 1, 1 }, { 0, 1, 1 }, { 1, 1, 1 },
			{ -1, 1, 0 }, { 0, 1, 0 }, { 1, 1, 0 },
			{ -1, 1, -1 }, { 0, 1, -1 }, { 1, 1, -1 }
		},
		{
			{ -1, 0, 1 }, { 0, 0, 1 }, { 1, 0, 1 },
			{ -1, 0, 0 }, { 0, 0, 0 }, { 1, 0, 0 },
			{ -1, 0, -1 }, { 0, 0, -1 }, { 1, 0, -1 }
		},
		{
			{ -1, -1, 1 }, { 0, -1, 1 }, { 1, -1, 1 },
			{ -1, -1, 0 }, { 0, -1, 0 }, { 1, -1, 0 },
			{ -1, -1, -1 }, { 0, -1, -1 }, { 1, -1, -1 }
		}
	};

	// Precalculated distances to each 3D offset cell.
	static const float distances[3][9] =
	{
		{
			sqrtf(3), sqrtf(2), sqrtf(3),
			sqrtf(2), 1, sqrtf(2),
			sqrtf(3), sqrtf(2), sqrtf(3)
		},
		{
			sqrtf(2), 1, sqrtf(2),
			1, 0, 1,
			sqrtf(2), 1, sqrtf(2)
		},
		{
			sqrtf(3), sqrtf(2), sqrtf(3),
			sqrtf(2), 1, sqrtf(2),
			sqrtf(3), sqrtf(2), sqrtf(3)
		}
	};

	unsigned int xCell = 0;
	unsigned int yCell = 0;
	unsigned int zCell = 0;

	for (size_t index = 0; index < grid.cellCount; ++index)
	{
		InfluenceMapCell* const cell = &grid.cells[index];

		if (cell->used)
		{
			float maxPosInfluence = 0.0f;
			float maxNegInfluence = 0.0f;

			for (size_t layerIndex = 0; layerIndex < 3; ++layerIndex)
			{
				for (size_t layerCell = 0; layerCell < 9; ++layerCell)
				{
					// Exponential falloff based on distance.
					const float cellFalloff =
						powf((1.0f - falloff), distances[layerIndex][layerCell]);

					// Ignore cells with no contribution.
					if (cellFalloff <= 0.0f)
						continue;

					const float cellInfluence = GetInfluenceAtCell(
						grid,
						layer,
						filterCells[layerIndex][layerCell][0] + xCell,
						filterCells[layerIndex][layerCell][1] + yCell,
						filterCells[layerIndex][layerCell][2] + zCell);

					// Choose the maximum influence to propagate.
					maxPosInfluence =
						std::max(cellInfluence * cellFalloff, maxPosInfluence);

					// Choose the minimum influence to propagate.
					maxNegInfluence =
						std::min(cellInfluence * cellFalloff, maxNegInfluence);
				}
			}

			// Add both influences, this works since they are initialized to 0.
			const float averageInfluence = maxPosInfluence + maxNegInfluence;

			// Interpolate between the current cell value and the propagated
			// value.  Use double buffering for accurate calculations.
			cell->buffer = std::min(
				std::max(
				cell->values[layer] * inertia + averageInfluence * (1.0f - inertia),
				-1.0f),
				1.0f);
		}

		xCell += 1;
		yCell += (xCell >= grid.xCellCount) ? 1 : 0;
		zCell += (yCell >= grid.yCellCount) ? 1 : 0;

		xCell %= grid.xCellCount;
		yCell %= grid.yCellCount;
	}

	for (size_t index = 0; index < grid.cellCount; ++index)
	{
		InfluenceMapCell* const cell = &grid.cells[index];

		cell->values[layer] = cell->buffer;
	}
}
//----------------------------------------------------------------------------