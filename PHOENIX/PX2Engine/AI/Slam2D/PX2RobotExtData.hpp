// PX2RobotExtData.hpp

#ifndef PX2ROBOTEXTDATA_HPP
#define PX2ROBOTEXTDATA_HPP

#include "Slam2DPlugin_Pre.hpp"
#include "PX2Vector2.hpp"

namespace PX2
{

	struct SLAM2D_DLL_ITEM RobotState
	{
		RobotState();
		~RobotState();

		// x坐标，y坐标，机器朝向，速度，角速度
		float PosX;
		float PosY;
		float Orientation;
		float LeftSpeed;
		float RightSpeed;
	};

	struct SLAM2D_DLL_ITEM EvaluationPara
	{
		EvaluationPara();
		~EvaluationPara();

		int Index;

		float DiffHeading;
		float Clearance;
		float Leftspped;
		float RightSpeed;
		float Speed;

		float LastValue;
	};

}

#endif