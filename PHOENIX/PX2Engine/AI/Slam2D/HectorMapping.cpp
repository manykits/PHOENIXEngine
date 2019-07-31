#include "HectorMapping.h"
#include "GridMap.h"
#include "HectorMapMutex.h"
#include <fstream>

HectorMapping::HectorMapping(): lastGetMapUpdateIndex(-100)
{
  mIsUpdateMap = true;

  p_map_resolution_ = 0.025;//0.08;//0.025; // 0.08
  p_map_size_ = 1024;
  p_map_start_x_ = 0.5; 
  p_map_start_y_ = 0.5;
  p_map_multi_res_levels_ = 3;
  p_update_factor_free_ = 0.4;
  p_update_factor_occupied_ = 0.9;
  p_map_update_distance_threshold_ = 0.2;
  p_map_update_angle_threshold_ = 0.2f;

  p_sqr_laser_min_dist_ = static_cast<float>(0.35 * 0.35);
  p_sqr_laser_max_dist_ = static_cast<float>(10 * 10);
  p_laser_z_min_value_ = static_cast<float>(-1.0);
  p_laser_z_max_value_ = static_cast<float>(1.0);

  slamProcessor = new hectorslam::HectorSlamProcessor(static_cast<float>(p_map_resolution_), 
                                                      p_map_size_, p_map_size_, 
                                                      Eigen::Vector2f(p_map_start_x_, p_map_start_y_), 
                                                      p_map_multi_res_levels_);
  slamProcessor->setUpdateFactorFree(p_update_factor_free_);
  slamProcessor->setUpdateFactorOccupied(p_update_factor_occupied_);
  slamProcessor->setMapUpdateMinDistDiff(p_map_update_distance_threshold_);
  slamProcessor->setMapUpdateMinAngleDiff(p_map_update_angle_threshold_);

  debug_index = 0;
  data_log_file.open("dataContainer_log.txt");


  int mapLevels = slamProcessor->getMapLevels();
  mapLevels = 1;
}

HectorMapping::~HectorMapping()
{
  delete slamProcessor;
  data_log_file.close();
}
void HectorMapping::scanCallback(const std::vector<_sData>& scan, bool usePointCloud, std::vector<_sData>& getData)
{
	if (!usePointCloud)
	{
		if (rosLaserScanToDataContainer(scan, laserScanContainer, slamProcessor->getScaleToMap()))
			slamProcessor->update(laserScanContainer, slamProcessor->getLastScanMatchPose());
	}
	else
	{
		PointCloud laser_point_cloud;
		laserToPointCloud(scan, laser_point_cloud, 30);
		if (rosPointCloudToDataContainer(laser_point_cloud, laserScanContainer, slamProcessor->getScaleToMap()))
			slamProcessor->update(laserScanContainer, slamProcessor->getLastScanMatchPose());
	}
	getData = dataGets;
}
bool HectorMapping::rosLaserScanToDataContainer(const std::vector<_sData>& scan, hectorslam::DataContainer& dataContainer, float scaleToMap)
{
	size_t size = scan.size();

	dataContainer.clear();

	dataContainer.setOrigo(Eigen::Vector2f::Zero());

	float maxRangeForContainer = RANGE_MAX - 0.1f;

	dataGets.clear();//
	dataGets.resize(size);//
	int indexForDataGet = 0;//

	for (size_t i = 0; i < size; ++i)
	{
		float rad = scan[i].angle * DEGREE_TO_RAD;
		float dist = scan[i].distance;
		dist = dist / 1000;//mm => m

		if ((RANGE_MIN < dist) && (dist < maxRangeForContainer))
		{
			dist *= scaleToMap;
			dataGets[indexForDataGet].angle = scan[i].angle;
			dataGets[indexForDataGet].distance = dist;//
			dataGets[indexForDataGet].signal = scan[i].signal;
			dataContainer.add(Eigen::Vector2f(cos(rad) * dist, sin(rad) * dist));
			indexForDataGet++;
		}
	}

	dataGets.resize(indexForDataGet);//

	return true;
}
void HectorMapping::laserToPointCloud(const std::vector<_sData>& scan_in, PointCloud& cloud_out, double range_cutoff)
{
	size_t size = scan_in.size();
	  
	dataGets.clear();
	dataGets.resize(size);

	cloud_out.points.resize(size);
	int indexForDataGet = 0;

	if (range_cutoff < 0)
		range_cutoff = RANGE_MAX;
	else
		range_cutoff = std::min(range_cutoff, (double)RANGE_MAX);
	  
	unsigned int indexForRanges = 0;
	unsigned int pointCount = 0;
	for (unsigned int index = 0; index < size; index++)
	{
		const _sData &sData = scan_in[index];
		float dist = sData.distance;
		float angle = sData.angle;
	  
		float distance = dist / 1000.0;

		dataGets[indexForDataGet].distance = distance * slamProcessor->getScaleToMap();
		dataGets[indexForDataGet].angle = angle;
		indexForDataGet++;

		if ((distance < range_cutoff) && (distance >= RANGE_MIN)) //if valid or preservative
		{
			float rad = angle * DEGREE_TO_RAD;
			cloud_out.points[pointCount].x = cos(rad) * distance;
			cloud_out.points[pointCount].y = sin(rad)* distance;
			cloud_out.points[pointCount].z = 0.0;
			cloud_out.points[pointCount].angle = angle;

			pointCount++;
		}

		indexForRanges++;
	}
	  
	dataGets.resize(indexForDataGet);
	cloud_out.points.resize(pointCount);
}

  bool HectorMapping::rosPointCloudToDataContainer(PointCloud& pointCloud, hectorslam::DataContainer& dataContainer, float scaleToMap)
  {
    size_t size = pointCloud.points.size();
  
    dataContainer.clear();
  
    Eigen::Vector3f laserPos (Eigen::Vector3f::Zero());//雷达坐标系在机器人坐标系中的位置
    dataContainer.setOrigo(Eigen::Vector2f(laserPos.x(), laserPos.y())*scaleToMap);
  
	dataGets.clear();
	dataGets.resize(size);
    int indexForDataGet = 0;
  
	for (size_t i = 0; i < size; ++i)
	{
		const Point& currPoint(pointCloud.points[i]);

		float dist_sqr = currPoint.x*currPoint.x + currPoint.y* currPoint.y;

		if ((dist_sqr > p_sqr_laser_min_dist_) && (dist_sqr < p_sqr_laser_max_dist_))
		{

			//if ((currPoint.x < 0.0f) && (dist_sqr < 0.01f))
			//{
			//	continue;
			//}

			float pointPosLaserFrameZ = currPoint.z - laserPos.z();

			if (pointPosLaserFrameZ > p_laser_z_min_value_ && pointPosLaserFrameZ < p_laser_z_max_value_)
			{
				dataContainer.add(Eigen::Vector2f(currPoint.x, currPoint.y) * scaleToMap);
				dataGets[indexForDataGet].angle = currPoint.angle;
				dataGets[indexForDataGet].distance = sqrt(dist_sqr) * scaleToMap;
				indexForDataGet++;
			}
		}
	}
  
	dataGets.resize(indexForDataGet);

    return true;
}

