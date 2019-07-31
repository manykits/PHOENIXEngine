#ifndef SMMAP_H
#define SMMAP_H
#include "map.h"
#include "harray2d.h"
#include "point.h"
#define SIGHT_INC 1


namespace GMapping {

/* 用于记录地图中 */
struct PointAccumulator
{
	typedef point<float> FloatPoint;
	/* before 
	PointAccumulator(int i=-1): acc(0,0), n(0), visits(0){assert(i==-1);}
	*/
	/*after begin*/
	PointAccumulator() : acc(0, 0), n(0), visits(0), op(0.0), mean_p(0,0)
	{
	}
	
	PointAccumulator(int i) : acc(0, 0), n(0), visits(0), op(0.0), mean_p(0, 0)
	{
		assert(i==-1);
	}
	/*after end*/

	inline void update(bool value, const Point& p=Point(0,0));

	/* 从世界坐标转换到地图坐标，需要做平均值，这样更加精确 */
	inline Point mean() const 
	{
		return 1./n*Point(acc.x, acc.y);
	}
	
	/* 返回是障碍物的概率 */
	inline operator double() const
	{
		return visits?(double)n*SIGHT_INC/(double)visits : -1;
	}

	/* 两个点集进行相加 */
	inline void add(const PointAccumulator& p) 
	{
		acc=acc+p.acc; 
		n+=p.n; 
		visits+=p.visits; 
	}
	
	static const PointAccumulator& Unknown();
	static PointAccumulator* unknown_ptr;
	FloatPoint acc;
	int n, visits;
	inline double entropy() const;

	Point mean_p;

	float op;
};

/* 这里的 p 为世界坐标 */
void PointAccumulator::update(bool value, const Point& p)
{
	if (value)
	{
		acc.x+= static_cast<float>(p.x);
		acc.y+= static_cast<float>(p.y); 
		n++; 
		visits+=SIGHT_INC;   /* occupancy */
		op = (float)n*SIGHT_INC / (float)visits;
		mean_p = 1. / n*Point(acc.x, acc.y);
	} else
		visits++;            /* free */
}

/* 计算信息熵 */
double PointAccumulator::entropy() const
{
	if (!visits)
		return -log(.5);

	if (n==visits || n==0)
		return 0;

	double x=(double)n*SIGHT_INC/(double)visits;

	return -( x*log(x)+ (1-x)*log(1-x) );
}


typedef Map<PointAccumulator,HierarchicalArray2D<PointAccumulator> > ScanMatcherMap;

}

#endif 
