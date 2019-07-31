// PX2EventFactoryImpl.cpp

#include "PX2EventFactoryImplement.hpp"
#include "PX2EventSpace.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EventFactoryImplement::EventFactoryImplement ()
	:
mEventQuantity(0)
{
}
//----------------------------------------------------------------------------
EventFactoryImplement::~EventFactoryImplement ()
{
}
//----------------------------------------------------------------------------
Event* EventFactoryImplement::CreateEventX ()
{
	return new0 Event();
}
//----------------------------------------------------------------------------
void EventFactoryImplement::DestoryEvent (Event* event)
{
	delete0(event);
}
//----------------------------------------------------------------------------
void EventFactoryImplement::RegisterEventSpace (EventSpace* eventSpace)
{
	EventSpaceMap::iterator it = mSpaces.find(eventSpace->GetName());

	if (it != mSpaces.end())
	{
		assertion(false, "'eventSpace' has already registered.\n");
	}

	eventSpace->SetSpace(mEventQuantity);
	mEventQuantity += eventSpace->GetEventQuantity();

	mSpaces.insert(std::make_pair(eventSpace->GetName(), eventSpace));
}
//----------------------------------------------------------------------------
 void EventFactoryImplement::UnRegisterEventSpace (const std::string &name)
{
	EventSpaceMap::iterator it = mSpaces.find(name);

	mSpaces.erase(it);
}
//----------------------------------------------------------------------------
 EventSpace* EventFactoryImplement::GetEventSpace (const std::string &name) const
 {
	 EventSpaceMap::const_iterator it = mSpaces.find(name);

	 if (it == mSpaces.end())
	 {
		 return 0;
	 }

	 return it->second;
 }
//----------------------------------------------------------------------------
 EventSpace* EventFactoryImplement::FindEventSpace (Event* event) const
 {
	 EventSpaceMap::const_iterator it;
	 
	 for (it=mSpaces.begin(); it!=mSpaces.end(); ++it)
	 {
		 if (it->second->IsIn(event))
			 return it->second;
	 }

	 return 0;
 }
 //----------------------------------------------------------------------------