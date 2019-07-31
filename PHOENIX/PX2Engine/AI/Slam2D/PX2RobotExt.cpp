// PX2RobotExt.cpp

#include "PX2RobotExt.hpp"
#include "PX2EngineSceneCanvas.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
RobotExt *RobotExt::New()
{
	RobotExt *obj = new0 RobotExt();
	return obj;
}
//----------------------------------------------------------------------------
void RobotExt::Delete(RobotExt* robot)
{
	delete0(robot);
	Robot::Set(0);
}
//----------------------------------------------------------------------------
const float DT = 0.1f;
//----------------------------------------------------------------------------
RobotExt::RobotExt()
{
	mRunSpeedTimer = 0.0f;
	mLeftSpeed = 0.0f;
	mRightSpeed = 0.0f;

	mMaxSpeed = 0.2f;
	mRadius = 0.2f;
	mPredictTime = 2.0f;

	mMaxAccelerate = 0.2f;
	mRadius = 0.16f;
}
//----------------------------------------------------------------------------
RobotExt::~RobotExt()
{
}
//----------------------------------------------------------------------------
void RobotExt::Update(float appseconds, float elpasedSeconds)
{
	Robot::Update(appseconds, elpasedSeconds);
	_UpdateTex();

	_UpdateLidarDataToMapData();

	mRobotState = _GetCurRobotState();
	
	if (!mIsAdjustToDirection)
		_UpdateVirtualRobot1(elpasedSeconds);
}
//----------------------------------------------------------------------------
void RobotExt::_UpdateVirtualRobot1(float elaplseSeconds)
{
	if (!mAgent)
		return;

	AISteeringPath &steerPath = mAgent->GetSteeringBehavior()->GetPath();

	std::vector<std::vector<RobotState> > outRobotStates;

	int selectIndex = -1;
	if (!steerPath.Finished() && mCurPathPlan)
	{
		// run
		APoint curWayPoint = steerPath.CurrentWaypoint();
		Vector2f curWayPoint2 = curWayPoint.To2();

		std::vector<Vector2f> obsts = GetNearObst(0.8f);

		RobotState ste = _GetCurRobotState();
		bool isHasSpeedPlan = false;
		std::vector<float> selectPlan = DynamicWindowApproach(ste,
			curWayPoint2, obsts, outRobotStates, isHasSpeedPlan);

		if (isHasSpeedPlan)
		{
			float leftSpeed = selectPlan[0];
			float rightSpeed = selectPlan[1];
			selectIndex = selectPlan[2];

			mLeftSpeed = leftSpeed;
			mRightSpeed = rightSpeed;

			_RunSpeed(elaplseSeconds);
		}
		else
		{

		}
	}
	else
	{
		mLeftSpeed = 0;
		mRightSpeed = 0;

		// stop
		if (mCurPathPlan)
		{
			_Stop();

			mCurPathPlan = 0;
		}
	}

	// draw paths
	for (int i = 0; i < (int)outRobotStates.size(); i++)
	{
		std::vector<RobotState> states = outRobotStates[i];
		if (states.size() > 1)
		{
			Float4 addColor = Float4::GREEN;
			if (selectIndex == i)
				addColor = Float4::RED;

			for (int j = 0; j < (int)states.size() - 1; j++)
			{
				RobotState roS0 = states[j];
				RobotState roS1 = states[j + 1];

				APoint pos0(roS0.PosX, roS0.PosY, 0.1f);
				APoint pos1(roS1.PosX, roS1.PosY, 0.1f);

				EngineSceneCanvas::GetSingleton().AddDebugLine(pos0, pos1,
					addColor);
			}
		}
	}
}
//----------------------------------------------------------------------------
void RobotExt::_RunSpeed(float elaplseSeconds)
{
	if (mArduino && mArduino->IsInitlized())
	{
		mRunSpeedTimer += elaplseSeconds;
		if (mRunSpeedTimer > 0.1f)
		{
			LeftRunSpeed(mLeftSpeed);
			RightRunSpeed(mRightSpeed);

			mRunSpeedTimer = 0.0f;
		}

		float dotVal = mDirection.Dot(AVector::UNIT_Y);
		float rad = Mathf::ACos(dotVal);

		AVector crossVal = mDirection.Cross(AVector::UNIT_Y);
		if (crossVal.Z() <= 0.0f)
		{
			/*_*/
		}
		else
		{
			rad = -rad;
		}
		mRotationRad = rad;
	}
	else
	{
		RobotState rs = Motion(mRobotState, mLeftSpeed, mRightSpeed,
			elaplseSeconds);
		mPosition = APoint(rs.PosX, rs.PosY, 0.0f);
		mRotationRad = rs.Orientation;
		mDirection = AVector(-Mathf::Sin(mRotationRad), Mathf::Cos(mRotationRad), 0.0f);
		mDirection.Normalize();
	}

	//PX2_LOG_INFO("LeftVal:%.2f -- RightVal:%.2f", mLeftSpeed, mRightSpeed);
}
//----------------------------------------------------------------------------
void RobotExt::_Stop()
{
	mArduino->Stop();
}
//----------------------------------------------------------------------------
void RobotExt::_UpdateLidarDataToMapData()
{
	if (!mLiDar)
		return;

	if (mMap2DLidarData.empty())
		return;

	int mapSize = mMapDataPtr->MapStruct.MapSize;
	int allSize = mapSize*mapSize;
	memset(&mMap2DLidarData[0], RobotMapData::MOT_NONE,
		allSize * sizeof(char));

	std::vector<RslidarDataComplete> lidarDatas = mLiDar->GetLiDarData();
	for (int i = 0; i < (int)lidarDatas.size(); i++)
	{
		RslidarDataComplete lidarData = lidarDatas[i];

		int signal = lidarData.signal;
		float angle = lidarData.angle;
		float distance = lidarData.distance;

		float rad = angle * Mathf::DEG_TO_RAD;
		float offsetX = Mathf::Cos(rad) * distance;
		float offsetY = Mathf::Sin(rad) * distance;

		APoint pos(offsetX, offsetY, 0.0f);
		APoint posLast = mMatrix * pos;

		SetMapValueAtPos(mMap2DLidarData, posLast, RobotMapData::MOT_OBJST);
	}

	RobotMapData rmd;
	mMap2DLidarData = rmd._ExtendLarger(mMap2DLidarData, mapSize, 
		RobotMapData::MOT_OBJST);
}
//----------------------------------------------------------------------------
void RobotExt::_UpdateTex()
{
	int mapSize = mMapDataPtr->MapStruct.MapSize;
	float resolution = mMapDataPtr->MapStruct.MapResolution;

	//if (mVoxelSection)
	//	mVoxelSection->SetAllBlocks(0);

	if (mIsSlamMapDataChanged && mIsSlamMapInited)
	{
		RobotMapData rmd;
		std::vector<unsigned char> map = rmd._ExtendLarger(mMapDataPtr->Map2DOrigin,
			mapSize, RobotMapData::MOT_OBJST);

		std::vector<unsigned char> map1 = rmd._ExtendLarger(map,
			mapSize, RobotMapData::MOT_OBJST);

		std::vector<unsigned char> map2 = rmd._ExtendLarger(map1,
			mapSize, RobotMapData::MOT_OBJST);

		std::vector<unsigned char> map2DUsing = rmd._ExtendLarger(map2,
			mapSize, RobotMapData::MOT_OBJST);

		int mapAllSize = mapSize * mapSize;
		int robotindex = 0;
		int x = 0;
		int y = 0;
		int halfSize = (int)(mapSize * 0.5f);
		char *dst = mTextureMap->GetData(0);
		int width = mTextureMap->GetWidth();
		for (int i = 0; i < mapAllSize; i++)
		{
			x = (i + width) % width;
			y = ((width - 1) - i / width);
			unsigned char *pixel = (unsigned char*)dst + y * width * 4 + x * 4;

			*(pixel + 0) = 0;	// b
			*(pixel + 1) = 0;	// g
			*(pixel + 2) = 0;	// r
			*(pixel + 3) = 255;

			bool isHasObst = false;
			int mapVal = RobotMapData::MOT_CANGO;
			if (!map2DUsing.empty())
			{
				mapVal = map2DUsing[i];

				if (RobotMapData::MOT_ROBOTPOS == mapVal)
				{
					// robot pos
					robotindex = i;

					*(pixel + 0) = 255;	// b
					*(pixel + 1) = 255;	// g
					*(pixel + 2) = 255;	// r
					*(pixel + 3) = 255;

					mRobotMapX = x;
					mRobotMapY = y;
					mMapDataPtr->MapStruct.IndexX = x;
					mMapDataPtr->MapStruct.IndexY = y;
				}
				else if (RobotMapData::MOT_NONE == mapVal)
				{
					// none
					*(pixel + 0) = 0;	// b
					*(pixel + 1) = 0;	// g
					*(pixel + 2) = 0;	// r
					*(pixel + 3) = 255;

					// set empty
				}
				else if (RobotMapData::MOT_CANGO == mapVal)
				{
					// space can go
					*(pixel + 0) = 0;
					*(pixel + 1) = 255;
					*(pixel + 2) = 0;
					*(pixel + 3) = 255;

					// set space can go
				}
				else if (RobotMapData::MOT_OBJST == mapVal)
				{
					// obst
					*(pixel + 0) = 0;
					*(pixel + 1) = 0;
					*(pixel + 2) = 255;
					*(pixel + 3) = 255;

					isHasObst = true;
				}
				else
				{
					// will never go here
					*(pixel + 0) = (unsigned char)mapVal;
					*(pixel + 1) = (unsigned char)mapVal;
					*(pixel + 2) = (unsigned char)mapVal;
					*(pixel + 3) = 255;
				}
			}

			int adjustVal = mMapDataPtr->SelfDrawMapData2D[i];
			if (RobotMapData::MOT_OBJST == adjustVal)
			{
				*(pixel + 0) = 255;
				*(pixel + 1) = 255;
				*(pixel + 2) = 255;
				*(pixel + 3) = 255;

				isHasObst = true;
			}
			else if (RobotMapData::MOT_CANGO == adjustVal)
			{
				*(pixel + 0) = 0;
				*(pixel + 1) = 255;
				*(pixel + 2) = 0;
				*(pixel + 3) = 255;

				isHasObst = false;
			}

			if (mMap2DLidarData[i] == RobotMapData::MOT_OBJST)
			{
				*(pixel + 0) = 255;
				*(pixel + 1) = 255;
				*(pixel + 2) = 0;
				*(pixel + 3) = 255;
			}

			if (isHasObst)
			{
#if defined PX2_USE_VOXEL
				float xPos = (x - halfSize) * resolution;
				float yPos = (halfSize - y) * resolution;
				APoint targetPos(xPos, yPos, 0.0f);
				APoint targetPos1(xPos, yPos, 0.05f);
				APoint targetPos2(xPos, yPos, 0.1f);

				// obst
				if (mVoxelSection)
				{
					//mVoxelSection->SetBlock(targetPos, 1);
					//mVoxelSection->SetBlock(targetPos1, 1);
					//mVoxelSection->SetBlock(targetPos2, 1);
				}
#endif
			}

			int drawVaL = mMapDataPtr->PathFinderLineDraw[i];
			if (0 != drawVaL)
			{
				*(pixel + 0) = 0;
				*(pixel + 1) = 0;
				*(pixel + 2) = (unsigned char)drawVaL;
				*(pixel + 3) = (unsigned char)255;
			}
		}

		if (mTextureMap && mapSize > 0)
			Renderer::UpdateAll(mTextureMap, 0);

		mIsSlamMapDataChanged = false;
	}
}
//----------------------------------------------------------------------------
RobotState RobotExt::Motion(RobotState curState, float leftSpeed,
	float rightSpeed, float elapsedSeconds)
{
	RobotState afterMoveState;

	float rotSpeed = rightSpeed - leftSpeed;
	float rotDist = rotSpeed * elapsedSeconds;
	float rotRad = (rotDist / mRadius);

	afterMoveState.Orientation = curState.Orientation + rotRad;
	afterMoveState.LeftSpeed = leftSpeed;
	afterMoveState.RightSpeed = rightSpeed;

	float speed = (leftSpeed + rightSpeed) / 2.0f;

	Vector2f direction(-Mathf::Sin(afterMoveState.Orientation), Mathf::Cos(afterMoveState.Orientation));
	direction.Normalize();
	Vector2f posCur = Vector2f(curState.PosX, curState.PosY) +
		direction * speed * elapsedSeconds;

	afterMoveState.PosX = posCur.X();
	afterMoveState.PosY = posCur.Y();

	return afterMoveState;
}
//----------------------------------------------------------------------------
std::vector<RobotState> RobotExt::GenerateTraj(RobotState initState,
	float leftSpeed, float rightSpeed)
{
	RobotState tempState = initState;
	vector<RobotState> trajectories;
	float time = 0;
	trajectories.push_back(initState);
	while (time < mPredictTime)
	{
		tempState = Motion(tempState, leftSpeed, rightSpeed, DT);
		trajectories.push_back(tempState);
		time += DT;
	}

	return trajectories;
}
//----------------------------------------------------------------------------
std::vector<float> RobotExt::CreateDW(RobotState curState)
{
	std::vector<float> dw(4);
	float tmpMinLeftSpeed = curState.LeftSpeed - mMaxAccelerate*DT;
	float tmpMaxLeftSpeed = curState.LeftSpeed + mMaxAccelerate*DT;

	float tmpMinRightSpeed = curState.RightSpeed - mMaxAccelerate*DT;
	float tmpMaxRightSpeed = curState.RightSpeed + mMaxAccelerate*DT;

	dw[0] = Mathf::Clamp(tmpMinLeftSpeed, mMinSpeed, mMaxSpeed);
	dw[1] = Mathf::Clamp(tmpMaxLeftSpeed, mMinSpeed, mMaxSpeed);

	dw[2] = Mathf::Clamp(tmpMinRightSpeed, mMinSpeed, mMaxSpeed);
	dw[3] = Mathf::Clamp(tmpMaxRightSpeed, mMinSpeed, mMaxSpeed);

	return dw;
}
//----------------------------------------------------------------------------
RobotState RobotExt::_GetCurRobotState()
{
	RobotState state;
	state.PosX = mPosition.X();
	state.PosY = mPosition.Y();
	state.LeftSpeed = mLeftSpeed;
	state.RightSpeed = mRightSpeed;
	state.Orientation = mRotationRad;

	return state;
}
//----------------------------------------------------------------------------
float RobotExt::CalcHeading(RobotState rState, const Vector2f &goal)
{
	AVector diff = APoint(goal.X(), goal.Y(), 0.0f)
		- APoint(rState.PosX, rState.PosY, 0.0f);
	float diffLength = diff.Normalize();
	if (diffLength < 0.01f)
		return 180.0f;

	AVector dir = AVector(-Mathf::Sin(rState.Orientation),
		Mathf::Cos(rState.Orientation), 0.0f);
	dir.Normalize();

	float dotVal = diff.Dot(dir);

	float rad = Mathf::ACos(dotVal);
	float degree = rad * Mathf::RAD_TO_DEG;

	return 180.0f - degree;
}
//----------------------------------------------------------------------------
float RobotExt::CalcClearance(RobotState rState, std::vector<Vector2f> &obs)
{
	float dist = 100.0f;
	float distTemp;
	int numObs = (int)obs.size();
	for (int i = 0; i < numObs; i++)
	{
		Vector2f diff = Vector2f(rState.PosX, rState.PosY) - obs[i];
		float length = diff.Length();

		distTemp = length - mRadius;

		if (distTemp < dist)
		{
			dist = distTemp;
		}
	}

	if (dist >= mRadius)
	{
		dist = mRadius;
	}

	return dist;
}
//----------------------------------------------------------------------------
float RobotExt::CalcBreakingDist(float velo)
{
	float stopDist = 0;
	while (velo > 0)
	{
		stopDist = stopDist + velo*DT;
		velo = velo - mMaxAccelerate*DT;
	}

	return stopDist;
}
//----------------------------------------------------------------------------
std::vector<Vector2f > RobotExt::GetNearObst(float radius)
{
	if (mMap2DLidarData.empty())
		return std::vector<Vector2f >();

	std::vector<Vector2f> obsts;

	if (mMapDataPtr)
	{
		int mapSize = mMapDataPtr->MapStruct.MapSize;
		float halfMapSize = mapSize * 0.5f;
		float resolution = mMapDataPtr->MapStruct.MapResolution;

		float fxPosStart = mPosition.X() - radius;
		int xPosStart = (int)(fxPosStart / resolution);
		xPosStart += halfMapSize;
		xPosStart = Math<int>::Clamp(xPosStart, 0, mapSize);

		float fxPosEnd = mPosition.X() + radius;
		int xPosEnd = (int)(fxPosEnd / resolution);
		xPosEnd += halfMapSize;
		xPosEnd = Math<int>::Clamp(xPosEnd, 0, mapSize);

		float fyPosStart = mPosition.Y() - radius;
		int yPosStart = (int)(fyPosStart / resolution);
		yPosStart = halfMapSize + yPosStart;
		yPosStart = Math<int>::Clamp(yPosStart, 0, mapSize);

		float fyPosEnd = mPosition.Y() + radius;
		int yPosEnd = (int)(fyPosEnd / resolution);
		yPosEnd = halfMapSize + yPosEnd;
		yPosEnd = Math<int>::Clamp(yPosEnd, 0, mapSize);

		float centerX = mPosition.X() / resolution;
		float centerY = mPosition.Y() / resolution;

		float radiusSquare = 0.14f*0.14f;

		for (int y = yPosStart; y < yPosEnd; y++)
		{
			for (int x = xPosStart; x < xPosEnd; x++)
			{
				float xPos = (x - halfMapSize) * resolution;
				float yPos = (y - halfMapSize) * resolution;

				AVector diff = APoint(xPos, yPos, 0.0f) - mPosition;
				float lengthSquare = diff.SquaredLength();
				if (lengthSquare < radiusSquare)
					continue;

				int i = y * mapSize + x;
				int mapVal = mMap2DLidarData[i];
				int mapVal2 = mMapDataPtr->SelfDrawMapData2D[i];

				bool isHasObst = false;
				if (RobotMapData::MOT_OBJST == mapVal)
				{
					isHasObst = true;
				}
				if (RobotMapData::MOT_OBJST == mapVal2)
				{
					isHasObst = true;
				}

				if (isHasObst)
				{
					obsts.push_back(Vector2f(xPos, yPos));
				}
			}
		}
	}

	return obsts;
}
//----------------------------------------------------------------------------
bool RobotExt::_IsInRightDirection(const AVector &dir)
{
	float valDot = dir.Dot(mDirection);
	float degree = Mathf::ACos(valDot) * Mathf::RAD_TO_DEG;
	if (degree < 8.0f)
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void RobotExt::_UpdateAdjustDirection(const AVector &dir)
{
	float valDot = dir.Dot(mDirection);
	float degree = Mathf::ACos(valDot) * Mathf::RAD_TO_DEG;
	if (degree < 8.0f)
	{
	}
	else
	{
		if (mDirection != AVector::ZERO)
		{
			float maxSpeed = mMaxSpeed * 0.3f;

			AVector right = mDirection.Cross(dir);
			if (right.Z() > 0.0f)
			{
				LeftRunSpeed(-maxSpeed);
				RightRunSpeed(maxSpeed);
			}
			else
			{
				LeftRunSpeed(maxSpeed);
				RightRunSpeed(-maxSpeed);
			}
		}
	}
}
//----------------------------------------------------------------------------
std::vector<float> RobotExt::DynamicWindowApproach(RobotState rState,
	const Vector2f &target, std::vector<Vector2f> &obstacle,
	std::vector<std::vector<RobotState> > &outRobotStates,
	bool &isHasSpeedPlan)
{
	// 0:leftMin, 1:leftMax, 2:rightMin, 3:rightMax
	std::vector<float> velocityAndOmegaRange = CreateDW(rState);

	std::vector<EvaluationPara> evalParas;
	float sumHeading = 0;
	float sumClearance = 0;
	float sumSpeed = 0;

	float SAMPLING_SPEED = 0.008f;

	for (float l = velocityAndOmegaRange[0]; l < velocityAndOmegaRange[1]; l += SAMPLING_SPEED)
	{
		for (float r = velocityAndOmegaRange[2]; r < velocityAndOmegaRange[3]; r += SAMPLING_SPEED)
		{
			std::vector<RobotState> trajectories = GenerateTraj(rState, l, r);

			RobotState stateBack = trajectories.back();

			float speed = (l + r) / 2.0f;

			float tempClearance = CalcClearance(stateBack, obstacle);
			float stopDist = CalcBreakingDist(speed);
			stopDist = 0.1f;

			if (tempClearance > stopDist)
			{
				outRobotStates.push_back(trajectories);

				EvaluationPara tempEvalPara;
				tempEvalPara.Index = (int)outRobotStates.size() - 1;

				tempEvalPara.DiffHeading = CalcHeading(stateBack, target);

				tempEvalPara.Clearance = tempClearance;
				tempEvalPara.Leftspped = l;
				tempEvalPara.RightSpeed = r;
				tempEvalPara.Speed = speed;

				sumSpeed += speed;
				sumHeading += tempEvalPara.DiffHeading;
				sumClearance += tempClearance;

				evalParas.push_back(tempEvalPara);
			}
		}
	}

	float selectedLeftSpeed = 0;
	float selectedRightSpeed = 0;
	int selectIndex = -1;

	std::vector<float> selVelocity;
	selVelocity.resize(3);

	if (evalParas.empty())
	{
		/*_*/
		isHasSpeedPlan = false;
	}
	else
	{
		float G = 0;
		EvaluationPara selectEva;
		for (vector<EvaluationPara>::iterator i = evalParas.begin(); i < evalParas.end(); i++)
		{
			float smoothHeading = i->DiffHeading / sumHeading;
			float smoothClearance = i->Clearance / sumClearance;
			float smoothVelocity = i->Speed / sumSpeed;

			const float WEIGHT_HEADING = 0.05f;
			const float WEIGHT_CLEARANCE = 0.2f;
			const float WEIGHT_VELOCITY = 0.065f;
			//const float WEIGHT_VELOCITY = 0.055f;

			float tempG = WEIGHT_HEADING*smoothHeading + WEIGHT_CLEARANCE*smoothClearance + WEIGHT_VELOCITY*smoothVelocity;
			(*i).LastValue = tempG;

			if (tempG > G)
			{
				G = tempG;
				selectedLeftSpeed = i->Leftspped;
				selectedRightSpeed = i->RightSpeed;
				selectIndex = (*i).Index;

				selectEva = *i;
			}
		}

		isHasSpeedPlan = true;
	}

	selVelocity[0] = selectedLeftSpeed;
	selVelocity[1] = selectedRightSpeed;
	selVelocity[2] = (float)selectIndex;

	return selVelocity;
}
//----------------------------------------------------------------------------