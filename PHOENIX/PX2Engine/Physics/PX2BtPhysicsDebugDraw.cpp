// PX2BtPhysicsDebugDraw.cpp

#include "PX2BtPhysicsDebugDraw.hpp"
#include "PX2BtPhysicsUtilities.hpp"
#include "PX2EngineCanvas.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PhysicsDebugDraw::PhysicsDebugDraw()
	: btIDebugDraw(),
	debugMode_(0)
{
}
//----------------------------------------------------------------------------
PhysicsDebugDraw::~PhysicsDebugDraw()
{
}
//----------------------------------------------------------------------------
void PhysicsDebugDraw::draw3dText(
	const btVector3& location, const char* textString)
{
	(void)location;
	(void)textString;
}
//----------------------------------------------------------------------------
void PhysicsDebugDraw::drawLine(
	const btVector3& from, const btVector3& to, const btVector3& color)
{
	APoint fromP = PhysicsUtilities::BtVector3ToVector3(from);
	APoint toP = PhysicsUtilities::BtVector3ToVector3(to);
	//EngineSceneCanvas::GetSingleton().AddDebugLine(fromP, toP, Float4::RED);

	//DebugDrawer::getSingleton().drawLine(
	//	Ogre::Vector3(from.m_floats[0], from.m_floats[1], from.m_floats[2]),
	//	Ogre::Vector3(to.m_floats[0], to.m_floats[1], to.m_floats[2]),
	//	Ogre::ColourValue(
	//	color.m_floats[0], color.m_floats[1], color.m_floats[2]));
}
//----------------------------------------------------------------------------
void PhysicsDebugDraw::drawContactPoint(
	const btVector3& pointOnB,
	const btVector3& normalOnB,
	btScalar distance,
	int lifeTime,
	const btVector3& color)
{
	(void)pointOnB;
	(void)normalOnB;
	(void)distance;
	(void)lifeTime;
	(void)color;
}
//----------------------------------------------------------------------------
int PhysicsDebugDraw::getDebugMode() const
{
	return debugMode_;
}
//----------------------------------------------------------------------------
void PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
	(void)warningString;
}
//----------------------------------------------------------------------------
void PhysicsDebugDraw::setDebugMode(int debugMode)
{
	debugMode_ = debugMode;
}
//----------------------------------------------------------------------------