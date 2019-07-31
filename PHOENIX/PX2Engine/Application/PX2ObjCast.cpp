// PX2ObjCast.cpp

#include "PX2ObjCast.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Controller *Cast::ToController(Object *obj)
{
	return DynamicCast<Controller>(obj);
}
//----------------------------------------------------------------------------
Controlledable *Cast::ToControlledable(Object *obj)
{
	return DynamicCast<Controlledable>(obj);
}
//----------------------------------------------------------------------------
Movable *Cast::ToMovable(Object *obj)
{
	return DynamicCast<Movable>(obj);
}
//----------------------------------------------------------------------------
SizeNode *Cast::ToSizeNode(Object *obj)
{
	return DynamicCast<SizeNode>(obj);
}
//----------------------------------------------------------------------------
Node *Cast::ToNode(Object *obj)
{
	return DynamicCast<Node>(obj);
}
//----------------------------------------------------------------------------
EffectableController *Cast::ToEffectController(Object *obj)
{
	return DynamicCast<EffectableController>(obj);
}
//----------------------------------------------------------------------------
Actor *Cast::ToActor(Object *obj)
{
	return DynamicCast<Actor>(obj);
}
//----------------------------------------------------------------------------
Object *Cast::ToO(void *obj)
{
	return (Object*)obj;
}
//----------------------------------------------------------------------------
ScriptController *Cast::ToSC(void *obj)
{
	return (ScriptController*)obj;
}
//----------------------------------------------------------------------------
GeneralServer *Cast::ToGS(void *obj)
{
	return (GeneralServer*)obj;
}
//----------------------------------------------------------------------------
GeneralClientConnector *Cast::ToGCC(void *obj)
{
	return (GeneralClientConnector*)obj;
}
//----------------------------------------------------------------------------