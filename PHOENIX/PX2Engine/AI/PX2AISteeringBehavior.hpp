// PX2SteeringBehavior.hpp

#ifndef PX2STEERINGBEHAVIOR_HPP
#define PX2STEERINGBEHAVIOR_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector3.hpp"
#include "PX2AISteeringPath.hpp"

namespace PX2
{

	class AIAgent;
	class AIAgentObject;
	class Wall2D;

	const float WanderRad = 1.2f;
	const float WanderDist = 2.0f;
	const float WanderJitterPerSec = 80.0f;
	const float WaypointSeekDist = 0.4f;

	class PX2_ENGINE_ITEM SteeringBehavior
	{
	public:

		enum summing_method { weighted_average, prioritized };

		std::vector<AIAgentObject*> GetNearbyObjects(float radius);

	private:

		enum behavior_type
		{
			none = 0x00000,
			seek = 0x00002,
			flee = 0x00004,
			arrive = 0x00008,
			wander = 0x00010,
			cohesion = 0x00020,
			separation = 0x00040,
			allignment = 0x00080,
			obstacle_avoidance = 0x00100,
			wall_avoidance = 0x00200,
			follow_path = 0x00400,
			pursuit = 0x00800,
			evade = 0x01000,
			interpose = 0x02000,
			hide = 0x04000,
			flock = 0x08000,
			offset_pursuit = 0x10000,
		};

	private:
		AIAgent*     mAgent;

		Vector3f    m_vSteeringForce;

		AIAgent*     mTargetAgent1;
		AIAgent*     mTargetAgent2;

		Vector3f    m_vTarget;

		float                 mDBoxLength;

		std::vector<Vector3f> mFeelers;

		//the length of the 'feeler/s' used in wall detection
		float mWallDetectionFeelerLength;

		Vector3f     m_vWanderTarget;

		float        mWanderJitter;
		float        mWanderRadius;
		float        mWanderDistance;

		float        mWeightSeparation;
		float        mWeightCohesion;
		float        mWeightAlignment;
		float        mWeightWander;
		float        mWeightObstacleAvoidance;
		float        mWeightWallAvoidance;
		float        mWeightSeek;
		float        mWeightFlee;
		float        mWeightArrive;
		float        mWeightPursuit;
		float        mWeightOffsetPursuit;
		float        mWeightInterpose;
		float        mWeightHide;
		float        mWeightEvade;
		float        mWeightFollowPath;

		float        mViewDistance;

		//pointer to any current path
		AISteeringPath mPath;

		float        mWaypointSeekDistSq;
		Vector3f     m_vOffset;

		//binary flags to indicate whether or not a behavior should be active
		int           mFlags;

		enum Deceleration { slow = 3, normal = 2, fast = 1 };

		Deceleration mDeceleration;

		bool          mCellSpaceOn;

		summing_method  mSummingMethod;

		bool On(behavior_type bt) { return (mFlags & bt) == bt; }

		bool AccumulateForce(Vector3f &sf, Vector3f forceToAdd);

		Vector3f Seek(Vector3f TargetPos);
		Vector3f Flee(Vector3f TargetPos);
		Vector3f Arrive(Vector3f TargetPos,
			Deceleration deceleration);
		Vector3f Pursuit(const AIAgent* agent);
		Vector3f OffsetPursuit(const AIAgent* agent, const Vector3f offset);
		Vector3f Evade(const AIAgent* agent);
		Vector3f Wander();
		Vector3f ObstacleAvoidance(const std::vector<AIAgentObject*>& obstacles);

		Vector3f WallAvoidance(const std::vector<Wall2D> &walls);
		Vector3f FollowPath();

		Vector3f Interpose(const AIAgent* agentA, const AIAgent* agentB);
		Vector3f Hide(const AIAgent* hunter, const std::vector<AIAgentObject*>& obstacles);
		Vector3f Cohesion(const std::vector<AIAgent*> &agents);
		Vector3f Separation(const std::vector<AIAgent*> &agents);
		Vector3f Alignment(const std::vector<AIAgent*> &agents);

		Vector3f CohesionPlus(const std::vector<AIAgent*> &agents);
		Vector3f SeparationPlus(const std::vector<AIAgent*> &agents);
		Vector3f AlignmentPlus(const std::vector<AIAgent*> &agents);

		Vector3f CalculateWeightedSum();
		Vector3f CalculatePrioritized();

		Vector3f GetHidingPosition(const Vector3f& posOb,
			const float radiusOb,
			const Vector3f& posHunter);

	public:
		SteeringBehavior(AIAgent* agent);
		virtual ~SteeringBehavior();

		Vector3f Calculate();
		float ForwardComponent();
		float SideComponent();
		void RenderAids();

