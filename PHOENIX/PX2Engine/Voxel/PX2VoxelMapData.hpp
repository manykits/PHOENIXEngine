// PX2VoxelMapData.hpp

#ifndef PX2VOXELMAPDATA_HPP
#define PX2VOXELMAPDATA_HPP

#include "PX2Object.hpp"
#include "PX2APoint.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	const std::string V_DATA = "Data/Voxel/";
	const std::string V_DATA_MAPS = V_DATA + "maps/";
	
	class V_BlockData
	{
	public:
		V_BlockData();
		~V_BlockData();

		int16_t IndexX;
		int16_t IndexY;
		int16_t IndexZ;
		int16_t Type;
	};
		
	class V_TitleData
	{
	public:
		V_TitleData();
		~V_TitleData();

		static bool Save(int mapID, V_TitleData *data);
		static bool Load(int mapID, int indexX, int indexY, int indexZ, V_TitleData *toData);

		int16_t Version;

		int16_t IndexX;
		int16_t IndexY;
		int16_t IndexZ;

		int16_t NumBlocks;
		Pointer1<V_BlockData> Blocks;
	};
	typedef Pointer0<V_TitleData> V_TitleDataPtr;

	class V_SectionData
	{
	public:
		V_SectionData();
		~V_SectionData();

		static bool Save(int mapID, V_SectionData *data);
		static bool Load(int mapID, V_SectionData *toData);

		int16_t Version;

		int16_t MinSizeX;
		int16_t MinSizeY;
		int16_t MinSizeZ;
		int16_t MaxSizeX;
		int16_t MaxSizeY;
		int16_t MaxSizeZ;

		int16_t TitleRangeXY;
		int16_t TitleRangeZ;

		std::vector<V_TitleDataPtr> Titles;
	};
	typedef Pointer0<V_SectionData> V_SectionDataPtr;

}

#endif
