// PX2RobotExt.hpp

#ifndef PX2ROBOTEXT_HPP
#define PX2ROBTOEXT_HPP

#include "Slam2DPlugin_Pre.hpp"
#include "PX2Robot.hpp"
#include "PX2RobotExtData.hpp"

namespace PX2
{
	
	class SLAM2D_DLL_ITEM RobotExt : public Robot
	{
	public:
		RobotExt();
		virtual ~RobotExt();

		static RobotExt *New();
		static void Delete(RobotExt* robot);

		virtual void Update(float appseconds, float elpasedSeconds);

	private:
		void _UpdateVirtualRobot1(float elaplseSeconds);
		std::vector<Vector2f > GetNearObst(float radius = 1.5f);
		virtual bool _IsInRightDirection(const AVector &dir);
		virtual void _UpdateAdjustDirection(const AVector &dir);

		std::vector<float> DynamicWindowApproach(RobotState rState, const Vector2f &target,
			std::vector<Vector2f> &obstacle, std::vector<std::vector<RobotState> > &outRobotStates, 
			bool &isHasSpeedPlan);
		RobotState Motion(RobotState curState, float leftSpeed, float rightSpeed, float elapsedSeconds);
		std::vector<RobotState> GenerateTraj(RobotState initState, float leftSpeed, float rightSpeed);
		std::vector<float> CreateDW(RobotState curState);
		float CalcClearance(RobotState rState, std::vector<Vector2f> &obsts);
		float CalcBreakingDist(float velo);
		float CalcHeading(RobotState rState, const Vector2f &goal);

		RobotState _GetCurRobotState();
		void _RunSpeed(float elaplseSeconds);
		void _Stop();

		void _UpdateLidarDataToMapData();
		void _UpdateTex();

		RobotState mRobotState;

		float mRunSpeedTimer;

		float mLeftSpeed;
		float mRightSpeed;

	private:

	};

}

#endif