// PX2AIInfluenceMap.cpp

#include "PX2AIInfluenceMap.hpp"
#include "PX2AIInfluenceMapUtilities.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
InfluenceMap::InfluenceMap(
	const InfluenceMapConfig& config, const Node &mesh)
{
	grid_ = InfluenceMapUtilities::BuildInfluenceMapGrid(config, mesh);

	memset(inertia_, 0, sizeof(inertia_[0]) * MAX_INFLUENCE_LAYERS);
	memset(falloff_, 0, sizeof(falloff_[0]) * MAX_INFLUENCE_LAYERS);
}
//----------------------------------------------------------------------------
InfluenceMap::~InfluenceMap()
{
	delete grid_->cells;
	delete grid_;
}
//----------------------------------------------------------------------------
void InfluenceMap::ClearInfluence(const size_t layer)
{
	if (layer < GetInfluenceLayers())
	{
		for (size_t index = 0; index < grid_->cellCount; ++index)
		{
			grid_->cells[index].values[layer] = 0.0f;
		}
	}
}
//----------------------------------------------------------------------------
const InfluenceMapCell* InfluenceMap::GetCellAt(const size_t index) const
{
	if (index < grid_->cellCount)
	{
		return &grid_->cells[index];
	}

	return 0;
}
//----------------------------------------------------------------------------
const InfluenceMapCell* InfluenceMap::GetCellAt(const Vector3f & position) const
{
	return InfluenceMapUtilities::GetCell(*grid_, position);
}
//----------------------------------------------------------------------------
float InfluenceMap::GetInfluenceAt(const Vector3f& position, 
	const size_t layer) const
{
	const InfluenceMapCell* const cell = GetCellAt(position);

	if (cell && layer < GetInfluenceLayers())
	{
		return cell->values[layer];
	}

	return 0;
}
//----------------------------------------------------------------------------
size_t InfluenceMap::GetInfluenceLayers() const
{
	return MAX_INFLUENCE_LAYERS;
}
//----------------------------------------------------------------------------
Vector3f InfluenceMap::GetMaximumBoundary() const
{
	return Vector3f(
		grid_->boundaryMax[0],
		grid_->boundaryMax[1],
		grid_->boundaryMax[2]);
}
//----------------------------------------------------------------------------
Vector3f InfluenceMap::GetMinimumBoundary() const
{
	return Vector3f(
		grid_->boundaryMin[0],
		grid_->boundaryMin[1],
		grid_->boundaryMin[2]);
}
//----------------------------------------------------------------------------
size_t InfluenceMap::GetXCellCount() const
{
	return grid_->xCellCount;
}
//----------------------------------------------------------------------------
size_t InfluenceMap::GetYCellCount() const
{
	return grid_->yCellCount;
}
//----------------------------------------------------------------------------
size_t InfluenceMap::GetZCellCount() const
{
	return grid_->zCellCount;
}
//----------------------------------------------------------------------------
void InfluenceMap::SetFalloff(const size_t layer, const float falloff)
{
	falloff_[layer] = InfluenceMapUtilities::ClampFalloff(falloff);
}
//----------------------------------------------------------------------------
void InfluenceMap::SetInertia(const size_t layer, const float inertia)
{
	inertia_[layer] = InfluenceMapUtilities::ClampInertia(inertia);
}
//----------------------------------------------------------------------------
void InfluenceMap::SetInfluence(const Vector3f& position,	
	const size_t layer,	const float value)
{
	InfluenceMapCell* const cell =
		InfluenceMapUtilities::GetCell(*grid_, position);

	if (cell && cell->used && layer < GetInfluenceLayers())
	{
		cell->values[layer] = std::max(std::min(value, 1.0f), -1.0f);
	}
}
//----------------------------------------------------------------------------
void InfluenceMap::SpreadInfluence(const size_t layer)
{
	const size_t maxIterations = 20;
	const float minPropagation = 0.01f;

	float currentPropagation = (1.0f - falloff_[layer]);

	for (size_t index = 0; index < maxIterations; ++index)
	{
		InfluenceMapUtilities::UpdateInfluenceGrid(
			*grid_, layer, inertia_[layer], falloff_[layer]);

		currentPropagation *= (1.0f - falloff_[layer]);

		if (currentPropagation <= minPropagation)
		{
			break;
		}
	}
}
//----------------------------------------------------------------------------