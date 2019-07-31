// PX2AIInfluenceMapUtilities.hpp

#ifndef PX2AIINFLUENCEMAPUTILITIES_HPP
#define PX2AIINFLUENCEMAPUTILITIES_HPP

#include "PX2CorePre.hpp"
#include "PX2AIInfluenceMap.hpp"

namespace PX2
{

	class InfluenceMapUtilities
	{
	public:
		static InfluenceMapGrid* BuildInfluenceMapGrid(
			const InfluenceMapConfig& config, const Node& sceneNode);

		static float ClampFalloff(const float falloff);
		static float ClampInertia(const float inertia);

		static void ClearInfluences(InfluenceMap& influenceMap);

		static InfluenceMapCell* GetCell(
			const InfluenceMapGrid& grid, const Vector3f& position);

		static bool InGridBounds(
			const InfluenceMapGrid& grid, const Vector3f& position);

		static bool GetCellIndex(const InfluenceMapGrid& grid,
			const Vector3f& position,
			size_t* const cellIndex);

		static bool GetCellIndexes(const InfluenceMapGrid& grid,
			const Vector3f& position, size_t* xCellIndex,
			size_t* yCellIndex, size_t* zCellIndex);

		static void UpdateInfluenceGrid(InfluenceMapGrid& grid,
			const size_t layer, const float inertia, const float falloff);
	};

}

#endif