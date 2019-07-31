#ifndef MAP_H
#define MAP_H
#include "point.h"
#include <assert.h>
#include "accessstate.h"
#include "array2d.h"


namespace GMapping {
/**
The cells have to define the special value Cell::Unknown to handle with the unallocated areas.
The cells have to define (int) constructor;
 */
typedef Array2D<double> DoubleArray2D;
/* 用于构建地图数据 */
template <class Cell, class Storage, const bool isClass=true>
class Map
{
public:
	Map(int mapSizeX, int mapSizeY, double delta);
	Map(const Point& center, double worldSizeX, double worldSizeY, double delta);
	Map(const Point& center, double xmin, double ymin, double xmax, double ymax, double delta);
	/* the standard implementation works filen in this case*/
	//Map(const Map& g);
	//Map& operator =(const Map& g);
	void resize(double xmin, double ymin, double xmax, double ymax);
	void grow(double xmin, double ymin, double xmax, double ymax);
	inline IntPoint world2map(const Point& p) const;
	inline Point map2world(const IntPoint& p) const;
	
	inline IntPoint world2map(double x, double y) const
	{
		return world2map(Point(x,y)); 
	}
	
	inline Point map2world(int x, int y) const
	{
		return map2world(IntPoint(x,y)); 
	}

	inline Point getCenter() const 
	{
		return m_center;
	}
	
	inline double getWorldSizeX() const 
	{
		return m_worldSizeX;
	}
	
	inline double getWorldSizeY() const 
	{
		return m_worldSizeY;
	}
	
	inline int getMapSizeX() const 
	{
		return m_mapSizeX;
	}
	
	inline int getMapSizeY() const 
	{
		return m_mapSizeY;
	}
	
	inline double getDelta() const 
	{
		return m_delta;
	}
	
	inline double getMapResolution() const 
	{
		return m_delta;
	}
	
	inline double getResolution() const 
	{
		return m_delta;
	}
	
	/* 这里得到的大小都是世界坐标 */
	inline void getSize(double & xmin, double& ymin, double& xmax, double& ymax) const 
	{
		Point min=map2world(0,0), max=map2world(IntPoint(m_mapSizeX-1, m_mapSizeY-1));
		xmin=min.x, ymin=min.y,  xmax=max.x, ymax=max.y;
	}

	inline Cell& cell(int x, int y) 
	{
		return cell(IntPoint(x, y));
	}
	inline Cell& cell(const IntPoint& p);

	inline const Cell& cell(int x, int y) const  
	{
		return cell(IntPoint(x, y));
	}
	inline const Cell& cell(const IntPoint& p) const;

	inline Cell& cell(double x, double y) 
	{
		return cell(Point(x, y));
	}
	inline Cell& cell(const Point& p);

	inline const Cell& cell(double x, double y) const 
	{
		return cell(Point(x, y));
	}

	inline bool isInside(int x, int y) const 
	{
		return m_storage.cellState(IntPoint(x,y))&Inside;
	}
	
	inline bool isInside(const IntPoint& p) const 
	{
		return m_storage.cellState(p)&Inside;
	}

	inline bool isInside(double x, double y) const 
	{
		return m_storage.cellState(world2map(x,y))&Inside;
	}

	inline bool isInside(const Point& p) const 
	{
		return m_storage.cellState(world2map(p))&Inside;
	}

	inline const Cell& cell(const Point& p) const;

	inline Storage& storage() 
	{
		return m_storage; 
	}
	
	inline const Storage& storage() const 
	{
		return m_storage; 
	}
	
	DoubleArray2D* toDoubleArray() const; /* 转换为 double 数组 */
    
	Map<double, DoubleArray2D, false>* toDoubleMap() const; /* 转换为 double 类型的地图 */

protected:
	Point m_center;
	double m_worldSizeX, m_worldSizeY, m_delta; /* 世界坐标的大小和分辨率 */
	Storage m_storage; /* 地图的数据存储，采用的双层结构，大小为 m_mapSizeX 和 m_mapSizeY */
	
	int m_mapSizeX, m_mapSizeY; /* 地图的大小 */

	int m_sizeX2, m_sizeY2;    /* 地图的偏移量,即: m_sizeX2=m_mapSizeX >> 1 */
	
