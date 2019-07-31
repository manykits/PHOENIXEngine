// PX2NavGridMap.inl

//----------------------------------------------------------------------------
inline int NaviPath::GetNumPoint() const
{
	return int(mPoints.size()); 
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
inline bool NavLogicMap::CanWalkGridCoord(int x, int y, int mask)
{
	return CanWalk(x, y, mask);
}
//----------------------------------------------------------------------------
inline bool NavLogicMap::IsPointInMap(const Point2D &point)
{
	return point.x >= 0 && point.x<mlPlaneWidth && point.y >= 0 
		&& point.y<mlPlaneHeight;
}
//----------------------------------------------------------------------------
inline unsigned char NavLogicMap::GetGridAttrib(int x, int y)
{
	return mMaskData[y*mlWidthMasks + x];
}
//----------------------------------------------------------------------------
inline bool NavLogicMap::CanWalk(int x, int y, int mask)
{
	return (mMaskData[y*mlWidthMasks + x] & mask) == 0;
}
//----------------------------------------------------------------------------
inline void NavLogicMap::CoordPixel2Grid(int &x, int &y)
{
	x /= mlMaskPixelWidth, y /= mlMaskPixelHeight;
}
//----------------------------------------------------------------------------
inline void NavLogicMap::CoordGrid2Pixel(int &x, int &y)
{
	x = x*mlMaskPixelWidth + mlMaskPixelWidth / 2;
	y = y*mlMaskPixelHeight + mlMaskPixelHeight / 2;
}
//----------------------------------------------------------------------------