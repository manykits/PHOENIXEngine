// PX2EventHandlerObject.cpp

#include "PX2EventHandlerObject.hpp"
#include "PX2Object.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ObjectEventHandler::ObjectEventHandler(Object *obj) :
mObject(obj)
{
}
//----------------------------------------------------------------------------
ObjectEventHandler::~ObjectEventHandler()
{
}
//----------------------------------------------------------------------------
ObjectEventHandler::ObjectEventHandler()
{
}
//----------------------------------------------------------------------------
void ObjectEventHandler::OnEvent(Event *event)
{
	mObject->OnEvent(event);
}
//----------------------------------------------------------------------------