void HectorMapping::getLastMatchPoseMapIndex(int &indexX, int &indexY, float &angle)
{
	Eigen::Vector3f robotPose;
	slamProcessor->getLastMatchPoseMap(robotPose);

	indexX = robotPose[0];
	indexY = robotPose[1];
	angle = robotPose[2];
}

void HectorMapping::getMap(std::vector<unsigned char>& map, float& angle)
{
  const hectorslam::GridMap& gridMap = slamProcessor->getGridMap(0);
  MapLockerInterface* mapMutex = slamProcessor->getMapMutex(0);
  int gridMapUpdateIndex = gridMap.getUpdateIndex();

  if (lastGetMapUpdateIndex != gridMapUpdateIndex)
  {
    map.clear();

    int sizeX = gridMap.getSizeX();
    int sizeY = gridMap.getSizeY();

    int size = sizeX * sizeY;

    map.resize(size);

    if (mapMutex)
    {
      mapMutex->lockMap();
    }

    for(int i=0; i < size; ++i)
    {
      if(!gridMap.isUpdated(i))
      {
        map[i] = 10;//not updated
      }
      if(gridMap.isFree(i))
      {
        map[i] = 200;
      }
      if(gridMap.isOccupied(i))
      {
        map[i] = 0;
      }
    }

    Eigen::Vector3f robotPose;
	slamProcessor->getLastMatchPoseMap(robotPose);

    int x = robotPose[0];
    int y = robotPose[1];
    angle = robotPose[2];

    int robotIndex = y * sizeX + x;

    if(robotIndex >= 0 && robotIndex < size)
      map[robotIndex] = 100;//robot coordinate


    lastGetMapUpdateIndex = gridMap.getUpdateIndex();

    if (mapMutex)
    {
      mapMutex->unlockMap();
    }
  }
}

void HectorMapping::setMap(const std::vector<unsigned char>& map, Eigen::Vector3f& hitPos)
{
	slamProcessor->lastMapUpdatePose = hitPos;
	slamProcessor->lastScanMatchPose = hitPos;

	int m = 0;
	//for (m = 0; m < slamProcessor->getMapLevels(); m++)
	{
		hectorslam::GridMap& gridMap = slamProcessor->getGridMap(m);
		MapLockerInterface* mapMutex = slamProcessor->getMapMutex(m);

		int sizeX = gridMap.getSizeX();
		int sizeY = gridMap.getSizeY();

		for (int i = 0; i < (int)map.size(); i++)
		{
			if (10 == map[i])
			{
				gridMap.setNotUpdated(i);
			}
			else if (200 == map[i])
			{
				gridMap.setFree(i);
			}
			else if (0 == map[i])
			{
				gridMap.setOccupied(i);
			}
		}

		gridMap.setUpdatedIndex();

		if (mapMutex)
		{
			mapMutex->lockMap();
		}
	}
}

int HectorMapping::getMapSize() const
{
	return p_map_size_;
}

float HectorMapping::getResolution() const
{
	return p_map_resolution_;
}

void HectorMapping::setUpdateMap(bool update)
{
	mIsUpdateMap = update;

	slamProcessor->setUpdateMap(mIsUpdateMap);
}

bool HectorMapping::isUpdateMap() const
{
	return mIsUpdateMap;
}

void HectorMapping::getPose(Eigen::Vector3f& poseUpdateMap, Eigen::Vector3f& poseUpdateWorld, Eigen::Vector3f& poseMatch)
{
	slamProcessor->getLastMatchPoseMap(poseUpdateMap);
	poseUpdateWorld = slamProcessor -> getLastMapUpdatePose();
	poseMatch = slamProcessor -> getLastScanMatchPose();
}
