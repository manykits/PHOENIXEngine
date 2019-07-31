// PX2EventSpaceRegisterMCR.hpp

#ifndef PX2EVENTSPACEREGISTERMCR_HPP
#define PX2EVENTSPACEREGISTERMCR_HPP

#include "PX2CorePre.hpp"

#define PX2_DECLARE_EVENT_BEGIN(Space) \
	class Space : public EventSpaceRegister<Space> \
	{ \
	public: \
		enum Event \
		{

#define PX2_EVENT(Event) Event,

#define PX2_DECLARE_EVENT_END(Space) \
			E_QUANTITY \
		}; \
		static const std::string mName; \
		static EventSpace *GetPtr (); \
		static EventSpace &GetRef (); \
		static std::string EventStrs[E_QUANTITY]; \
	private: \
		static Space mSpace; \
	};

#define PX2_IMPLEMENT_EVENT(Space) const std::string Space::mName(#Space); \
	Space Space::mSpace; \
	\
	EventSpace *Space::GetPtr() \
	{ \
		return &mSpace; \
	} \
	\
	EventSpace &Space::GetRef() \
	{ \
		return mSpace; \
	}

#define PX2_CREATEEVENT(Space, EventType) Space::CreateEventX(Space::EventType)
#define PX2_CREATEEVENTEX(Space, EventType) Space::CreateEventX(Space::EventType, std::string(#Space) + "::" + #EventType)

#endif