// PX2EngineEventHandler.cpp

#include "PX2EngineEventHandler.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EngineEventHandler::EngineEventHandler()
{
}
//----------------------------------------------------------------------------
EngineEventHandler::~EngineEventHandler()
{
}
//----------------------------------------------------------------------------
void EngineEventHandler::OnEvent(Event *event)
{
	const std::string &eventTypeStr = event->GetEventTypeStr();
	if (eventTypeStr.empty())
		return;

	const std::string &dataStr0 = event->GetDataStr0();
	const std::string &dataStr1 = event->GetDataStr1();
	const std::string &dataStr2 = event->GetDataStr2();

	std::map<std::string, std::vector<std::string> >::iterator it = 
		mEventCalls.find(eventTypeStr);

	if (it != mEventCalls.end())
	{
		const std::vector<std::string> &strVecs = it->second;
		for (int i = 0; i < (int)strVecs.size(); i++)
		{
			std::string callStr = strVecs[i];
			callStr += "()";
			PX2_SC_LUA->CallString(callStr);
		}
	}
}
//----------------------------------------------------------------------------
void EngineEventHandler::AddEventCall(const std::string &className,
	const std::string &eventName, const std::string &callFun)
{
	std::string classEventName = className + "::" + eventName;
	mEventCalls[classEventName].push_back(callFun);
}
//----------------------------------------------------------------------------