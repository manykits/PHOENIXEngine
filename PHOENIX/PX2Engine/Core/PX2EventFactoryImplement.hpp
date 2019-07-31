// PX2EventFactoryImpl.hpp

#ifndef PX2EVENTFACTORYIMPLEMENT_HPP
#define PX2EVENTFACTORYIMPLEMENT_HPP

#include "PX2CorePre.hpp"
#include "PX2Event.hpp"
#include "PX2Memory.hpp"

namespace PX2
{
	class Event;
	class EventSpace;

	/// 事件工厂实现类
	class PX2_ENGINE_ITEM EventFactoryImplement
	{
	public:
		EventFactoryImplement ();
		~EventFactoryImplement ();

		// 事件
		Event* CreateEventX ();
		void DestoryEvent (Event* event);

		// 事件空间
		void RegisterEventSpace (EventSpace* eventSpace);
		void UnRegisterEventSpace (const std::string &name);
		EventSpace* GetEventSpace (const std::string &name) const;
		EventSpace* FindEventSpace (Event* event) const;

	private:
		typedef std::map<std::string, EventSpace*> EventSpaceMap;

		EventSpaceMap mSpaces;
		int mEventQuantity;
	};

}

#endif