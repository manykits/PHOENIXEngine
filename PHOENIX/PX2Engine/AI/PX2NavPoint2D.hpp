// PX2NavPoint2D.hpp

#ifndef PX2NAVPOINT2D_HPP
#define PX2NAVPOINT2D_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class Point2D
	{
	public:
		Point2D(){ x = 0; y = 0; }
		Point2D(int x1, int y1) : x(x1), y(y1){}
		Point2D(const Point2D &rhs) : x(rhs.x), y(rhs.y){}
		Point2D &operator =(const Point2D &rhs){ x = rhs.x, y = rhs.y; return *this; }
		Point2D operator +(const Point2D &rhs) const
		{
			return Point2D(x + rhs.x, y + rhs.y);
		}
		Point2D operator -(const Point2D &rhs) const
		{
			return Point2D(x - rhs.x, y - rhs.y);
		}
		Point2D operator *(const Point2D &rhs) const
		{
			return Point2D(x*rhs.x, y*rhs.y);
		}
		Point2D operator /(const Point2D &rhs) const
		{
			return Point2D(x / rhs.x, y / rhs.y);
		}

		Point2D operator *(int scale) const
		{
			return Point2D(x*scale, y*scale);
		}

		Point2D operator /(int scale) const
		{
			return Point2D(x / scale, y / scale);
		}

		bool operator==(const Point2D &rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}

		bool operator!=(const Point2D &rhs) const
		{
			return x != rhs.x || y != rhs.y;
		}

		Point2D &operator +=(const Point2D &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Point2D &operator -=(const Point2D &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Point2D &operator *=(float scale)
		{
			x = int(x*scale);
			y = int(y*scale);
			return *this;
		}

		Point2D &operator /=(float scale)
		{
			x = int(x / scale);
			y = int(y / scale);
			return *this;
		}

		int LengthSq(){ return x*x + y*y; }
		int Length(){ return (int)sqrt(double(x*x + y*y)); }

	public:
		int x;
		int y;
	};

}

#endif