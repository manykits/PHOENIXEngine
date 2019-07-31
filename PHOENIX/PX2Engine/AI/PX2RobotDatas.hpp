// PX2RobotDatas.hpp

#ifndef PX2ROBOTDATAS_HPP
#define PX2ROBOTDATAS_HPP

#include "PX2CorePre.hpp"
#include "PX2APoint.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM RobotMapDataStruct
	{
	public:
		RobotMapDataStruct();
		~RobotMapDataStruct();

		int MapSize;
		float MapResolution;
		int IndexX;
		int IndexY;
		APoint CurPos;
		float CurAngle;
	};

	class PX2_ENGINE_ITEM RobotMapData : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(RobotMapData);

	public:
		RobotMapData();
		virtual ~RobotMapData();

		enum MapObstType
		{
			MOT_OBJST = 0,
			MOT_OBJST_LIDAR = 1,
			MOT_NONE = 10,
			MOT_ROBOTPOS = 100,
			MOT_CANGO = 200,
			MOT_MAX_TYPE
		};

		void Initlize(int mapSize, float resolution);

	public:
		std::vector<unsigned char> _ExtendLarger(
			std::vector<unsigned char> &mapping, int mapSize, int equalValue);
		void _SetMapUsingData(std::vector<unsigned char> &maping, int mapSize,
			int x, int y, int val);

		RobotMapDataStruct MapStruct;
		std::vector<unsigned char> Map2DOrigin;

		std::vector<unsigned char> SelfDrawMapData2D;
		std::vector<unsigned char> PathFinderLineDraw;
	};

	PX2_REGISTER_STREAM(RobotMapData);
	typedef Pointer0<RobotMapData> RobotMapDataPtr;

}

#endif