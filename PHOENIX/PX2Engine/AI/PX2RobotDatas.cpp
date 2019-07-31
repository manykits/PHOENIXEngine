// PX2RobotDatas.cpp

#include "PX2RobotDatas.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
RobotMapDataStruct::RobotMapDataStruct()
{
	MapSize = 256;
	MapResolution = 0.1f;
	IndexX = 0;
	IndexY = 0;
	CurAngle = 0.0f;
}
//----------------------------------------------------------------------------
RobotMapDataStruct::~RobotMapDataStruct()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, Object, RobotMapData);
PX2_IMPLEMENT_STREAM(RobotMapData);
PX2_IMPLEMENT_FACTORY(RobotMapData);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, RobotMapData);
//----------------------------------------------------------------------------
RobotMapData::RobotMapData()
{
}
//----------------------------------------------------------------------------
RobotMapData::~RobotMapData()
{
}
//----------------------------------------------------------------------------
std::vector<unsigned char> RobotMapData::_ExtendLarger(
	std::vector<unsigned char> &mapping, int mapSize, int equalValue)
{
	std::vector<unsigned char> retMapping = mapping;

	int x = 0;
	int y = 0;
	int mapVal = 200;
	int index = 0;
	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			index = i * mapSize + j;

			x = j;
			y = i;

			mapVal = mapping[index];

			bool isHasObst = false;
			if (equalValue == mapVal)
			{
				isHasObst = true;
			}

			if (isHasObst)
			{
				_SetMapUsingData(retMapping, mapSize, x, y, equalValue);

				// left
				int left = x - 1;
				if (left >= 0)
				{
					_SetMapUsingData(retMapping, mapSize, left, y, equalValue);
				}

				left = x - 2;
				if (left >= 0)
				{
					_SetMapUsingData(retMapping, mapSize, left, y, equalValue);
				}

				// right
				int right = x + 1;
				if (right < MapStruct.MapSize)
				{
					_SetMapUsingData(retMapping, mapSize, right, y, equalValue);
				}

				right = x + 2;
				if (right < MapStruct.MapSize)
				{
					_SetMapUsingData(retMapping, mapSize, right, y, equalValue);
				}

				// down
				int down = y - 1;
				if (down >= 0)
				{
					_SetMapUsingData(retMapping, mapSize, x, down, equalValue);
				}

				down = y - 2;
				if (down >= 0)
				{
					_SetMapUsingData(retMapping, mapSize, x, down, equalValue);
				}

				// up
				int up = y + 1;
				if (up < MapStruct.MapSize)
				{
					_SetMapUsingData(retMapping, mapSize, x, up, equalValue);
				}

				up = y + 2;
				if (up < MapStruct.MapSize)
				{
					_SetMapUsingData(retMapping, mapSize, x, up, equalValue);
				}
			}
		}
	}

	return retMapping;
}
//----------------------------------------------------------------------------
void RobotMapData::Initlize(int mapSize, float resolution)
{
	int mapAllSize = mapSize * mapSize;

	MapStruct.MapSize = mapSize;
	MapStruct.MapResolution = resolution;

	// 200 is default
	SelfDrawMapData2D.resize(mapAllSize);
	memset(&(SelfDrawMapData2D[0]), RobotMapData::MOT_NONE, (int)mapAllSize);

	PathFinderLineDraw.resize(mapAllSize);
	memset(&(PathFinderLineDraw[0]), 0, (int)mapAllSize);
}
//----------------------------------------------------------------------------
void RobotMapData::_SetMapUsingData(std::vector<unsigned char> &maping,
	int mapSize, int x, int y, int val)
{
	int index = y*mapSize + x;
	maping[index] = (unsigned char)val;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
RobotMapData::RobotMapData(LoadConstructor value) :
Object(value)
{
}
//----------------------------------------------------------------------------
void RobotMapData::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(MapStruct.MapSize);
	source.Read(MapStruct.MapResolution);
	source.Read(MapStruct.IndexX);
	source.Read(MapStruct.IndexY);
	source.ReadAggregate(MapStruct.CurPos);
	source.Read(MapStruct.CurAngle);

	int numDatas = 0;
	source.Read(numDatas);

	Map2DOrigin.resize(numDatas);
	for (int i = 0; i < numDatas; i++)
	{
		source.Read<uint8_t>(Map2DOrigin[i]);
	}

	SelfDrawMapData2D.resize(numDatas);
	for (int i = 0; i < numDatas; i++)
	{
		source.Read<uint8_t>(SelfDrawMapData2D[i]);
	}

	PathFinderLineDraw.resize(numDatas);
	for (int i = 0; i < numDatas; i++)
	{
		source.Read<uint8_t>(PathFinderLineDraw[i]);
	}
	memset(&PathFinderLineDraw[0], 0, numDatas);

	PX2_END_DEBUG_STREAM_LOAD(RobotMapData, source);
}
//----------------------------------------------------------------------------
void RobotMapData::Link(InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void RobotMapData::PostLink()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool RobotMapData::Register(OutStream& target) const
{
	if (Object::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void RobotMapData::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(MapStruct.MapSize);
	target.Write(MapStruct.MapResolution);
	target.Write(MapStruct.IndexX);
	target.Write(MapStruct.IndexY);
	target.WriteAggregate(MapStruct.CurPos);
	target.Write(MapStruct.CurAngle);

	int numDatas = (int)Map2DOrigin.size();
	target.Write(numDatas);
	for (int i = 0; i < numDatas; i++)
	{
		target.Write<uint8_t>(Map2DOrigin[i]);
	}
	for (int i = 0; i < numDatas; i++)
	{
		target.Write<uint8_t>(SelfDrawMapData2D[i]);
	}
	for (int i = 0; i < numDatas; i++)
	{
		target.Write<uint8_t>(PathFinderLineDraw[i]);
	}

	PX2_END_DEBUG_STREAM_SAVE(RobotMapData, target);
}
//----------------------------------------------------------------------------
int RobotMapData::GetStreamingSize(Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(MapStruct.MapSize);
	size += sizeof(MapStruct.MapResolution);
	size += sizeof(MapStruct.IndexX);
	size += sizeof(MapStruct.IndexY);
	size += sizeof(MapStruct.CurPos);
	size += sizeof(MapStruct.CurAngle);

	int numDatas = (int)Map2DOrigin.size();
	size += sizeof(numDatas);
	size += numDatas * sizeof(uint8_t);
	size += numDatas * sizeof(uint8_t);
	size += numDatas * sizeof(uint8_t);

	return size;
}
//----------------------------------------------------------------------------
