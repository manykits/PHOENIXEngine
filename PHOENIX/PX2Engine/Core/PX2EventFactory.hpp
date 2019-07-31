// PX2EventChannel.hpp

#ifndef PX2EVENTFACTORY_HPP
#define PX2EVENTFACTORY_HPP

#include "PX2EventFactoryImplement.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM EventFactory
	{
	public:
		// 单件
		static EventFactory &GetInstance ();
		static EventFactory* GetInstancePtr ();

		// 事件
		Event* CreateEventX ();
		void DestoryEvent (Event* event);

		// 事件空间
		void RegisterEventSpace (EventSpace* eventSpace);
		void UnRegisterEventSpace (const std::string &name);
		EventSpace* GetEventSpace (const std::string &name) const;
		EventSpace* FindEventSpace (Event* event) const;

	private:
		EventFactory ();
		~EventFactory ();

		EventFactoryImplement* mEventImplement;
	};

}

#endif