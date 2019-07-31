// PX2AIWayMoveController.cpp

#include "PX2AIWayMoveController.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AIWayMoveController::AIWayMoveController()
{

}
//----------------------------------------------------------------------------
AIWayMoveController::~AIWayMoveController()
{

}
//----------------------------------------------------------------------------
InterpCurveFloat3Controller *AIWayMoveController::GetWayPointCtrl()
{
	return mWayPointCtrls;
}
//----------------------------------------------------------------------------