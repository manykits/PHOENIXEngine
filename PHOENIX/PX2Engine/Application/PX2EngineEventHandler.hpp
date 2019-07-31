// PX2EngineEventHandler.hpp

#ifndef PX2ENGINEEVENTHANDLER_HPP
#define PX2EGNINEEVENTHANDLER_HPP

#include "PX2EventHandler.hpp"
#include "PX2Event.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EngineEventHandler : public EventHandler, public Singleton<EngineEventHandler>
	{
	public:
		EngineEventHandler();
		virtual ~EngineEventHandler();

		virtual void OnEvent(Event *event);

		void AddEventCall(const std::string &className,
			const std::string &eventName, const std::string &callFun);

	protected:
		std::map<std::string, std::vector<std::string> > mEventCalls;
	};

#define PX2_EEH EngineEventHandler::GetSingleton()

}

#endif
