// PX2BtPhysicsDebugDraw.hpp

#ifndef PX2BTPHYSICSDEBUGDRAW_HPP
#define PX2BTPHYSICSDEBUGDRAW_HPP

#include "LinearMath/btIDebugDraw.h"

namespace PX2
{

	class PhysicsDebugDraw : public btIDebugDraw
	{
	public:
		PhysicsDebugDraw();
		virtual ~PhysicsDebugDraw();

		virtual void draw3dText(const btVector3& location, const char* textString);

		virtual void drawContactPoint(
			const btVector3& pointOnB,
			const btVector3& normalOnB,
			btScalar distance,
			int lifeTime,
			const btVector3& color);

		virtual void drawLine(
			const btVector3& from, const btVector3& to, const btVector3& color);

		virtual int getDebugMode() const;

		virtual void reportErrorWarning(const char* warningString);

		virtual void setDebugMode(int debugMode);

	private:
		int debugMode_;

		PhysicsDebugDraw(const PhysicsDebugDraw&);
		PhysicsDebugDraw& operator=(const PhysicsDebugDraw&);
	};


}

#endif