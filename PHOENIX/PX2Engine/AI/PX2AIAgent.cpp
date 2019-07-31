// PX2AIAgent.cpp

#include "PX2AIAgent.hpp"
#include "PX2BtPhysicsUtilities.hpp"
#include "PX2AIAgentUtilities.hpp"
#include "PX2Node.hpp"
#include "PX2AIAgentObject.hpp"
#include "PX2AISteeringPath.hpp"
#include "PX2AIAgentWorld.hpp"
#include "PX2BtPhysicsWorld.hpp"
#include "PX2Math.hpp"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "Bullet/BulletCollision/CollisionShapes/btCollisionShape.h"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "Bullet/BulletCollision/CollisionShapes/btCollisionShape.h"
#include "PX2NavMoveManager.hpp"
#include "PX2Robot.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, AIAgentBase, AIAgent);
PX2_IMPLEMENT_STREAM(AIAgent);
PX2_IMPLEMENT_FACTORY(AIAgent);
PX2_IMPLEMENT_DEFAULT_NAMES(AIAgentBase, AIAgent);
//----------------------------------------------------------------------------
const float AIAgent::DEFAULT_AGENT_HEALTH = 100.0f;
const float AIAgent::DEFAULT_AGENT_HEIGHT = 1.6f;  // meters (5.2 feet)
const float AIAgent::DEFAULT_AGENT_MAX_FORCE = 1000.0f;  // newtons (kg*_/s^2)
const float AIAgent::DEFAULT_AGENT_MAX_SPEED = 7.0f;  // _/s (23.0 ft/s)
const float AIAgent::DEFAULT_AGENT_SPEED = 0.0f;  // m/s (0 ft/s)
const float AIAgent::DEFAULT_AGENT_TARGET_RADIUS = 0.5f;  // meters (1.64 feet)
const float AIAgent::DEFAULT_AGENT_WALKABLE_CLIMB = DEFAULT_AGENT_RADIUS / 2.0f;
const float AIAgent::DEFAULT_AGENT_WALKABLE_SLOPE = 45.0f;
const std::string AIAgent::DEFAULT_AGENT_TEAM = "team1";
//----------------------------------------------------------------------------
AIAgent::AIAgent():
AIAgentBase(0, AT_AGENT),
mHealth(DEFAULT_AGENT_HEALTH),
mHeight(DEFAULT_AGENT_HEIGHT),
mIsHasPath(false),
mMaxForce(DEFAULT_AGENT_MAX_FORCE),
mMaxSpeed(DEFAULT_AGENT_MAX_SPEED),
mPhysicsRaduis(DEFAULT_AGENT_RADIUS),
mSpeed(DEFAULT_AGENT_SPEED),
mTargetRadius(DEFAULT_AGENT_TARGET_RADIUS),
mTeam(DEFAULT_AGENT_TEAM),
mTarget(Vector3f::ZERO)
{
	SetName("AIAgent");

	if (!IsRegistedToScriptSystem())
		RegistToScriptSystem();

	SetForward(Vector3f::UNIT_Y);

	mIsEnableForwarding = true;
	mForwarding = AVector::UNIT_Y;
	mSmoother = new0 Smoother<AVector>(16, AVector::UNIT_Y);

	mForcing = AVector::UNIT_Y;
	mSmootherForcing = new0 Smoother<AVector>(24, AVector::UNIT_Y);

	mLastMinObject = 0;

	mSteeringBehavior = new0 SteeringBehavior(this);
}
//----------------------------------------------------------------------------
AIAgent::AIAgent(Node *node) :
AIAgentBase(node),
mHealth(DEFAULT_AGENT_HEALTH),
mHeight(DEFAULT_AGENT_HEIGHT),
mIsHasPath(false),
mMaxForce(DEFAULT_AGENT_MAX_FORCE),
mMaxSpeed(DEFAULT_AGENT_MAX_SPEED),
mSpeed(DEFAULT_AGENT_SPEED),
mTargetRadius(DEFAULT_AGENT_TARGET_RADIUS),
mTeam(DEFAULT_AGENT_TEAM),
mTarget(Vector3f::ZERO)
{
	SetName("AIAgent");

	if (!IsRegistedToScriptSystem())
		RegistToScriptSystem();

	SetForward(Vector3f::UNIT_Y);

	mIsEnableForwarding = true;
	mForwarding = AVector::UNIT_Y;
	mSmoother = new0 Smoother<AVector>(16, AVector::UNIT_Y);

	mForcing = AVector::UNIT_Y;
	mSmootherForcing = new0 Smoother<AVector>(24, AVector::UNIT_Y);

	mLastMinObject = 0;

	mSteeringBehavior = new0 SteeringBehavior(this);
}
//----------------------------------------------------------------------------
AIAgent::~AIAgent()
{
	if (mSmoother)
	{
		delete0(mSmoother);
	}

	if (mSmootherForcing)
	{
		delete0(mSmootherForcing);
	}

	if (mSteeringBehavior)
	{
		delete0(mSteeringBehavior);
	}
}
//----------------------------------------------------------------------------
void AIAgent::InitializeCapsule()
{
	AIAgentUtilities::CreateRigidBodyCapsule(this);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void AIAgent::RegistProperties()
{
	AIAgentBase::RegistProperties();

	AddPropertyClass("AIAgent");

	AddProperty("Health", Object::PT_FLOAT, mHealth);
	AddProperty("Height", Object::PT_FLOAT, mHeight);
	AddProperty("MaxForce", Object::PT_FLOAT, mMaxForce);
	AddProperty("MaxSpeed", Object::PT_FLOAT, mMaxSpeed);
	AddProperty("Speed", Object::PT_FLOAT, mSpeed);
	AddProperty("Target", Object::PT_APOINT3, mTarget);
	AddProperty("TargetRadius", Object::PT_FLOAT, mTargetRadius);
}
//----------------------------------------------------------------------------
void AIAgent::OnPropertyChanged(const PropertyObject &obj)
{
	AIAgentBase::OnPropertyChanged(obj);

	if (obj.Name == "Health")
	{
		SetHealth(PX2_ANY_AS(obj.Data, float));
	}
	else if (obj.Name == "Height")
	{
		SetHeight(PX2_ANY_AS(obj.Data, float));
	}
	else if (obj.Name == "MaxForce")
	{
		SetMaxForce(PX2_ANY_AS(obj.Data, float));
	}
	else if (obj.Name == "MaxSpeed")
	{
		SetMaxSpeed(PX2_ANY_AS(obj.Data, float));
	}
	else if (obj.Name == "Speed")
	{
		SetSpeed(PX2_ANY_AS(obj.Data, float));
	}
	else if (obj.Name == "Target")
	{
		SetTarget(PX2_ANY_AS(obj.Data, APoint));
	}
	else if (obj.Name == "TargetRadius")
	{
		SetTargetRadius(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------
void AIAgent::RemovePath()
{
	mIsHasPath = false;
}
//----------------------------------------------------------------------------
void AIAgent::EnableForwarding(bool enable)
{
	mIsEnableForwarding = enable;
}
//----------------------------------------------------------------------------
bool AIAgent::IsForwardingEnabled() const
{
	return mIsEnableForwarding;
}
//----------------------------------------------------------------------------
void AIAgent::SetForward(const AVector& forward)
{
	if (forward == AVector::ZERO)
		return;

	Vector3f up = Vector3f::UNIT_Z;
	AVector dir = forward;
	dir.Normalize();
	AVector right = dir.Cross(AVector::UNIT_Z);

	HMatrix mat(right, dir, up, APoint::ORIGIN, true);
	SetRotate(mat);
}
//----------------------------------------------------------------------------
void AIAgent::SetForwarding(const AVector& forwarding)
{
	if (mRobot)
	{
		/*_*/
	}
	else
	{
		mForwarding = forwarding;
		float length = mForwarding.Normalize();
		if (0.0f == length)
			return;

		if (!mIsEnableForwarding)
		{
			SetForward(mForwarding);
		}
	}
}
//----------------------------------------------------------------------------
void AIAgent::SetHeight(float height)
{
	mHeight = Mathf::Max(0.0f, height);

	if (mRigidBody)
	{
		AIAgentUtilities::UpdateRigidBodyCapsule(this);
	}
}
//----------------------------------------------------------------------------
void AIAgent::SetMaxForce(float force)
{
	mMaxForce = Mathf::Max(0.0f, force);
}
//----------------------------------------------------------------------------
void AIAgent::SetMaxSpeed(float speed)
{
	mMaxSpeed = Mathf::Max(0.0f, speed);
}
//----------------------------------------------------------------------------
void AIAgent::SetPath(const AISteeringPath& path)
{
	mSteeringBehavior->SetPath(path.GetPath());
	mIsHasPath = true;
}
//----------------------------------------------------------------------------
bool AIAgent::IsPathOver() const
{
	return false;
}
//----------------------------------------------------------------------------
void AIAgent::SetPhysicsRadius(float radius)
{
	mPhysicsRaduis = Mathf::Max(0.0f, radius);

	if (mRigidBody)
	{
		AIAgentUtilities::UpdateRigidBodyCapsule(this);
	}
}
//----------------------------------------------------------------------------
float AIAgent::GetPhysicsRadius() const
{
	return mPhysicsRaduis;
}
//----------------------------------------------------------------------------
void AIAgent::SetRobot(Robot *robot)
{
	mRobot = robot;
	if (mRobot)
	{
		mRobot->_SetAIAgent(this);
	}
}
//----------------------------------------------------------------------------
Robot *AIAgent::GetRobot()
{
	return mRobot;
}
//----------------------------------------------------------------------------
void AIAgent::SetSpeed(float speed)
{
	mSpeed = speed;
}
//----------------------------------------------------------------------------
void AIAgent::SetTarget(const APoint& target)
{
	mTarget = target;
}
//----------------------------------------------------------------------------
void AIAgent::SetTargetRadius(float radius)
{
	mTargetRadius = Mathf::Max(0.0f, radius);
}
//----------------------------------------------------------------------------
void AIAgent::SetTeam(const std::string& team)
{
	mTeam = team;
}
//----------------------------------------------------------------------------
void AIAgent::SetVelocity(const AVector& velocity)
{
	if (mRobot)
	{

	}
	else
	{
		if (mRigidBody)
		{
			PhysicsUtilities::SetRigidBodyVelocity(
				mRigidBody, PhysicsUtilities::Vector3ToBtVector3(velocity));
		}
	}

	mVelocity = velocity;

	SetSpeed(Vector3f(velocity.X(), velocity.Y(), 0.0f).Length());
}
//----------------------------------------------------------------------------
AVector AIAgent::GetRight() const
{
	if (mRobot)
	{
		return mRobot->GetRight();
	}
	else
	{
		if (mRigidBody)
		{
			const btQuaternion quaterion = mRigidBody->getOrientation();
			HQuaternion quat(quaterion.w(), quaterion.x(), quaterion.y(),
				quaterion.z());
			HMatrix mat;
			quat.ToRotationMatrix(mat);
			AVector right;
			mat.GetColumn(0, right);
			return right;
		}
		else if (mNode)
		{
			HMatrix rotMat = mNode->WorldTransform.GetRotate();
			AVector right;
			rotMat.GetColumn(0, right);
			return right;
		}
	}

	return AVector::UNIT_X;
}
//----------------------------------------------------------------------------
AVector AIAgent::GetUp() const
{
	if (mRobot)
	{
		return AVector::UNIT_Z;
	}
	else
	{
		if (mRigidBody)
		{
			const btQuaternion quaterion = mRigidBody->getOrientation();
			HQuaternion quat(quaterion.w(), quaterion.x(), quaterion.y(),
				quaterion.z());
			HMatrix mat;
			quat.ToRotationMatrix(mat);
			AVector up;
			mat.GetColumn(2, up);
			return up;
		}
		else if (mNode)
		{
			HMatrix rotMat = mNode->WorldTransform.GetRotate();
			AVector up;
			rotMat.GetColumn(2, up);
			return up;
		}
	}

	return AVector::UNIT_Z;
}
//----------------------------------------------------------------------------
AVector AIAgent::GetForward() const
{
	if (mRobot)
	{
		AVector dir = mRobot->GetDirection();
		dir.Normalize();
		return dir;
	}
	else
	{
		if (mRigidBody)
		{
			const btQuaternion quaterion = mRigidBody->getOrientation();

			HQuaternion quat = HQuaternion(
				quaterion.w(), quaterion.x(), quaterion.y(), quaterion.z());
			HMatrix mat;
			quat.ToRotationMatrix(mat);
			HPoint col;
			mat.GetColumn(1, col);
			return Vector3f(col[0], col[1], col[2]);
		}
		else if (mNode)
		{
			return mNode->WorldTransform.GetDirection();
		}
	}

	return Vector3f::UNIT_Y;
}
//----------------------------------------------------------------------------
void AIAgent::SetHealth(float health)
{
	mHealth = health;
}
//----------------------------------------------------------------------------
float AIAgent::GetHealth() const
{
	return mHealth;
}
//----------------------------------------------------------------------------
float AIAgent::GetMaxForce() const
{
	return mMaxForce;
}
//----------------------------------------------------------------------------
float AIAgent::GetMaxSpeed() const
{
	return mMaxSpeed;
}
//----------------------------------------------------------------------------
float AIAgent::GetSpeed() const
{
	if (mRobot)
	{
		return mSpeed;
	}
	else
	{
		if (mRigidBody)
		{
			const btVector3 velocity = mRigidBody->getLinearVelocity();
			return Vector3f(velocity.x(), velocity.y(), 0.0f).Length();
		}
		else
		{
			return mSpeed;
		}
	}
}
//----------------------------------------------------------------------------
AVector AIAgent::GetVelocity() const
{
	if (mRobot)
	{
		return AVector::ZERO;

	}
	else
	{
		if (mRigidBody)
		{
			btVector3 velocity = mRigidBody->getLinearVelocity();
			return AVector(velocity.x(), velocity.y(), velocity.z());
		}
		return GetForward() * mSpeed;
	}
}
//----------------------------------------------------------------------------
AVector AIAgent::GetVelocityNoPhysics() const
{
	return mVelocity;
}
//----------------------------------------------------------------------------
APoint AIAgent::GetTarget() const
{
	return mTarget;
}
//----------------------------------------------------------------------------
float AIAgent::GetTargetRadius() const
{
	return mTargetRadius;
}
//----------------------------------------------------------------------------
void AIAgent::ApplyForce(const AVector &force)
{
	if (mRobot)
	{
	}
	else
	{
		if (mRigidBody)
		{
			PhysicsUtilities::ApplyForce(
				mRigidBody, btVector3(force.X(), force.Y(), force.Z()));
		}
	}
}
//----------------------------------------------------------------------------
void AIAgent::ApplyForcing(const AVector &force)
{
	mForcing = force;
}
//----------------------------------------------------------------------------
SteeringBehavior *AIAgent::GetSteeringBehavior()
{
	return mSteeringBehavior;
}
//----------------------------------------------------------------------------
void AIAgent::_Update(double applicationTime, double elapsedTime)
{
	AIAgentBase::_Update(applicationTime, elapsedTime);

	if (mSteeringBehavior)
	{
		std::vector<AIAgentObject*> objs = mAgentWorld->GetObjects();
		bool isGoingToCollide = mSteeringBehavior->IsGoingToCollide(objs,
			0.3f);
		if (isGoingToCollide)
		{

		}
		else
		{

		}

		AVector force = mSteeringBehavior->Calculate();
		ApplyForce(force);

		if (!mRobot)
		{
			float mass = GetMass();
			float maxSpeed = GetMaxSpeed();

			AVector acce = force / mass;
			AVector curVelocity = GetVelocityNoPhysics();
			curVelocity.Z() = 0.0f;

			AVector newVelocity = curVelocity + acce * (float)elapsedTime;
			newVelocity.Z() = 0.0f;	

			AVector newVelocityNormalize = newVelocity;
			newVelocityNormalize.Normalize();

			float newSpeed = newVelocity.Length();
			if (newSpeed > maxSpeed)
			{
				newVelocity = newVelocityNormalize * maxSpeed;
			}
			SetVelocity(newVelocity);
			SetForward(newVelocityNormalize);
		}
		else
		{
		}
	}
}
//----------------------------------------------------------------------------
void AIAgent::SetAIAgentWorld(AIAgentWorld *agentWorld)
{
	mAgentWorld = agentWorld;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
AIAgent::AIAgent(LoadConstructor value) :
AIAgentBase(value)
{
	mForwarding = AVector::UNIT_Y;
	mSmoother = new0 Smoother<AVector>(16, AVector::UNIT_Y);

	mForcing = AVector::UNIT_Y;
	mSmootherForcing = new0 Smoother<AVector>(24, AVector::UNIT_Y);
}
//----------------------------------------------------------------------------
void AIAgent::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	AIAgentBase::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mHealth);
	source.Read(mHeight);
	source.Read(mMaxForce);
	source.Read(mMaxSpeed);
	source.Read(mSpeed);
	source.ReadAggregate(mTarget);
	source.Read(mTargetRadius);
	source.ReadString(mTeam);
	source.ReadBool(mIsEnableForwarding);

	PX2_END_DEBUG_STREAM_LOAD(AIAgent, source);
}
//----------------------------------------------------------------------------
void AIAgent::Link(InStream& source)
{
	AIAgentBase::Link(source);
}
//----------------------------------------------------------------------------
void AIAgent::PostLink()
{
	AIAgentBase::PostLink();
}
//----------------------------------------------------------------------------
bool AIAgent::Register(OutStream& target) const
{
	if (AIAgentBase::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AIAgent::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	AIAgentBase::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mHealth);
	target.Write(mHeight);
	target.Write(mMaxForce);
	target.Write(mMaxSpeed);
	target.Write(mSpeed);
	target.WriteAggregate(mTarget);
	target.Write(mTargetRadius);
	target.WriteString(mTeam);
	target.WriteBool(mIsEnableForwarding);

	PX2_END_DEBUG_STREAM_SAVE(AIAgent, target);
}
//----------------------------------------------------------------------------
int AIAgent::GetStreamingSize(Stream &stream) const
{
	int size = AIAgentBase::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mHealth);
	size += sizeof(mHeight);
	size += sizeof(mMaxForce);
	size += sizeof(mMaxSpeed);
	size += sizeof(mSpeed);
	size += sizeof(mTarget);
	size += sizeof(mTargetRadius);
	size += PX2_STRINGSIZE(mTeam);
	size += PX2_BOOLSIZE(mIsEnableForwarding);

	return size;
}
//----------------------------------------------------------------------------
