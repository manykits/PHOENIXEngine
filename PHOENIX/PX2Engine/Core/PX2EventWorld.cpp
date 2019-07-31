// PX2EventWorld.cpp

#include "PX2EventWorld.hpp"
#include "PX2Event.hpp"
#include "PX2EventFactory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
_EventHanderNode::_EventHanderNode() :
Pre(0),
Next(0),
Handler(0)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
EventWorld::EventWorld ()
{
	mIsShoutdown = false;

	mHandlerNodeFirst = 0;
	mHandlerNodeLast = 0;
}
//----------------------------------------------------------------------------
EventWorld::~EventWorld ()
{
	for (int i = 0; i < (int)mUpdateEventList.size(); i++)
	{
		Event *ent = mUpdateEventList[i];
		_DestoryEvent(ent);
	}
	mUpdateEventList.clear();

	_EventHanderNode *next = mHandlerNodeFirst;
	while (next)
	{
		_EventHanderNode *curNode = next;
		next = curNode->Next;

		delete0(curNode);
	}
}
//----------------------------------------------------------------------------
void EventWorld::Shutdown (bool shutdown)
{
	mIsShoutdown = shutdown;
}
//----------------------------------------------------------------------------
bool EventWorld::IsShutdown () const
{
	return mIsShoutdown;
}
//----------------------------------------------------------------------------
void EventWorld::ComeIn (EventHandler *handler)
{
	if (mIsShoutdown) return;

	assertion(handler!=0, "handler must not be 0.");

	if (!handler) return;

	if (handler->IsInWorld(this)) return;

	_EventHanderNode *node = new0 _EventHanderNode();
	node->Handler = handler;

	if (0 == mHandlerNodeFirst)
		mHandlerNodeFirst = node;

	if (mHandlerNodeLast)
		mHandlerNodeLast->Next = node;
	node->Pre = mHandlerNodeLast;

	mHandlerNodeLast = node;

	handler->_Enter(this);
}
//----------------------------------------------------------------------------
void EventWorld::GoOut(EventHandler *handler)
{
	if (!handler) return;

	if (!handler->IsInWorld(this))	return;

	handler->_Leave();

	bool removed = false;

	_EventHanderNode *next = mHandlerNodeFirst;
	while (next)
	{
		_EventHanderNode *curNode = next;
		next = curNode->Next;

		// delete
		if (handler == curNode->Handler)
		{
			// handler cur
			auto it = mHanderNodeCurMap.begin();
			for (; it != mHanderNodeCurMap.end(); it++)
			{
				_EventHanderNode * ehn = it->second;

				if (ehn == curNode)
				{
					it->second = next;
				}
			}

			// first
			if (mHandlerNodeFirst == curNode)
				mHandlerNodeFirst = mHandlerNodeFirst->Next;

			// last
			if (mHandlerNodeLast == curNode)
				mHandlerNodeLast = curNode->Pre;

			// delete cur;
			if (curNode->Pre)
				curNode->Pre->Next = next;
			if (next)
			{
				next->Pre = curNode->Pre;
			}

			delete0(curNode);
			removed = true;
		}
	}

	assertion(removed, "");
}
//----------------------------------------------------------------------------
void EventWorld::Update (float detalTime)
{
	if (mIsShoutdown)
		return;

	_UpdateEvent(detalTime);
}
//----------------------------------------------------------------------------
void EventWorld::BroadcastingLocalEvent (Event* event)
{
	if (event->IsSystemEvent())
	{
		assertion(false, "event can't be system event.");
	}

	_BroadcastingEvent(event);
}
//----------------------------------------------------------------------------
void EventWorld::BroadcastingNetEvent (Event* event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------
void EventWorld::_UpdateEvent(float detalTime)
{
	for (int i = 0; i < (int)mUpdateEventList.size(); i++)
	{
		Event *ent = mUpdateEventList[i];
		ent->Update(detalTime);
		if (ent->IsDelayActted())
		{
			_ProcessEvent(ent);
		}
	}

	auto it = mUpdateEventList.begin();
	for (; it != mUpdateEventList.end();)
	{
		Event *ent = *it;
		if (ent->IsDelayActted())
		{
			_DestoryEvent(ent);
			it = mUpdateEventList.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
void EventWorld::_BroadcastingEvent(Event* event)
{
	if (IsShutdown())
	{
		EventFactory::GetInstance().DestoryEvent(event);
		return;
	}

	if (event->IsDoDelay())
	{
		mUpdateEventList.push_back(event);
	}
	else
	{
		_ProcessEvent(event);
		_DestoryEvent(event);
	}
}
//----------------------------------------------------------------------------
void EventWorld::_ProcessEvent(Event *event)
{
	// 系统消息
	if (event->IsSystemEvent())
	{
	}
	// 非系统消息
	else
	{
		EventHandler* receiver = event->GetReceiver();
		if (receiver)
		{ // 有接收者
			receiver->_Execute(event);
		}
		else
		{ // 没有接收者，轮询

			_EventHanderNode *node = mHandlerNodeFirst;
			mHanderNodeCurMap[event] = node;

			while (mHanderNodeCurMap[event])
			{
				_EventHanderNode *nodeCur = mHanderNodeCurMap[event];
				mHanderNodeCurMap[event] = mHanderNodeCurMap[event]->Next;

				if (nodeCur && nodeCur->Handler->GetChannel().IsListening((event)->GetChannel()))
					nodeCur->Handler->_Execute(event);
			}
		}
	}

	// 消息处理完，将其删除
	mHanderNodeCurMap.erase(event);
}
//----------------------------------------------------------------------------
void EventWorld::_DestoryEvent(Event *event)
{
	EventFactory::GetInstance().DestoryEvent(event);
}
//----------------------------------------------------------------------------