// PX2AIInfluenceMap.hpp

#ifndef PX2AIINFLUENCEMAP_HPP
#define PX2AIINFLUENCEMAP_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	class Node;

#define MAX_INFLUENCE_LAYERS 10

	struct InfluenceMapConfig
	{
		float cellHeight;
		float cellWidth;
		float boundaryMin[3];
		float boundaryMax[3];
	};

	struct InfluenceMapValueToCell
	{
		float value;
		size_t toCellIndex;
	};

	struct InfluenceMapCell
	{
		float values[MAX_INFLUENCE_LAYERS];
		float buffer;
		InfluenceMapValueToCell* valueToCells;
		bool used;
	};

	struct InfluenceMapGrid
	{
		// Stores the three dimensional cells that can make up the influence map grid.
		// To find the correct index:
		// xIndex + yIndex * xCellCount + zIndex * xCellCount * yCellCount
		InfluenceMapCell* cells;

		size_t cellCount;
		size_t xCellCount;
		size_t yCellCount;
		size_t zCellCount;

		float cellHeight;
		float cellWidth;

		float boundaryMin[3];
		float boundaryMax[3];
	};

	class PX2_ENGINE_ITEM InfluenceMap
	{
	public:
		InfluenceMap(
			const InfluenceMapConfig& config, const Node &mesh);
		~InfluenceMap();

		void ClearInfluence(const size_t layer);

		const InfluenceMapCell* GetCellAt(const size_t index) const;
		const InfluenceMapCell* GetCellAt(const Vector3f & position) const;

		size_t GetCellCount() const;
		float GetCellHeight() const;
		float GetCellWidth() const;

		float GetInfluenceAt(const Vector3f& position, const size_t layer) const;

		size_t GetInfluenceLayers() const;

		Vector3f GetMaximumBoundary() const;
		Vector3f GetMinimumBoundary() const;
		size_t GetXCellCount() const;
		size_t GetYCellCount() const;
		size_t GetZCellCount() const;

		void SetFalloff(const size_t layer, const float falloff);

		void SetInertia(const size_t layer, const float inertia);

		void SetInfluence(
			const Vector3f& position,
			const size_t layer,
			const float value);
		void SpreadInfluence(const size_t layer);

	private:
		InfluenceMapGrid* grid_;

		// Value between 0 and 1.
		float inertia_[MAX_INFLUENCE_LAYERS];
		// Value between 0 and 1.
		float falloff_[MAX_INFLUENCE_LAYERS];

		InfluenceMap(const InfluenceMap&);
		InfluenceMap operator=(const InfluenceMap&);
	};

}

#endif