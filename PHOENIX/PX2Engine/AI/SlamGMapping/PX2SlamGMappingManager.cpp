// PX2SlamGMappingManager.cpp

#include "PX2SlamGMappingManager.hpp"
#include "PX2AILiDar.hpp"
#include "PX2Thread.hpp"
#include "PX2Project.hpp"
#include "PX2Robot.hpp"
#include "PX2Application.hpp"
#include "PX2DynLibManager.hpp"
#include "PX2Robot.hpp"
#include <iostream>

#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

using namespace PX2;
using namespace std;
using namespace GMapping;

//----------------------------------------------------------------------------
SlamGMappingManager::SlamGMappingManager():
	mGSPLaserBeamCount(180),
	mIsInitlized(false)
{
}
//----------------------------------------------------------------------------
SlamGMappingManager::~SlamGMappingManager()
{
}
//----------------------------------------------------------------------------
void SlamGMappingManager::Initlize()
{
	PX2_EW.ComeIn(this);


}
//----------------------------------------------------------------------------
void SlamGMappingManager::_Initlize(int count)
{
	mGSPLaserBeamCount = count;

	mSensorMap = new0 SensorMap();
	mGridSlamProcessor = new0 GridSlamProcessor();

	double gsp_laser_angle_increment_ = M_PI * 2 / mGSPLaserBeamCount;
	GMapping::OrientedPoint gmap_pose(0.0, 0.0, 0.0);
	double maxRange_ = 6; // 激光的最大距离

	mRangeSensor = new0 GMapping::RangeSensor("FLASER",
		mGSPLaserBeamCount,
		fabs(gsp_laser_angle_increment_),
		gmap_pose,
		0.0,
		maxRange_);

	mSensorMap->insert(make_pair(mRangeSensor->getName(), mRangeSensor));

	mGridSlamProcessor->setSensorMap(*mSensorMap);

	double maxUrange = 50.0;
	double maxrange = 10;
	double sigma = 0.05;
	int kernelSize = 1;
	double lstep = 0.05;
	double astep = 0.05;
	int iterations = 5;
	double lsigma = 0.075;
	double ogain = 3;
	int lskip = 0;

	double srr = 0.01, srt = 0.01, str = 0.01, stt = 0.01;

	double linearUpdate = 0; /* 线性更新的条件 */
	double angularUpdate = 0; /* 角度更新的条件 */
	double resampleThreshold = 0.5;

	bool generateMap = true;

	mGridSlamProcessor->setMatchingParameters(maxUrange, maxrange, sigma, kernelSize, lstep, astep, iterations, lsigma, ogain, lskip);
	mGridSlamProcessor->setMotionModelParameters(srr, srt, str, stt);
	mGridSlamProcessor->setUpdateDistances(linearUpdate, angularUpdate, resampleThreshold);
	mGridSlamProcessor->setgenerateMap(generateMap);

	OrientedPoint initialPose(0.0f, 0.0f, 0.0f);

	int particles = 30;
	double xmin = -20;
	double ymin = -20;
	double xmax = 20;
	double ymax = 20;
	double delta = 0.1;
	float size = (xmax - xmin) / delta;
	mGridSlamProcessor->init(particles, xmin, ymin, xmax, ymax, delta, initialPose);

	GMapping::sampleGaussian(1, time(NULL));

	GridSlamProcessor* ap, *copy = mGridSlamProcessor->clone();
	ap = mGridSlamProcessor; mGridSlamProcessor = copy; copy = ap;

	Robot *robot = Robot::GetSingletonPtr();
	if (robot)
	{
		if (!robot->IsSlamMapInited())
		{
			robot->InitSlamMap(size, delta, true);
		}
	}

	mIsInitlized = true;
}
//----------------------------------------------------------------------------
void SlamGMappingManager::Terminate()
{
	if (mSensorMap)
	{
		delete0(mSensorMap);
	}

	if (mGridSlamProcessor)
	{
		delete0(mGridSlamProcessor);
	}

	if (mRangeSensor)
	{
		delete0(mRangeSensor);
	}

	PX2_EW.GoOut(this);
}
//----------------------------------------------------------------------------
void SlamGMappingManager::Update(double appSeconds, double elapsedSeconds)
{
	if (!Project::GetSingletonPtr())
		return;

	Robot *robot = Robot::GetSingletonPtr();
	if (!robot)
		return;

	LiDar *liDar = robot->GetLidar();
	if (!liDar)
		return;

	std::vector<RslidarDataComplete> lData = liDar->GetLiDarData();
	int ldDataSize = lData.size();
	if (0 == ldDataSize)
		return;

	double ranges_double[1000];

	LiDarSlamData lsd0;
	lsd0.Datas.resize(ldDataSize);
	for (int i = 0; i < (int)lData.size(); i++)
	{
		lsd0.Datas[i].angle = lData[i].angle;
		lsd0.Datas[i].signal = (uint8_t)lData[i].signal;
		lsd0.Datas[i].distance = lData[i].distance;

		ranges_double[i] = lsd0.Datas[i].distance;
	}

	if (!mIsInitlized)
		_Initlize(ldDataSize);

	LiDarSlamData &lsd = lsd0;

	double time_r = 0.0;
	GMapping::RangeReading reading(mGSPLaserBeamCount, ranges_double, mRangeSensor, time_r);

	GMapping::OrientedPoint gmap_pose;
	gmap_pose.x =0;
	gmap_pose.y = 0;
	gmap_pose.theta = 0;
	reading.setPose(gmap_pose);

	cv::Size dsize = cv::Size(1000, 1000);
	Mat image2 = Mat(dsize, CV_32F);

	bool processed = mGridSlamProcessor->processScan(reading);
	if (processed)
	{
		unsigned int best_idx = mGridSlamProcessor->getBestParticleIndex();

		double theta = mGridSlamProcessor->getParticles()[best_idx].pose.theta;
		theta = atan2(sin(theta), cos(theta));

		OrientedPoint estimatePos = mGridSlamProcessor->getParticles()[best_idx];

		{
			DoubleArray2D* mymap = mGridSlamProcessor->getParticles()[best_idx].map.toDoubleArray();

			int image_h = mymap->getXSize();
			int image_w = mymap->getYSize();

			double occ_thresh_ = 0.25;

			float *imagedata = (float *)malloc(image_w*image_h * sizeof(float));
			memset(imagedata, 0x00, image_w*image_h * sizeof(float));

			std::vector<unsigned char> imageDataVec;
			imageDataVec.resize(image_w*image_h);

			for (int j = 0; j < image_h; j++)
			{
				for (int i = 0; i < image_w; i++)
				{
					double occ = mymap->cell(j, i);

					if (occ < 0)
					{
						/* 未开发区域 */
						imagedata[j*image_w + i] = 0.5;

						imageDataVec[j*image_w + i] = RobotMapData::MOT_NONE;
					}
					else if (occ > occ_thresh_)     /* occ_thresh_ = 0.25 */
					{
						/* 障碍物区域 */
						imagedata[j*image_w + i] = 0;

						imageDataVec[j*image_w + i] = RobotMapData::MOT_OBJST;
					}
					else
					{
						/* free 区域 */
						imagedata[j*image_w + i] = 1;

						imageDataVec[j*image_w + i] = RobotMapData::MOT_CANGO;
					}
				}
			}

			robot->SetSlam2DMap(imageDataVec);

			//Mat map(image_h, image_w, CV_32F, imagedata);
			//resize(map, image2, dsize);
			//imshow("Image", image2);

			delete mymap;
			free(imagedata);
		}
	}
}
//----------------------------------------------------------------------------