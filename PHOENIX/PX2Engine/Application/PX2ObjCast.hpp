// PX2ObjCast.hpp

#ifndef PX2OBJCAST_HPP
#define PX2OBJCAST_HPP

#include "PX2Core.hpp"
#include "PX2Node.hpp"
#include "PX2ScriptController.hpp"
#include "PX2EffectableController.hpp"
#include "PX2GeneralServer.hpp"
#include "PX2GeneralClientConnector.hpp"
#include "PX2SizeNode.hpp"
#include "PX2Actor.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Cast
	{
	public:
		static Controller *ToController(Object *obj);
		static Controlledable *ToControlledable(Object *obj);
		static Movable *ToMovable(Object *obj);
		static Node *ToNode(Object *obj);
		static SizeNode *ToSizeNode(Object *obj);
		static EffectableController *ToEffectController(Object *obj);
		static Actor *ToActor(Object *obj);
		static Object *ToO(void *obj);
		static ScriptController *ToSC(void *obj);
		static GeneralServer *ToGS(void *obj);
		static GeneralClientConnector *ToGCC(void *obj);
	};

}

#endif