// PX2EventWorld.hpp

#ifndef PX2EVENTWORLD_HPP
#define PX2EVENTWORLD_HPP

#include "PX2CorePre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{
	class _EventHanderNode
	{
	public:
		_EventHanderNode();

		_EventHanderNode *Pre;
		_EventHanderNode *Next;

		EventHandler *Handler;
	};


	/// 消息世界
	class PX2_ENGINE_ITEM EventWorld : public Singleton<EventWorld>
	{
	public:
		EventWorld ();
		~EventWorld ();

		void Shutdown (bool shutdown);
		bool IsShutdown () const;

		void Update (float detalTime);

		// 消息句柄
		void ComeIn (EventHandler *handler);
		void GoOut (EventHandler *handler);

		void BroadcastingLocalEvent (Event* event);
		void BroadcastingNetEvent (Event* event);

	private:
		void _UpdateEvent(float detalTime);
		void _BroadcastingEvent (Event* event);
		void _ProcessEvent(Event *event);
		void _DestoryEvent(Event *event);

		bool mIsShoutdown;

		std::vector<Event *> mUpdateEventList;

		_EventHanderNode *mHandlerNodeFirst;
		_EventHanderNode *mHandlerNodeLast;
		std::map<Event*, _EventHanderNode*> mHanderNodeCurMap;
	};

	typedef Pointer0<EventWorld> EventWorldPtr;

#define PX2_EW EventWorld::GetSingleton()

}

#endif