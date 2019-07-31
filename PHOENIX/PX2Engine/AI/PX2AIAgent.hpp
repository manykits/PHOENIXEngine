// PX2AIAgent.hpp

#ifndef PX2AIAGENT_HPP
#define PX2AIAGENT_HPP

#include "PX2CorePre.hpp"
#include "PX2Controller.hpp"
#include "PX2HQuaternion.hpp"
#include "PX2AIAgentBase.hpp"
#include "PX2NavMoveUnit.hpp"
#include "PX2Smoother.hpp"
#include "PX2AISteeringBehavior.hpp"
#include "PX2AISteeringPath.hpp"

namespace PX2
{

	class AIAgentGroup;
	class AIAgentObject;

	class PX2_ENGINE_ITEM AIAgent : public AIAgentBase
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(AIAgent);
		PX2_DECLARE_PROPERTY;

		friend class AIAgentGroup;
	public:
		AIAgent();
		AIAgent(Node *node);
		virtual ~AIAgent();

		void InitializeCapsule();

		static const float DEFAULT_AGENT_HEALTH;
		static const float DEFAULT_AGENT_HEIGHT;
		static const float DEFAULT_AGENT_MAX_FORCE;
		static const float DEFAULT_AGENT_MAX_SPEED;
		static const float DEFAULT_AGENT_SPEED;
		static const float DEFAULT_AGENT_TARGET_RADIUS;
		static const float DEFAULT_AGENT_WALKABLE_CLIMB;
		static const float DEFAULT_AGENT_WALKABLE_SLOPE;
		static const std::string DEFAULT_AGENT_TEAM;

	public:	
		void SetForward(const AVector& forward);
		void EnableForwarding(bool enable);
		bool IsForwardingEnabled() const;
		void SetForwarding(const AVector& forwarding);

		void SetHeight(float  height);
		float GetHeight() const;

		void SetMaxForce(float force);
		float GetMaxForce() const;

		void SetMaxSpeed(float speed);
		float GetMaxSpeed() const;

		void SetPath(const AISteeringPath& path);
		void RemovePath();
		bool IsPathOver() const;

		void SetSpeed(float speed);
		float GetSpeed() const;

		void SetTarget(const APoint& target);
		APoint GetTarget() const;

		void SetTargetRadius(float radius);
		float GetTargetRadius() const;

		void SetTeam(const std::string& team);

		void SetVelocity(const AVector& velocity);
		AVector GetVelocity() const;
		AVector GetVelocityNoPhysics() const;

		AVector GetRight() const;
		AVector GetUp() const;
		AVector GetForward() const;

		void SetHealth(float health);
		float GetHealth() const;

		void SetPhysicsRadius(float radius);
		float GetPhysicsRadius() const;

		// Robot
	public:
		void SetRobot(Robot *robot);
		Robot *GetRobot();

	public:
		void ApplyForce(const AVector &force);
		void ApplyForcing(const AVector &force);

		SteeringBehavior *GetSteeringBehavior();

	public_internal:
		void SetAIAgentWorld(AIAgentWorld *agentWorld);

	private:
		virtual void _Update(double applicationTime, double elapsedTime);

		SteeringBehavior *mSteeringBehavior;

		bool mIsHasPath;

		float mHealth;
		float mHeight;
		float mPhysicsRaduis;
		float mMaxForce;
		float mMaxSpeed;
		AVector mVelocity;
		float mSpeed;
		APoint mTarget;
		float mTargetRadius;
		std::string mTeam;

		bool mIsEnableForwarding;
		AVector mForwarding;
		Smoother<AVector> *mSmoother;

		AVector mForcing;
		Smoother<AVector> *mSmootherForcing;

		AIAgentObject *mLastMinObject;
	};

	PX2_REGISTER_STREAM(AIAgent);
	typedef Pointer0<AIAgent> AIAgentControllerPtr;
#include "PX2AIAgent.inl"

}

#endif