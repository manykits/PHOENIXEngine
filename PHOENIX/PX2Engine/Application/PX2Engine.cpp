// PX2Engine.cpp

#include "PX2Engine.hpp"
#include "PX2Creater.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Engine::Engine()
{
}
//----------------------------------------------------------------------------
Engine::~Engine()
{
}
//----------------------------------------------------------------------------
Actor *Engine::CreateActor()
{
	return new0 Actor();
}
//----------------------------------------------------------------------------
Actor *Engine::CreateActorBox()
{
	Actor *actor = new0 Actor();

	Movable *movBox = PX2_CREATER.CreateMovable_Box();
	actor->AttachChild(movBox);

	return actor;
}
//----------------------------------------------------------------------------