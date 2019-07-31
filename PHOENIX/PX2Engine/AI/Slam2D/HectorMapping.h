#ifndef HECTOR_MAPPING_ROS_H__
#define HECTOR_MAPPING_ROS_H__

#include "HectorSlamProcessor.h"

#include "DataPointContainer.h"
#include "MapLockerInterface.h"

#include "PX2Thread.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"

#include <fstream>

#include "Eigen/Core"
#include "buffer.h"

#define  M_PI 3.14159265358979323846
#define ANGLE_MIN_DATA 0.0
#define ANGLE_MAX_DATA (2*M_PI)
// m
#define RANGE_MIN 0.2
#define RANGE_MAX 6.0
#define DEGREE_TO_RAD (M_PI/180.0)

struct Point
{
  float x;
  float y;
  float z;
  float angle;
};

struct PointCloud
{
  std::vector<Point> points;
};

class HectorDrawings;
class HectorDebugInfoProvider;

class HectorMapping
{
public:
  HectorMapping();
  ~HectorMapping();
  void getPose(Eigen::Vector3f& poseUpdateMap, Eigen::Vector3f& poseUpdateWorld, Eigen::Vector3f& poseMatch);
  void getMap(std::vector<unsigned char>& map, float& angle);
  void setMap(const std::vector<unsigned char>& map, Eigen::Vector3f& hitPos);

  int getMapSize() const;
  float getResolution() const;

  void setUpdateMap(bool update);
  bool isUpdateMap() const;

  void scanCallback(const std::vector<_sData>& scan, bool usePointCloud, std::vector<_sData>& getData);
  bool rosLaserScanToDataContainer(const std::vector<_sData>& scan, hectorslam::DataContainer& dataContainer, float scaleToMap);
  void laserToPointCloud(const std::vector<_sData>& scan_in, PointCloud& cloud_out, double range_cutoff);

  bool rosPointCloudToDataContainer(PointCloud& pointCloud, hectorslam::DataContainer& dataContainer, float scaleToMap);

  void getLastMatchPoseMapIndex(int &indexX, int &indexY, float &angle);

protected:
  int lastGetMapUpdateIndex;

  hectorslam::HectorSlamProcessor* slamProcessor;
  hectorslam::DataContainer laserScanContainer;

  double p_update_factor_free_;
  double p_update_factor_occupied_;
  double p_map_update_distance_threshold_;
  double p_map_update_angle_threshold_;

  double p_map_resolution_;
  int p_map_size_;
  double p_map_start_x_;
  double p_map_start_y_;
  int p_map_multi_res_levels_;

  float p_sqr_laser_min_dist_;
  float p_sqr_laser_max_dist_;
  float p_laser_z_min_value_;
  float p_laser_z_max_value_;

  PX2::Mutex guv_mutex_;

  int debug_index;
  std::ofstream data_log_file;
  //std::vector<float> dataGet;
  std::vector<_sData> dataGets;

  bool mIsUpdateMap;
};

#endif