		void SetTarget(const Vector3f t) { m_vTarget = t; }
		void SetTargetAgent1(AIAgent* Agent) { mTargetAgent1 = Agent; }
		void SetTargetAgent2(AIAgent* Agent) { mTargetAgent2 = Agent; }
		void SetOffset(const Vector3f offset) { m_vOffset = offset; }
		Vector3f GetOffset()const { return m_vOffset; }
		void SetPath(std::list<Vector3f> new_path) { mPath.Set(new_path); }
		AISteeringPath &GetPath() { return mPath; }

		Vector3f Force()const { return m_vSteeringForce; }

		void ToggleSpacePartitioningOnOff() { mCellSpaceOn = !mCellSpaceOn; }
		bool IsSpacePartitioningOn()const { return mCellSpaceOn; }

		void SetSummingMethod(summing_method sm) { mSummingMethod = sm; }

		void FleeOn() { mFlags |= flee; }
		void SeekOn() { mFlags |= seek; }
		void ArriveOn() { mFlags |= arrive; }
		void WanderOn() { mFlags |= wander; }
		void PursuitOn(AIAgent* v) { mFlags |= pursuit; mTargetAgent1 = v; }
		void EvadeOn(AIAgent* v) { mFlags |= evade; mTargetAgent1 = v; }
		void CohesionOn() { mFlags |= cohesion; }
		void SeparationOn() { mFlags |= separation; }
		void AlignmentOn() { mFlags |= allignment; }
		void ObstacleAvoidanceOn() { mFlags |= obstacle_avoidance; }
		void WallAvoidanceOn() { mFlags |= wall_avoidance; }
		void FollowPathOn() { mFlags |= follow_path; }
		void InterposeOn(AIAgent* v1, AIAgent* v2) { mFlags |= interpose; mTargetAgent1 = v1; mTargetAgent2 = v2; }
		void HideOn(AIAgent* v) { mFlags |= hide; mTargetAgent1 = v; }
		void OffsetPursuitOn(AIAgent* v1, const Vector3f offset) { mFlags |= offset_pursuit; m_vOffset = offset; mTargetAgent1 = v1; }
		void FlockingOn() { CohesionOn(); AlignmentOn(); SeparationOn(); WanderOn(); }

		void FleeOff() { if (On(flee))   mFlags ^= flee; }
		void SeekOff() { if (On(seek))   mFlags ^= seek; }
		void ArriveOff() { if (On(arrive)) mFlags ^= arrive; }
		void WanderOff() { if (On(wander)) mFlags ^= wander; }
		void PursuitOff() { if (On(pursuit)) mFlags ^= pursuit; }
		void EvadeOff() { if (On(evade)) mFlags ^= evade; }
		void CohesionOff() { if (On(cohesion)) mFlags ^= cohesion; }
		void SeparationOff() { if (On(separation)) mFlags ^= separation; }
		void AlignmentOff() { if (On(allignment)) mFlags ^= allignment; }
		void ObstacleAvoidanceOff() { if (On(obstacle_avoidance)) mFlags ^= obstacle_avoidance; }
		void WallAvoidanceOff() { if (On(wall_avoidance)) mFlags ^= wall_avoidance; }
		void FollowPathOff() { if (On(follow_path)) mFlags ^= follow_path; }
		void InterposeOff() { if (On(interpose)) mFlags ^= interpose; }
		void HideOff() { if (On(hide)) mFlags ^= hide; }
		void OffsetPursuitOff() { if (On(offset_pursuit)) mFlags ^= offset_pursuit; }
		void FlockingOff() { CohesionOff(); AlignmentOff(); SeparationOff(); WanderOff(); }

		bool isFleeOn() { return On(flee); }
		bool isSeekOn() { return On(seek); }
		bool isArriveOn() { return On(arrive); }
		bool isWanderOn() { return On(wander); }
		bool isPursuitOn() { return On(pursuit); }
		bool isEvadeOn() { return On(evade); }
		bool isCohesionOn() { return On(cohesion); }
		bool isSeparationOn() { return On(separation); }
		bool isAlignmentOn() { return On(allignment); }
		bool isObstacleAvoidanceOn() { return On(obstacle_avoidance); }
		bool isWallAvoidanceOn() { return On(wall_avoidance); }
		bool isFollowPathOn() { return On(follow_path); }
		bool isInterposeOn() { return On(interpose); }
		bool isHideOn() { return On(hide); }
		bool isOffsetPursuitOn() { return On(offset_pursuit); }

		float DBoxLength()const { return mDBoxLength; }
		const std::vector<Vector3f>& GetFeelers()const { return mFeelers; }

		float WanderJitter()const { return mWanderJitter; }
		float WanderDistance()const { return mWanderDistance; }
		float WanderRadius()const { return mWanderRadius; }

		float SeparationWeight()const { return mWeightSeparation; }
		float AlignmentWeight()const { return mWeightAlignment; }
		float CohesionWeight()const { return mWeightCohesion; }

		bool IsGoingToCollide(const std::vector<AIAgentObject*>& obstacles,
			float length);
	};

}

#endif