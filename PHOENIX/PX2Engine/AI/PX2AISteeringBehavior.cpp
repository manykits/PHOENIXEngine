// PX2AISteeringBehavior.cpp

#include "PX2AISteeringBehavior.hpp"
#include "PX2AIAgent.hpp"
#include "PX2AIAgentObject.hpp"
#include "PX2AIAgentWorld.hpp"
#include "PX2AISteeringParamLoader.hpp"
#include "PX2AIPath.hpp"
using namespace PX2;
using namespace std;

//------------------------------------------------------------------------
SteeringBehavior::SteeringBehavior(AIAgent* agent):
	mAgent(agent),
	mFlags(0),
	mDBoxLength(Prm.MinDetectionBoxLength),
	mWeightCohesion(Prm.CohesionWeight),
	mWeightAlignment(Prm.AlignmentWeight),
	mWeightSeparation(Prm.SeparationWeight),
	mWeightObstacleAvoidance(Prm.ObstacleAvoidanceWeight),
	mWeightWander(Prm.WanderWeight),
	mWeightWallAvoidance(Prm.WallAvoidanceWeight),
	mViewDistance(Prm.ViewDistance),
	mWallDetectionFeelerLength(Prm.WallDetectionFeelerLength),
	mFeelers(3),
	mDeceleration(normal),
	mTargetAgent1(0),
	mTargetAgent2(0),
	mWanderDistance(WanderDist),
	mWanderJitter(WanderJitterPerSec),
	mWanderRadius(WanderRad),
	mWaypointSeekDistSq(WaypointSeekDist*WaypointSeekDist),
	mWeightSeek(Prm.SeekWeight),
	mWeightFlee(Prm.FleeWeight),
	mWeightArrive(Prm.ArriveWeight),
	mWeightPursuit(Prm.PursuitWeight),
	mWeightOffsetPursuit(Prm.OffsetPursuitWeight),
	mWeightInterpose(Prm.InterposeWeight),
	mWeightHide(Prm.HideWeight),
	mWeightEvade(Prm.EvadeWeight),
	mWeightFollowPath(Prm.FollowPathWeight),
	mCellSpaceOn(false),
	mSummingMethod(prioritized)
{
	//stuff for the wander behavior
	float theta = Mathf::UnitRandom() * Mathf::TWO_PI;

	//create a vector to a target position on the wander circle
	m_vWanderTarget = Vector3f(mWanderRadius * Mathf::Cos(theta),
		mWanderRadius * Mathf::Sin(theta), 0.0f);

	FollowPathOn();
	//SeekOn();
	//ObstacleAvoidanceOn();
}
//------------------------------------------------------------------------
SteeringBehavior::~SteeringBehavior()
{
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Calculate()
{
	mPath.Update();

	//reset the steering force
	m_vSteeringForce = Vector3f::ZERO;

	//use space partitioning to calculate the neighbours of this vehicle
	//if switched on. If not, use the standard tagging system
	if (!IsSpacePartitioningOn())
	{
		//tag neighbors if any of the following 3 group behaviors are switched on
		if (On(separation) || On(allignment) || On(cohesion))
		{
			mAgent->GetAIAgentWorld()->TagVehiclesWithinViewRange(mAgent, mViewDistance);
		}
	}
	else
	{
		//calculate neighbours in cell-space if any of the following 3 group
		//behaviors are switched on
		if (On(separation) || On(allignment) || On(cohesion))
		{
			mAgent->GetAIAgentWorld()->CellSpace()->CalculateNeighbors(mAgent->GetPosition(), mViewDistance);
		}
	}

	switch (mSummingMethod)
	{
	case weighted_average:

		m_vSteeringForce = CalculateWeightedSum(); break;

	case prioritized:

		m_vSteeringForce = CalculatePrioritized(); break;

	default:m_vSteeringForce = Vector3f::ZERO;

	}//end switch

	m_vSteeringForce.Z() = 0.0f;
	return m_vSteeringForce;
}
//------------------------------------------------------------------------
float SteeringBehavior::ForwardComponent()
{
	return mAgent->GetForward().Dot(m_vSteeringForce);
}
//------------------------------------------------------------------------
float SteeringBehavior::SideComponent()
{
	return mAgent->GetRight().Dot(m_vSteeringForce);
}
//------------------------------------------------------------------------
bool SteeringBehavior::AccumulateForce(Vector3f &runningTot,
	Vector3f forceToAdd)
{
	float magnitudeSoFar = runningTot.Length();
	float magnitudeRemaining = mAgent->GetMaxForce() - 
		magnitudeSoFar;

	if (magnitudeRemaining <= 0.0) 
		return false;

	float magnitudeToAdd = forceToAdd.Length();

	if (magnitudeToAdd < magnitudeRemaining)
	{
		runningTot += forceToAdd;
	}
	else
	{
		forceToAdd.Normalize();
		runningTot += (forceToAdd * magnitudeRemaining);
	}

	return true;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::CalculatePrioritized()
{
	Vector3f force;

	if (On(wall_avoidance))
	{
		force = WallAvoidance(mAgent->GetAIAgentWorld()->GetWalls()) *
			mWeightWallAvoidance;

		if (!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}

	if (On(obstacle_avoidance))
	{
		force = ObstacleAvoidance(mAgent->GetAIAgentWorld()->GetObjects()) *
			mWeightObstacleAvoidance;

		if (!AccumulateForce(m_vSteeringForce, force)) 
			return m_vSteeringForce;
	}

	if (On(evade))
	{
		assert(mTargetAgent1 && "Evade target not assigned");

		force = Evade(mTargetAgent1) * mWeightEvade;

		if (!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}


	if (On(flee))
	{
		force = Flee(mAgent->GetTarget()) * mWeightFlee;

		if (!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}

	if (!IsSpacePartitioningOn())
	{
		if (On(separation))
		{
			force = Separation(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightSeparation;

			if (!AccumulateForce(m_vSteeringForce, force))
				return m_vSteeringForce;
		}

		if (On(allignment))
		{
			force = Alignment(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightAlignment;

			if (!AccumulateForce(m_vSteeringForce, force)) 
				return m_vSteeringForce;
		}

		if (On(cohesion))
		{
			force = Cohesion(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightCohesion;

			if (!AccumulateForce(m_vSteeringForce, force)) 
				return m_vSteeringForce;
		}
	}
	else
	{

		if (On(separation))
		{
			force = SeparationPlus(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightSeparation;

			if (!AccumulateForce(m_vSteeringForce, force))
				return m_vSteeringForce;
		}

		if (On(allignment))
		{
			force = AlignmentPlus(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightAlignment;

			if (!AccumulateForce(m_vSteeringForce, force)) 
				return m_vSteeringForce;
		}

		if (On(cohesion))
		{
			force = CohesionPlus(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightCohesion;

			if (!AccumulateForce(m_vSteeringForce, force)) 
				return m_vSteeringForce;
		}
	}

	if (On(seek))
	{
		force = Seek(mAgent->GetTarget()) * mWeightSeek;

		if (!AccumulateForce(m_vSteeringForce, force)) 
			return m_vSteeringForce;
	}


	if (On(arrive))
	{
		force = Arrive(mAgent->GetTarget(), mDeceleration) * mWeightArrive;

		if (!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}

	if (On(wander))
	{
		force = Wander() * mWeightWander;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(pursuit))
	{
		assert(mTargetAgent1 && "pursuit target not assigned");

		force = Pursuit(mTargetAgent1) * mWeightPursuit;

		if (!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}

	if (On(offset_pursuit))
	{
		assert(mTargetAgent1 && "pursuit target not assigned");
		assert(Vector3f::ZERO != m_vOffset && "No offset assigned");

		force = OffsetPursuit(mTargetAgent1, m_vOffset);

		if (!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}

	if (On(interpose))
	{
		assert(mTargetAgent1 && mTargetAgent2 && "Interpose agents not assigned");

		force = Interpose(mTargetAgent1, mTargetAgent2) * mWeightInterpose;

		if (!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}

	if (On(hide))
	{
		assert(mTargetAgent1 && "Hide target not assigned");

		force = Hide(mTargetAgent1, mAgent->GetAIAgentWorld()->GetObjects()) * mWeightHide;

		if (!AccumulateForce(m_vSteeringForce, force)) 
			return m_vSteeringForce;
	}


	if (On(follow_path))
	{
		force = FollowPath() * mWeightFollowPath;

		if (!AccumulateForce(m_vSteeringForce, force)) 
			return m_vSteeringForce;
	}

	return m_vSteeringForce;
}
//----------------------------------------------------------------------------
Vector3f SteeringBehavior::CalculateWeightedSum()
{
	if (On(wall_avoidance))
	{
		m_vSteeringForce += WallAvoidance(
			mAgent->GetAIAgentWorld()->GetWalls()) * mWeightWallAvoidance;
	}

	if (On(obstacle_avoidance))
	{
		m_vSteeringForce += ObstacleAvoidance(
			mAgent->GetAIAgentWorld()->GetObjects()) * 
			mWeightObstacleAvoidance;
	}

	if (On(evade))
	{
		assert(mTargetAgent1 && "Evade target not assigned");

		m_vSteeringForce += Evade(mTargetAgent1) * mWeightEvade;
	}

	if (!IsSpacePartitioningOn())
	{
		if (On(separation))
		{
			m_vSteeringForce += Separation(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightSeparation;
		}

		if (On(allignment))
		{
			m_vSteeringForce += Alignment(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightAlignment;
		}

		if (On(cohesion))
		{
			m_vSteeringForce += Cohesion(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightCohesion;
		}
	}
	else
	{
		if (On(separation))
		{
			m_vSteeringForce += SeparationPlus(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightSeparation;
		}

		if (On(allignment))
		{
			m_vSteeringForce += AlignmentPlus(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightAlignment;
		}

		if (On(cohesion))
		{
			m_vSteeringForce += CohesionPlus(mAgent->GetAIAgentWorld()->GetAgents()) * mWeightCohesion;
		}
	}


	if (On(wander))
	{
		m_vSteeringForce += Wander() * mWeightWander;
	}

	if (On(seek))
	{
		m_vSteeringForce += Seek(mAgent->GetTarget()) * mWeightSeek;
	}

	if (On(flee))
	{
		m_vSteeringForce += Flee(mAgent->GetTarget()) * mWeightFlee;
	}

	if (On(arrive))
	{
		m_vSteeringForce += Arrive(mAgent->GetTarget(), mDeceleration) * mWeightArrive;
	}

	if (On(pursuit))
	{
		assert(mTargetAgent1 && "pursuit target not assigned");

		m_vSteeringForce += Pursuit(mTargetAgent1) * mWeightPursuit;
	}

	if (On(offset_pursuit))
	{
		assert(mTargetAgent1 && "pursuit target not assigned");
		assert(m_vOffset!=Vector3f::ZERO && "No offset assigned");

		m_vSteeringForce += OffsetPursuit(mTargetAgent1, m_vOffset) * mWeightOffsetPursuit;
	}

	if (On(interpose))
	{
		assert(mTargetAgent1 && mTargetAgent2 && "Interpose agents not assigned");

		m_vSteeringForce += Interpose(mTargetAgent1, mTargetAgent2) * mWeightInterpose;
	}

	if (On(hide))
	{
		assert(mTargetAgent1 && "Hide target not assigned");

		m_vSteeringForce += Hide(mTargetAgent1, mAgent->GetAIAgentWorld()->GetObjects()) * mWeightHide;
	}

	if (On(follow_path))
	{
		m_vSteeringForce += FollowPath() * mWeightFollowPath;
	}

	float length = m_vSteeringForce.Length();
	if (length > mAgent->GetMaxForce())
	{
		m_vSteeringForce.Normalize();
		m_vSteeringForce = m_vSteeringForce *  mAgent->GetMaxForce();
	}

	return m_vSteeringForce;
}
//----------------------------------------------------------------------------
Vector3f SteeringBehavior::Seek(Vector3f targetPos)
{
	Vector3f dir = targetPos - mAgent->GetPosition();
	dir.Normalize();
	Vector3f desiredVelocity = dir * mAgent->GetMaxSpeed();

	return desiredVelocity - mAgent->GetVelocity();
}
//----------------------------------------------------------------------------
Vector3f SteeringBehavior::Flee(Vector3f targetPos)
{
	Vector3f dir = mAgent->GetPosition().To3() - targetPos;
	dir.Normalize();
	Vector3f desiredVelocity = dir * mAgent->GetMaxSpeed();

	return (desiredVelocity - mAgent->GetVelocity());
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Arrive(Vector3f targetPos,
	Deceleration deceleration)
{
	Vector3f toTarget = targetPos - mAgent->GetPosition();
	float dist = toTarget.Length();

	if (dist > 0)
	{
		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration..
		const float DecelerationTweaker = 0.3;

		//calculate the speed required to reach the target given the desired
		//deceleration
		float speed = dist / ((float)deceleration * DecelerationTweaker);

		//make sure the velocity does not exceed the max
		speed = Mathf::Min(speed, mAgent->GetMaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the toTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vector3f desiredVelocity = toTarget * speed / dist;

		return desiredVelocity - mAgent->GetVelocity();
	}

	return Vector3f::ZERO;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Pursuit(const AIAgent* evader)
{
	Vector3f toEvader = evader->GetPosition() 
		- mAgent->GetPosition();

	float relativeHeading = 
		mAgent->GetForward().Dot(evader->GetForward());

	if ((toEvader.Dot(mAgent->GetForward()) > 0) &&
		(relativeHeading < -0.95))  //acos(0.95)=18 degs
	{
		return Seek(evader->GetPosition());
	}

	float lookAheadTime = toEvader.Length() /
		(mAgent->GetMaxSpeed() + evader->GetSpeed());

	return Seek(evader->GetPosition() +
		evader->GetVelocity() * lookAheadTime);
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Evade(const AIAgent* pursuer)
{
	/* Not necessary to include the check for facing direction this time */

	Vector3f toPursuer = pursuer->GetPosition() -
		mAgent->GetPosition();

	//uncomment the following two lines to have Evade only consider pursuers 
	//within a 'threat range'
	const float ThreatRange = 100.0;
	if (toPursuer.SquaredLength() > ThreatRange * ThreatRange) return Vector3f();

	//the lookahead time is propotional to the distance between the pursuer
	//and the pursuer; and is inversely proportional to the sum of the
	//agents' velocities
	float lookAheadTime = toPursuer.Length() /
		(mAgent->GetMaxSpeed() + pursuer->GetSpeed());

	//now flee away from predicted future position of the pursuer
	return Flee(pursuer->GetPosition() +
		pursuer->GetVelocity() * lookAheadTime);
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Wander()
{
	//and steer towards it
	return Vector3f::ZERO;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::ObstacleAvoidance(
	const std::vector<AIAgentObject*>& obstacles)
{
	float speed = mAgent->GetSpeed();
	float maxSpeed = mAgent->GetMaxSpeed();

	mDBoxLength = Prm.MinDetectionBoxLength + (speed / maxSpeed) *
		Prm.MinDetectionBoxLength;

	mAgent->GetAIAgentWorld()->TagObstaclesWithinViewRange(mAgent,
		mDBoxLength);

	AIAgentObject* closestIntersectingObstacle = NULL;
	float distToClosestIP = Mathf::MAX_REAL;

	Vector3f localPosOfClosestObstacle;
	std::vector<AIAgentObject*>::const_iterator curOb = obstacles.begin();

	while (curOb != obstacles.end())
	{
		//if the obstacle has been tagged within range proceed
		if ((*curOb)->IsTagged())
		{
			HMatrix localMat =
				HMatrix(mAgent->GetRight(), mAgent->GetForward(),
					mAgent->GetUp(), mAgent->GetPosition(), true);

			APoint curObjPos = (*curOb)->GetPosition();
			Vector3f localPos = localMat.Inverse() * curObjPos;

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (localPos.Y() >= 0)
			{
				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				float expandedRadius = (*curOb)->GetRadius() + mAgent->GetRadius();

				if (fabs(localPos.X()) < expandedRadius)
				{
					//now to do a line/circle intersection test. The center of the 
					//circle is represented by (cX, cY). The intersection points are 
					//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
					//We only need to look at the smallest positive value of x because
					//that will be the closest point of intersection.
					float cX = localPos.X();
					float cY = localPos.Y();

					//we only need to calculate the sqrt part of the above equation once
					float sqrtPart = sqrt(expandedRadius*expandedRadius - cX*cX);

					float ip = cY - sqrtPart;

					if (ip <= 0.0)
					{
						ip = cY + sqrtPart;
					}

					//test to see if this is the closest so far. If it is keep a
					//record of the obstacle and its local coordinates
					if (ip < distToClosestIP)
					{
						distToClosestIP = ip;

						closestIntersectingObstacle = *curOb;

						localPosOfClosestObstacle = localPos;
					}
				}
			}
		}

		++curOb;
	}

	AVector SteeringForce;
	if (closestIntersectingObstacle)
	{
		//the closer the agent is to an object, the stronger the 
		//steering force should be
		float distVal = (mDBoxLength - localPosOfClosestObstacle.Y());
		if (distVal < 0.0f)
			distVal = 0.0f;

		float multiplier = 1.0 + distVal / mDBoxLength;

		//calculate the lateral force
		SteeringForce.X() = (closestIntersectingObstacle->GetRadius() -
			localPosOfClosestObstacle.X())  * multiplier;
	
		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const float BrakingWeight = 0.3;

		SteeringForce.Y() = (closestIntersectingObstacle->GetRadius() -
			localPosOfClosestObstacle.Y()) *
			BrakingWeight;
	}

	//finally, convert the steering vector from local to world space
	HMatrix mat =
		HMatrix(mAgent->GetRight(), mAgent->GetForward(),
			mAgent->GetUp(), APoint::ORIGIN, true);

	AVector vec = mat * SteeringForce;
	return vec;
}
//------------------------------------------------------------------------
std::vector<AIAgentObject*> SteeringBehavior::GetNearbyObjects(float radius)
{
	std::vector<AIAgentObject*> objsOut;

	mAgent->GetAIAgentWorld()->TagObstaclesWithinViewRange(mAgent,
		radius);

	const std::vector<AIAgentObject*>& obstacles =
		mAgent->GetAIAgentWorld()->GetObjects();

	AIAgentObject* closestIntersectingObstacle = NULL;
	float distToClosestIP = Mathf::MAX_REAL;

	Vector3f localPosOfClosestObstacle;
	std::vector<AIAgentObject*>::const_iterator curOb = obstacles.begin();

	while (curOb != obstacles.end())
	{
		if ((*curOb)->IsTagged())
		{
			objsOut.push_back(*curOb);
		}

		curOb++;
	}

	return objsOut;
}
//------------------------------------------------------------------------
bool SteeringBehavior::IsGoingToCollide(
	const std::vector<AIAgentObject*>& obstacles, float length)
{
	mAgent->GetAIAgentWorld()->TagObstaclesWithinViewRange(mAgent,
		length);

	AIAgentObject* closestIntersectingObstacle = 0;
	float distToClosestIP = Mathf::MAX_REAL;

	Vector3f localPosOfClosestObstacle;
	std::vector<AIAgentObject*>::const_iterator curOb = obstacles.begin();

	while (curOb != obstacles.end())
	{
		if ((*curOb)->IsTagged())
		{
			HMatrix localMat =
				HMatrix(mAgent->GetRight(), mAgent->GetForward(),
					mAgent->GetUp(), mAgent->GetPosition(), true);

			APoint curObjPos = (*curOb)->GetPosition();
			Vector3f localPos = localMat.Inverse() * curObjPos;

			if (localPos.Y() >= 0)
			{
				float expandedRadius = (*curOb)->GetRadius() + mAgent->GetRadius();

				if (fabs(localPos.X()) < expandedRadius)
				{
					float cX = localPos.X();
					float cY = localPos.Y();

					//we only need to calculate the sqrt part of the above equation once
					float sqrtPart = sqrt(expandedRadius*expandedRadius - cX*cX);

					float ip = cY - sqrtPart;

					if (ip <= 0.0)
					{
						ip = cY + sqrtPart;
					}

					if (ip < distToClosestIP)
					{
						distToClosestIP = ip;

						closestIntersectingObstacle = *curOb;

						localPosOfClosestObstacle = localPos;
					}
				}
			}
		}

		++curOb;
	}

	if (closestIntersectingObstacle)
	{
		return true;
	}

	return false;
}
//------------------------------------------------------------------------
inline bool LineIntersection2D(Vector3f A, Vector3f B, Vector3f C,
	Vector3f D, float& dist, Vector3f& point)
{

	float rTop = (A.Y() - C.Y())*(D.X() - C.X()) - (A.X()- C.X())*(D.Y() - C.Y());
	float rBot = (B.X() - A.X())*(D.Y() - C.Y()) - (B.Y() - A.Y())*(D.X() - C.X());

	float sTop = (A.Y() - C.Y())*(B.X() - A.X()) - (A.X()- C.X())*(B.Y() - A.Y());
	float sBot = (B.X() - A.X())*(D.Y() - C.Y()) - (B.Y() - A.Y())*(D.X() - C.X());

	if ((rBot == 0) || (sBot == 0))
	{
		//lines are parallel
		return false;
	}

	float r = rTop / rBot;
	float s = sTop / sBot;

	if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
	{
		Vector3f distVec = A - B;
		float length = distVec.Length();

		dist = length * r;

		point = A + r * (B - A);

		return true;
	}

	else
	{
		dist = 0;

		return false;
	}
}
Vector3f _Vec2To3(Vector2f vec2)
{
	return Vector3f(vec2[0], vec2[1], vec2[2]);
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::WallAvoidance(const std::vector<Wall2D>& walls)
{
	float DistToThisIP = 0.0;
	float distToClosestIP = Mathf::MAX_REAL;

	//this will hold an index into the vector of walls
	int ClosestWall = -1;

	Vector3f SteeringForce,
		point,         //used for storing temporary info
		ClosestPoint;  //holds the closest intersection point

					   //examine each feeler in turn
	for (unsigned int flr = 0; flr<mFeelers.size(); ++flr)
	{
		//run through each wall checking for any intersection points
		for (unsigned int w = 0; w<walls.size(); ++w)
		{
			if (LineIntersection2D(mAgent->GetPosition(),
				mFeelers[flr],
				_Vec2To3(walls[w].From()),
				_Vec2To3(walls[w].To()),
				DistToThisIP,
				point))
			{
				//is this the closest found so far? If so keep a record
				if (DistToThisIP < distToClosestIP)
				{
					distToClosestIP = DistToThisIP;

					ClosestWall = w;

					ClosestPoint = point;
				}
			}
		}//next wall


		 //if an intersection point has been detected, calculate a force  
		 //that will direct the agent away
		if (ClosestWall >= 0)
		{
			//calculate by what distance the projected position of the agent
			//will overshoot the wall
			Vector3f OverShoot = mFeelers[flr] - ClosestPoint;

			//create a force in the direction of the wall normal, with a 
			//magnitude of the overshoot
			Vector2f normal2 = walls[ClosestWall].Normal();
			Vector3f normal3(normal2.X(), normal2.Y(), 0.0f);
			SteeringForce = normal3 * OverShoot.Length();
		}

	}//next feeler

	return SteeringForce;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Separation(const vector<AIAgent*> &neighbors)
{
	Vector3f SteeringForce;

	for (unsigned int a = 0; a<neighbors.size(); ++a)
	{
		if ((neighbors[a] != mAgent) && neighbors[a]->IsTagged() &&
			(neighbors[a] != mTargetAgent1))
		{
			Vector3f ToAgent = mAgent->GetPosition() 
				- neighbors[a]->GetPosition();

			float length = ToAgent.Normalize();
			SteeringForce += ToAgent / length;
		}
	}

	return SteeringForce;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Alignment(const vector<AIAgent*>& neighbors)
{
	//used to record the average heading of the neighbors
	Vector3f AverageHeading;

	//used to count the number of vehicles in the neighborhood
	int    NeighborCount = 0;

	//iterate through all the tagged vehicles and sum their heading vectors  
	for (unsigned int a = 0; a<neighbors.size(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined  is close enough ***also make sure it doesn't
		//include any evade target ***
		if ((neighbors[a] != mAgent) && neighbors[a]->IsTagged() &&
			(neighbors[a] != mTargetAgent1))
		{
			AverageHeading += neighbors[a]->GetForward();

			++NeighborCount;
		}
	}

	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (NeighborCount > 0)
	{
		AverageHeading /= (float)NeighborCount;

		AverageHeading -= mAgent->GetForward();
	}

	return AverageHeading;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Cohesion(const vector<AIAgent*> &neighbors)
{
	//first find the center of mass of all the agents
	Vector3f CenterOfMass, SteeringForce;

	int NeighborCount = 0;

	//iterate through the neighbors and sum up all the position vectors
	for (unsigned int a = 0; a<neighbors.size(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined is close enough ***also make sure it doesn't
		//include the evade target ***
		if ((neighbors[a] != mAgent) && neighbors[a]->IsTagged() &&
			(neighbors[a] != mTargetAgent1))
		{
			CenterOfMass += neighbors[a]->GetPosition();

			++NeighborCount;
		}
	}

	if (NeighborCount > 0)
	{
		//the center of mass is the average of the sum of positions
		CenterOfMass /= (float)NeighborCount;

		//now seek towards that position
		SteeringForce = Seek(CenterOfMass);
	}

	SteeringForce.Normalize();

	return SteeringForce;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::SeparationPlus( 
	const vector<AIAgent*> &neighbors)
{
	Vector3f SteeringForce;

	//iterate through the neighbors and sum up all the position vectors
	for (AIAgentBase* pV = mAgent->GetAIAgentWorld()->CellSpace()->Begin();
		!mAgent->GetAIAgentWorld()->CellSpace()->End();
		pV = mAgent->GetAIAgentWorld()->CellSpace()->Next())
	{
		if (pV != mAgent)
		{
			Vector3f ToAgent = mAgent->GetPosition() - 
				pV->GetPosition();

			float length = ToAgent.Normalize();
			
			SteeringForce += ToAgent / length;
		}

	}

	return SteeringForce;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::AlignmentPlus(const vector<AIAgent*> &neighbors)
{
	//This will record the average heading of the neighbors
	Vector3f AverageHeading;

	//This count the number of vehicles in the neighborhood
	float    NeighborCount = 0.0;

	//iterate through the neighbors and sum up all the position vectors
	for (AIAgent* pV = mAgent->GetAIAgentWorld()->CellSpace()->Begin();
		!mAgent->GetAIAgentWorld()->CellSpace()->End();
		pV = mAgent->GetAIAgentWorld()->CellSpace()->Next())
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined  is close enough
		if (pV != mAgent)
		{
			AverageHeading += pV->GetForward();

			++NeighborCount;
		}

	}

	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (NeighborCount > 0.0)
	{
		AverageHeading /= NeighborCount;

		AverageHeading -= mAgent->GetForward();
	}

	return AverageHeading;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::CohesionPlus(const vector<AIAgent*> &neighbors)
{
	//first find the center of mass of all the agents
	Vector3f CenterOfMass, SteeringForce;

	int NeighborCount = 0;

	//iterate through the neighbors and sum up all the position vectors
	for (AIAgent* pV = mAgent->GetAIAgentWorld()->CellSpace()->Begin();
		!mAgent->GetAIAgentWorld()->CellSpace()->End();
		pV = mAgent->GetAIAgentWorld()->CellSpace()->Next())
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined is close enough
		if (pV != mAgent)
		{
			CenterOfMass += pV->GetPosition();

			++NeighborCount;
		}
	}

	if (NeighborCount > 0)
	{
		//the center of mass is the average of the sum of positions
		CenterOfMass /= (float)NeighborCount;

		//now seek towards that position
		SteeringForce = Seek(CenterOfMass);
	}

	SteeringForce.Normalize();

	return SteeringForce;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Interpose(const AIAgent* agentA,
	const AIAgent* agentB)
{
	Vector3f midPoint = (agentA->GetPosition() +
		agentB->GetPosition()) / 2.0;

	Vector3f vecDist = mAgent->GetPosition().To3() - midPoint;
	float length = vecDist.Length();

	float TimeToReachMidPoint = length / mAgent->GetMaxSpeed();

	Vector3f APos = agentA->GetPosition() + agentA->GetVelocity() * TimeToReachMidPoint;
	Vector3f BPos = agentB->GetPosition() + agentB->GetVelocity() * TimeToReachMidPoint;

	midPoint = (APos + BPos) / 2.0;

	return Arrive(midPoint, fast);
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::Hide(const AIAgent* hunter,
	const vector<AIAgentObject*>& obstacles)
{
	float    DistToClosest = Mathf::MAX_REAL;
	Vector3f BestHidingSpot;

	std::vector<AIAgentObject*>::const_iterator curOb = obstacles.begin();
	std::vector<AIAgentObject*>::const_iterator closest;

	while (curOb != obstacles.end())
	{
		//calculate the position of the hiding spot for this obstacle
		Vector3f HidingSpot = GetHidingPosition((*curOb)->GetPosition(),
			(*curOb)->GetRadius(),
			hunter->GetPosition());

		//work in distance-squared space to find the closest hiding
		//spot to the agent
		Vector3f vecDist = HidingSpot - mAgent->GetPosition();
		float dist = vecDist.Length();

		if (dist < DistToClosest)
		{
			DistToClosest = dist;

			BestHidingSpot = HidingSpot;

			closest = curOb;
		}

		++curOb;

	}//end while


	if (DistToClosest == Mathf::MAX_REAL)
	{
		return Evade(hunter);
	}

	//else use Arrive on the hiding spot
	return Arrive(BestHidingSpot, fast);
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::GetHidingPosition(const Vector3f& posOb,
	const float     radiusOb,
	const Vector3f& posHunter)
{
	//calculate how far away the agent is to be from the chosen obstacle's
	//bounding radius
	const float DistanceFromBoundary = 30.0;
	float       DistAway = radiusOb + DistanceFromBoundary;

	//calculate the heading toward the object from the hunter
	Vector3f ToOb = (posOb - posHunter);
	ToOb.Normalize();

	//scale it to size and add to the obstacles position to get
	//the hiding spot.
	return (ToOb * DistAway) + posOb;
}
//------------------------------------------------------------------------
Vector3f SteeringBehavior::FollowPath()
{
	if (mPath.GetPath().size() == 0)
		return Vector3f::ZERO;

	//move to next target if close enough to current target (working in
	//distance squared space)
	if (!mPath.Finished())
	{
		APoint curWayPoint = mPath.CurrentWaypoint();
		Vector3f vec = curWayPoint - mAgent->GetPosition();
		float lengthSquare = vec.SquaredLength();

		if (lengthSquare < mWaypointSeekDistSq)
		{
			mPath.SetNextWaypoint();
		}
	}

	if (!mPath.Finished())
	{
		APoint curWayPoint = mPath.CurrentWaypoint();
		return Seek(curWayPoint);
	}
	else
	{
		return AVector::ZERO;
	}
}

//------------------------------------------------------------------------
Vector3f SteeringBehavior::OffsetPursuit(const AIAgent* leader,
	const Vector3f offset)
{
	HMatrix localMat =
		HMatrix(leader->GetRight(), leader->GetForward(),
			leader->GetUp(), leader->GetPosition(), true);

	//calculate the offset's position in world space
	APoint worldOffsetPos = localMat * APoint(offset.X(), offset.Y(), offset.Z());

	Vector3f ToOffset = worldOffsetPos.To3() - mAgent->GetPosition();

	float lookAheadTime = ToOffset.Length() /
		(mAgent->GetMaxSpeed() + leader->GetSpeed());

	//now Arrive at the predicted future position of the offset
	return Arrive(worldOffsetPos + leader->GetVelocity() * lookAheadTime, fast);
}
//------------------------------------------------------------------------
void SteeringBehavior::RenderAids()
{
	//gdi->TransparentText();
	//gdi->TextColor(Cgdi::grey);

	//int NextSlot = 0; int SlotSize = 20;

	//if (KEYDOWN(VK_INSERT)) { mAgent->SetMaxForce(mAgent->MaxForce() + 1000.0f*mAgent->TimeElapsed()); }
	//if (KEYDOWN(VK_DELETE)) { if (mAgent->MaxForce() > 0.2f) mAgent->SetMaxForce(mAgent->MaxForce() - 1000.0f*mAgent->TimeElapsed()); }
	//if (KEYDOWN(VK_HOME)) { mAgent->SetMaxSpeed(mAgent->MaxSpeed() + 50.0f*mAgent->TimeElapsed()); }
	//if (KEYDOWN(VK_END)) { if (mAgent->MaxSpeed() > 0.2f) mAgent->SetMaxSpeed(mAgent->MaxSpeed() - 50.0f*mAgent->TimeElapsed()); }

	//if (mAgent->MaxForce() < 0) mAgent->SetMaxForce(0.0f);
	//if (mAgent->MaxSpeed() < 0) mAgent->SetMaxSpeed(0.0f);

	//if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "MaxForce(Ins/Del):"); gdi->TextAtPos(160, NextSlot, ttos(mAgent->MaxForce() / Prm.SteeringForceTweaker)); NextSlot += SlotSize; }
	//if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "MaxSpeed(Home/End):"); gdi->TextAtPos(160, NextSlot, ttos(mAgent->MaxSpeed())); NextSlot += SlotSize; }

	////render the steering force
	//if (mAgent->World()->RenderSteeringForce())
	//{
	//	gdi->RedPen();
	//	Vector3f F = (m_vSteeringForce / Prm.SteeringForceTweaker) * Prm.VehicleScale;
	//	gdi->Line(mAgent->Pos(), mAgent->Pos() + F);
	//}

	////render wander stuff if relevant
	//if (On(wander) && mAgent->World()->RenderWanderCircle())
	//{
	//	if (KEYDOWN('F')) { mWanderJitter += 1.0f*mAgent->TimeElapsed(); Clamp(mWanderJitter, 0.0f, 100.0f); }
	//	if (KEYDOWN('V')) { mWanderJitter -= 1.0f*mAgent->TimeElapsed(); Clamp(mWanderJitter, 0.0f, 100.0f); }
	//	if (KEYDOWN('G')) { mWanderDistance += 2.0f*mAgent->TimeElapsed(); Clamp(mWanderDistance, 0.0f, 50.0f); }
	//	if (KEYDOWN('B')) { mWanderDistance -= 2.0f*mAgent->TimeElapsed(); Clamp(mWanderDistance, 0.0f, 50.0f); }
	//	if (KEYDOWN('H')) { mWanderRadius += 2.0f*mAgent->TimeElapsed(); Clamp(mWanderRadius, 0.0f, 100.0f); }
	//	if (KEYDOWN('N')) { mWanderRadius -= 2.0f*mAgent->TimeElapsed(); Clamp(mWanderRadius, 0.0f, 100.0f); }


	//	if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Jitter(F/V): "); gdi->TextAtPos(160, NextSlot, ttos(mWanderJitter)); NextSlot += SlotSize; }
	//	if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Distance(G/B): "); gdi->TextAtPos(160, NextSlot, ttos(mWanderDistance)); NextSlot += SlotSize; }
	//	if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Radius(H/N): "); gdi->TextAtPos(160, NextSlot, ttos(mWanderRadius)); NextSlot += SlotSize; }


	//	//calculate the center of the wander circle
	//	Vector3f m_vTCC = PointToWorldSpace(Vector3f(mWanderDistance*mAgent->GetRadius(), 0),
	//		mAgent->Heading(),
	//		mAgent->Side(),
	//		mAgent->Pos());
	//	//draw the wander circle
	//	gdi->GreenPen();
	//	gdi->HollowBrush();
	//	gdi->Circle(m_vTCC, mWanderRadius*mAgent->GetRadius());

	//	//draw the wander target
	//	gdi->RedPen();
	//	gdi->Circle(PointToWorldSpace((m_vWanderTarget + Vector3f(mWanderDistance, 0))*mAgent->GetRadius(),
	//		mAgent->Heading(),
	//		mAgent->Side(),
	//		mAgent->Pos()), 3);
	//}

	////render the detection box if relevant
	//if (mAgent->World()->RenderDetectionBox())
	//{
	//	gdi->GreyPen();

	//	//a vertex buffer rqd for drawing the detection box
	//	static std::vector<Vector3f> box(4);

	//	float length = Prm.MinDetectionBoxLength +
	//		(mAgent->Speed() / mAgent->MaxSpeed()) *
	//		Prm.MinDetectionBoxLength;

	//	//verts for the detection box buffer
	//	box[0] = Vector3f(0, mAgent->GetRadius());
	//	box[1] = Vector3f(length, mAgent->GetRadius());
	//	box[2] = Vector3f(length, -mAgent->GetRadius());
	//	box[3] = Vector3f(0, -mAgent->GetRadius());


	//	if (!mAgent->isSmoothingOn())
	//	{
	//		box = WorldTransform(box, mAgent->Pos(), mAgent->Heading(), mAgent->Side());
	//		gdi->ClosedShape(box);
	//	}
	//	else
	//	{
	//		box = WorldTransform(box, mAgent->Pos(), mAgent->SmoothedHeading(), mAgent->SmoothedHeading().Perp());
	//		gdi->ClosedShape(box);
	//	}


	//	//////////////////////////////////////////////////////////////////////////
	//	//the detection box length is proportional to the agent's velocity
	//	mDBoxLength = Prm.MinDetectionBoxLength +
	//		(mAgent->Speed() / mAgent->MaxSpeed()) *
	//		Prm.MinDetectionBoxLength;

	//	//tag all obstacles within range of the box for processing
	//	mAgent->World()->TagObstaclesWithinViewRange(mAgent, mDBoxLength);

	//	//this will keep track of the closest intersecting obstacle (CIB)
	//	BaseGameEntity* closestIntersectingObstacle = NULL;

	//	//this will be used to track the distance to the CIB
	//	float distToClosestIP = MaxDouble;

	//	//this will record the transformed local coordinates of the CIB
	//	Vector3f localPosOfClosestObstacle;

	//	std::vector<BaseGameEntity*>::const_iterator curOb = mAgent->World()->Obstacles().begin();

	//	while (curOb != mAgent->World()->Obstacles().end())
	//	{
	//		//if the obstacle has been tagged within range proceed
	//		if ((*curOb)->IsTagged())
	//		{
	//			//calculate this obstacle's position in local space
	//			Vector3f localPos = PointToLocalSpace((*curOb)->Pos(),
	//				mAgent->Heading(),
	//				mAgent->Side(),
	//				mAgent->Pos());

	//			//if the local position has a negative x value then it must lay
	//			//behind the agent. (in which case it can be ignored)
	//			if (localPos.X() >= 0)
	//			{
	//				//if the distance from the x axis to the object's position is less
	//				//than its radius + half the width of the detection box then there
	//				//is a potential intersection.
	//				if (fabs(localPos.Y()) < ((*curOb)->GetRadius() + mAgent->GetRadius()))
	//				{
	//					gdi->ThickRedPen();
	//					gdi->ClosedShape(box);
	//				}
	//			}
	//		}

	//		++curOb;
	//	}


	//	/////////////////////////////////////////////////////
	//}

	////render the wall avoidnace feelers
	//if (On(wall_avoidance) && mAgent->World()->RenderFeelers())
	//{
	//	gdi->OrangePen();

	//	for (unsigned int flr = 0; flr<mFeelers.size(); ++flr)
	//	{

	//		gdi->Line(mAgent->Pos(), mFeelers[flr]);
	//	}
	//}

	////render path info
	//if (On(follow_path) && mAgent->World()->RenderPath())
	//{
	//	mPath.Render();
	//}


	//if (On(separation))
	//{
	//	if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Separation(S/X):"); gdi->TextAtPos(160, NextSlot, ttos(mWeightSeparation / Prm.SteeringForceTweaker)); NextSlot += SlotSize; }

	//	if (KEYDOWN('S')) { mWeightSeparation += 200 * mAgent->TimeElapsed(); Clamp(mWeightSeparation, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
	//	if (KEYDOWN('X')) { mWeightSeparation -= 200 * mAgent->TimeElapsed(); Clamp(mWeightSeparation, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
	//}

	//if (On(allignment))
	//{
	//	if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Alignment(A/Z):"); gdi->TextAtPos(160, NextSlot, ttos(mWeightAlignment / Prm.SteeringForceTweaker)); NextSlot += SlotSize; }

	//	if (KEYDOWN('A')) { mWeightAlignment += 200 * mAgent->TimeElapsed(); Clamp(mWeightAlignment, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
	//	if (KEYDOWN('Z')) { mWeightAlignment -= 200 * mAgent->TimeElapsed(); Clamp(mWeightAlignment, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
	//}

	//if (On(cohesion))
	//{
	//	if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "Cohesion(D/C):"); gdi->TextAtPos(160, NextSlot, ttos(mWeightCohesion / Prm.SteeringForceTweaker)); NextSlot += SlotSize; }
	//	if (KEYDOWN('D')) { mWeightCohesion += 200 * mAgent->TimeElapsed(); Clamp(mWeightCohesion, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
	//	if (KEYDOWN('C')) { mWeightCohesion -= 200 * mAgent->TimeElapsed(); Clamp(mWeightCohesion, 0.0f, 50.0f * Prm.SteeringForceTweaker); }
	//}

	//if (On(follow_path))
	//{
	//	float sd = sqrt(mWaypointSeekDistSq);
	//	if (mAgent->ID() == 0) { gdi->TextAtPos(5, NextSlot, "SeekDistance(D/C):"); gdi->TextAtPos(160, NextSlot, ttos(sd)); NextSlot += SlotSize; }

	//	if (KEYDOWN('D')) { mWaypointSeekDistSq += 1.0; }
	//	if (KEYDOWN('C')) { mWaypointSeekDistSq -= 1.0; Clamp(mWaypointSeekDistSq, 0.0f, 400.0f); }
	//}
}





