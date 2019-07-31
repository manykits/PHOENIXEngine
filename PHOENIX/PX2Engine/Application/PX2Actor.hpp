// PX2Actor.hpp

#ifndef PX2ACTOR_HPP
#define PX2ACTOR_HPP

#include "PX2Node.hpp"
#include "PX2AIAgentBase.hpp"
#include "PX2TriMesh.hpp"

namespace PX2
{
	class AIAgent;
	class Scene;
	class Robot;

	class PX2_ENGINE_ITEM Actor : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(Actor);
		PX2_DECLARE_STREAM(Actor);

	public:
		enum AIType
		{
			AIT_NONE,
			AIT_AGENT,
			AIT_AGENTOBJECT,
			AIT_MAX_TYPE
		};
		Actor(AIType at = AIT_AGENT);
		virtual ~Actor();
		void SetAIType(AIType at);
		AIType GetAIType() const;

		void SetTexture(const std::string &filePath,
			const Float4 &offset=Float4(0.0f, 0.0f, 1.0f, 1.0f));

		AIAgentBase *GetAIAgentBase();
		AIAgent *GetAIAgent();

		enum PhysicsShapeType
		{
			PST_GENERAL,
			PST_INFINITEPLANE,
			PST_MESH,
			PST_MAX_TYPE
		};
		void SetPhysicsShapeType(PhysicsShapeType type, Movable *meshMov=0);
		PhysicsShapeType GetPhysicsShapeType() const;

	public:
		void OnEvent(Event *ent);

		virtual void OnBeAttached();
		virtual void OnBeDetach();
		void RemoveAgentStuff(Scene *scene);

		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		// AI
	private:
		void _CreatePhysics();

		AIType mAIType;
		AIAgentBasePtr mAgentBase;
		PhysicsShapeType mPhysicsShapeType;
		MovablePtr mPhysicsMovable;

		// Robot
	public:
		void SetRobot(Robot *robot);
		Robot *GetRobot();

	private:
		Robot *mRobot;
	};

	PX2_REGISTER_STREAM(Actor);
	typedef Pointer0<Actor> ActorPtr;

}

#endif