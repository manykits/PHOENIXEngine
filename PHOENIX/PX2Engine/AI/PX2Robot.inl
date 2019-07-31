// PX2Slam.inl

//----------------------------------------------------------------------------
inline void Robot::SetMaxSpeed(float maxSpeed)
{
	mMaxSpeed = maxSpeed;
}
//----------------------------------------------------------------------------
inline float Robot::GetMaxSpeed() const
{
	return mMaxSpeed;
}
//----------------------------------------------------------------------------
inline void Robot::SetMinSpeed(float minSpeed)
{
	mMinSpeed = minSpeed;
}
//----------------------------------------------------------------------------
inline float Robot::GetMinSpeed() const
{
	return mMinSpeed;
}
//----------------------------------------------------------------------------
inline void Robot::SetMaxAccelerate(float accele)
{
	mMaxAccelerate = accele;
}
//----------------------------------------------------------------------------
inline float Robot::GetMaxAccelerate() const
{
	return mMaxAccelerate;
}
//----------------------------------------------------------------------------
inline void Robot::SetPredictTime(float predictTime)
{
	mPredictTime = predictTime;
}
//----------------------------------------------------------------------------
inline float Robot::GetPredictTime() const
{
	return mPredictTime;
}
//----------------------------------------------------------------------------
inline void Robot::SetRadius(float radius)
{
	mRadius = radius;
}
//----------------------------------------------------------------------------
inline float Robot::GetRadius() const
{
	return mRadius;
}
//----------------------------------------------------------------------------
inline void Robot::SetLidarOffset(float offsetDist)
{
	mLidarOffset = offsetDist;
}
//----------------------------------------------------------------------------
inline float Robot::GetLidarOffset() const
{
	return mLidarOffset;
}
//----------------------------------------------------------------------------
inline void Robot::SetMotoRate(int rate)
{
	mMotoRate = rate;
}
//----------------------------------------------------------------------------
inline int Robot::GetMotoRate() const
{
	return mMotoRate;
}
//----------------------------------------------------------------------------
inline void Robot::SetWheelRadius(float radius)
{
	mWheelRadius = radius;
}
//----------------------------------------------------------------------------
inline float Robot::GetWheelRadius() const
{
	return mWheelRadius;
}
//----------------------------------------------------------------------------
inline const AVector &Robot::GetDirection() const
{
	return mDirection;
}
//----------------------------------------------------------------------------
inline const AVector &Robot::GetUp() const
{
	return mUp;
}
//----------------------------------------------------------------------------
inline const AVector &Robot::GetRight() const
{
	return mRight;
}
//----------------------------------------------------------------------------
inline const HMatrix &Robot::GetMatrix() const
{
	return mMatrix;
}
//----------------------------------------------------------------------------
inline int Robot::GetMapSize() const
{
	return mMapDataPtr->MapStruct.MapSize;
}
//----------------------------------------------------------------------------
inline float Robot::GetResolution() const
{
	return mMapDataPtr->MapStruct.MapResolution;
}
//----------------------------------------------------------------------------
inline float Robot::GetSlam2DDegree() const
{
	return m2DSlamAngle;
}
//----------------------------------------------------------------------------
inline void Robot::SetSlamMapUpdate(bool update)
{
	mIsSlamMapDoUpdate = update;
}
//----------------------------------------------------------------------------
inline bool Robot::IsSlamMapUpdate() const
{
	return mIsSlamMapDoUpdate;
}
//----------------------------------------------------------------------------
inline float Robot::GetOffsetDegree() const
{
	return mOffsetDegree;
}
//----------------------------------------------------------------------------
inline Texture2D *Robot::GetTextureMap()
{
	return mTextureMap;
}
//----------------------------------------------------------------------------
inline UIFPicBox *Robot::GetUIFPicBoxMap()
{
	return mPicBoxMap;
}
//----------------------------------------------------------------------------
inline PathingGraph *Robot::GetPathingGraph()
{
	return mPathGraph;
}
//----------------------------------------------------------------------------
inline Robot::RoleType Robot::GetRoleType() const
{
	return mRoleType;
}
//----------------------------------------------------------------------------
inline LiDar *Robot::GetLidar()
{
	return mLiDar;
}
//----------------------------------------------------------------------------
inline Arduino *Robot::GetArduino()
{
	return mArduino;
}
//----------------------------------------------------------------------------