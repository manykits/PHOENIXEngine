#ifndef _UTILFUNCTIONS_H_
#define _UTILFUNCTIONS_H_

#include <cmath>

namespace util{

	double const _M_PI = 3.14159265358979323846;
static inline float normalize_angle_pos(float angle)
{
	return fmod(fmod(angle, 2.0f*_M_PI) + 2.0f*_M_PI, 2.0f*_M_PI);
}

static inline float normalize_angle(float angle)
{
  float a = normalize_angle_pos(angle);
  if (a > _M_PI){
	  a -= 2.0f*_M_PI;
  }
  return a;
}

static inline float sqr(float val)
{
  return val*val;
}

static inline int sign(int x)
{
  return x > 0 ? 1 : -1;
}

template<typename T>
static T toDeg(const T radVal)
{
  return radVal * static_cast<T>(180.0 / _M_PI);
}

template<typename T>
static T toRad(const T degVal)
{
  return degVal * static_cast<T>(_M_PI / 180.0);
}

static bool poseDifferenceLargerThan(const Eigen::Vector3f& pose1, const Eigen::Vector3f& pose2, float distanceDiffThresh, float angleDiffThresh)
{
  //check distance
  if ( ( (pose1.head<2>() - pose2.head<2>()).norm() ) > distanceDiffThresh){
    return true;
  }

  if (angleDiffThresh > 0.0f)
  {
	  float angleDiff = (pose1.z() - pose2.z());

	  if (angleDiff > _M_PI) {
		  angleDiff -= _M_PI * 2.0f;
	  }
	  else if (angleDiff < -_M_PI) {
		  angleDiff += _M_PI * 2.0f;
	  }

	  if (abs(angleDiff) > angleDiffThresh){
		  return true;
	  }
  }

  return false;
}

}

#endif
