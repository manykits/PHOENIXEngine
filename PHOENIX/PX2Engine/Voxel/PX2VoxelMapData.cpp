// PX2VoxelMapDta.cpp

#include "PX2VoxelMapData.hpp"
#include "PX2FileIO.hpp"
#include "PX2StringHelp.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
V_BlockData::V_BlockData()
{
	IndexX = 0;
	IndexY = 0;
	IndexZ = 0;
	Type = 0;
}
//----------------------------------------------------------------------------
V_BlockData::~V_BlockData()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
V_TitleData::V_TitleData()
{
	Version = 0;

	IndexX = 0;
	IndexY = 0;
	IndexZ = 0;

	NumBlocks = 0;
	Blocks = 0;
}
//----------------------------------------------------------------------------
V_TitleData::~V_TitleData()
{
}
//----------------------------------------------------------------------------
bool V_TitleData::Save(int mapID, V_TitleData *data)
{
	std::string mapsDir = V_DATA_MAPS;
	std::string mapIDDir = StringHelp::IntToString(mapID) + "/";
	std::string mapIDStr = StringHelp::IntToString(mapID);

	std::string str0 = StringHelp::IntToString(data->IndexX);
	std::string str1 = StringHelp::IntToString(data->IndexY);
	std::string str2 = StringHelp::IntToString(data->IndexZ);

	std::string path = mapsDir + mapIDDir + str0 + "_" + str1 + "_" + str2 + ".title";

	FileIO fo;
	if (!fo.Open(path, FileIO::FM_DEFAULT_WRITE))
		return false;

	fo.Write(2, &data->Version);

	fo.Write(2, &data->IndexX);
	fo.Write(2, &data->IndexY);
	fo.Write(2, &data->IndexZ);

	fo.Write(2, &data->NumBlocks);

	for (int i = 0; i < data->NumBlocks; i++)
	{
		V_BlockData &bd = data->Blocks[i];
		
		fo.Write(2, &bd.IndexX);
		fo.Write(2, &bd.IndexY);
		fo.Write(2, &bd.IndexZ);
		fo.Write(2, &bd.Type);
	}

	fo.Close();

	return true;
}
//----------------------------------------------------------------------------
bool V_TitleData::Load(int mapID, int indexX, int indexY, int indexZ,
	V_TitleData *toData)
{
	std::string mapsDir = V_DATA_MAPS;
	std::string mapIDDir = StringHelp::IntToString(mapID) + "/";
	std::string mapIDStr = StringHelp::IntToString(mapID);

	std::string str0 = StringHelp::IntToString(indexX);
	std::string str1 = StringHelp::IntToString(indexY);
	std::string str2 = StringHelp::IntToString(indexZ);

	std::string path = mapsDir + mapIDDir + str0 + "_" + str1 + "_" + str2 + 
		".title";

	if (!PX2_RM.IsFileFloderExist(path))
		return false;

	FileIO fo;
	if (!fo.Open(path, FileIO::FM_DEFAULT_READ))
		return false;

	fo.Read(2, &toData->Version);

	fo.Read(2, &toData->IndexX);
	fo.Read(2, &toData->IndexY);
	fo.Read(2, &toData->IndexZ);

	fo.Read(2, &toData->NumBlocks);

	if (toData->NumBlocks > 0)
	{
		toData->Blocks = new1<V_BlockData>(toData->NumBlocks);

		for (int i = 0; i < toData->NumBlocks; i++)
		{
			V_BlockData bd;

			fo.Read(2, &bd.IndexX);
			fo.Read(2, &bd.IndexY);
			fo.Read(2, &bd.IndexZ);
			fo.Read(2, &bd.Type);

			toData->Blocks[i] = bd;
		}
	}

	fo.Close();

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
V_SectionData::V_SectionData()
{
	Version = 0;

	MinSizeX = 0;
	MinSizeY = 0;
	MinSizeZ = 0;
	MaxSizeX = 0;
	MaxSizeY = 0;
	MaxSizeZ = 0;

	TitleRangeXY = 0;
	TitleRangeZ = 0;
}
//----------------------------------------------------------------------------
V_SectionData::~V_SectionData()
{
}
//----------------------------------------------------------------------------
bool V_SectionData::Save(int mapID, V_SectionData *data)
{
	std::string mapsDir = V_DATA_MAPS;
	std::string mapIDDir = StringHelp::IntToString(mapID) + "/";
	std::string mapIDStr = StringHelp::IntToString(mapID);

	if (!PX2_RM.IsFileFloderExist(mapsDir + mapIDDir))
	{
		PX2_RM.CreateFloder(mapsDir, mapIDDir);
	}

	std::string path = mapsDir + mapIDDir + mapIDStr + ".section";

	FileIO fo;
	if (!fo.Open(path, FileIO::FM_DEFAULT_WRITE))
		return false;

	fo.Write(2, &data->Version);

	fo.Write(2, &data->MinSizeX);
	fo.Write(2, &data->MinSizeY);
	fo.Write(2, &data->MinSizeZ);
	fo.Write(2, &data->MaxSizeX);
	fo.Write(2, &data->MaxSizeY);
	fo.Write(2, &data->MaxSizeZ);

	fo.Write(2, &data->TitleRangeXY);
	fo.Write(2, &data->TitleRangeZ);

	fo.Close();

	int numTitleX = data->MaxSizeX - data->MinSizeX + 1;
	int numTitleY = data->MaxSizeY - data->MinSizeY + 1;
	int numTitleZ = data->MaxSizeZ - data->MinSizeZ + 1;

	for (int i = 0; i < numTitleX; i++)
	{
		for (int j = 0; j < numTitleY; j++)
		{
			for (int k = 0; k < numTitleZ; k++)
			{
				int index = i * (numTitleY*numTitleZ) + j * numTitleZ + k;

				V_TitleData *td = data->Titles[index];
				if (td && td->NumBlocks>0)
				{
					V_TitleData::Save(mapID, td);
				}
			}
		}
	}

	return true;
}
//----------------------------------------------------------------------------
bool V_SectionData::Load(int mapID, V_SectionData *toData)
{
	std::string mapsDir = V_DATA_MAPS;
	std::string mapIDDir = StringHelp::IntToString(mapID) + "/";
	std::string mapIDStr = StringHelp::IntToString(mapID);
	std::string path = mapsDir + mapIDDir + mapIDStr + ".section";

	FileIO fo;
	if (!fo.Open(path, FileIO::FM_DEFAULT_READ))
		return false;

	fo.Read(2, &toData->Version);

	fo.Read(2, &toData->MinSizeX);
	fo.Read(2, &toData->MinSizeY);
	fo.Read(2, &toData->MinSizeZ);
	fo.Read(2, &toData->MaxSizeX);
	fo.Read(2, &toData->MaxSizeY);
	fo.Read(2, &toData->MaxSizeZ);

	fo.Read(2, &toData->TitleRangeXY);
	fo.Read(2, &toData->TitleRangeZ);

	fo.Close();

	return true;
}
//----------------------------------------------------------------------------