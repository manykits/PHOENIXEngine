// PX2NavMoveUnit.hpp

#ifndef PX2NAVMOVEUNIT_HPP
#define PX2NAVMOVEUNIT_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector2.hpp"
#include "PX2NavGridMap.hpp"

namespace PX2
{

	class MoveManager;

	class MoveUnit
	{
		enum
		{
			STATE_STOP = 0,
			STATE_MOVE
		};

		friend class MoveManager;

	public:
		void SetMaskRadius(float radius);

		void ClearObstacle();
		bool IsStop() const;

		void LeaveMap();
		void SetPosition(float x, float y);
		void SetSpeed(float speed);
		void SetTarget(float x, float y); //往这个目标点直线走
		void SetTarget(MoveUnit *target, int range);
		void Stop();
		bool IsOnObstacle();

		void Update(float dtime);

		void GetPosition(float &x, float &y);
		void GetDirection(float &x, float &y);

	private:
		MoveUnit();
		~MoveUnit();
		void SetObstacle(const Vector2f &pos);
		void GoPath(const Vector2f &finalgoal);
		void CheckNewPath();
		bool GetNearGoal(Vector2f &goal); //路径太长，寻路太耗，所以缩短， 返回是否缩短了

	private:
		float mSpeed;
		float mAccumMoveLen;

		int mState;
		Vector2f mTargetPos;

		NaviPath mNaviPath;
		int mCurPathPoint;
		int mMoveSegLen;

		Vector2f mCurPos;
		Vector2f mCurDir;

		float mMaskRadius;
		Vector2f mMaskStopPoint;
		bool mMaskStop;
	};

}

#endif