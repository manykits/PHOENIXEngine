#ifndef RANGEREADING_H
#define RANGEREADING_H

#include <vector>
#include "sensorreading.h"
#include "rangesensor.h"

namespace GMapping{

/* 继承这个类 public std::vector<double>， 主要是用于保存激光数据  */
class RangeReading: public SensorReading, public std::vector<double>
{
	public:
		
		RangeReading(const RangeSensor* rs, double time=0);

		/* 构造函数 */
		RangeReading(unsigned int n_beams, const double* d, const RangeSensor* rs, double time=0);
		
		virtual ~RangeReading();
		
		/* 读取机器人的位置信息 */
		inline const OrientedPoint& getPose() const 
		{
			return m_pose;
		}

		/* 设置机器人的位置信息 */
		inline void setPose(const OrientedPoint& pose) 
		{
			m_pose=pose;
		}
		
		/* 直观的理解：消除一些距离很近的点, 在调用的过程中，这个值好像设置为分辨率 */
		unsigned int rawView(double* v, double density=0.) const;
		
		std::vector<Point> cartesianForm(double maxRange=1e6) const;
		
		unsigned int activeBeams(double density=0.) const;
	
protected:
		OrientedPoint m_pose;
};

};

#endif
