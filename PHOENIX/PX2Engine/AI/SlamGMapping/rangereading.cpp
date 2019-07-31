#include <limits>
#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include "gvalues.h"
#include "rangereading.h"

namespace GMapping{

using namespace std;

RangeReading::RangeReading(const RangeSensor* rs, double time):
	SensorReading(rs,time)
{
}

/* 构造函数 */
RangeReading::RangeReading(unsigned int n_beams, const double* d, const RangeSensor* rs, double time):
	SensorReading(rs,time)
{
	assert(n_beams == rs->beams().size());

	resize(n_beams);
	
	/* 实际的雷达数据 */
	/* 因为继承了类 public std::vector<double>， 因此可以直接赋值处理 */
	for (unsigned int i=0; i<size(); i++)
		(*this)[i]=d[i];
}

RangeReading::~RangeReading()
{
//	cerr << __PRETTY_FUNCTION__ << ": CAZZZZZZZZZZZZZZZZZZZZOOOOOOOOOOO" << endl;
}

/* 直观的理解：消除一些距离很近的点, 在调用的过程中，这个值好像设置为分辨率 */
/* 在本项目中，这个函数没有使用 */
unsigned int RangeReading::rawView(double* v, double density) const
{
	/* 直接读取雷达数据 */
	if (density==0)
	{
		for (unsigned int i=0; i<size(); i++)
			v[i]=(*this)[i];
	}
	else 
	{
		Point lastPoint(0,0); /* 传感器的位置 */
		uint suppressed=0;

		for (unsigned int i=0; i<size(); i++)
		{
			const RangeSensor* rs=dynamic_cast<const RangeSensor*>(getSensor());
			
			assert(rs);
			
			Point lp(cos(rs->beams()[i].pose.theta)*(*this)[i],sin(rs->beams()[i].pose.theta)*(*this)[i]);   /* 转换成位置坐标 */

			Point dp = lastPoint - lp;

			double distance=sqrt(dp*dp);
			
			if (distance<density)
			{
			  //				v[i]=MAXDOUBLE;
				v[i]=std::numeric_limits<double>::max(); /* 设置成最大值，直接忽略 */
				suppressed++;
			}
			else
			{
				lastPoint=lp;
				v[i]=(*this)[i];
			}
			//std::cerr<< __PRETTY_FUNCTION__ << std::endl;
			//std::cerr<< "suppressed " << suppressed <<"/"<<size() << std::endl;
		}
	}

	//	return size();
	return static_cast<unsigned int>(size());
};


/* 这个函数与上面的函数相同，在本程序中没有调用， XXXXXXXXXXXXXXXXXXXXXXX  */
unsigned int RangeReading::activeBeams(double density) const
{
	if (density==0.)
		return size();

	int ab=0;
	Point lastPoint(0,0);
	
	uint suppressed=0;
	
	for (unsigned int i=0; i<size(); i++)
	{
		const RangeSensor* rs=dynamic_cast<const RangeSensor*>(getSensor());

		assert(rs);

		Point lp(cos(rs->beams()[i].pose.theta)*(*this)[i],sin(rs->beams()[i].pose.theta)*(*this)[i]);

		Point dp=lastPoint-lp;
		
		double distance=sqrt(dp*dp);
		
		if (distance<density)
		{
			suppressed++;
		}
		else{
			lastPoint=lp;
			ab++;
		}
	}
	
	return ab;
}


/* 这个函数与上面的函数相同，在本程序中没有调用， XXXXXXXXXXXXXXXXXXXXXXX  */
std::vector<Point> RangeReading::cartesianForm(double maxRange) const
{
	const RangeSensor* rangeSensor=dynamic_cast<const RangeSensor*>(getSensor());

	assert(rangeSensor && rangeSensor->beams().size());
	//	uint m_beams=rangeSensor->beams().size();

	uint m_beams=static_cast<unsigned int>(rangeSensor->beams().size());

	std::vector<Point> cartesianPoints(m_beams);

	double px,py,ps,pc;

	px = rangeSensor->getPose().x;  /* px 与 py 表示激光器在机器人坐标系中的坐标位置 */
	py = rangeSensor->getPose().y;
	ps = sin(rangeSensor->getPose().theta);
	pc = cos(rangeSensor->getPose().theta);

	for (unsigned int i=0; i<m_beams; i++)
	{
		const double& rho=(*this)[i]; /* 距离值 */
		const double& s=rangeSensor->beams()[i].s;
		const double& c=rangeSensor->beams()[i].c;

		if (rho>=maxRange)
		{
			cartesianPoints[i] = Point(0,0);
		}
		else 
		{
			/* rangeSensor->beams()[i].pose.x 和 y 表示在激光器坐标系中的位置 */
			Point p = Point(rangeSensor->beams()[i].pose.x+c*rho, rangeSensor->beams()[i].pose.y+s*rho);
			
			/* 这是一个旋转操作， 转换到机器人的坐标系 */
			cartesianPoints[i].x = px+pc*p.x-ps*p.y;
			cartesianPoints[i].y = py+ps*p.x+pc*p.y;
		}
	}
	
	return cartesianPoints;
}

};

