// PX2AIAgentBase.hpp

#ifndef PX2AIAGENTBASE_HPP
#define PX2AIAGENTBASE_HPP

#include "PX2CorePre.hpp"
#include "PX2Controller.hpp"
#include "PX2Node.hpp"
#include "PX2HQuaternion.hpp"

class btRigidBody;

namespace PX2
{

	class AIAgentWorld;
	class Robot;

	class PX2_ENGINE_ITEM AIAgentBase : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(AIAgentBase);
		PX2_DECLARE_PROPERTY;

		static const float DEFAULT_AGENT_MASS;
		static const float DEFAULT_AGENT_RADIUS;

	public:
		enum AgentType
		{
			AT_AGENT,
			AT_OBJCT,
			AT_MAX_TYPE
		};

		AIAgentBase(Node *node = 0, AgentType at = AT_AGENT);
		virtual ~AIAgentBase();
		
		virtual void Enable(bool enable);

		void UsePhysics(bool usePhysics);
		bool IsUsePhysics() const;

		// set get
		virtual void SetMass(float mass);
		float GetMass() const;
		void SetMassZeroAvoid(bool avoid);
		bool IsMassZeroAvoid() const;

		// Set Physics also set position
		virtual void SetOrientation(const HQuaternion& quaternion);
		HQuaternion GetOrientation() const;

		virtual void SetRotate(const HMatrix& mat);

		virtual void SetPosition(const APoint& position);
		APoint GetPosition() const;

		void SetRadius(float radius);
		virtual float GetRadius() const;

		// world
		AIAgentWorld* GetAIAgentWorld();
		const AIAgentWorld* GetAIAgentWorld() const;

		Node *GetNode();
		const Node *GetNode() const;

		btRigidBody* GetRigidBody();
		const btRigidBody* GetRigidBody() const;

		bool IsTagged() const { return mIsTag; }
		void Tag() { mIsTag = true; }
		void UnTag() { mIsTag = false; }

	public_internal:
		void SetNode(Node *node);
		void SetAgentWorld(AIAgentWorld *agentWorld);
		void SetRigidBody(btRigidBody* rigidBody);

		void _RefreshRigidValue();

	protected:
		virtual void _InitUpdate(double applicationTime, double elapsedTime);
		virtual void _Update(double applicationTime, double elapsedTime);

		AgentType mAgentType;

		bool mIsUsePhysics;
		AIAgentWorld* mAgentWorld;
		btRigidBody* mRigidBody;
		Node *mNode;
		float mMass;
		bool mIsMassZeroAvoid;
		float mRadius;

		bool mIsTag;

	protected:
		Robot *mRobot;
	};

	PX2_REGISTER_STREAM(AIAgentBase);
	typedef Pointer0<AIAgentBase> AIAgentBasePtr;

}

#endif