	static const Cell m_unknown;
};

typedef Map<double, DoubleArray2D, false> DoubleMap;

template <class Cell, class Storage, const bool isClass>

const Cell  Map<Cell,Storage,isClass>::m_unknown = Cell(-1);

/*
int mapSizeX,  地图的大小
int mapSizeY, 
double delta   类似于地图的分辨率
*/
template <class Cell, class Storage, const bool isClass>
Map<Cell,Storage,isClass>::Map(int mapSizeX, int mapSizeY, double delta):
m_storage(mapSizeX, mapSizeY) /* 地图的存储结构，是分层存储结构 */
{
	/* delta 相当于分辨率 */
	m_worldSizeX=mapSizeX * delta;
	m_worldSizeY=mapSizeY * delta;
	m_delta=delta;
	m_center=Point(0.5*m_worldSizeX, 0.5*m_worldSizeY); /* 相当于地图的中心点 */
	m_sizeX2=m_mapSizeX >> 1; /* 地图的偏移量 */
	m_sizeY2=m_mapSizeY >> 1;
}

/*
const Point& center,   地图中心
double worldSizeX, double worldSizeY,  世界坐标大小
double delta 分辨率
注意：worldSizeX 与 m_mapSizeX 不一定为 m_delta 的整数倍关系，因为这里面有取整操作
      另外还有 getPatchSize 的操作
*/
template <class Cell, class Storage, const bool isClass>
Map<Cell,Storage,isClass>::Map(const Point& center, double worldSizeX, double worldSizeY, double delta):
m_storage((int)ceil(worldSizeX/delta), (int)ceil(worldSizeY/delta))
{
	m_center = center;
	m_worldSizeX=worldSizeX;
	m_worldSizeY=worldSizeY;
	m_delta=delta;
	m_mapSizeX=m_storage.getXSize()<<m_storage.getPatchSize();
	m_mapSizeY=m_storage.getYSize()<<m_storage.getPatchSize();
	m_sizeX2=m_mapSizeX>>1;
	m_sizeY2=m_mapSizeY>>1;
}

/*
const Point& center,    地图中心点
double xmin, double ymin,  定义地图的最小点， 这是世界坐标的最大值和最小值
double xmax, double ymax,  定义地图的最大点
double delta 分辨率
*/
template <class Cell, class Storage, const bool isClass>
Map<Cell,Storage,isClass>::Map(const Point& center, double xmin, double ymin, double xmax, double ymax, double delta):
m_storage((int)ceil((xmax-xmin)/delta), (int)ceil((ymax-ymin)/delta))
{
	m_center     = center;
	m_worldSizeX = xmax-xmin;
	m_worldSizeY = ymax-ymin;
	m_delta      = delta;
	m_mapSizeX = m_storage.getXSize()<<m_storage.getPatchSize();
	m_mapSizeY = m_storage.getYSize()<<m_storage.getPatchSize();
	m_sizeX2=(int)round((m_center.x-xmin) / m_delta);
	m_sizeY2=(int)round((m_center.y-ymin) / m_delta);
}

/*
重新定义地图的大小
double xmin, double ymin, 世界坐标的最大值和最小值
double xmax, double ymax
*/
template <class Cell, class Storage, const bool isClass>
void Map<Cell,Storage,isClass>::resize(double xmin, double ymin, double xmax, double ymax)
{
	IntPoint imin = world2map(xmin, ymin); /* 地图坐标的最大值和最小值 */
	IntPoint imax = world2map(xmax, ymax);
	int pxmin, pymin, pxmax, pymax;

	/* 分层存储结构第一层的大小 */
	pxmin=(int)floor((float)imin.x/(1<<m_storage.getPatchMagnitude()));
	pxmax=(int)ceil ((float)imax.x/(1<<m_storage.getPatchMagnitude()));
	pymin=(int)floor((float)imin.y/(1<<m_storage.getPatchMagnitude()));
	pymax=(int)ceil ((float)imax.y/(1<<m_storage.getPatchMagnitude()));
	
	/* resize 操作，实际的操作在 harray2d.h 中完成，void HierarchicalArray2D<Cell>::resize(int xmin, int ymin, int xmax, int ymax) */
	m_storage.resize(pxmin, pymin, pxmax, pymax);
	
	m_mapSizeX = m_storage.getXSize()<<m_storage.getPatchSize(); /* 地图的大小 */
	m_mapSizeY = m_storage.getYSize()<<m_storage.getPatchSize();
	
	m_worldSizeX = xmax-xmin;   /* 世界坐标的大小 */
	m_worldSizeY = ymax-ymin;
	
	m_sizeX2 -= pxmin*(1<<m_storage.getPatchMagnitude());
	m_sizeY2 -= pymin*(1<<m_storage.getPatchMagnitude());
}

/*
重新定义地图的大小，注意与 resize函数 区别， 这个函数没有使用 XXXXXXXXXXXX
double xmin, double ymin, 世界坐标的最大值和最小值
double xmax, double ymax
*/
template <class Cell, class Storage, const bool isClass>
void Map<Cell,Storage,isClass>::grow(double xmin, double ymin, double xmax, double ymax)
{
	IntPoint imin=world2map(xmin, ymin);
	IntPoint imax=world2map(xmax, ymax);
	if (isInside(imin) && isInside(imax))
		return;
	imin=min(imin, IntPoint(0,0));
	imax=max(imax, IntPoint(m_mapSizeX-1,m_mapSizeY-1));
	int pxmin, pymin, pxmax, pymax;
	pxmin=(int)floor((float)imin.x/(1<<m_storage.getPatchMagnitude()));
	pxmax=(int)ceil((float)imax.x/(1<<m_storage.getPatchMagnitude()));
	pymin=(int)floor((float)imin.y/(1<<m_storage.getPatchMagnitude()));
	pymax=(int)ceil((float)imax.y/(1<<m_storage.getPatchMagnitude()));
	m_storage.resize(pxmin, pymin, pxmax, pymax);
	m_mapSizeX=m_storage.getXSize()<<m_storage.getPatchSize();
	m_mapSizeY=m_storage.getYSize()<<m_storage.getPatchSize();
	m_worldSizeX=xmax-xmin;
	m_worldSizeY=ymax-ymin;
	m_sizeX2-=pxmin*(1<<m_storage.getPatchMagnitude());
	m_sizeY2-=pymin*(1<<m_storage.getPatchMagnitude());
}

/* 世界坐标到地图坐标的转换, 注意他们坐标系的不一样 */
template <class Cell, class Storage, const bool isClass>
IntPoint Map<Cell,Storage,isClass>::world2map(const Point& p) const
{
	return IntPoint( (int)round((p.x-m_center.x)/m_delta)+m_sizeX2, (int)round((p.y-m_center.y)/m_delta)+m_sizeY2);
}

/* 地图坐标到世界坐标的转换 */
template <class Cell, class Storage, const bool isClass>
Point Map<Cell,Storage,isClass>::map2world(const IntPoint& p) const
{
	return Point( (p.x-m_sizeX2)*m_delta, (p.y-m_sizeY2)*m_delta)+m_center;
}

/* 返回地图cell 的状态, 注意和下面函数的区别  */
template <class Cell, class Storage, const bool isClass>
Cell& Map<Cell,Storage,isClass>::cell(const IntPoint& p) 
{
	/* cellState 实际的实现在 array2d.h 头文件中实现 */
	AccessibilityState s = m_storage.cellState(p);
	
	if (!s&Inside)
		assert(0);
	//if (s&Allocated) return m_storage.cell(p); assert(0);

	// this will never happend. Just to satify the compiler..
	return m_storage.cell(p);
}

template <class Cell, class Storage, const bool isClass>
Cell& Map<Cell,Storage,isClass>::cell(const Point& p) 
{
	IntPoint ip=world2map(p);

	/* cellState 实际的实现在 array2d.h 头文件中实现 */
	AccessibilityState s=m_storage.cellState(ip); 
	if (! s&Inside)
		assert(0);
	//if (s&Allocated) return m_storage.cell(ip); assert(0);

	// this will never happend. Just to satify the compiler..
	return m_storage.cell(ip);
}

template <class Cell, class Storage, const bool isClass>
const Cell& Map<Cell,Storage,isClass>::cell(const IntPoint& p) const 
{
	AccessibilityState s=m_storage.cellState(p);
	//if (! s&Inside) assert(0);
	if (s&Allocated)
		return m_storage.cell(p);
	return m_unknown;
}

template <class Cell, class Storage, const bool isClass>
const  Cell& Map<Cell,Storage,isClass>::cell(const Point& p) const 
{
	IntPoint ip=world2map(p);
	AccessibilityState s=m_storage.cellState(ip);
	//if (! s&Inside) assert(0);
	if (s&Allocated)
		return m_storage.cell(ip);
	return  m_unknown;
}

//FIXME check why the last line of the map is corrupted.
template <class Cell, class Storage, const bool isClass>
DoubleArray2D* Map<Cell,Storage,isClass>::toDoubleArray() const
{
	DoubleArray2D* darr=new DoubleArray2D(getMapSizeX()-1, getMapSizeY()-1);
	/* x ，y为地图坐标 */
	for(int x=0; x<getMapSizeX()-1; x++)
		for(int y=0; y<getMapSizeY()-1; y++)
		{
			IntPoint p(x,y);
			darr->cell(p)=double(cell(p));
		}

	return darr;
}

template <class Cell, class Storage, const bool isClass>
Map<double, DoubleArray2D, false>* Map<Cell,Storage,isClass>::toDoubleMap() const
{
	//FIXME size the map so that m_center will be setted accordingly
	Point pmin=map2world(IntPoint(0,0));
	Point pmax=map2world(getMapSizeX()-1,getMapSizeY()-1);
	Point center=(pmax+pmin)*0.5;

	Map<double, DoubleArray2D, false>*  plainMap=new Map<double, DoubleArray2D, false>(center, (pmax-pmin).x, (pmax-pmin).y, getDelta());

	for(int x=0; x<getMapSizeX()-1; x++)
		for(int y=0; y<getMapSizeY()-1; y++)
		{
			IntPoint p(x,y);
			plainMap->cell(p) = double(cell(p));
		}

	return plainMap;
}

};

#endif

