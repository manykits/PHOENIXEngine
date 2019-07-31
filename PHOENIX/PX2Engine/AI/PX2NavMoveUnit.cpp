// PX2NavMoveUnit.cpp

#include "PX2NavMoveUnit.hpp"
#include "PX2NavMoveManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
inline Point2D _Vector2Point2D(const Vector2f &vec)
{
	return Point2D(int(vec.X()*FIXPOINT_SCALE), int(vec.Y()*FIXPOINT_SCALE));
}
//----------------------------------------------------------------------------
inline Vector2f _Point2D2RVOVec(const Point2D &pt)
{
	return Vector2f(pt.x / FIXPOINT_SCALE, pt.y / FIXPOINT_SCALE);
}
//----------------------------------------------------------------------------
MoveUnit::MoveUnit():
mState(STATE_STOP),
mCurPathPoint(-1),
mMaskStop(false),
mSpeed(0.0f),
mMaskRadius(0.0f),
mMoveSegLen(0),
mAccumMoveLen(0.0f)
{
}
//----------------------------------------------------------------------------
MoveUnit::~MoveUnit()
{
}
//----------------------------------------------------------------------------
void MoveUnit::SetMaskRadius(float radius)
{
	mMaskRadius = radius*1.2f;
}
//----------------------------------------------------------------------------
void MoveUnit::LeaveMap()
{
}
//----------------------------------------------------------------------------
void MoveUnit::SetPosition(float x, float y)
{
	mCurPos = Vector2f(x, y);
}
//----------------------------------------------------------------------------
void MoveUnit::SetSpeed(float speed)
{
	mSpeed = speed;
}
//----------------------------------------------------------------------------
inline float _Abs(const Vector2f &vector)
{
	return std::sqrt(vector.Dot(vector));
}
//----------------------------------------------------------------------------
bool MoveUnit::GetNearGoal(Vector2f &goal)
{
	MoveManager *moveMan = MoveManager::GetSingletonPtr();
	if (!moveMan)
		return false;

	const Vector2f &curpos = mCurPos;

	Vector2f vec = goal - curpos;
	float len = _Abs(vec);
	float maxlen = moveMan->GetGridSizeX()*10.0f;

	if (len > maxlen*1.2f) //·ÀÖ¹Ê£ÏÂµÄÄÇ¶ÎÌ«¶Ì
	{
		goal = curpos + vec*(maxlen / len);
		return true;
	}
	else
	{
		return false;
	}
}
//----------------------------------------------------------------------------
void MoveUnit::SetTarget(float x, float y)
{
	MoveManager::GetSingletonPtr()->LimitPointInRange(x, y);

	mTargetPos = Vector2f(x, y);

	GoPath(mTargetPos);
}
//----------------------------------------------------------------------------
void MoveUnit::Stop()
{
	mState = STATE_STOP;
	SetObstacle(mCurPos);
}
//----------------------------------------------------------------------------
void MoveUnit::SetObstacle(const Vector2f &pos)
{
	MoveManager *moveMan = MoveManager::GetSingletonPtr();
	if (!moveMan)
		return;

	ClearObstacle();

	mMaskStopPoint = pos;
	Point2D pt = _Vector2Point2D(pos);
	int r = int(mMaskRadius*FIXPOINT_SCALE) - 1;

	moveMan->GetMap()->UpdateMask(pt.x - r, pt.y - r, pt.x + r, pt.y + r, true);

	mMaskStop = true;
}
//----------------------------------------------------------------------------
void MoveUnit::ClearObstacle()
{
	MoveManager *moveMan = MoveManager::GetSingletonPtr();
	if (!moveMan)
		return;

	if (mMaskStop)
	{
		Point2D pt = _Vector2Point2D(mMaskStopPoint);
		int r = int(mMaskRadius*FIXPOINT_SCALE) - 1;

		if (moveMan->GetMap())
		{
			moveMan->GetMap()->UpdateMask(pt.x - r, pt.y - r, pt.x + r, pt.y + r, false);
		}
		mMaskStop = false;
	}
}
//----------------------------------------------------------------------------
bool MoveUnit::IsOnObstacle()
{
	MoveManager *moveMan = MoveManager::GetSingletonPtr();
	if (!moveMan)
		return false;

	if (moveMan->GetMap())
	{
		Point2D pt = _Vector2Point2D(mCurPos);
		if (!moveMan->GetMap()->CanWalk(pt.x, pt.y, NavLogicMap::maskStop)) return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void MoveUnit::GoPath(const Vector2f &finalgoal)
{
	MoveManager *moveMan = MoveManager::GetSingletonPtr();
	if (!moveMan)
		return;

	Vector2f goal = finalgoal;
	bool changed = GetNearGoal(goal);
	Point2D start = _Vector2Point2D(mCurPos);
	Point2D end = _Vector2Point2D(goal);

	NavLogicMap *pmap = moveMan->GetMap();
	bool isfind = false;

	PX2_UNUSED(changed);
	PX2_UNUSED(pmap);

	if (start == end)
	{
		isfind = false;
	}
	else
	{
		isfind = moveMan->GetMap()->FindNaviPath(mNaviPath, start, end);
		assertion(isfind, "should find");
	}

	if (!isfind)
	{
		Stop();
		return;
	}

	assert(mNaviPath.GetNumPoint() > 1);

	mNaviPath.SmoothPath();
	mNaviPath.ComputePathSegLen();
	mCurPathPoint = 0;
	mMoveSegLen = 0;
	mAccumMoveLen = 0.0f;
	mState = STATE_MOVE;

	ClearObstacle();
}
//----------------------------------------------------------------------------
void MoveUnit::Update(float dtime)
{
	if (mState == STATE_STOP)
	{
		return;
	}

	assert(mCurPathPoint < mNaviPath.GetNumPoint() - 1);

	if (!mNaviPath.IsPathOK(mCurPathPoint, mMoveSegLen))
	{
		CheckNewPath();
		return;
	}

	mAccumMoveLen += mSpeed*dtime*FIXPOINT_SCALE;
	int movelen = int(mAccumMoveLen);
	mAccumMoveLen -= movelen;

	Point2D curpt = mNaviPath.MoveOnPath(mCurPathPoint, mMoveSegLen, movelen);
	if (mCurPathPoint == mNaviPath.GetNumPoint() - 1)
	{
		mCurPos = Vector2f(mNaviPath.GetPathGoal().x / FIXPOINT_SCALE, 
			mNaviPath.GetPathGoal().y / FIXPOINT_SCALE);
		CheckNewPath();
		return;
	}

	mCurPos = Vector2f(curpt.x / FIXPOINT_SCALE, curpt.y / FIXPOINT_SCALE);

	Point2D vec = mNaviPath.GetPathPoint(mCurPathPoint + 1) - 
		mNaviPath.GetPathPoint(mCurPathPoint);
	mCurDir = Vector2f(vec.x / FIXPOINT_SCALE, vec.y / FIXPOINT_SCALE);
	mCurDir.Normalize();
}
//----------------------------------------------------------------------------
void MoveUnit::GetPosition(float &x, float &y)
{
	x = mCurPos.X();
	y = mCurPos.Y();
}
//----------------------------------------------------------------------------
void MoveUnit::GetDirection(float &x, float &y)
{
	x = mCurDir.X();
	y = mCurDir.Y();
}
//----------------------------------------------------------------------------
void MoveUnit::CheckNewPath()
{
	Vector2f vec = mTargetPos - mCurPos;
	float r = 0.1f;

	float lengthSqr = vec.SquaredLength();
	if (lengthSqr > r*r)
	{
		GoPath(mTargetPos);
	}
	else
	{
		Stop();
	}
}
//----------------------------------------------------------------------------