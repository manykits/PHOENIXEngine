// PX2Slam2DManager.hpp

#ifndef PX2SLAM2DMANAGER_HPP
#define PX2SLAM2DMANAGER_HPP

#include "Slam2DPlugin.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2APoint.hpp"
#include "PX2EventHandler.hpp"
#include "PX2HMatrix.hpp"
#include "PX2Timestamp.hpp"
#include "PX2Thread.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "HectorMapping.h"

namespace PX2
{

	class LiDar;
	class Thread;

	class SLAM2D_DLL_ITEM Slam2DManager : public Singleton<Slam2DManager>, public EventHandler
	{
	public:
		Slam2DManager();
		virtual ~Slam2DManager();

		void Initlize();
		void Terminate();
		void Update(double appSeconds, double elapsedSeconds);

		static bool IsRun;

		class LiDarSlamData
		{
		public:
			Timestamp TStamp;
			std::vector<sData> Datas;
		};
		std::vector<LiDarSlamData> LiDarSlamDatas;

	private:
		void _UpdateInit();
		void _PrintSystemInfo();

		Thread *mThread;
		Thread *mThreadProcessData;
	};

#define PX2_Slam2DM Slam2DManager::GetSingleton()

}

#endif