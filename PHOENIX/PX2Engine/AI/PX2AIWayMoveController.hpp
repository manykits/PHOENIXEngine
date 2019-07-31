// PX2AIWayMoveController.hpp

#ifndef PX2AIWAYMOVECONTOLLER_HPP
#define PX2AIWAYMOVECONTOLLER_HPP

#include "PX2Controller.hpp"
#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM AIWayMoveController : public Controller
	{
	public:
		AIWayMoveController();
		virtual ~AIWayMoveController();

		InterpCurveFloat3Controller *GetWayPointCtrl();
		float GetWayPointLong() const;

	private:
		InterpCurveFloat3ControllerPtr mWayPointCtrls;
		float mWayPointsLong;
		float mCurWayPointDistance;
	};

}

#endif