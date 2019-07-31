#ifndef _SCANMATCHER_H_
#define _SCANMATCHER_H_

#include "Eigen/Geometry"
#include "DataPointContainer.h"
#include "UtilFunctions.h"
#include <iostream>


namespace hectorslam{

template<typename ConcreteOccGridMapUtil>
class ScanMatcher
{
public:

  ScanMatcher()
  {}

  ~ScanMatcher()
  {}

  Eigen::Vector3f matchData(const Eigen::Vector3f& beginEstimateWorld, ConcreteOccGridMapUtil& gridMapUtil, const DataContainer& dataContainer, Eigen::Matrix3f& covMatrix, int maxIterations)
  {
    if (dataContainer.getSize() != 0) {

      Eigen::Vector3f beginEstimateMap(gridMapUtil.getMapCoordsPose(beginEstimateWorld));

      Eigen::Vector3f estimate(beginEstimateMap);

      estimateTransformationLogLh(estimate, gridMapUtil, dataContainer);

      int numIter = maxIterations;


      for (int i = 0; i < numIter; ++i) {

        estimateTransformationLogLh(estimate, gridMapUtil, dataContainer);
      }

      estimate[2] = util::normalize_angle(estimate[2]);

      covMatrix = Eigen::Matrix3f::Zero();

      covMatrix = H;

      return gridMapUtil.getWorldCoordsPose(estimate);
    }

    return beginEstimateWorld;
  }

protected:

  bool estimateTransformationLogLh(Eigen::Vector3f& estimate, ConcreteOccGridMapUtil& gridMapUtil, const DataContainer& dataPoints)
  {
    gridMapUtil.getCompleteHessianDerivs(estimate, dataPoints, H, dTr);

    if ((H(0, 0) != 0.0f) && (H(1, 1) != 0.0f)) {


      Eigen::Vector3f searchDir (H.inverse() * dTr);

      if (searchDir[2] > 0.2f) {
        searchDir[2] = 0.2f;
        std::cout << "SearchDir angle change too large\n";
      } else if (searchDir[2] < -0.2f) {
        searchDir[2] = -0.2f;
        std::cout << "SearchDir angle change too large\n";
      }

      updateEstimatedPose(estimate, searchDir);
      return true;
    }
    return false;
  }

  void updateEstimatedPose(Eigen::Vector3f& estimate, const Eigen::Vector3f& change)
  {
    estimate += change;
  }

  void drawScan(const Eigen::Vector3f& pose, const ConcreteOccGridMapUtil& gridMapUtil, const DataContainer& dataContainer)
  {
    Eigen::Affine2f transform(gridMapUtil.getTransformForState(pose));

    int size = dataContainer.getSize();
    for (int i = 0; i < size; ++i) {
      const Eigen::Vector2f& currPoint (dataContainer.getVecEntry(i));
    }
  }

protected:
  Eigen::Vector3f dTr;
  Eigen::Matrix3f H;
};

}


#endif
