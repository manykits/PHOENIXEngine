// PX2Robot.cpp

#include "PX2Robot.hpp"
#include "PX2Creater.hpp"
#include "PX2Project.hpp"
#include "PX2JSONData.hpp"
#include "PX2Timestamp.hpp"
#include "PX2Timespan.hpp"
#include "PX2VoxelPre.hpp"
#if defined PX2_USE_VOXEL
#include "PX2VoxelSection.hpp"
#endif
#include "PX2AIES.hpp"
#include "PX2Renderer.hpp"
#include "PX2Arduino.hpp"
#include "PX2ResourceManager.hpp"
#if defined PX2_USE_SLAM2D
#include "Slam2DPlugin.hpp"
#endif
#include "PX2PluginManager.hpp"
#include "PX2AIAgent.hpp"
#include "PX2EngineSceneCanvas.hpp"
#include "PX2AISteeringPath.hpp"
#include "PX2Creater.hpp"
#include "PX2Actor.hpp"
using namespace PX2;

#if defined _WIN32 || defined WIN32
#include <windows.h> 
#endif
//----------------------------------------------------------------------------
#if defined PX2_USE_SLAM2D
Slam2DPlugin *Robot::mSlam2DPlugin = 0;
#endif
//----------------------------------------------------------------------------
Robot::Robot() :
mLiDar(0),
mOffsetDegree(0.0f),
mRobotMapX(0),
mRobotMapY(0),
m2DSlamAngle(0.0f),
mIsHasAxis(false),
mAgent(0),
mRotationRad(0.0f),
mIsSlamMapDoUpdate(true)
{
	mMaxSpeed = 0.1f;
	mMinSpeed = 0.0f;
	mMaxAccelerate = 0.4f;
	mPredictTime = 6.0f;
	mRadius = 0.16f;
	mLidarOffset = 0.0f;
	mMotoRate = 35;
	mWheelRadius = 0.05f;

	mRoleType = RT_MASTER;

	mPosition = APoint::ORIGIN;

	mDirection = AVector::UNIT_Y;
	mRotationRad = 0.0f;
	mRight = AVector::UNIT_X;
	mUp = AVector::UNIT_Z;

	// leidar
	mPicBoxMap = new0 UIFPicBox();
	mPicBoxMap->GetUIPicBox()->GetMaterialInstance()
		->GetMaterial()->GetPixelShader(0, 0)->SetFilter(0,
		Shader::SF_NEAREST);

	mMapDataPtr = new0 RobotMapData();
	_RecreateMapTex(mMapDataPtr->MapStruct.MapSize);

	mRobotTex = new0 UIFPicBox();
	mPicBoxMap->AttachChild(mRobotTex);
	mRobotTex->LocalTransform.SetTranslateY(-1.0f);
	mRobotTex->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mRobotTex->SetSize(2.0f, 6.0f);
	mRobotTex->SetPivot(0.5f, 0.0f);
	mRobotTex->SetAlphaSelfCtrled(true);

	UIFPicBox *tex = new0 UIFPicBox();
	mRobotTex->AttachChild(tex);
	tex->LocalTransform.SetTranslateY(-1.0f);
	tex->GetUIPicBox()->SetTexture("Data/engine/white.png");
	tex->SetSize(2.0f, 2.0f);
	tex->SetColor(Float3::RED);
	tex->SetAnchorHor(0.5f, 0.5f);
	tex->SetAnchorVer(0.0f, 0.0f);
	tex->SetPivot(0.5f, 0.5f);
	tex->SetAlphaSelfCtrled(true);

	mPathGraph = new0 PathingGraph();

	mIsSlamMapInited = false;
	mIsSlamMapDataChanged = true;

	mArduino = new0 Arduino();

	mIsAdjustToDirection = false;

	mVoxelSection = 0;
}
//----------------------------------------------------------------------------
Robot::~Robot()
{
	if (mLiDar)
	{
		delete0(mLiDar);
	}
#if defined PX2_USE_SLAM2D
	if (mSlam2DPlugin)
		PX2_PLUGINMAN.UninstallPlugin(mSlam2DPlugin);
#endif
}
//----------------------------------------------------------------------------
Robot *Robot::New()
{
	return new0 Robot();
}
//----------------------------------------------------------------------------
void Robot::Delete(Robot* robot)
{
	delete0(robot);
	Robot::Set(0);
}
//----------------------------------------------------------------------------
void Robot::InitlizeSlam2D()
{
#if defined PX2_USE_SLAM2D
	mSlam2DPlugin = new Slam2DPlugin();
	PX2_PLUGINMAN.InstallPlugin(mSlam2DPlugin);
#endif
}
//----------------------------------------------------------------------------
void Robot::TernimateSlam2D()
{
#if defined PX2_USE_SLAM2D
	if (mSlam2DPlugin)
		PX2_PLUGINMAN.UninstallPlugin(mSlam2DPlugin);
#endif
}
//----------------------------------------------------------------------------
bool Robot::SaveMap(const std::string &filename)
{
	_UpdateMapDataToPathFinder();

	std::string writeablePath = PX2_RM.GetWriteablePath();
	std::string dstPath = writeablePath + "Write_ZERONE/";
	if (!PX2_RM.IsFileFloderExist(dstPath))
	{
		PX2_RM.CreateFloder(writeablePath, "Write_ZERONE/");
	}

	if (mMapDataPtr)
	{
		mMapDataPtr->SetResourcePath(filename);

		OutStream outStream;
		outStream.Insert(mMapDataPtr);

		if (outStream.Save(dstPath+filename))
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool Robot::LoadMap(const std::string &filename, const APoint &initPos,
	float curAngle)
{
	mMapDataInit = RobotMapData();

	std::string writeablePath = PX2_RM.GetWriteablePath();
	std::string dstPath = writeablePath + "Write_ZERONE/" + filename;

	InStream inStream;
	if (inStream.Load(dstPath))
	{
		RobotMapData *data = DynamicCast<RobotMapData>(inStream.GetObjectAt(0));
		mMapDataPtr = data;
		if (data)
		{
			mMapDataInit = *data;

			if (initPos != APoint::ORIGIN)
			{
				mMapDataInit.MapStruct.CurPos = initPos;
			}
			if (0.0f != curAngle)
			{
				mMapDataInit.MapStruct.CurAngle = curAngle;
			}
		}
	}

	if (mMapDataPtr)
	{
		InitSlamMap(mMapDataPtr->MapStruct.MapSize,
			mMapDataPtr->MapStruct.MapResolution, false);

		_UpdateMapDataToPathFinder();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
int _Roundle(float number)
{
	return (int)((number > 0.0f) ? floor(number + 0.5f) : ceil(number - 0.5f));
} 
//----------------------------------------------------------------------------
void Robot::_UpdateMapDataToPathFinder()
{
	int mapSize = mMapDataPtr->MapStruct.MapSize;
	int mapAllSize = mapSize * mapSize;
	float resolution = mMapDataPtr->MapStruct.MapResolution;

	RobotMapData rmd;
	std::vector<unsigned char> map = rmd._ExtendLarger(mMapDataPtr->Map2DOrigin,
		mMapDataPtr->MapStruct.MapSize, RobotMapData::MOT_OBJST);
	std::vector<unsigned char> map1 = rmd._ExtendLarger(map,
		mMapDataPtr->MapStruct.MapSize, RobotMapData::MOT_OBJST);

	// mask
	std::vector<unsigned char> maskData;
	maskData.resize(mapAllSize);
	memset(&maskData[0], 0, maskData.size());

	int x = 0;
	int y = 0;
	for (int i = 0; i < mapAllSize; i++)
	{
		x = (i + mapSize) % mapSize;
		y = ((mapSize - 1) - i / mapSize);

		bool isHasObst = false;

		int mapObstValue = map1[i];
		if (RobotMapData::MOT_OBJST == mapObstValue)
		{
			isHasObst = true;
		}

		int selfDrawValue = mMapDataPtr->SelfDrawMapData2D[i];
		if (RobotMapData::MOT_OBJST == selfDrawValue)
		{
			isHasObst = true;
		}
		else if (RobotMapData::MOT_NONE == selfDrawValue)
		{
			// do nothing
		}
		else if (RobotMapData::MOT_CANGO == selfDrawValue)
		{
			isHasObst = false;
		}

		int graphMapSize = mPathGraph->GetMapSize();
		float xPercent = (float)x / (float)mapSize;
		float yPercent = (float)y / (float)mapSize;
		float fXIndex = xPercent * (float)graphMapSize;
		float fYIndex = yPercent * (float)graphMapSize;

		int xIndex = _Roundle(fXIndex);
		int yIndex = _Roundle(fYIndex);

		if (isHasObst)
		{
			_SetGraphValue(xIndex, yIndex, 1000.0f);
			_SetGraphValue(xIndex - 1, yIndex, 1000.0f);
			_SetGraphValue(xIndex + 1, yIndex, 1000.0f);
			_SetGraphValue(xIndex, yIndex - 1, 1000.0f);
			_SetGraphValue(xIndex, yIndex + 1, 1000.0f);

			_SetGraphValue(xIndex - 1, yIndex - 1, 1000.0f);
			_SetGraphValue(xIndex + 1, yIndex + 1, 1000.0f);
			_SetGraphValue(xIndex - 1, yIndex + 1, 1000.0f);
			_SetGraphValue(xIndex + 1, yIndex - 1, 1000.0f);
			
			maskData[i] = 1;
		}
		else
		{
			_SetGraphValue(xIndex, yIndex, PATHING_DEFAULT_ARC_WEIGHT);

			maskData[i] = 0;
		}
	}

	mPathGraph->SetMapData(maskData, mapSize, resolution);
}
//----------------------------------------------------------------------------
void Robot::_SetGraphValue(int x, int y, float val)
{
	int graphMapSize = mPathGraph->GetMapSize();
	if (x >= 0 && x < graphMapSize && y >= 0 && y < graphMapSize)
	{
		int iIndex = x + y * graphMapSize;
		mPathGraph->SetArcValue(iIndex, val);
	}
}
//----------------------------------------------------------------------------
const APoint &Robot::GetPosition() const
{
	return mPosition;
}
//----------------------------------------------------------------------------
RobotMapData &Robot::GetInitMapData()
{
	return mMapDataInit;
}
//----------------------------------------------------------------------------
void Robot::ClearInitMapData()
{
	mMapDataInit = RobotMapData();
}
//----------------------------------------------------------------------------
RobotMapData *Robot::GetCurMapData()
{
	return mMapDataPtr;
}
//----------------------------------------------------------------------------
void Robot::SetObstMapValueAtPos(const APoint &pos, float range, int val)
{
	if (!mIsSlamMapInited)
		return;

	int mapSize = mMapDataPtr->MapStruct.MapSize;
	float mapReso = mMapDataPtr->MapStruct.MapResolution;

	float mapAllSize = mapSize * mapReso;

	float texXStart = 0.5f + ((pos.X() - range) / mapAllSize);
	float texXEnd = 0.5f + ((pos.X() + range) / mapAllSize);
	if (texXStart <= 0.0f || texXStart>1.0f)
		return;
	if (texXEnd <= 0.0f || texXEnd > 1.0f)
		return;

	float texYStart = 0.5f + ((pos.Y() - range) / mapAllSize);
	float texYEnd = 0.5f + ((pos.Y() + range) / mapAllSize);
	if (texYStart <= 0.0f || texYStart > 1.0f)
		return;
	if (texYEnd <= 0.0f || texYEnd > 1.0f)
		return;

	int iTexXStart = (int)(texXStart*mapSize);
	int iTexXEnd = (int)(texXEnd*mapSize);
	int iTexYStart = (int)(texYStart*mapSize);
	int iTexYEnd = (int)(texYEnd*mapSize);

	for (int x = iTexXStart; x < iTexXEnd; x++)
	{
		for (int y = iTexYStart; y < iTexYEnd; y++)
		{
			int index = y*mapSize + x;
			mMapDataPtr->SelfDrawMapData2D[index] = (unsigned char)val;
		}
	}

	mIsSlamMapDataChanged = true;
}
//----------------------------------------------------------------------------
void Robot::SetPathFinderLineValueAtPos(const APoint &pos, float range, int val)
{
	if (!mIsSlamMapInited)
		return;

	int mapSize = mMapDataPtr->MapStruct.MapSize;
	float mapReso = mMapDataPtr->MapStruct.MapResolution;

	float mapAllSize = mapSize * mapReso;

	float texXStart = 0.5f + ((pos.X() - range) / mapAllSize);
	float texXEnd = 0.5f + ((pos.X() + range) / mapAllSize);

	float texYStart = 0.5f + ((pos.Y() - range) / mapAllSize);
	float texYEnd = 0.5f + ((pos.Y() + range) / mapAllSize);

	int iTexXStart = (int)(texXStart*mapSize);
	int iTexXEnd = (int)(texXEnd*mapSize);
	int iTexYStart = (int)(texYStart*mapSize);
	int iTexYEnd = (int)(texYEnd*mapSize);

	for (int x = iTexXStart; x < iTexXEnd; x++)
	{
		for (int y = iTexYStart; y < iTexYEnd; y++)
		{
			int index = y*mapSize + x;
			mMapDataPtr->PathFinderLineDraw[index] = (unsigned char)val;
		}
	}

	mIsSlamMapDataChanged = true;
}
//----------------------------------------------------------------------------
void Robot::SetMapValueAtPos(std::vector<unsigned char> &datas,
	const APoint &pos, int val)
{
	int mapSize = mMapDataPtr->MapStruct.MapSize;
	float mapReso = mMapDataPtr->MapStruct.MapResolution;

	float mapAllSize = mapSize * mapReso;

	float u = 0.5f + pos.X() / mapAllSize;
	float v = 0.5f + pos.Y() / mapAllSize;

	int x = (int)(u*mapSize);
	int y = (int)(v*mapSize);

	int index = y*mapSize + x;
	if (0 <= index && index < (int)datas.size())
		datas[index] = (unsigned char)val;

	mIsSlamMapDataChanged = true;
}
//----------------------------------------------------------------------------
void Robot::_RecreateMapTex(int mapSize)
{
	Texture::Format formatLidar = Texture::TF_A8R8G8B8;
	mTextureMap = new0 Texture2D(formatLidar, mapSize, mapSize, 1);
	unsigned char *pDest = (unsigned char*)mTextureMap->GetData(0);
	int offsetDst = 0;
	for (int row = 0; row < mapSize; ++row)
	{
		for (int col = 0; col < mapSize; ++col)
		{
			pDest[offsetDst + 0] = 0; // b
			pDest[offsetDst + 1] = 0; // g
			pDest[offsetDst + 2] = 0; // r
			pDest[offsetDst + 3] = 255;

			offsetDst += 4;
		}
	}

	mPicBoxMap->GetUIPicBox()->SetTexture(mTextureMap);
}
//----------------------------------------------------------------------------
void Robot::Update(float appseconds, float elpasedSeconds)
{
	PX2_UNUSED(appseconds);
	PX2_UNUSED(elpasedSeconds);

	if (mArduino)
	{
		mArduino->Update(elpasedSeconds);
	}

	if (mIsHasAxis)
	{
		Arduino::AxisObj curAxisObj = mArduino->GetCurAxisObj();
		
		float rad = curAxisObj.Y * Mathf::DEG_TO_RAD;
		float x = Mathf::Cos(rad);
		float y = Mathf::Sign(rad);

		mAxisDirection = AVector(x, y, 0.0f);
		mAxisUp = AVector::UNIT_Z;
		mAxisRight = mAxisDirection.Cross(mAxisUp);
		mAxisRotMatrix = HMatrix(mAxisRight, mAxisDirection, mAxisUp, APoint::ORIGIN, true);
	}

	if (mLiDar)
	{
		mLiDar->Update(appseconds, elpasedSeconds);
	}

	if (mIsAdjustToDirection)
	{
		if (!_IsInRightDirection(mAdjustToDirection))
		{
			_UpdateAdjustDirection(mAdjustToDirection);
		}
		else
		{
			mIsAdjustToDirection = false;
		}
	}

	_UpdateTransform(elpasedSeconds);
}
//----------------------------------------------------------------------------
void Robot::AdjustToDirection(const AVector &dir)
{
	mIsAdjustToDirection = true;
	mAdjustToDirection = dir;
}
//----------------------------------------------------------------------------
bool Robot::IsSlamMapInited() const
{
	return mIsSlamMapInited;
}
//----------------------------------------------------------------------------
void Robot::InitSlamMap(int mapSize, float resolution, bool clearCurMapData)
{
	float perc = 10.0f;
	mPathGraph->InitSlamMap((int)(mapSize / perc), resolution*perc);

	int allSize = mapSize * mapSize;
	mMap2DLidarData.resize(allSize);
	memset(&mMap2DLidarData[0], RobotMapData::MOT_NONE, 
		allSize * sizeof(char));

	if (clearCurMapData)
		mMapDataPtr->Initlize(mapSize, resolution);

	bool isNeedReCreateMapTex = false;
	if (!mTextureMap)
	{
		isNeedReCreateMapTex = true;
	}
	else
	{
		if (mapSize != mTextureMap->GetWidth())
			isNeedReCreateMapTex = true;
	}

	if (isNeedReCreateMapTex)
	{
		_RecreateMapTex(mapSize);
	}

	mIsSlamMapInited = true;
	mIsSlamMapDataChanged = true;
}
//----------------------------------------------------------------------------
void Robot::SetSlam2DMap(std::vector<unsigned char> &map)
{
	if (!mIsSlamMapInited)
		return;

	if (!map.empty())
	{
		mMapDataPtr->Map2DOrigin = map;
		mIsSlamMapDataChanged = true;
	}
}
//----------------------------------------------------------------------------
void Robot::SetSlam2DPosition(const APoint &pos, float angle)
{
	if (mSlamPos == pos && m2DSlamAngle == angle)
		return;

	mSlamPos = pos;
	m2DSlamAngle = angle;
}
//----------------------------------------------------------------------------
void Robot::_UpdateTransform(float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

	mPosition = mSlamPos;
	float angle = m2DSlamAngle;

	mMapDataPtr->MapStruct.CurPos = mPosition;
	mMapDataPtr->MapStruct.CurAngle = angle;

	float xDir = Mathf::Sin(-angle);
	float yDir = Mathf::Cos(angle);
	AVector dir = AVector(xDir, yDir, 0.0f);
	dir.Normalize();
	mDirection = dir;
	mDirection.Normalize();

	mRight = mDirection.Cross(AVector::UNIT_Z);
	mUp = AVector::UNIT_Z;
	mMatrix = HMatrix(mRight, mDirection, mUp, mPosition, true);

	if (mLiDar)
	{
		mLiDar->SetCurTransform(mMatrix);
	}

	HMatrix mat;
	mat.MakeRotation(AVector::UNIT_Y, -angle);
	mRobotTex->LocalTransform.SetRotate(mat);

	float mapWidth = mMapDataPtr->MapStruct.MapSize *
		mMapDataPtr->MapStruct.MapResolution;

	float xPos = (mPosition.X() + mapWidth*0.5f) / mapWidth;
	float yPos = (mPosition.Y() + mapWidth*0.5f) / mapWidth;

	mRobotTex->SetAnchorHor(xPos, xPos);
	mRobotTex->SetAnchorVer(yPos, yPos);
}
//----------------------------------------------------------------------------
void Robot::SetRoleType(RoleType rtType)
{
	mRoleType = rtType;
}
//----------------------------------------------------------------------------
void Robot::CreateLidar()
{
	if (mLiDar)
	{
		mLiDar->Close();
		mLiDar = 0;
	}

	mLiDar = new0 LiDar();
	LiDar::LiDarType lidarType = mLiDar->GetLiDarType();
	if (LiDar::LT_III == lidarType)
	{
		mOffsetDegree = -90.0f;
	}
	else if (LiDar::LT_RP == lidarType)
	{
		mOffsetDegree = 90.0f;
	}
}
//----------------------------------------------------------------------------
bool Robot::LidarOpen(const std::string &portIP, int baudratePort)
{
	if (mLiDar)
	{
		return mLiDar->Open(portIP, baudratePort);
	}

	return false;
}
//----------------------------------------------------------------------------
void Robot::SetArduino(Arduino *arduino)
{
	mArduino = arduino;
}
//----------------------------------------------------------------------------
bool Robot::IsArduinoConnected() const
{
	return mArduino->IsInitlized();
}
//----------------------------------------------------------------------------
void Robot::LeftRunSpeed(float speed)
{
	if (mArduino)
		mArduino->RunSpeed(0, speed, mWheelRadius, mMotoRate);
}
//----------------------------------------------------------------------------
void Robot::RightRunSpeed(float speed)
{
	if (mArduino)
		mArduino->RunSpeed(1, speed, mWheelRadius, mMotoRate);
}
//----------------------------------------------------------------------------
float Robot::GetYawCrossValue() const
{
	LiDar::LiDarType lidarType = mLiDar->GetLiDarType();
	if (LiDar::LT_III == lidarType)
	{
		return -1.0f;
	}
	else if (LiDar::LT_RP == lidarType)
	{
		return 1.0f;
	}

	return 1.0f;
}
//----------------------------------------------------------------------------
void Robot::SetVoxelSection(VoxelSection *vs)
{
	mVoxelSection = vs;
}
//----------------------------------------------------------------------------
VoxelSection *Robot::GetVoxelSection()
{
	return mVoxelSection;
}
//----------------------------------------------------------------------------
AVector &Robot::GetAxisDirection()
{
	return mAxisDirection;
}
//----------------------------------------------------------------------------
HMatrix &Robot::GetAxisRotMatrix()
{
	return mAxisRotMatrix;
}
//----------------------------------------------------------------------------
void Robot::GoTarget(const APoint &targetPos, PathType type)
{
	mGoTargetPos = targetPos;

	std::vector<PathingNode*> vec;
	AISteeringPath aiAgentPath;

	int posIndex = 0;
	APoint firstGoPos;

	if (type == PT_A)
	{
		mCurPathPlan = mPathGraph->FindPath(mPosition, mGoTargetPos);
		if (mCurPathPlan)
		{
			PathingNodeList list = mCurPathPlan->m_path;

			auto it = list.begin();
			for (; it != list.end(); it++)
			{
				PathingNode *node = *it;

				APoint pos = node->GetPos();
				aiAgentPath.AddWayPoint(pos);

				posIndex++;

				if (2 == posIndex)
				{
					firstGoPos = pos;
				}

				SetPathFinderLineValueAtPos(node->GetPos(), 0.05f, 200);
			}

			mCurPathPlan->ResetPath();
		}
	}

	if (mAgent)
	{
		mAgent->SetPath(aiAgentPath);
	}

	if (posIndex >= 2)
	{
		AVector direction = firstGoPos - mPosition;
		float dist = direction.Normalize();
		if (dist > 0.0f)
		{
			AdjustToDirection(direction);
		}
	}
}
//----------------------------------------------------------------------------
void Robot::_SetAIAgent(AIAgent *agent)
{
	mAgent = agent;
}
//----------------------------------------------------------------------------
AIAgent *Robot::GetAIAgent()
{
	return mAgent;
}
//----------------------------------------------------------------------------
bool Robot::_IsInRightDirection(const AVector &dir)
{
	float valDot = dir.Dot(mDirection);
	float degree = Mathf::ACos(valDot) * Mathf::RAD_TO_DEG;
	if (degree < 10.0f)
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Robot::_UpdateAdjustDirection(const AVector &dir)
{
	float valDot = dir.Dot(mDirection);
	float degree = Mathf::ACos(valDot) * Mathf::RAD_TO_DEG;
	if (degree < 12.0f)
	{
	}
	else
	{
		if (mDirection != AVector::ZERO)
		{
			AVector right = mDirection.Cross(dir);
			if (right.Z() > 0.0f)
			{
				// move left
				mArduino->Run(Arduino::SDT_LEFT, 25);
			}
			else
			{
				// move right
				mArduino->Run(Arduino::SDT_RIGHT, 25);
			}
		}
	}
}
//----------------------------------------------------------------------------