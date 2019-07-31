// PX2EventHandler.hpp

#ifndef PX2EVENTHANDLER_HPP
#define PX2EVENTHANDLER_HPP

#include "PX2CorePre.hpp"
#include "PX2EventChannel.hpp"

namespace PX2
{

	class Event;
	class EventWorld;

	/// 消息句柄
	class PX2_ENGINE_ITEM EventHandler
	{
	public:
		EventHandler ();
		virtual ~EventHandler ();

		// 频道
		void SetChannel (const EventChannel &channel);
		const EventChannel &GetChannel () const;
		void MergeChannel (const EventChannel &channel);
		void DisMergeChannel (const EventChannel &channel);
		void ClearChannel ();
		bool IsListening (const EventChannel &channel);

		// 世界
		EventWorld* GetWorld ();
		bool IsInWorld () const;
		bool IsInWorld(EventWorld* world) const;

		// 重载,进行相应的消息处理
		virtual void OnEvent (Event *event);
		
public_internal:
		void _Execute (Event* event);
		void _Enter(EventWorld* world);
		void _Leave();

	private:
		bool mEntered;
		bool mEnableUpdate;
		EventWorld* mWorld;
		EventChannel mChannel;
	};

}

#endif