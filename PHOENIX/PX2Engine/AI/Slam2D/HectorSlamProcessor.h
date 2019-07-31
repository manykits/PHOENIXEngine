#ifndef _HECTORSLAMPROCESSOR_H_
#define _HECTORSLAMPROCESSOR_H_

#include "GridMap.h"
#include "OccGridMapUtilConfig.h"
#include "ScanMatcher.h"
#include "DataPointContainer.h"

#include "UtilFunctions.h"

#include "MapLockerInterface.h"

#include "MapRepresentationInterface.h"
#include "MapRepMultiMap.h"

#include <float.h>

namespace hectorslam{

class HectorSlamProcessor
{
public:

  HectorSlamProcessor(float mapResolution, 
                      int mapSizeX, int mapSizeY , 
                      const Eigen::Vector2f& startCoords, 
                      int multi_res_size)
  {
	  mIsUpdateMap = true;
	
    mapRep = new MapRepMultiMap(mapResolution, mapSizeX, mapSizeY, multi_res_size, startCoords);

    float totalMapSizeX = mapResolution * static_cast<float>(mapSizeX);
    float mid_offset_x = totalMapSizeX * startCoords.x();

    float totalMapSizeY = mapResolution * static_cast<float>(mapSizeY);
    float mid_offset_y = totalMapSizeY * startCoords.y();

    mapTworld = Eigen::AlignedScaling2f(1 / mapResolution, 1 / mapResolution) * Eigen::Translation2f(mid_offset_x, mid_offset_y);

    this->reset();

    this->setMapUpdateMinDistDiff(0.4f *1.0f);
    this->setMapUpdateMinAngleDiff(0.13f * 1.0f);
  }

  ~HectorSlamProcessor()
  {
    delete mapRep;
  }

  void setUpdateMap(bool update)
  {
	  mIsUpdateMap = update;
  }

  void update(const DataContainer& dataContainer, const Eigen::Vector3f& poseHintWorld)
  {
    Eigen::Vector3f newPoseEstimateWorld;

    newPoseEstimateWorld = (mapRep->matchData(poseHintWorld, dataContainer, lastScanMatchCov));

    lastScanMatchPose = newPoseEstimateWorld;

	if (mIsUpdateMap)
	{
		if (util::poseDifferenceLargerThan(newPoseEstimateWorld,
			lastMapUpdatePose,
			paramMinDistanceDiffForMapUpdate,
			paramMinAngleDiffForMapUpdate))
		{

			mapRep->updateByScan(dataContainer, newPoseEstimateWorld);

			mapRep->onMapUpdated();
			lastMapUpdatePose = newPoseEstimateWorld;
		}
	}
  }

  void reset()
  {
    lastMapUpdatePose = Eigen::Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
    lastScanMatchPose = Eigen::Vector3f::Zero();

    mapRep->reset();
  }
  
  const Eigen::Vector3f& getLastScanMatchPose() const { return lastScanMatchPose; };
  const Eigen::Vector3f& getLastMapUpdatePose() const {return lastMapUpdatePose;};
  const Eigen::Matrix3f& getLastScanMatchCovariance() const { return lastScanMatchCov; };
  float getScaleToMap() const { return mapRep->getScaleToMap(); };
  int getMapLevels() const { return mapRep->getMapLevels(); };
  const GridMap& getGridMap(int mapLevel = 0) const { return mapRep->getGridMap(mapLevel); };
  GridMap& getGridMap(int mapLevel = 0) { return mapRep->getGridMap(mapLevel); };

  void addMapMutex(int i, MapLockerInterface* mapMutex) { mapRep->addMapMutex(i, mapMutex); };
  MapLockerInterface* getMapMutex(int i) { return mapRep->getMapMutex(i); };

  void setUpdateFactorFree(float free_factor) { mapRep->setUpdateFactorFree(free_factor); };
  void setUpdateFactorOccupied(float occupied_factor) { mapRep->setUpdateFactorOccupied(occupied_factor); };
  void setMapUpdateMinDistDiff(float minDist) { paramMinDistanceDiffForMapUpdate = minDist; };
  void setMapUpdateMinAngleDiff(float angleChange) { paramMinAngleDiffForMapUpdate = angleChange; };

  void getLastMatchPoseMap(Eigen::Vector3f& poseMap)
  {
      Eigen::Vector2f mapCoords (mapTworld * lastScanMatchPose.head<2>());
      poseMap =  Eigen::Vector3f(mapCoords[0], mapCoords[1], lastScanMatchPose[2]);
  }

  public:
	bool mIsUpdateMap;
	MapRepresentationInterface* mapRep;

	Eigen::Vector3f lastMapUpdatePose;
	Eigen::Vector3f lastScanMatchPose;
	Eigen::Matrix3f lastScanMatchCov;

	float paramMinDistanceDiffForMapUpdate;
	float paramMinAngleDiffForMapUpdate;

	Eigen::Affine2f mapTworld;
};//class HectorSlamProcessor

}//namespace hectorslam

#endif
