// PX2NavGridMap.cpp

#include "PX2NavGridMap.hpp"
#include "PX2Math.hpp"
#include "PX2NavBitArray.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
NaviPath::NaviPath() :
mMap(0)
{
}
//----------------------------------------------------------------------------
NaviPath::~NaviPath()
{
}
//----------------------------------------------------------------------------
Point2D NaviPath::GetPathPoint(int i)
{
	assert(i < (int)mPoints.size());
	return Point2D(mPoints[i].x, mPoints[i].y);
}
//----------------------------------------------------------------------------
Point2D NaviPath::GetPathGoal()
{
	assert((int)mPoints.size() > 1);
	stPathPoint &pt = mPoints[mPoints.size() - 1];
	return Point2D(pt.x, pt.y);
}
//----------------------------------------------------------------------------
int NaviPath::GetPathSegLen(int i)
{
	assert(i<int(mPoints.size()) - 1);
	return mPoints[i].len;
}
//----------------------------------------------------------------------------
const int MAX_NAVIGRID = 5000;
//----------------------------------------------------------------------------
bool  IsPointInCircle(int x, int y, int ox, int oy, int radius)
{
	int r2 = (x - ox)*(x - ox) + (y - oy)*(y - oy);
	return r2<radius*radius ? 1 : 0;
}
//----------------------------------------------------------------------------
#define MY_PI 3.1415926f
inline float mycos(float angle)
{
	return cos(angle*MY_PI / 180.0f);
}
//----------------------------------------------------------------------------
inline float mysin(float angle)
{
	return sin(angle*MY_PI / 180.0f);
}
//----------------------------------------------------------------------------
int FaceAngleBetween(int from_x, int from_y, int to_x, int to_y)
{
	int dx = to_x - from_x;
	int dy = to_y - from_y;

	double length = sqrt(double(dx*dx + dy*dy));
	if (length < 0.5) return 0;

	double tmp = dx / length;
	double angle = acos(tmp) * 180 / MY_PI;
	if (dy < 0) angle = (360 - angle);

	return int(angle);
}
//----------------------------------------------------------------------------
bool IsPointInFan(int x, int y, int ox, int oy, int radius, int face_angle,
	int angle_range)
{
	float EPSILON = 5.0f;
	if (!IsPointInCircle(x, y, ox, oy, radius)) return 0;

	float r = sqrt(float((x - ox)*(x - ox) + (y - oy)*(y - oy)));
	if (r < EPSILON) return 1;

	float fangle = float(face_angle);
	float mydot = ((x - ox) / r)*mycos(fangle) + ((y - oy) / r)*mysin(fangle);
	return mydot>mycos(float(angle_range)*0.5f) ? 1 : 0;
}
//----------------------------------------------------------------------------
bool IsPointInRectangle(int x, int y, int ox, int oy, int xlen, int ylen,
	int face_angle)
{
	int x1 = (int)((x - ox)*mycos((float)face_angle) + 
		(y - oy)*mysin((float)face_angle));
	int y1 = (int)(-(x - ox)*mysin((float)face_angle) +
		(y - oy)*mycos((float)face_angle));

	if (x1>0 && x1<xlen && y1>-ylen / 2 && y1<ylen / 2) return 1;
	else return 0;
}
//----------------------------------------------------------------------------
bool NaviPath::IsPointInSegment(int x, int y, int x1, int y1, int x2, int y2)
{
	if (x1 <= x2)
	{
		if (x<x1 || x>x2) return false;
	}
	else if (x<x2 || x>x1) return false;

	if (y1 <= y2)
	{
		if (y<y1 || y>y2) return false;
	}
	else if (y<y2 || y>y1) return false;

	int r = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
	if (r == 0) return true;
	int dist = (x - x1)*(y2 - y1) - (y - y1)*(x2 - x1);
	dist = dist*dist / r;

	return dist <= 2;
}
//----------------------------------------------------------------------------
bool NaviPath::IsPointOnPath(const Point2D &pt)
{
	for (int i = 0; i<int(mPoints.size()) - 1; i++)
	{
		if (IsPointInSegment(pt.x, pt.y, mPoints[i].x, mPoints[i].y, 
			mPoints[i + 1].x, mPoints[i + 1].y)) return true;
	}
	return false;
}
//----------------------------------------------------------------------------
int NaviPath::GetPathLength()
{
	int len = 0;
	for (int i = 0; i<int(mPoints.size()) - 1; i++)
	{
		stPathPoint &p1 = mPoints[i];
		stPathPoint &p2 = mPoints[i + 1];

		len += (int)sqrt(float((p1.x - p2.x)*(p1.x - p2.x) + 
			(p1.y - p2.y)*(p1.y - p2.y)));
	}

	return len;
}
//----------------------------------------------------------------------------
Point2D NaviPath::GetPathPosition(int curpt, int offset)
{
	if (curpt >= GetNumPoint() - 1) return GetPathPoint(curpt);
	else
	{
		Point2D p1 = GetPathPoint(curpt);
		Point2D p2 = GetPathPoint(curpt + 1);

		int seglen = GetPathSegLen(curpt);
		return Point2D(p1.x + (p2.x - p1.x)*offset / seglen, p1.y +
			(p2.y - p1.y)*offset / seglen);
	}
}
//----------------------------------------------------------------------------
Point2D NaviPath::MoveOnPath(int &curpt, int &offset, int movelen)
{
	offset += movelen;
	while (offset >= mPoints[curpt].len)
	{
		offset -= mPoints[curpt].len;
		curpt++;
		if (curpt == int(mPoints.size()) - 1)
		{
			offset = 0;
			return Point2D(mPoints[curpt].x, mPoints[curpt].y);
		}
	}

	return GetPathPosition(curpt, offset);
}
//----------------------------------------------------------------------------
bool NaviPath::IsPathOK(int curpt, int offset)
{
	assert(curpt < int(mPoints.size()) - 1);
	for (int i = 0; i<3; i++)
	{
		while (offset >= mPoints[curpt].len)
		{
			offset -= mPoints[curpt].len;
			curpt++;
			if (mMap->GetGridUpdateTime(mPoints[curpt].x,
				mPoints[curpt].y) > mGridUpdateAccum) return false;
			if (curpt == int(mPoints.size()) - 1) return true;
		}

		Point2D pos = GetPathPosition(curpt, offset);
		if (mMap->GetGridUpdateTime(pos.x, pos.y) > mGridUpdateAccum) 
			return false;

		offset += mMap->mlMaskPixelWidth;
	}

	return true;
}
//----------------------------------------------------------------------------
void NaviPath::AddPathPoint(int x, int y)
{
	if (mPoints.size() > 0)
	{
		stPathPoint &tmp = mPoints[mPoints.size() - 1];
		if (x == tmp.x && y == tmp.y) return;
	}
	stPathPoint pt;
	pt.x = x;
	pt.y = y;

	mPoints.push_back(pt);
}
//----------------------------------------------------------------------------
void NaviPath::Reset(NavLogicMap *pmap)
{
	mPoints.resize(0);
	mMap = pmap;
	mGridUpdateAccum = pmap->mGridUpdateAccum;
}
//----------------------------------------------------------------------------
void NaviPath::Revert()
{
	stPathPoint tmp;
	size_t num = mPoints.size();
	for (size_t i = 0; i<num / 2; i++)
	{
		tmp = mPoints[i];
		mPoints[i] = mPoints[num - 1 - i];
		mPoints[num - 1 - i] = tmp;
	}
}
//----------------------------------------------------------------------------
int  NaviPath::FindNextVisiblePoint(int i)
{
	assert(mPoints.size()>1 && i<int(mPoints.size()));
	Point2D begin(mPoints[i].x, mPoints[i].y);

	int next = i + 1;
	while (next < int(mPoints.size()))
	{
		bool b = mMap->LineOfSightTest(begin, Point2D(mPoints[next].x,
			mPoints[next].y));
		assert(b == mMap->LineOfSightTest1(begin, Point2D(mPoints[next].x,
			mPoints[next].y)));
		if (!b) break;
		next++;
	}
	return next - 1;
}
//----------------------------------------------------------------------------
void NaviPath::LooseGoal(int radius)
//改变path, 让path的goal在和原来目标距离radius的地方
{
	if (radius <= 0) return;

	Point2D goal = GetPathGoal();
	int i;
	for (i = 0; i<int(mPoints.size()); i++)
	{
		int dx = mPoints[i].x - goal.x;
		int dy = mPoints[i].y - goal.y;
		if (dx*dx + dy*dy <= radius*radius) break;
	}

	mPoints.resize(i + 1);
}
//----------------------------------------------------------------------------
void NaviPath::SmoothPath()
{
	int begin = 0;
	while (1)
	{
		int end = FindNextVisiblePoint(begin);
		if (end == begin) break; //到终点了
		if (end > begin + 1) //之间有多余的路径点
		{
			mPoints.erase(mPoints.begin() + begin + 1,
				mPoints.begin() + end);
		}
		begin++;
	}
}
//----------------------------------------------------------------------------
void NaviPath::ComputePathSegLen()
{
	for (size_t i = 0; i<mPoints.size() - 1; i++)
	{
		Point2D p1 = GetPathPoint((int)i);
		Point2D p2 = GetPathPoint((int)i + 1);
		mPoints[i].len = (p2 - p1).Length();
	}
	mPoints.back().len = 0;
}
//----------------------------------------------------------------------------
NavLogicMap::NavLogicMap() :
mMaskData(NULL), mVisitMap(NULL), mGridUpdateTime(NULL)
{
	mClosedNodes.reserve(MAX_NAVIGRID);
}
//----------------------------------------------------------------------------
NavLogicMap::~NavLogicMap()
{
	if (mMaskData) delete[] mMaskData;
	delete mVisitMap;
	if (mGridUpdateTime) delete[] mGridUpdateTime;
}
//----------------------------------------------------------------------------
void NavLogicMap::CreateGraph(int lenghA, int lenghB, int cellsA, int cellsB,
	unsigned char *pmskbits)
{
	mlWidthMasks = cellsA;
	mlHeightMasks = cellsB;

	mlMaskPixelWidth = lenghA / mlWidthMasks;
	mlMaskPixelHeight = lenghB / mlHeightMasks;

	mlPlaneWidth = mlWidthMasks*mlMaskPixelWidth;
	mlPlaneHeight = mlHeightMasks*mlMaskPixelHeight;

	mVisitMap = new BitArray2D(mlWidthMasks, mlHeightMasks);
	if (pmskbits)
	{
		mMaskData = new unsigned char[mlWidthMasks*mlHeightMasks];
		memcpy(mMaskData, pmskbits, mlWidthMasks*mlHeightMasks);
	}
	else memset(mMaskData, 0, mlWidthMasks*mlHeightMasks);

	mGridUpdateTime = new unsigned int[mlWidthMasks*mlHeightMasks];
	memset(mGridUpdateTime, 0, mlWidthMasks*mlHeightMasks*sizeof(int));
}
//----------------------------------------------------------------------------
static Point2D sEndGrid;
inline int ComputeHeuticsCost(int x, int y)
{
	return (abs(x - sEndGrid.x) + abs(y - sEndGrid.y)) * 2 / 3;
}
//----------------------------------------------------------------------------
static std::vector<stPathHeapNode>::iterator FindHeapNodeInArray(
	std::vector<stPathHeapNode> &array, unsigned int index)
{
	std::vector<stPathHeapNode>::iterator iter = array.begin();
	for (; iter != array.end(); iter++)
	{
		if (iter->index == index) return iter;
	}
	return array.end();
}
//----------------------------------------------------------------------------
void NavLogicMap::TryGotoNeighbour(stPathHeapNode &from, int dx, int dy)
{
	int x1 = INDEX_TO_X(from.index) + dx;
	int y1 = INDEX_TO_Y(from.index) + dy;
	if (x1<0 || x1 >= mlWidthMasks || y1<0 || y1 >= mlHeightMasks) 
		return; //超出地图外

	unsigned char frombits = 
		GetGridAttrib(INDEX_TO_X(from.index), INDEX_TO_Y(from.index));
	unsigned char tobits = GetGridAttrib(x1, y1);

	if (tobits & maskStatic)
	{
		return; //静态阻挡，不能通过
	}

	int addaval = int(tobits&maskDynamic) - int(frombits&maskDynamic);
	if (addaval < 0) addaval = 0;
	else addaval *= 5; //从低阻挡到高阻挡相当于绕5格

	unsigned int curindex = MAKEINDEX(x1, y1);
	int aval = from.aval + addaval;
	if (mVisitMap->GetBit(x1, y1))//已经访问过了
	{
		std::vector<stPathHeapNode>::iterator iter = 
			FindHeapNodeInArray(mPathHeap.mAllNodes, curindex);
		if (iter != mPathHeap.mAllNodes.end())
		{
			if (aval < iter->aval)
			{
				iter->aval = aval;
				iter->from = from.index;
				mPathHeap.AdjustNode(iter);
			}
		}
		return;
	}

	stPathHeapNode tmpnode;
	tmpnode.from = from.index;
	tmpnode.index = curindex;
	tmpnode.aval = aval;
	tmpnode.hval = ComputeHeuticsCost(x1, y1);
	mPathHeap.PushNode(tmpnode);

	mVisitMap->SetBit(x1, y1, 1);
}
//----------------------------------------------------------------------------
int NavLogicMap::FindNaviPath_Internal(NaviPath &path, const Point2D &start, 
	const Point2D &input_end)
{
	Point2D scale(mlMaskPixelWidth, mlMaskPixelHeight);
	Point2D end = input_end;
	Point2D startgrid = start / scale;
	sEndGrid = end / scale;

	if (!CanWalk(sEndGrid.x, sEndGrid.y, maskStatic))
	{
		if (!FindPointWalkable(end, 2, maskStatic)) 
			return 1;

		sEndGrid = end / scale;
	}

	mVisitMap->Clear();
	mPathHeap.Init();
	mClosedNodes.resize(0);

	stPathHeapNode node;
	node.from = node.index = MAKEINDEX(startgrid.x, startgrid.y);
	node.hval = node.aval = 0;
	mPathHeap.PushNode(node);
	mVisitMap->SetBit(startgrid.x, startgrid.y, 1);

	bool bfind = false;
	unsigned int endindex = MAKEINDEX(sEndGrid.x, sEndGrid.y);
	while (mPathHeap.HasNode())
	{
		mPathHeap.PopNode(node);
		mClosedNodes.push_back(node);
		if (mClosedNodes.size() >= size_t(MAX_NAVIGRID))
			break;

		if (node.index == endindex)
		{
			bfind = true;
			break;
		}
		else
		{
			TryGotoNeighbour(node, -1, 0);
			TryGotoNeighbour(node, +1, 0);
			TryGotoNeighbour(node, 0, -1);
			TryGotoNeighbour(node, 0, +1);
		}
	}

	if (bfind)
	{
		path.Reset(this);
		path.AddPathPoint(end.x, end.y);

		while (node.from != node.index)
		{
			int x = INDEX_TO_X(node.index)*mlMaskPixelWidth + 
				mlMaskPixelWidth / 2;
			int y = INDEX_TO_Y(node.index)*mlMaskPixelHeight +
				mlMaskPixelHeight / 2;
			path.AddPathPoint(x, y);
			//assert( CanWalkPixelCoord(x, y ) );

			std::vector<stPathHeapNode>::iterator iter = 
				FindHeapNodeInArray(mClosedNodes, node.from);
			assert(iter != mClosedNodes.end());
			node = *iter;
		}

		path.AddPathPoint(start.x, start.y);
		path.Revert();
		//assert( CanWalkPixelCoord(pathend.x, pathend.y) );

		assert(path.mPoints.size() > 1);
		return 0;
	}
	return 2;
}
//----------------------------------------------------------------------------
bool NavLogicMap::FindNaviPath(NaviPath &path, const Point2D &start,
	const Point2D &end)
{
	int naviret = FindNaviPath_Internal(path, start, end);
	if (naviret == 0) 
		return true;
	
	return false;
}
//----------------------------------------------------------------------------
Point2D NavLogicMap::DoMotion(const Point2D &start, const Point2D &delta)
{
	return start + delta;
}
//----------------------------------------------------------------------------
//返回到下一个格子
static Point2D LineIntersectGrid(const Point2D &pt1, const Point2D &pt2, 
	const Point2D &start, const Point2D &dp)
{
	int y1 = pt1.y*dp.x;
	int y2 = pt2.y*dp.x;
	if (dp.x > 0)
	{
		int y = dp.y*(pt2.x - start.x) + start.y*dp.x;
		if (y == y2) return Point2D(1, 1);
		else if (y >= y1 && y<y2) return Point2D(1, 0);
	}
	else if (dp.x < 0)
	{
		int y = dp.y*(pt1.x - start.x) + start.y*dp.x;

		if (y == y2) return Point2D(0, 1);
		else if (y>y2 && y <= y1) return Point2D(-1, 0);
	}

	if (dp.y > 0) return Point2D(0, 1);
	else return Point2D(0, -1);
}
//----------------------------------------------------------------------------
bool NavLogicMap::LineOfSightTest(const Point2D &start, const Point2D &end, 
	int mask)
{
	Point2D step(mlMaskPixelWidth, mlMaskPixelHeight);
	Point2D startgrid(start.x / mlMaskPixelWidth, 
		start.y / mlMaskPixelHeight);
	Point2D endgrid(end.x / mlMaskPixelWidth,
		end.y / mlMaskPixelHeight);
	Point2D dp = end - start;

	Point2D curgrid = startgrid;
	if (!CanWalk(curgrid.x, curgrid.y, mask)) return false;
	while (curgrid != endgrid)
	{
		Point2D pt1 = curgrid * step;
		curgrid += LineIntersectGrid(pt1, pt1 + step, start, dp);
		if (!CanWalk(curgrid.x, curgrid.y, mask)) return false;
	}
	return true;
}
//----------------------------------------------------------------------------
typedef struct {
	Point2D s_StartPt;
	Point2D s_dPt;
	Point2D s_CurGrid;
} GridPoint;
//----------------------------------------------------------------------------
static void LineIntersectGrid(const Point2D *pt1, const Point2D *pt2,
	GridPoint *pstGP)
{
	int dx = pstGP->s_dPt.x;
	int y1 = pt1->y * dx;
	int y2 = pt2->y * dx;
	if (dx > 0)
	{
		int y = pstGP->s_dPt.y*(pt2->x - pstGP->s_StartPt.x) + 
			pstGP->s_StartPt.y*dx;
		if (y == y2)
		{
			pstGP->s_CurGrid.x++;
			pstGP->s_CurGrid.y++;
			return;
		}
		if (y >= y1 && y<y2)
		{
			pstGP->s_CurGrid.x++;
			return;
		}
	}
	else if (dx < 0)
	{
		int y = pstGP->s_dPt.y*(pt1->x - pstGP->s_StartPt.x) +
			pstGP->s_StartPt.y*dx;
		if (y == y2)
		{
			pstGP->s_CurGrid.y++;
			return;
		}
		if (y>y2 && y <= y1)
		{
			pstGP->s_CurGrid.x--;
			return;
		}
	}

	if (pstGP->s_dPt.y > 0) pstGP->s_CurGrid.y++;
	else pstGP->s_CurGrid.y--;
}
//----------------------------------------------------------------------------
bool CMapMask_IsPixelLineThrough(NavLogicMap *pmap, int lPixelX1, int lPixelY1,
	int lPixelX2, int lPixelY2, int mask)
{
	Point2D pt1, pt2;
	int grid_sx;
	int grid_sy;
	int grid_ex;
	int grid_ey;
	GridPoint stGP;

	stGP.s_StartPt.x = lPixelX1;
	stGP.s_StartPt.y = lPixelY1;
	stGP.s_dPt.x = lPixelX2 - lPixelX1;
	stGP.s_dPt.y = lPixelY2 - lPixelY1;

	grid_sx = lPixelX1 / pmap->mlMaskPixelWidth;
	grid_sy = lPixelY1 / pmap->mlMaskPixelHeight;
	grid_ex = lPixelX2 / pmap->mlMaskPixelWidth;
	grid_ey = lPixelY2 / pmap->mlMaskPixelHeight;

	stGP.s_CurGrid.x = grid_sx;
	stGP.s_CurGrid.y = grid_sy;
	if (!pmap->CanWalk(grid_sx, grid_sy, mask)) return false;
	while (stGP.s_CurGrid.x != grid_ex || stGP.s_CurGrid.y != grid_ey)
	{
		pt1.x = stGP.s_CurGrid.x * pmap->mlMaskPixelWidth;
		pt1.y = stGP.s_CurGrid.y * pmap->mlMaskPixelHeight;
		pt2.x = pt1.x + pmap->mlMaskPixelWidth;
		pt2.y = pt1.y + pmap->mlMaskPixelHeight;

		LineIntersectGrid(&pt1, &pt2, &stGP);
		if (!pmap->CanWalk(stGP.s_CurGrid.x, stGP.s_CurGrid.y, mask)) 
			return false;
	}
	return true;
}
//----------------------------------------------------------------------------
bool NavLogicMap::LineOfSightTest1(const Point2D &start, const Point2D &end, 
	int mask)
{
	return CMapMask_IsPixelLineThrough(this, start.x, start.y, end.x, end.y,
		mask);
}
//----------------------------------------------------------------------------
bool NavLogicMap::LineOfSightTest2(const Point2D &start, const Point2D &end, 
	int mask)
{
	int   dx, dy, dm, dn, m, n, k, u, v, l, sum;

	dx = end.x - start.x, dy = end.y - start.y;
	if (dx == 0)
	{
		u = start.y, v = end.y;
		if (dy < 0) u = end.y, v = start.y;
		for (l = u; l <= v; l++)
		{
			if (!CanWalkPixelCoord(end.x, l, mask)) return false;
		}
		return true;
	}
	if (dy == 0)
	{
		u = start.x, v = end.x;
		if (dx < 0) u = end.x, v = start.x;
		for (l = u; l <= v; l++)
		{
			if (!CanWalkPixelCoord(l, end.y, mask)) return false;
		}
		return true;
	}

	dm = dn = 1;
	if (dx < 0) dx = -dx, dm = -1;
	if (dy < 0) dy = -dy, dn = -1;
	m = dx, n = dy, k = 1, u = start.x, v = start.y;
	if (dx<dy) m = dy, n = dx, k = dm, dm = dn, dn = k, k = 0, u = start.y, v = start.x;
	l = 0;
	sum = m;

	if (!CanWalkPixelCoord(start.x, start.y, mask)) return false;
	while (sum != 0)
	{
		sum--;
		l += n;
		u += dm;
		if (l >= m)
		{
			v += dn;
			l -= m;
		}
		if (k == 1)
		{
			if (!CanWalkPixelCoord(u, v, mask)) return false;
		}
		else
		{
			if (!CanWalkPixelCoord(v, u, mask)) return false;
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool NavLogicMap::FindPointWalkable(Point2D &point, int range, int mask)
{
	Point2D grid(point);
	CoordPixel2Grid(grid.x, grid.y);

	if (CanWalk(grid.x, grid.y, mask)) return true;

	for (int i = 1; i <= range; i++)
	{
		int x, y;
		unsigned int flags = 0;
		int sx = grid.x - i; if (sx < 0) 
		{ sx = 0; flags |= 1; }
		int sy = grid.y - i; if (sy < 0) 
		{ sy = 0; flags |= 2; }
		int ex = grid.x + i; if (ex >= mlWidthMasks) 
		{ ex = mlWidthMasks - 1; flags |= 4; }
		int ey = grid.y + i; if (ey >= mlHeightMasks)
		{ ey = mlHeightMasks - 1; flags |= 8; }

		if (!(flags & 2))
		{
			for (x = sx; x <= ex; x++)
			{
				if (CanWalk(x, sy, mask))
				{ point.x = x, point.y = sy; goto find_end; }
			}
		}

		if (!(flags & 8))
		{
			for (x = sx; x <= ex; x++)
			{
				if (CanWalk(x, ey, mask))
				{ point.x = x, point.y = ey; goto find_end; }
			}
		}

		if (!(flags & 1))
		{
			for (y = sy + 1; y <= ey - 1; y++)
			{
				if (CanWalk(sx, y, mask))
				{ point.x = sx, point.y = y; goto find_end; }
			}
		}

		if (!(flags & 4))
		{
			for (y = sy + 1; y <= ey - 1; y++)
			{
				if (CanWalk(ex, y, mask))
				{ point.x = ex, point.y = y; goto find_end; }
			}
		}
	}
	return false;

find_end:
	CoordGrid2Pixel(point.x, point.y);
	return true;
}
//----------------------------------------------------------------------------
bool NavLogicMap::CanWalkPixelCoord(int x, int y, int mask)
{
	if (!IsPointInMap(Point2D(x, y))) return false;

	int x1(x), y1(y);
	CoordPixel2Grid(x1, y1);
	return CanWalk(x1, y1, mask);
}
//----------------------------------------------------------------------------
bool NavLogicMap::IsInSameGrid(const Point2D &p1, const Point2D &p2)
{
	if ((p1.x / mlMaskPixelWidth) == (p2.x / mlMaskPixelWidth) && 
		(p1.y / mlMaskPixelHeight) == (p2.y / mlMaskPixelHeight)) return true;
	else return false;
}
//----------------------------------------------------------------------------
void NavLogicMap::UpdateMask(int x1, int y1, int x2, int y2, bool b)
{
	mGridUpdateAccum++;

	assert(x1<x2 && y1<y2);
	if (x1<0) x1 = 0;
	if (y1<0) y1 = 0;
	if (x2 >= mlPlaneWidth) x2 = mlPlaneWidth - 1;
	if (y2 >= mlPlaneHeight) y2 = mlPlaneHeight - 1;

	int gx1(x1), gx2(x2), gy1(y1), gy2(y2);
	CoordPixel2Grid(gx1, gy1);
	CoordPixel2Grid(gx2, gy2);

	for (int y = gy1; y <= gy2; y++)
	{
		for (int x = gx1; x <= gx2; x++)
		{
			unsigned char &bits = mMaskData[y*mlWidthMasks + x];
			if (b)
			{
				mGridUpdateTime[y*mlWidthMasks + x] = mGridUpdateAccum;
				bits++;
				//assert(bits < maskDynamic);
			}
			else
			{
				assert(bits > 0);
				bits--;
			}
		}
	}
}
//----------------------------------------------------------------------------
unsigned int NavLogicMap::GetGridUpdateTime(int x, int y)
{
	int gx(x), gy(y);
	CoordPixel2Grid(gx, gy);

	return mGridUpdateTime[gy*mlWidthMasks + gx];
}
//----------------------------------------------------------------------------
class stGoalGrid
{
public:
	~stGoalGrid()
	{

	}
	stGoalGrid()
	{

	}

	bool operator < (const stGoalGrid &rhs) const
	{
		return dirprior < rhs.dirprior;
	}

	Point2D grid;
	int dirprior; //距离越小，优先级越大
};
//----------------------------------------------------------------------------
bool NavLogicMap::FindSimplePath_Internal(NaviPath &path, const Point2D &start,
	const Point2D &end, int range)
{
	stGoalGrid gridbuf[100];
	int count = 0;
	assert(range * 4 <= 100);

	Point2D endgrid(end.x / mlMaskPixelWidth, end.y / mlMaskPixelHeight);
	for (int y = endgrid.y - range; y <= endgrid.y + range; y++)
	{
		if (y<0 || y >= mlHeightMasks) continue;
		int x1 = endgrid.x - range;
		int x2 = endgrid.x + range;

		if (x1 >= 0 && CanWalk(x1, y)) 
			gridbuf[count++].grid = Point2D(x1, y);
		if (x2<mlWidthMasks && CanWalk(x2, y))
			gridbuf[count++].grid = Point2D(x2, y);
	}

	for (int x = endgrid.x - range + 1; x <= endgrid.x + range - 1; x++)
	{
		if (x<0 || x >= mlWidthMasks) continue;
		int y1 = endgrid.y - range;
		int y2 = endgrid.y + range;

		if (y1 >= 0 && CanWalk(x, y1))
			gridbuf[count++].grid = Point2D(x, y1);
		if (y2<mlHeightMasks && CanWalk(x, y2))
			gridbuf[count++].grid = Point2D(x, y2);
	}

	if (count == 0) return false;
	assert(count < 100);

	//对每个goalgrid, 计算寻路优先级
	Point2D dp = end - start;
	Point2D step(mlMaskPixelWidth, mlMaskPixelHeight);
	Point2D halfstep(mlMaskPixelWidth / 2, mlMaskPixelHeight / 2);

	int i;
	for (i = 0; i<count; i++)
	{
		Point2D goalpt = gridbuf[i].grid * step + halfstep - start;
		gridbuf[i].dirprior = goalpt.x*dp.x + goalpt.y*dp.y;
	}

	std::sort(&gridbuf[0], &gridbuf[count]);

	for (i = 0; i<1/*count*/; i++)
	{
		Point2D goalpt = gridbuf[i].grid * step + halfstep;
		if (start == goalpt) 
			continue;

		if (FindNaviPath_Internal(path, start, goalpt) == 0)
			return true;
	}
	return false;
}
//----------------------------------------------------------------------------
bool NavLogicMap::FindDirFaceMask(Point2D &point, const Point2D &center, 
	int radius, int mask)
{
	const int NUM_SEG = 18; //把圆周分为多少等分
	const float TWOPI = 3.1415926f * 2;

	for (int i = 0; i<NUM_SEG; i++)
	{
		float angle = i*TWOPI / NUM_SEG;
		int x = int(center.x + radius*cos(angle));
		int y = int(center.y + radius*sin(angle));
		if (!CanWalkPixelCoord(x, y, mask))
		{
			point.x = x;
			point.y = y;
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------