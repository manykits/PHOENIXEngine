// PX2SlamGMappingManager.hpp

#ifndef PX2SLAMGMAPPINGMANAGER_HPP
#define PX2SLAMGMAPPINGMANAGER_HPP

#include "SlamGMappingPlugin.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2APoint.hpp"
#include "PX2EventHandler.hpp"
#include "PX2HMatrix.hpp"
#include "PX2Timestamp.hpp"
#include "PX2Thread.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "sensor.h"
#include "gridslamprocessor.h"
#include "rangesensor.h"

namespace PX2
{

	class SLAMGMAPPING_DLL_ITEM SlamGMappingManager : public Singleton<SlamGMappingManager>, public EventHandler
	{
	public:
		SlamGMappingManager();
		virtual ~SlamGMappingManager();

		void Initlize();
		void Terminate();
		void Update(double appSeconds, double elapsedSeconds);

		typedef struct _sData
		{
			_sData()
			{
				signal = 0;
				angle = 0.0;
				distance = 0.0;
			}
			uint8_t signal;
			float   angle;
			float   distance;

		} sData;

		class LiDarSlamData
		{
		public:
			Timestamp TStamp;
			std::vector<sData> Datas;
		};
		std::vector<LiDarSlamData> LiDarSlamDatas;

	private:
		void _Initlize(int count);

		GMapping::SensorMap *mSensorMap;
		GMapping::GridSlamProcessor *mGridSlamProcessor;
		GMapping::RangeSensor *mRangeSensor;
		int mGSPLaserBeamCount;
		bool mIsInitlized;
	};

#define PX2_SlamGMappingM SlamGMappingManager::GetSingleton()

}

#endif